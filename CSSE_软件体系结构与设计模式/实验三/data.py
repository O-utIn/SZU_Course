import pandas as pd
import re
import numpy as np
from typing import List, Dict, Optional, Tuple
from abc import ABC, abstractmethod
import matplotlib.pyplot as plt

# 全局配置
plt.rcParams["font.sans-serif"] = ["SimHei"]
plt.rcParams["axes.unicode_minus"] = False

# ===================== 结构型设计模式：适配器模式（统一数据读取） =====================
class DataReader(ABC):
    """统一数据读取接口（目标接口）"""
    @abstractmethod
    def read_data(self, file_path: str) -> pd.DataFrame:
        pass

class ExcelDataReader:
    """适配者：Excel原始读取类"""
    def read_excel(self, file_path: str) -> pd.DataFrame:
        return pd.read_excel(file_path, sheet_name=0)

class ExcelAdapter(DataReader):
    """适配器：统一Excel读取格式"""
    def __init__(self):
        self.adaptee = ExcelDataReader()

    def read_data(self, file_path: str) -> pd.DataFrame:
        df = self.adaptee.read_excel(file_path)
        df.columns = [col.strip() for col in df.columns]
        return df

# ===================== 结构型设计模式：组合模式（参数层级管理） =====================
class ParamComponent(ABC):
    """抽象组件：所有参数的统一接口"""
    @abstractmethod
    def get_info(self) -> Dict:
        pass

class QuantitativeParam(ParamComponent):
    """叶子节点：定量参数（0.2≤V≤0.5）"""
    def __init__(self, name: str, lower: float, upper: float, actual: float):
        self.name = name
        self.lower = lower
        self.upper = upper
        self.actual = actual
        self.is_ok = self._check_ok()

    def _check_ok(self) -> bool:
        if self.lower is not None and self.actual < self.lower:
            return False
        if self.upper is not None and self.actual > self.upper:
            return False
        return True

    def get_info(self) -> Dict:
        return {
            "类型": "定量",
            "检查项目": self.name,
            "下界": self.lower,
            "上界": self.upper,
            "实际值": self.actual,
            "isOK": self.is_ok
        }

class QualitativeParam(ParamComponent):
    """叶子节点：定性参数（文字描述）"""
    def __init__(self, name: str, standard: str, actual: str):
        self.name = name
        self.standard = standard
        self.actual = actual
        self.is_ok = self._check_ok()

    def _check_ok(self) -> bool:
        return str(self.actual).strip().upper() == "OK"

    def get_info(self) -> Dict:
        return {
            "类型": "定性",
            "检查项目": self.name,
            "标准描述": self.standard,
            "检验结果": self.actual,
            "isOK": self.is_ok
        }

class BatchComposite(ParamComponent):
    """组合节点：批次（包含多个参数）"""
    def __init__(self, batch_no: str, material_code: str):
        self.batch_no = batch_no
        self.material_code = material_code
        self.children: List[ParamComponent] = []

    def add(self, component: ParamComponent):
        self.children.append(component)

    def get_info(self) -> Dict:
        params_info = [child.get_info() for child in self.children]
        # 统计定量
        quan_list = [p for p in params_info if p["类型"] == "定量"]
        quan_ok = sum(1 for p in quan_list if p["isOK"])
        quan_total = len(quan_list)
        # 统计定性
        qual_list = [p for p in params_info if p["类型"] == "定性"]
        qual_ok = sum(1 for p in qual_list if p["isOK"])
        qual_total = len(qual_list)

        return {
            "批号": self.batch_no,
            "物料品号": self.material_code,
            "参数详情": params_info,
            "定量合格数": quan_ok,
            "定量总数": quan_total,
            "定量合格率": round(quan_ok / quan_total, 4) if quan_total > 0 else 0,
            "定性合格数": qual_ok,
            "定性总数": qual_total,
            "定性合格率": round(qual_ok / qual_total, 4) if qual_total > 0 else 0,
            "最终isOK": quan_ok == quan_total and qual_ok == qual_total
        }

# ===================== 结构型设计模式：装饰器模式（动态扩展统计功能） =====================
class StatDecorator(ParamComponent, ABC):
    """装饰器抽象类"""
    def __init__(self, component: ParamComponent):
        self.component = component

class ScoreDecorator(StatDecorator):
    """评分装饰器"""
    def get_info(self) -> Dict:
        info = self.component.get_info()
        info["综合评分"] = info["定量合格率"] * 0.6 + info["定性合格率"] * 0.4
        return info

class PurityDecorator(StatDecorator):
    """参数纯度装饰器"""
    def get_info(self) -> Dict:
        info = self.component.get_info()
        params = info["参数详情"]
        valid = sum(1 for p in params if p["isOK"])
        info["参数纯度"] = round(valid / len(params), 4) if params else 0
        return info

# ===================== 核心业务类 =====================
class QualityDataProcessor:
    def __init__(self, file_paths: List[str], limit_rows: int = 2000):
        self.reader = ExcelAdapter()
        self.limit_rows = limit_rows  # 限制读取行数
        self.df_list = []
        for fp in file_paths:
            df = self.reader.read_data(fp)
            df = df.head(self.limit_rows)  # 只取前2000条
            self.df_list.append(df)
        self.df = pd.concat(self.df_list, ignore_index=True)
        self.batch_data: Dict[str, ParamComponent] = {}

    def extract_quantitative_bound(self, standard: str) -> Optional[Tuple[float, float]]:
        """正则提取定量参数上下界"""
        if pd.isna(standard):
            return None
        pattern = r"([\d\.]+)?\s*≤?\s*V\s*≤?\s*([\d\.]+)?"
        match = re.search(pattern, str(standard).strip())
        if not match:
            return None
        lower = float(match.group(1)) if match.group(1) else None
        upper = float(match.group(2)) if match.group(2) else None
        return lower, upper

    def parse_batch_data(self):
        """解析所有批次：分离定量/定性参数"""
        batches = self.df["批号"].dropna().unique()
        for batch in batches:
            batch_df = self.df[self.df["批号"] == batch]
            material = batch_df["物料品号"].iloc[0] if "物料品号" in batch_df.columns else "未知"
            batch_node = BatchComposite(batch, material)

            for _, row in batch_df.iterrows():
                item = row.get("检查项目", "未知项目")
                standard = row.get("标准参数", "")
                actual_val = row.get("检验值", None)

                bound = self.extract_quantitative_bound(standard)
                if bound and not pd.isna(actual_val):
                    lower, upper = bound
                    param = QuantitativeParam(item, lower, upper, float(actual_val))
                else:
                    param = QualitativeParam(item, str(standard), str(actual_val))
                batch_node.add(param)

            # 双重装饰：纯度 + 评分
            decorated = ScoreDecorator(PurityDecorator(batch_node))
            self.batch_data[batch] = decorated

    def check_isok_error(self) -> pd.DataFrame:
        """任务2：检查原始IsOK错误的批次"""
        errors = []
        for batch, node in self.batch_data.items():
            info = node.get_info()
            correct_isok = info["最终isOK"]
            original_isok = self.df[self.df["批号"] == batch]["IsOK"].iloc[0]
            if str(original_isok).strip() != str(correct_isok):
                errors.append({
                    "批号": batch,
                    "物料品号": info["物料品号"],
                    "原始IsOK": original_isok,
                    "正确IsOK": correct_isok
                })
        return pd.DataFrame(errors)

    def build_quality_table(self) -> pd.DataFrame:
        """任务3：构建标准质检表"""
        table = []
        for node in self.batch_data.values():
            info = node.get_info()
            table.append({
                "批号": info["批号"],
                "物料品号": info["物料品号"],
                "定量合格率": info["定量合格率"],
                "定性合格率": info["定性合格率"],
                "参数纯度": info["参数纯度"],
                "综合评分": info["综合评分"],
                "正确IsOK": info["最终isOK"]
            })
        return pd.DataFrame(table)

    def get_top_n_material(self, n: int = 5) -> pd.DataFrame:
        """任务4：获取TopN优质物料向量"""
        qt = self.build_quality_table()
        # 排序规则：综合评分 > 定量合格率 > 定性合格率
        qt = qt.sort_values(by=["综合评分", "定量合格率", "定性合格率"], ascending=False)
        qt = qt.drop_duplicates("物料品号").head(n)
        return qt

# ===================== 主程序 =====================
if __name__ == "__main__":
    # 1. 配置文件路径
    FILES = [
        "品质检验(2024年06月).xlsx",
        "品质检验(2024年07月).xlsx",
        "品质检验(2024年08月).xlsx"
    ]

    print("=" * 50)
    print("实验三：光缆品质检验数据分析系统（仅读取前2000条）")
    print("=" * 50)

    # 2. 初始化，限制读取2000条
    processor = QualityDataProcessor(FILES, limit_rows=2000)
    processor.parse_batch_data()

    # 3. 任务2：输出IsOK错误批次
    print("\n【任务2】IsOK字段错误批次：")
    error_df = processor.check_isok_error()
    if error_df.empty:
        print("✅ 无错误批次")
    else:
        print(error_df.to_string(index=False))
        error_df.to_excel("IsOK错误批次表.xlsx", index=False)

    # 4. 任务3：输出质检表
    print("\n【任务3】质检总表（前10条）：")
    quality_table = processor.build_quality_table()
    print(quality_table.head(10).to_string(index=False))
    quality_table.to_excel("质检参数总表.xlsx", index=False)

    # 5. 任务4：输出TopN物料
    print("\n【任务4】Top5优质物料参数向量：")
    top_df = processor.get_top_n_material(5)
    print(top_df.to_string(index=False))
    top_df.to_excel("Top5优质物料表.xlsx", index=False)

    # 6. 可视化
    plt.figure(figsize=(12, 5))
    plt.subplot(1, 2, 1)
    plt.bar(top_df["物料品号"].astype(str), top_df["综合评分"], color="#4285F4")
    plt.title("Top5物料综合评分")
    plt.xticks(rotation=30)

    plt.subplot(1, 2, 2)
    x = np.arange(len(top_df))
    plt.bar(x - 0.2, top_df["定量合格率"], 0.4, label="定量合格率")
    plt.bar(x + 0.2, top_df["定性合格率"], 0.4, label="定性合格率")
    plt.title("定量/定性合格率对比")
    plt.xticks(x, top_df["物料品号"].astype(str), rotation=30)
    plt.legend()
    plt.tight_layout()
    plt.show()

    print("\n✅ 实验三所有任务执行完成！")