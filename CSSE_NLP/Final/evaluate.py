import pandas as pd
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, classification_report


def run_evaluation():
    df = pd.read_csv('测试集预测结果.csv', encoding='utf-8-sig')

    # 确保 true_label 和 predicted_label 都是整数
    # 过滤掉测试集中可能存在的缺失真实标签的行
    df = df.dropna(subset=['true_label'])
    df = df[df['true_label'] != -1]

    y_true = df['true_label'].astype(int)
    y_pred = df['predicted_label'].astype(int)

    print("===== 独立测试集评估结果 =====")
    print(f"测试样本量: {len(df)}")
    print(f"准确率 (Accuracy):  {accuracy_score(y_true, y_pred):.4f}")
    print(f"精确率 (Precision): {precision_score(y_true, y_pred):.4f}")
    print(f"召回率 (Recall):    {recall_score(y_true, y_pred):.4f}")
    print(f"F1 分数 (F1-Score):  {f1_score(y_true, y_pred):.4f}")
    print("\n详细报告:")
    print(classification_report(y_true, y_pred, target_names=['正常', '诈骗']))


if __name__ == "__main__":
    run_evaluation()