import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from abc import ABC, abstractmethod
from sklearn.ensemble import RandomForestRegressor
from sklearn.model_selection import train_test_split
from sklearn.metrics import r2_score, mean_squared_error, mean_absolute_error
import warnings

warnings.filterwarnings('ignore') # 忽略部分 pandas 的警告

# ==========================================
# 全局配置
# ==========================================
plt.rcParams["font.sans-serif"] = ["SimHei", "Arial Unicode MS"] # 支持中文显示
plt.rcParams["axes.unicode_minus"] = False

# 标准化的8个工艺参数名称
PROCESS_FEATURES = [
    '缆芯外径', '护套外径', '挤出内模', '挤出外模', 
    '螺杆速度', '螺杆电流', '生产速度', '实际生产速度'
]

# ==========================================
# 1. 结构型模式：数据读取适配器
# ==========================================
class RealDataExcelAdapter:
    
    def __init__(self, data_dir='/workspace/', max_rows=1000):
        self.data_dir = data_dir
        self.max_rows = max_rows 

    def _map_process_feature(self, name):
        name = str(name).strip()
        if '内护套' in name: return None  
        if '缆芯外径' in name: return '缆芯外径'
        if '护套外径' in name: return '护套外径'
        if '挤出内模' in name: return '挤出内模'
        if '挤出外模' in name: return '挤出外模'
        if '螺杆速度' in name and '挤塑' in name: return '螺杆速度' 
        if '螺杆电流' in name: return '螺杆电流'
        if '实际生产速度' in name: return '实际生产速度'
        if '生产速度' in name: return '生产速度'
        return None

    def load_process_data(self) -> pd.DataFrame:
        all_files = os.listdir(self.data_dir)
        files = [os.path.join(self.data_dir, f) for f in all_files 
                 if 'HT_工序' in f and f.endswith('.xlsx') and not f.startswith('~')]
        
        if not files:
            raise FileNotFoundError(f"❌ 目录检索失败：在 [{self.data_dir}] 目录下没找到工序表！")
            
        df_list = []
        for f in files:
            print(f"  -> 正在读取工序表 (前{self.max_rows}行): {os.path.basename(f)}")
            df = pd.read_excel(f, usecols=['批号', '项目名称', '项目记录结果'], nrows=self.max_rows)
            df['标准特征名'] = df['项目名称'].apply(self._map_process_feature)
            df = df.dropna(subset=['标准特征名'])
            df['数值结果'] = pd.to_numeric(df['项目记录结果'], errors='coerce')
            pivoted = pd.pivot_table(df, index='批号', columns='标准特征名', values='数值结果', aggfunc='mean')
            df_list.append(pivoted)
            
        return pd.concat(df_list).reset_index()

    def load_quality_data(self) -> pd.DataFrame:
        all_files = os.listdir(self.data_dir)
        files = [os.path.join(self.data_dir, f) for f in all_files 
                 if '品质检验' in f and f.endswith('.xlsx') and not f.startswith('~')]
        
        if not files:
            raise FileNotFoundError(f"❌ 目录检索失败：在 [{self.data_dir}] 目录下没找到品质表！")
            
        df_list = []
        for f in files:
            print(f"  -> 正在读取品检表 (前{self.max_rows}行): {os.path.basename(f)}")
            df = pd.read_excel(f, usecols=['批号', '检查项目', '检验值'], nrows=self.max_rows)
            df['数值检验值'] = pd.to_numeric(df['检验值'], errors='coerce')
            df = df.dropna(subset=['数值检验值'])
            
            # 【核心修复1】给检验项目统一加上 [检验] 前缀，防止与工序表重名！
            df['检查项目'] = df['检查项目'].apply(lambda x: f"[检验]{str(x).strip()}")
            
            pivoted = pd.pivot_table(df, index='批号', columns='检查项目', values='数值检验值', aggfunc='mean')
            df_list.append(pivoted)
            
        return pd.concat(df_list).reset_index()

# ==========================================
# 2. 行为型模式：策略模式 
# ==========================================
class IRegressionStrategy(ABC):
    @abstractmethod
    def train_and_predict(self, X_train, X_test, y_train, y_test) -> dict:
        pass

class RandomForestStrategy(IRegressionStrategy):
    def __init__(self, n_estimators=100, random_state=42):
        self.model = RandomForestRegressor(n_estimators=n_estimators, random_state=random_state, n_jobs=-1)
        
    def train_and_predict(self, X_train, X_test, y_train, y_test) -> dict:
        self.model.fit(X_train, y_train)
        y_pred = self.model.predict(X_test)
        
        return {
            "R2_Score": r2_score(y_test, y_pred),
            "MSE": mean_squared_error(y_test, y_pred),
            "MAE": mean_absolute_error(y_test, y_pred)
        }

# ==========================================
# 3. 行为型模式：模板方法模式 (流水线 Pipeline)
# ==========================================
class DataAnalysisPipeline(ABC):
    
    def __init__(self, ml_strategy: IRegressionStrategy):
        self.ml_strategy = ml_strategy
        self.merged_data = None
        self.correlation_targets = [] # 需求2.1：存储 10 个质量指标
        self.regression_targets = []  # 需求2.2：存储 5 个质量指标
        
    def run_pipeline(self):
        print("="*80)
        print("🚀 启动光缆质量数据挖掘管道... (严格匹配实验报告要求)")
        print("="*80)
        
        process_df, quality_df = self.load_data()
        success = self.merge_and_clean_data(process_df, quality_df)
        
        if success:
            self.analyze_correlation()
            self.train_models()
        
        print("\n✅ 所有流水线任务执行完毕。")

    @abstractmethod
    def load_data(self) -> tuple:
        pass

    def merge_and_clean_data(self, process_df: pd.DataFrame, quality_df: pd.DataFrame) -> bool:
        print("\n[2/4] 正在根据 [批号] 对齐数据，并筛选定量特征...")
        merged = pd.merge(process_df, quality_df, on="批号", how="inner")
        
        if len(merged) == 0:
            print("\n⚠️ 错误：未找到批号交集，请调大适配器中的 max_rows！")
            return False
            
        merged.fillna(merged.mean(numeric_only=True), inplace=True)
        
        # 找出所有品质检验列
        quality_cols = [c for c in quality_df.columns if c != '批号']
        
        # 【高级过滤机制】：剔除方差为0的无意义列，彻底消灭皮尔逊系数中的 NaN !
        valid_cols = []
        for col in quality_cols:
            if merged[col].nunique() > 1: # 只要数据有起伏(不同值超过1个)，方差就不为0
                valid_cols.append(col)
                
        # 统计有效样本量，进行降序排列
        target_counts = merged[valid_cols].count().sort_values(ascending=False)
        
        # ✅ 需求 2.1：选出前 10 个生成 8x10 相关性表
        self.correlation_targets = target_counts.head(10).index.tolist()
        
        # ✅ 需求 2.2：在 10 个中选出最具代表性（数据最多）的前 5 个建立回归器
        self.regression_targets = self.correlation_targets[:5]
        
        print(f"  -> 成功对齐样本: {len(merged)} 条")
        print(f"  -> [需求2.1] 提取 Top 10 质量指标用于关联分析:\n     {self.correlation_targets}")
        print(f"  -> [需求2.2] 提取 Top 5 质量指标用于机器学习:\n     {self.regression_targets}")
        
        self.merged_data = merged
        return True

    def analyze_correlation(self):
        print(f"\n[3/4] 需求 2.1: 正在生成 {len(PROCESS_FEATURES)}x{len(self.correlation_targets)} 关联分析表 (Pearson Correlation)...")
        
        exist_features = [c for c in PROCESS_FEATURES if c in self.merged_data.columns]
        exist_targets = [c for c in self.correlation_targets if c in self.merged_data.columns]
        
        df_sub = self.merged_data[exist_features + exist_targets]
        df_sub = df_sub.apply(pd.to_numeric, errors='coerce')
        corr_matrix = df_sub.corr(method='pearson')
        
        # 切割出 8x10 的矩阵
        target_corr = corr_matrix.loc[exist_features, exist_targets]
        
        print("\n" + "=" * 80)
        print(f"📊 8x{len(exist_targets)} 关联分析表输出如下：")
        print("=" * 80)
        print(target_corr.round(3).to_string())
        
        # ✅ 贴心功能：自动导出 Excel，实验报告直接 Copy 进去即可
        export_path = "/workspace/8x10_关联分析表.xlsx"
        target_corr.to_excel(export_path)
        print(f"\n💡 关联分析表已成功导出至 Excel: '{export_path}' (可直接粘贴至报告)")
        
        # 绘制热力图 (图片加宽以容纳10个列)
        plt.figure(figsize=(14, 6))
        sns.heatmap(target_corr, annot=True, cmap="coolwarm", fmt=".2f", center=0)
        plt.title("光缆工艺参数 vs 检验指标 (Top10) Pearson 关联分析")
        plt.tight_layout()
        plt.savefig("/workspace/Correlation_8x10_Matrix.png")
        plt.show()

    def train_models(self):
        print("\n[4/4] 需求 2.2: 正在训练 5 个回归预测器 (Random Forest)...")
        exist_features = [c for c in PROCESS_FEATURES if c in self.merged_data.columns]
        X = self.merged_data[exist_features]
        
        # 仅对 Top 5 进行预测建模
        for target in self.regression_targets:
            if target not in self.merged_data.columns:
                continue
                
            valid_idx = self.merged_data[target].notna()
            X_valid = X[valid_idx]
            y_valid = self.merged_data.loc[valid_idx, target]
            
            if len(X_valid) < 5:
                continue
                
            X_train, X_test, y_train, y_test = train_test_split(X_valid, y_valid, test_size=0.2, random_state=42)
            
            try:
                metrics = self.ml_strategy.train_and_predict(X_train, X_test, y_train, y_test)
                print(f"🎯 预测目标: {target:<18} | "
                      f"有效样本: {len(X_valid):<4} | "
                      f"R²(决定系数): {metrics['R2_Score']:>7.4f} | "
                      f"MSE: {metrics['MSE']:>7.4f}")
            except Exception as e:
                print(f"⚠️ 目标 [{target}] 训练失败: {e}")

# ==========================================
# 4. 具体的流水线实现：对接真实 Excel
# ==========================================
class OpticalCableRealDataPipeline(DataAnalysisPipeline):
    
    def load_data(self) -> tuple:
        print("\n[1/4] 开始扫描并加载 /workspace/ 目录下的 Excel 文件...")
        # 建议保持 5000 行，以确保在剔除方差为 0 的列后，依然能凑齐 10 个有效质量指标
        adapter = RealDataExcelAdapter(data_dir="/workspace/", max_rows=5000) 
        process_df = adapter.load_process_data()
        quality_df = adapter.load_quality_data()
        return process_df, quality_df

# ==========================================
# 5. 主程序启动
# ==========================================
if __name__ == "__main__":
    try:
        rf_strategy = RandomForestStrategy(n_estimators=100)
        pipeline = OpticalCableRealDataPipeline(ml_strategy=rf_strategy)
        pipeline.run_pipeline()
    except Exception as e:
        print(f"\n❌ 程序执行发生异常: {e}")