import pandas as pd
import numpy as np
import torch
import torch.nn.functional as F
from torch_geometric.nn import GCNConv
from sentence_transformers import SentenceTransformer
from sklearn.metrics.pairwise import cosine_similarity
from openai import OpenAI
import json
import re
import matplotlib.pyplot as plt
import warnings
warnings.simplefilter(action='ignore', category=FutureWarning)

# --- 配置与超参数 ---
API_KEY = "sk-33d8b25535bf4daa9492bb28e4db2269"
BASE_URL = "https://dashscope.aliyuncs.com/compatible-mode/v1"
MODEL_NAME = "qwen-max"
SIM_THRESHOLD = 0.95
MASK_RATIO = 0.5
CONFIDENCE_LIMIT = 0.8
HIDDEN_DIM = 64
EPOCHS = 100

client = OpenAI(api_key=API_KEY, base_url=BASE_URL)


def get_llm_pseudo_label(content):
    """调用 LLM 获取伪标签"""
    prompt = f"""你是一个专业电信诈骗评估专家。请分析以下对话内容是否为诈骗。
    要求：1. 如果确定是诈骗，结果填1，否则填0。2. 给出置信度(0.0-1.0)。3. 严格JSON格式：{{"result": 0或1, "confidence": 分数}}
    内容：{content}"""
    try:
        completion = client.chat.completions.create(model=MODEL_NAME, messages=[{"role": "user", "content": prompt}],
                                                    timeout=10)
        res_text = completion.choices[0].message.content
        match = re.search(r'\{[\s\S]*?\}', res_text)
        if match:
            data = json.loads(match.group())
            return data['result'], data['confidence']
        return -1, 0.0
    except:
        return -1, 0.0


# 1. 数据加载与合并
print("正在读取训练集与测试集...")
df_train = pd.read_csv('通话数据互动策略结果/训练集结果.csv', encoding='utf-8-sig')
df_test = pd.read_csv('通话数据互动策略结果/测试集结果.csv', encoding='utf-8-sig')

df_train['split'] = 'train'
df_test['split'] = 'test'
df = pd.concat([df_train, df_test], ignore_index=True)

# 统一真实标签 (is_fraud 列)
df['true_label'] = df['is_fraud'].map({True: 1, False: 0, 'True': 1, 'False': 0, 1: 1, 0: 0}).fillna(-1).astype(int)

# 2. LLM 混合标注 (Mixed-Initiative Labeling)
# 仅针对训练集中标注缺失的数据进行 LLM 补全
print("正在执行 LLM 辅助标注...")
df['label_integrated'] = df['true_label']
pseudo_count = 0
for i, row in df.iterrows():
    if row['split'] == 'train' and row['true_label'] == -1:
        res, conf = get_llm_pseudo_label(row['specific_dialogue_content'])
        if conf >= CONFIDENCE_LIMIT:
            df.at[i, 'label_integrated'] = res
            pseudo_count += 1
print(f"LLM 成功生成高置信度伪标签数量: {pseudo_count}")

# 3. 特征提取 (Text Embedding)
print("正在提取文本 Embedding 特征...")
embed_model = SentenceTransformer('paraphrase-multilingual-MiniLM-L12-v2')
embeddings = embed_model.encode(df['specific_dialogue_content'].tolist())
x_content = torch.tensor(embeddings, dtype=torch.float)

# 4. 构建图结构 (转导学习模式)
print("正在构建全局相似度图...")
sim_matrix = cosine_similarity(embeddings)
edge_indices = np.argwhere(sim_matrix > SIM_THRESHOLD)
edge_indices = edge_indices[edge_indices[:, 0] != edge_indices[:, 1]]
edge_index = torch.tensor(edge_indices.T, dtype=torch.long)


# 5. 准备集成标签特征 (y')
def create_label_features(df):
    num_nodes = len(df)
    label_feat = torch.zeros((num_nodes, 2))
    for i, row in df.iterrows():
        # 只有训练集的标签（真实或高置信度伪标签）能作为特征输入
        # 测试集在输入端必须为 0，防止泄露
        if row['split'] == 'train' and row['label_integrated'] != -1:
            label_feat[i, int(row['label_integrated'])] = 1.0
    return label_feat


label_features = create_label_features(df)


# 6. GLPN 模型定义
class GLPN(torch.nn.Module):
    def __init__(self, in_channels, hidden_channels, out_channels):
        super(GLPN, self).__init__()
        self.conv1 = GCNConv(in_channels, hidden_channels)
        self.conv2 = GCNConv(hidden_channels, out_channels)

    def forward(self, x, edge_index, label_feat, mask_ratio=0.5, training=True):
        curr_feat = label_feat.clone()
        if training:
            # Global Random Mask (GRM) 策略
            mask = torch.rand(curr_feat.size(0)) < mask_ratio
            curr_feat[mask] = 0.0

        # 拼接原始特征和标签特征
        x_combined = torch.cat([x, curr_feat], dim=-1)
        x = F.relu(self.conv1(x_combined, edge_index))
        x = F.dropout(x, p=0.5, training=training)
        return F.log_softmax(self.conv2(x, edge_index), dim=1)


# 7. 训练循环
# train_mask 仅包含训练集中有标签的样本
train_mask = torch.tensor((df['split'] == 'train').values & (df['true_label'] != -1).values)
y_true = torch.tensor(df['true_label'].values, dtype=torch.long)

model = GLPN(in_channels=x_content.shape[1] + 2, hidden_channels=HIDDEN_DIM, out_channels=2)
optimizer = torch.optim.Adam(model.parameters(), lr=0.01, weight_decay=5e-4)

loss_history = []  # 记录 Loss

print("开始训练...")
model.train()
for epoch in range(EPOCHS):
    optimizer.zero_grad()
    out = model(x_content, edge_index, label_features, mask_ratio=MASK_RATIO, training=True)
    loss = F.nll_loss(out[train_mask], y_true[train_mask])
    loss.backward()
    optimizer.step()

    loss_val = loss.item()
    loss_history.append(loss_val)
    if epoch % 10 == 0:
        print(f"Epoch {epoch:03d}, Loss: {loss_val:.4f}")

# 绘制并保存 Loss 曲线
plt.figure(figsize=(10, 6))
plt.plot(range(len(loss_history)), loss_history, label='Training Loss', color='darkorange', linewidth=2)
plt.title('GLPN Training Loss')
plt.xlabel('Epochs')
plt.ylabel('Loss')
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend()
plt.savefig('glpn_loss_curve.png')
print("Loss 变化图已保存至 'glpn_loss_curve.png'")
plt.show()

# 8. 执行预测并提取测试集结果
model.eval()
with torch.no_grad():
    # 推理时 training=False，不再进行随机遮掩
    logits = model(x_content, edge_index, label_features, training=False)
    preds = logits.argmax(dim=1)

df['predicted_label'] = preds.numpy()
test_results = df[df['split'] == 'test'].copy()
test_results.to_csv('测试集预测结果.csv', index=False, encoding='utf-8-sig')
print("测试集预测完成！结果已存入 '测试集预测结果.csv'")