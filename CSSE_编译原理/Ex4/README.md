# 实验四：自底向上的语法分析程序设计（LR 系列）

## 1. 文档依据

- `4 实验四_自底向上的语法分析程序设计.doc`

## 2. 实验目标

- 掌握自底向上语法分析思想。
- 理解 LR(0) 项目集闭包、活前缀 DFA、分析表构造与分析过程。

## 3. 文档要求要点

- 文档将实验划分为 6 个部分（部分标注为选做）：
- LR(0) 项目闭包
- LR(0) 活前缀 DFA 构造
- LR(0) 分析表构造
- 基于 LR 分析表的语法分析
- SLR(1) 分析表构造（选做）
- LR(1) 分析表构造（选做）

## 4. 目录结构

```text
ex4-自底向上的语法分析程序设计
├── 4 实验四_自底向上的语法分析程序设计.doc
├── lr0_parser.cpp
└── examples/
    ├── grammar1.txt
    ├── grammar2.txt
    ├── grammar3.txt
    ├── example1.txt
    ├── example2.txt
    └── example3.txt
```

## 5. 编译与运行

```bash
cd ex4-自底向上的语法分析程序设计
clang++ -std=c++17 -O2 -o lr0_parser lr0_parser.cpp
./lr0_parser examples/grammar1.txt
```

可将 `examples/grammar1.txt` 替换为 `examples/grammar2.txt` 或 `examples/grammar3.txt` 测试。

## 6. 提交建议

- 报告与代码分开提交。
- 不提交可执行文件。
- 需提交：源代码 + 实验报告（Word 文档，命名为 `ex4_学号_姓名`）。
- 鼓励使用大模型工具辅助，但需在报告中注明具体使用位置与方式。
