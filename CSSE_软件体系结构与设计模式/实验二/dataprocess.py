import os
import re
from abc import ABC, abstractmethod
from dataclasses import dataclass, field
from typing import List, Dict, Tuple, Any
from collections import defaultdict
import openpyxl

# ==========================================
# 1. 领域实体层 (Entity/Domain Model) - 满足需求 1, 7
# 采用强类型封装，拒绝"字典满天飞"，解决缺乏领域模型的缺陷
# ==========================================

@dataclass
class ProcessRecord:
    """工序记录实体：封装单行数据的强类型属性"""
    batch_no: str            # 批号
    material_code: str       # 物料品号 (新增)
    creation_date: str       # 创建日期 (新增)
    core_diameter: float     # 缆芯外径
    sheath_diameter: float   # 护套外径
    inner_mold: float        # 挤出内模
    outer_mold: float        # 挤出外模
    screw_speed: float       # 螺杆速度
    screw_current: float     # 螺杆电流
    production_speed: float  # 生产速度 (新增记录依据)
    actual_speed: float      # 实际生产速度
    device_name: str         # 设备名称 (新增)
    remarks: str             # 备注信息 (新增)
    
    # 衍生属性
    operator_name: str = ""  # 操机手 (由备注解析得出)
    is_valid: bool = True    # 数据是否有效标志
    invalid_reason: str = "" # 无效原因

    @property
    def record_key(self) -> Tuple[str, str, str]:
        """需求2：联合主键 <批号、物料品号、创建日期>"""
        return (str(self.batch_no), str(self.material_code), str(self.creation_date))

    @property
    def parameter_vector(self) -> Tuple:
        """需求3：工艺参数向量定义"""
        return (self.core_diameter, self.sheath_diameter, self.inner_mold, 
                self.outer_mold, self.screw_speed, self.screw_current, 
                self.actual_speed, self.device_name)

@dataclass
class QualityInspectionRecord:
    """品质检验记录实体：为需求7预留的扩展"""
    material_code: str
    quality_score: float
    # 其他品检属性...


# ==========================================
# 2. 校验规则引擎 (Strategy Pattern) - 满足需求 2
# 满足开闭原则 (OCP)，新增规则无需修改主流程
# ==========================================

class IValidationRule(ABC):
    @abstractmethod
    def validate(self, record: ProcessRecord) -> Tuple[bool, str]:
        pass

class NotNullRule(IValidationRule):
    """校验10个核心字段是否为空"""
    def validate(self, record: ProcessRecord) -> Tuple[bool, str]:
        check_fields = [
            record.core_diameter, record.sheath_diameter, record.inner_mold,
            record.outer_mold, record.screw_speed, record.screw_current,
            record.production_speed, record.actual_speed, record.creation_date, record.device_name
        ]
        if any(v is None or str(v).strip() == "" or v == 'N/A' for v in check_fields):
            return False, "核心关键字段存在空值"
        return True, ""

class SizeLogicRule(IValidationRule):
    """校验外径和内模大小逻辑"""
    def validate(self, record: ProcessRecord) -> Tuple[bool, str]:
        try:
            if float(record.core_diameter) >= float(record.sheath_diameter):
                return False, "逻辑错误: 缆芯外径 >= 护套外径"
            if float(record.inner_mold) >= float(record.outer_mold):
                return False, "逻辑错误: 挤出内模 >= 挤出外模"
        except (ValueError, TypeError):
            return False, "数值转换异常，无法比较大小"
        return True, ""

class ValidationEngine:
    """规则引擎：驱动所有注册的规则运行"""
    def __init__(self):
        self.rules: List[IValidationRule] = []

    def add_rule(self, rule: IValidationRule):
        self.rules.append(rule)

    def process(self, record: ProcessRecord) -> ProcessRecord:
        for rule in self.rules:
            is_pass, reason = rule.validate(record)
            if not is_pass:
                record.is_valid = False
                record.invalid_reason = reason
                break
        return record


# ==========================================
# 3. 数据访问与解析层 (DIP, SRP) - 满足需求 1, 6, 7
# 隔离第三方库，具备极强的容错能力
# ==========================================

class RemarkParserUtil:
    """工具类：隔离解析逻辑"""
    @staticmethod
    def extract_operator(remark: str) -> str:
        """需求6：从备注中提取操机手 (如: '主机手杜伟' -> '杜伟')"""
        if not remark:
            return "未知操作手"
        match = re.search(r'主机手([\w\u4e00-\u9fa5]+)', str(remark))
        return match.group(1).strip() if match else "未知操作手"

class IDataLoader(ABC):
    """数据加载接口：为需求7品检数据导入奠定抽象基础"""
    @abstractmethod
    def load_data(self, file_path: str) -> List[Any]:
        pass

class ProcessRecordExcelLoader(IDataLoader):
    """具体实现：针对新格式的Excel读取"""
    def load_data(self, file_path: str) -> List[ProcessRecord]:
        # 实际业务中这里将利用 openpyxl 按行列循环解析
        # 此处仅为演示体系结构，由于没有真实的Excel附件，此处用硬编码模拟解析结果
        print(f"[I/O 操作] 正在从新客户格式解析 Excel 文件: {file_path}")
        
        # 模拟从 Excel 读出的数据行（包含脏数据测试用例）
        mock_data = [
            # 正常记录
            ProcessRecord("B001", "M-A", "2026-04-01", 10.0, 15.0, 5.0, 8.0, 120, 45, 50, 48, "挤塑机A", "返工 主机手杜伟 跟班黄刚超"),
            ProcessRecord("B002", "M-A", "2026-04-01", 10.0, 15.0, 5.0, 8.0, 120, 45, 50, 48, "挤塑机A", "早班 主机手张三"),
            ProcessRecord("B003", "M-B", "2026-04-02", 12.0, 18.0, 6.0, 9.0, 130, 50, 55, 52, "挤塑机B", "主机手杜伟"),
            # 需求2测试：缆芯 > 护套 (逻辑错误)
            ProcessRecord("B004", "M-C", "2026-04-02", 20.0, 15.0, 5.0, 8.0, 120, 45, 50, 48, "挤塑机A", "主机手李四"),
            # 需求2测试：存在空值 (缺失实际生产速度)
            ProcessRecord("B005", "M-A", "2026-04-03", 10.0, 15.0, 5.0, 8.0, 120, 45, 50, None, "挤塑机A", "主机手杜伟")
        ]
        
        # 数据加载时进行基础预处理
        for record in mock_data:
            record.operator_name = RemarkParserUtil.extract_operator(record.remarks)
            
        return mock_data

class QualityInspectionExcelLoader(IDataLoader):
    """需求7预留：新增品质检验读取器，与工序读取器互不干扰"""
    def load_data(self, file_path: str) -> List[QualityInspectionRecord]:
        print(f"[I/O 操作] 正在解析品质检验文件: {file_path}")
        return []


# ==========================================
# 4. 统计分析服务层 (Generic/Template Pattern) - 满足需求 3, 4, 5, 6
# 将算法泛型化，实现纯度逻辑的高效复用
# ==========================================

class StatisticsService:
    @staticmethod
    def count_batches_per_material(records: List[ProcessRecord]) -> Dict[str, set]:
        """需求4：统计每个物料品号有多少次生产（不同的批号）"""
        stats = defaultdict(set)
        for r in records:
            stats[r.material_code].add(r.batch_no)
        return {k: len(v) for k, v in stats.items()}

    @staticmethod
    def generic_purity_calculation(records: List[ProcessRecord], group_by_attr: str) -> Dict[str, Dict[Tuple, Any]]:
        """
        需求3与需求6的泛型复用算法：
        根据指定的维度(物料号 或 操机手)进行分组，计算参数向量的统计信息和纯度
        返回结构: {分组Key: {向量: {'count': 次数, 'purity': 纯度}, 'total_unique_vectors': 总数}}
        """
        # 第一步：分组并计数
        # group_data = { GroupKey: { Vector: Count } }
        group_data = defaultdict(lambda: defaultdict(int))
        
        for r in records:
            group_key = getattr(r, group_by_attr) # 动态获取分组字段 (material_code 或 operator_name)
            vector = r.parameter_vector
            group_data[group_key][vector] += 1
            
        # 第二步：计算纯度
        result = {}
        for group_key, vectors_map in group_data.items():
            distinct_vector_count = len(vectors_map) # 该对象的所有不同工艺参数向量的数量
            
            result[group_key] = {
                "total_unique_vectors": distinct_vector_count,
                "vectors_detail": {}
            }
            
            for vector, count in vectors_map.items():
                # 纯度计算公式：V在P中出现的次数 / P的所有不同工艺参数向量的数量
                purity = count / distinct_vector_count
                result[group_key]["vectors_detail"][vector] = {
                    "count": count,
                    "purity": purity
                }
        return result


# ==========================================
# 5. 持久化层模拟 (Repository Pattern)
# 将验证通过和不通过的数据分开存储
# ==========================================

class ProcessRecordRepository:
    def __init__(self):
        self.valid_records: List[ProcessRecord] = []
        self.invalid_records: List[ProcessRecord] = []
        
    def save(self, record: ProcessRecord):
        if record.is_valid:
            self.valid_records.append(record)
        else:
            self.invalid_records.append(record)

    def print_invalid_report(self):
        """展示无效生产记录表"""
        print("\n=== [独立表] 无效生产记录表 (供客户讨论) ===")
        for r in self.invalid_records:
            print(f"联合主键 {r.record_key} -> 拦截原因: {r.invalid_reason}")


# ==========================================
# 主流程 (Main) - 组装各个组件，避免“上帝函数”
# ==========================================

if __name__ == '__main__':
    # 1. 依赖注入与组件装配
    loader = ProcessRecordExcelLoader()
    engine = ValidationEngine()
    engine.add_rule(NotNullRule())
    engine.add_rule(SizeLogicRule())
    repository = ProcessRecordRepository()
    
    # 2. 执行数据加载与清洗校验 (满足需求1, 2)
    raw_data = loader.load_data("new_format_data.xlsx")
    for record in raw_data:
        validated_record = engine.process(record)
        repository.save(validated_record)
        
    # 3. 输出需求2：无效数据表
    repository.print_invalid_report()
    
    # 获取有效数据用于后续统计
    valid_data = repository.valid_records
    stats_service = StatisticsService()
    
    print("\n=== 需求4：统计每个物料品号的生产批号次数 ===")
    batch_counts = stats_service.count_batches_per_material(valid_data)
    for mat_code, count in batch_counts.items():
        print(f"物料品号: {mat_code}, 生产批次总数: {count}")

    print("\n=== 需求3、5：物料品号的工艺参数向量及纯度 ===")
    # 复用算法：按 物料品号 进行计算
    material_purity_stats = stats_service.generic_purity_calculation(valid_data, group_by_attr="material_code")
    for mat_code, data in material_purity_stats.items():
        print(f"物料品号: {mat_code} (包含 {data['total_unique_vectors']} 种不同参数)")
        for vec, detail in data['vectors_detail'].items():
            print(f"  -> 参数: {vec}")
            print(f"     生产次数: {detail['count']} 次 | 纯度指标: {detail['purity']:.2f}")

    print("\n=== 需求6：按操作手的工艺参数纯度统计 (复用同一套算法) ===")
    # 复用算法：仅替换入参为 operator_name
    operator_purity_stats = stats_service.generic_purity_calculation(valid_data, group_by_attr="operator_name")
    for op_name, data in operator_purity_stats.items():
        print(f"操机手: {op_name} (包含 {data['total_unique_vectors']} 种不同参数)")
        for vec, detail in data['vectors_detail'].items():
             print(f"  -> 参数: {vec}")
             print(f"     生产次数: {detail['count']} 次 | 纯度指标: {detail['purity']:.2f}")

    print("\n=== 需求7：未来扩展演示 ===")
    quality_loader = QualityInspectionExcelLoader()
    quality_data = quality_loader.load_data("quality_test.xlsx")
    print("架构已支持 QualityInspectionExcelLoader 热拔插，无需修改现有统计与工序代码。")