#ifndef LEXER_H
#define LEXER_H

#include "Token.h"
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Lexer {
private:
    ifstream file;
    int line;      // 当前行号
    int col;       // 当前列号
    char currentChar;
    bool eof;      // 是否到达文件末尾

    // 读取下一个字符并更新行列计数
    void advance();

    // 跳过空白字符
    void skipWhitespace();

    // 跳过注释（支持单行注释和多行注释）
    void skipComment();

    // 判断给定字符串是否为关键字
    bool isKeyword(const string &lexeme);

    // 关键字列表（区分大小写）
    vector<string> keywords;
public:
    Lexer(const string &filename);
    Token getNextToken();
};

#endif // LEXER_H