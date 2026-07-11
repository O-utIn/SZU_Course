#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using namespace std;

// 定义单词类型
enum TokenType {
    KEYWORD = 1,      // 关键字
    IDENTIFIER = 2,   // 标识符
    INTEGER = 3,      // 整数
    FLOAT = 4,        // 小数
    DELIMITER = 5,    // 分隔符
    ARITH_OP = 6,     // 四则运算符
    COMP_OP = 7,      // 比较运算符
    ASSIGN_OP = 8,    // 赋值运算符
    ERROR_TOKEN = -1, // 错误单词
    END_OF_FILE = 0   // 文件结束标记
};
// 定义单词结构：类型、词本身、行号、列号
struct Token {
    int type;
    string lexeme;
    int line;
    int col;
};

#endif // TOKEN_H