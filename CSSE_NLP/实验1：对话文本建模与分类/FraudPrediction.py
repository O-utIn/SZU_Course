import pandas as pd
import numpy as np
import os
import time
from openai import OpenAI
from sklearn.metrics import accuracy_score
from tqdm import tqdm
from concurrent.futures import ThreadPoolExecutor, as_completed


# ==========================================
# 1. 配置参数
# ==========================================
class Config:
    API_KEY = ""
    BASE_URL = "https://dashscope.aliyuncs.com/compatible-mode/v1"
    MODEL_NAME = "qwen3-max"

    # 并发线程数
    MAX_WORKERS = 8

    # 文件路径
    input_test_file = '测试集结果.csv'

    # 输出文件
    output_full_result = '测试集结果_LLM全量预测.csv'
    output_error_analysis = '测试集_错误案例分析.csv'


# 初始化客户端
client = OpenAI(api_key=Config.API_KEY, base_url=Config.BASE_URL)


# ==========================================
# 2. 核心功能函数
# ==========================================

def load_and_clean_data(filepath):
    """读取数据并标准化标签"""
    if not os.path.exists(filepath):
        raise FileNotFoundError(f"找不到文件: {filepath}")

    try:
        df = pd.read_csv(filepath, encoding='utf-8-sig')
    except:
        df = pd.read_csv(filepath, encoding='gbk')

    # 标签映射：统一转为 0 和 1
    map_dict = {'TRUE': 1, 'FALSE': 0, True: 1, False: 0, '1': 1, '0': 0, 1: 1, 0: 0}
    df['label_id'] = df['is_fraud'].map(map_dict).fillna(0).astype(int)
    return df


def get_llm_prediction(content):
    """单次调用大模型"""
    prompt = f"""
你是一名反电信诈骗专家。请分析以下对话内容，判断其是否属于电信诈骗。
对话内容：{content}
如果是诈骗，仅输出数字 1；如果不属于诈骗，仅输出数字 0。不要输出其他内容。
"""
    retries = 3
    for i in range(retries):
        try:
            completion = client.chat.completions.create(
                model=Config.MODEL_NAME,
                messages=[
                    {"role": "system", "content": "You are a helpful assistant."},
                    {"role": "user", "content": prompt},
                ],
                stream=False
            )
            res = completion.choices[0].message.content.strip()
            if "1" in res: return 1
            return 0
        except Exception:
            if i == retries - 1: return 0  # 失败默认返回0
            time.sleep(1)


def process_dataset_multithread(df):
    """多线程处理整个数据集"""
    total = len(df)
    print(f">>> 开始全量处理测试集，共 {total} 条数据...")
    print(f"    模型: {Config.MODEL_NAME} | 线程数: {Config.MAX_WORKERS}")

    results = [0] * total

    with ThreadPoolExecutor(max_workers=Config.MAX_WORKERS) as executor:
        future_to_idx = {
            executor.submit(get_llm_prediction, row['specific_dialogue_content']): idx
            for idx, row in df.iterrows()
        }

        for future in tqdm(as_completed(future_to_idx), total=total, desc="进度"):
            idx = future_to_idx[future]
            try:
                results[idx] = future.result()
            except:
                results[idx] = 0

    df['prediction'] = pd.Series(results, index=df.index)
    return df


def calculate_metrics(df):
    """计算准确率指标"""
    y_true = df['label_id'].values
    y_pred = df['prediction'].values

    acc = accuracy_score(y_true, y_pred)

    fraud_df = df[df['label_id'] == 1]
    acc_fraud = accuracy_score(fraud_df['label_id'], fraud_df['prediction']) if len(fraud_df) > 0 else 0.0

    non_fraud_df = df[df['label_id'] == 0]
    acc_nonfraud = accuracy_score(non_fraud_df['label_id'], non_fraud_df['prediction']) if len(
        non_fraud_df) > 0 else 0.0

    print(f"\n========= 核心指标统计 =========")
    print(f"总准确率 (Accuracy):           {acc:.2%}")
    print(f"诈骗样本识别率 (Recall Fraud):   {acc_fraud:.2%}")
    print(f"非诈骗样本识别率 (Recall Non):  {acc_nonfraud:.2%}")


# ==========================================
# 3. 错误案例分析
# ==========================================

def perform_error_analysis(df):
    """
    筛选错误案例，打印并保存
    """
    print(f"\n========= 错误案例深度分析 =========")

    # 筛选 False Negative (漏报: 真1测0)
    fn_df = df[(df['label_id'] == 1) & (df['prediction'] == 0)].copy()
    fn_df['error_type'] = '漏报(FN)'

    # 筛选 False Positive (误报: 真0测1)
    fp_df = df[(df['label_id'] == 0) & (df['prediction'] == 1)].copy()
    fp_df['error_type'] = '误报(FP)'

    print(f"错误总数: {len(fn_df) + len(fp_df)}")
    print(f"1. 漏报 (没识别出的诈骗): {len(fn_df)} 例")
    print(f"2. 误报 (把正常当成诈骗): {len(fp_df)} 例")
    print("-" * 50)

    # 打印典型案例供报告使用
    print("\n【典型漏报案例 (FN)】:")
    if not fn_df.empty:
        for i, row in fn_df.head(3).iterrows():
            print(f"> 内容: {row['specific_dialogue_content']}")
            print(f"> 类型: {row.get('fraud_type', '未知')} | 标签: 诈骗 -> 预测: 正常")
            print("~" * 30)

    print("\n【典型误报案例 (FP)】:")
    if not fp_df.empty:
        for i, row in fp_df.head(3).iterrows():
            print(f"> 内容: {row['specific_dialogue_content']}")
            print(f"> 标签: 正常 -> 预测: 诈骗")
            print("~" * 30)

    # 保存错误案例
    error_df = pd.concat([fn_df, fp_df])
    # 将 error_type 列移动到前面
    cols = ['error_type', 'specific_dialogue_content', 'prediction', 'is_fraud'] + [c for c in df.columns if
                                                                                    c not in ['error_type',
                                                                                              'specific_dialogue_content',
                                                                                              'prediction', 'is_fraud',
                                                                                              'label_id']]
    try:
        error_df = error_df[cols]
    except:
        pass  # 如果列名不匹配

    error_df.to_csv(Config.output_error_analysis, index=False, encoding='utf-8-sig')
    print(f"\n错误列表已保存至: {Config.output_error_analysis}")


# ==========================================
# 4. 主程序入口
# ==========================================

if __name__ == "__main__":
    # 1. 加载测试集
    print("1. 加载数据...")
    df_test = load_and_clean_data(Config.input_test_file)
    print(f"   测试集数量: {len(df_test)}")

    # 2. 全量预测 (多线程)
    start_time = time.time()
    df_result = process_dataset_multithread(df_test)
    print(f"\n耗时: {time.time() - start_time:.2f}秒")

    # 3. 计算指标
    calculate_metrics(df_result)

    # 4. 执行错误分析
    perform_error_analysis(df_result)

    # 5. 保存完整结果
    df_result.drop(columns=['label_id'], inplace=True, errors='ignore')
    df_result.to_csv(Config.output_full_result, index=False, encoding='utf-8-sig')
    print(f"完整结果已保存至: {Config.output_full_result}")