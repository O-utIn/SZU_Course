import os
import re
import json
import time
from collections import Counter, defaultdict
from concurrent.futures import ThreadPoolExecutor, as_completed

import pandas as pd
from tqdm import tqdm

try:
    import openai
except Exception:
    openai = None


class Config:
    API_KEY = ""
    BASE_URL = "https://dashscope.aliyuncs.com/compatible-mode/v1"
    MODEL_NAME = "qwen3-max"
    MAX_WORKERS = 8
    # 设置为 False 则禁用外部 LLM 调用，使用本地规则分类（便于快速运行和调试）
    USE_LLM = False
    input_test_file = os.path.join('通话数据互动策略结果', '测试集结果.csv')
    output_full_result = os.path.join('通话数据互动策略结果', '测试集结果_LLM全量预测.csv')
    output_analysis = os.path.join('通话数据互动策略结果', '测试集_行为策略共现分析.csv')


CATEGORY_MAP = {
    '请求': '请求',
    '陈述': '陈述',
    '确认': '确认',
    '拒绝': '拒绝',
    '其他': '其他',
}


def extract_left_utterances(dialogue_text):
    # 提取所有 'left:' 后面的发言块，作为一个 utterance 单位
    # 适配多行字段
    parts = re.split(r'(?i)left:\s*', dialogue_text)
    utterances = []
    for p in parts[1:]:
        # 截取到下一个 right: 或 end
        u = re.split(r'(?i)right:\s*', p)[0].strip()
        # 规范化空白
        u = re.sub(r'\s+', ' ', u)
        if u:
            utterances.append(u)
    return utterances


def build_prompt(utterance):
    return (
        "请判断下列句子的对话行为类别，仅在五类中选择并只输出类别名称（不要多余说明）：\n"
        "类别选项：请求、陈述、确认、拒绝、其他。\n"
        f"句子：{utterance}\n"
        "请只返回其中之一：请求 或 陈述 或 确认 或 拒绝 或 其他。"
    )


def call_llm(prompt, config: Config, max_retries=3):
    # 使用 OpenAI 兼容接口（若 openai 可用），否则返回 None
    if openai is None:
        return None
    openai.api_key = config.API_KEY
    openai.api_base = config.BASE_URL
    for attempt in range(max_retries):
        try:
            resp = openai.ChatCompletion.create(
                model=config.MODEL_NAME,
                messages=[
                    {"role": "system", "content": "你是一个中文对话发言行为分类器，回答必须严格为指定的类别中文名称。"},
                    {"role": "user", "content": prompt},
                ],
                temperature=0.0,
                max_tokens=32,
            )
            text = resp['choices'][0]['message']['content'].strip()
            # 只保留首个词
            text = text.splitlines()[0].strip()
            # 规范化
            for k in CATEGORY_MAP:
                if k in text:
                    return CATEGORY_MAP[k]
            # 如果返回英文或其他，尝试映射
            if text.lower().startswith('request'):
                return '请求'
            if text.lower().startswith('statement'):
                return '陈述'
            if text.lower().startswith('confirmation'):
                return '确认'
            if text.lower().startswith('rejection'):
                return '拒绝'
            return '其他'
        except Exception as e:
            wait = 1 + attempt * 2
            time.sleep(wait)
    return None


def rule_based_classify(utt):
    # 备选的规则分类器，在无法调用 LLM 时使用
    utt_l = utt.lower()
    if any(w in utt_l for w in ['请问', '能否', '可以', '帮我', '帮忙', '麻烦', '要不要', '发给我', '告诉我', '给我']):
        return '请求'
    if any(w in utt_l for w in ['不是', '不行', '不要', '不可以', '拒绝', '没用']):
        return '拒绝'
    if any(w in utt_l for w in ['好的', '是的', '没问题', '收到', '明白', '行', '好吧']):
        return '确认'
    # 默认认为是陈述
    return '陈述'


def classify_utterances(utterances, config: Config):
    results = []
    cache = {}
    use_llm = (openai is not None) and getattr(config, 'USE_LLM', True)

    with ThreadPoolExecutor(max_workers=config.MAX_WORKERS) as exe:
        futures = {}
        for utt in utterances:
            key = utt
            if key in cache:
                continue
            if use_llm:
                prompt = build_prompt(utt)
                futures[exe.submit(call_llm, prompt, config)] = key
            else:
                cache[key] = rule_based_classify(utt)

        for fut in tqdm(as_completed(futures), total=len(futures), desc='LLM calls'):
            key = futures[fut]
            try:
                res = fut.result()
            except Exception:
                res = None
            if not res:
                res = rule_based_classify(key)
            cache[key] = res

    # build results preserving order
    for utt in utterances:
        results.append(cache.get(utt, rule_based_classify(utt)))
    return results


def analyze_and_save(df, config: Config):
    behaviors = ['请求', '陈述', '确认', '拒绝', '其他']

    # 提取每对话的行为占比并保存
    rows = []
    for _, r in df.iterrows():
        utterances = r['_left_utts']
        cats = r['_left_cats']
        total = max(1, len(cats))
        cnt = Counter(cats)
        prop = {b: cnt.get(b, 0) / total for b in behaviors}
        out = {
            'specific_dialogue_content': r['specific_dialogue_content'],
            'interaction_strategy': r.get('interaction_strategy', ''),
            'is_fraud': r.get('is_fraud', ''),
        }
        for b in behaviors:
            out[f'prop_{b}'] = prop[b]
            out[f'cnt_{b}'] = cnt.get(b, 0)
        rows.append(out)

    df_prop = pd.DataFrame(rows)

    # 规范化 interaction_strategy 字段，取第一个标签
    def normalize_strategy(x):
        if pd.isna(x):
            return ''
        if isinstance(x, str):
            return x.split('/')[0].split(',')[0].strip()
        return str(x)

    df_prop['strategy_norm'] = df_prop['interaction_strategy'].map(normalize_strategy)

    # 按 is_fraud 分组并计算每个 behavior x strategy 的均值
    results = []
    behaviors_short = behaviors

    strategies = df_prop['strategy_norm'].unique()
    for s in strategies:
        if s == '':
            continue
        sub = df_prop[df_prop['strategy_norm'] == s]
        fraud_sub = sub[sub['is_fraud'].astype(str).str.lower().isin(['true', '1', 't', 'y'])]
        nonfraud_sub = sub[~sub['is_fraud'].astype(str).str.lower().isin(['true', '1', 't', 'y'])]
        for b in behaviors_short:
            mean_f = fraud_sub[f'prop_{b}'].mean() if len(fraud_sub) > 0 else 0.0
            mean_nf = nonfraud_sub[f'prop_{b}'].mean() if len(nonfraud_sub) > 0 else 0.0
            uplift = (mean_f / mean_nf) if mean_nf > 0 else (float('inf') if mean_f > 0 else 0.0)
            results.append({
                'strategy': s,
                'behavior': b,
                'mean_prop_fraud': mean_f,
                'mean_prop_nonfraud': mean_nf,
                'uplift': uplift,
                'n_fraud': len(fraud_sub),
                'n_nonfraud': len(nonfraud_sub),
            })

    df_res = pd.DataFrame(results)
    df_res = df_res.sort_values(by=['uplift'], ascending=False)
    # 保存为带 BOM 的 UTF-8，避免 Windows Excel 中文乱码；同时写入 GBK 副本以兼容旧环境
    df_res.to_csv(config.output_analysis, index=False, encoding='utf_8_sig')
    try:
        gbk_analysis = config.output_analysis.replace('.csv', '_gbk.csv')
        df_res.to_csv(gbk_analysis, index=False, encoding='gbk', errors='replace')
    except Exception:
        gbk_analysis = None
    print('已保存分析结果到', config.output_analysis, '、GBK 副本：', gbk_analysis)


def main():
    cfg = Config()
    print('读取数据：', cfg.input_test_file)
    df = pd.read_csv(cfg.input_test_file)
    # 提取 left utterances
    left_utts = []
    for txt in tqdm(df['specific_dialogue_content'].fillna(''), desc='提取 left'):
        left_utts.append(extract_left_utterances(txt))
    df['_left_utts'] = left_utts

    # 扁平化去重 utterances 用于批量分类
    all_utts = []
    for l in df['_left_utts']:
        all_utts.extend(l)
    unique_utts = list(dict.fromkeys(all_utts))

    print('待分类独立 utterances:', len(unique_utts))
    utt_to_cat = {}
    if len(unique_utts) > 0:
        cats = classify_utterances(unique_utts, cfg)
        utt_to_cat = dict(zip(unique_utts, cats))

    # 将类别回填到每个对话
    left_cats = []
    for l in df['_left_utts']:
        left_cats.append([utt_to_cat.get(u, '其他') for u in l])
    df['_left_cats'] = left_cats

    # 添加计数与比例列到原始表并保存
    behaviors = ['请求', '陈述', '确认', '拒绝', '其他']
    for b in behaviors:
        df[f'cnt_{b}'] = df['_left_cats'].apply(lambda cats: Counter(cats).get(b, 0))
    df['total_left'] = df['_left_cats'].apply(len)
    for b in behaviors:
        df[f'prop_{b}'] = df.apply(lambda r: r[f'cnt_{b}'] / max(1, r['total_left']), axis=1)

    # 保存完整预测结果：带 BOM 的 UTF-8，并写入 GBK 副本
    df.to_csv(cfg.output_full_result, index=False, encoding='utf_8_sig')
    try:
        gbk_full = cfg.output_full_result.replace('.csv', '_gbk.csv')
        df.to_csv(gbk_full, index=False, encoding='gbk', errors='replace')
    except Exception:
        gbk_full = None
    print('已保存带预测的完整结果到', cfg.output_full_result, '、GBK 副本：', gbk_full)

    analyze_and_save(df, cfg)


if __name__ == '__main__':
    main()
