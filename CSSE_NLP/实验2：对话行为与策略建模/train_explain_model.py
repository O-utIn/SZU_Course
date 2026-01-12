import os
import pandas as pd
import numpy as np

from sklearn.model_selection import train_test_split
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import roc_auc_score, accuracy_score, precision_score, recall_score


class Config:
    input_file = os.path.join('通话数据互动策略结果', '测试集结果_LLM全量预测.csv')
    output_feat = os.path.join('通话数据互动策略结果', '逻辑回归_特征系数.csv')
    output_metrics = os.path.join('通话数据互动策略结果', '逻辑回归_模型评估.csv')


def normalize_strategy(s):
    if pd.isna(s):
        return ''
    if isinstance(s, str):
        return s.split('/')[0].split(',')[0].strip()
    return str(s)


def load_data(path):
    df = pd.read_csv(path, encoding='utf_8_sig')
    # strategy normalization: try existing 'strategy_norm' or derive from 'interaction_strategy'
    if 'strategy_norm' in df.columns:
        df['strategy_norm'] = df['strategy_norm'].fillna('').astype(str)
    else:
        df['strategy_norm'] = df['interaction_strategy'].fillna('').astype(str).map(normalize_strategy)

    # Ensure behavior proportion columns exist
    behaviors = ['请求', '陈述', '确认', '拒绝', '其他']
    for b in behaviors:
        col = f'prop_{b}'
        if col not in df.columns:
            df[col] = 0.0

    # target
    df['is_fraud_bin'] = df['is_fraud'].astype(str).str.lower().isin(['true', '1', 't', 'y']).astype(int)
    return df


def build_features(df):
    behaviors = ['请求', '陈述', '确认', '拒绝', '其他']
    feat_cols = [f'prop_{b}' for b in behaviors]

    # strategy one-hot
    strat_dummies = pd.get_dummies(df['strategy_norm'].fillna(''), prefix='strat')

    X_base = df[feat_cols].fillna(0.0).reset_index(drop=True)
    strat_dummies = strat_dummies.reset_index(drop=True)

    # interaction features: prop_b * strat_onehot
    interactions = []
    for b in feat_cols:
        for s in strat_dummies.columns:
            colname = f'{b}_x_{s}'
            interactions.append(colname)
            X_base[colname] = X_base[b] * strat_dummies[s]

    X = pd.concat([X_base, strat_dummies], axis=1)
    feature_names = list(X.columns)
    return X.values, feature_names


def train_and_explain(X, y, feature_names):
    # stratified split
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42, stratify=y)

    clf = LogisticRegression(max_iter=2000, class_weight='balanced', solver='liblinear')
    clf.fit(X_train, y_train)

    y_prob = clf.predict_proba(X_test)[:, 1]
    y_pred = clf.predict(X_test)

    metrics = {
        'roc_auc': roc_auc_score(y_test, y_prob),
        'accuracy': accuracy_score(y_test, y_pred),
        'precision': precision_score(y_test, y_pred, zero_division=0),
        'recall': recall_score(y_test, y_pred, zero_division=0),
        'n_train_pos': int(y_train.sum()),
        'n_train_neg': int((1 - y_train).sum()),
    }

    coefs = clf.coef_[0]
    coef_df = pd.DataFrame({'feature': feature_names, 'coef': coefs})
    coef_df['abs_coef'] = coef_df['coef'].abs()
    coef_df = coef_df.sort_values(by='coef', ascending=False)
    return clf, metrics, coef_df


def main():
    cfg = Config()
    print('加载数据：', cfg.input_file)
    df = load_data(cfg.input_file)
    X, feature_names = build_features(df)
    y = df['is_fraud_bin'].values

    clf, metrics, coef_df = train_and_explain(X, y, feature_names)

    # 保存结果
    coef_df.to_csv(cfg.output_feat, index=False, encoding='utf_8_sig')
    pd.DataFrame([metrics]).to_csv(cfg.output_metrics, index=False, encoding='utf_8_sig')

    print('模型评估：', metrics)
    print('已保存特征系数到', cfg.output_feat)
    print('已保存模型评估到', cfg.output_metrics)


if __name__ == '__main__':
    main()
