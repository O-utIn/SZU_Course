#include "Lexer.h"
#include <iostream>
#include <cctype>
#include <cstdlib>

Lexer::Lexer(const string &filename) : line(1), col(0), eof(false) {
    // 初始化关键字列表
    keywords = {
        "int", "float", "program", "const", "def",
        "begin", "end", "call", "let", "if", "then",
        "else", "fi", "while", "do", "endwh", "return",
        "and", "or"
    };

    file.open(filename);
    if (!file.is_open()) {
        cerr << "无法打开文件: " << filename << endl;
        exit(1);
    }
    advance(); // 读取第一个字符
}

void Lexer::advance() {
    // 读取字符并更新行号、列号
    if (file.get(currentChar)) {
        col++;
        if (currentChar == '\n') {
            line++;
            col = 0;
        }
    } else {
        eof = true;
    }
}

void Lexer::skipWhitespace() {
    while (!eof && isspace(currentChar)) {
        advance();
    }
}

void Lexer::skipComment() {
    if (currentChar == '/') {
        char next = file.peek();
        if (next == '/') {  // 单行注释
            while (!eof && currentChar != '\n') {
                advance();
            }
            if (currentChar == '\n')
                advance();
        } else if (next == '*') { // 多行注释
            advance(); // 消耗 '/'
            advance(); // 消耗 '*'
            while (!eof) {
                if (currentChar == '*') {
                    char next2 = file.peek();
                    if (next2 == '/') {
                        advance(); // 消耗 '*'
                        advance(); // 消耗 '/'
                        break;
                    } else {
                        advance();
                    }
                } else {
                    advance();
                }
            }
        }
    }
}

// 判断是否为关键字
bool Lexer::isKeyword(const string &lexeme) {
    for (const string &kw : keywords) {
        if (kw == lexeme) {
            return true;
        }
    }
    return false;
}

Token Lexer::getNextToken() {
    Token token;
    skipWhitespace();
    while (!eof && currentChar == '/') {
        char next = file.peek();
        if (next == '/' || next == '*') {
            skipComment();
            skipWhitespace();
        } else {
            break;
        }
    }

    int tokenStartLine = line;
    int tokenStartCol = col;

    if (eof) {
        token.type = END_OF_FILE;
        token.lexeme = "";
        return token;
    }

    // 标识符或关键字
    if (isalpha(currentChar)) {
        string lexeme;
        lexeme.push_back(currentChar);
        advance();
        while (!eof && isalnum(currentChar)) {
            lexeme.push_back(currentChar);
            advance();
        }
        if (lexeme.size() > 32) {
            token.type = ERROR_TOKEN;
            token.lexeme = "Error";
        } else {
            token.type = isKeyword(lexeme) ? KEYWORD : IDENTIFIER;
            token.lexeme = lexeme;
        }
        token.line = tokenStartLine;
        token.col = tokenStartCol;
        return token;
    }

    // 数字：整数或小数
    else if (isdigit(currentChar)) {
        string lexeme;

        if (currentChar == '0') {
            lexeme.push_back(currentChar);
            advance();

            // 0 开头跟数字 → 错误
            if (!eof && isdigit(currentChar)) {
                while (!eof && isdigit(currentChar)) {
                    lexeme.push_back(currentChar);
                    advance();
                }
                token.type = ERROR_TOKEN;
                token.lexeme = "Error";
                token.line = tokenStartLine;
                token.col = tokenStartCol;
                return token;
            }

            // 0 后是小数点
            if (!eof && currentChar == '.') {
                lexeme.push_back(currentChar);
                advance();
                // 小数点后无数字 → 错误
                if (eof || !isdigit(currentChar)) {
                    token.type = ERROR_TOKEN;
                    token.lexeme = "Error";
                    token.line = tokenStartLine;
                    token.col = tokenStartCol;
                    return token;
                }
                // 读取小数部分
                while (!eof && isdigit(currentChar)) {
                    lexeme.push_back(currentChar);
                    advance();
                }
                // 合法浮点数
                token.type = FLOAT;
                token.lexeme = lexeme;
                token.line = tokenStartLine;
                token.col = tokenStartCol;
                return token;
            }
            // 单纯 0 → 合法整数
            else {
                token.type = INTEGER;
                token.lexeme = lexeme;
                token.line = tokenStartLine;
                token.col = tokenStartCol;
                return token;
            }
        }
        // 非 0 开头数字
        else {
            while (!eof && isdigit(currentChar)) {
                lexeme.push_back(currentChar);
                advance();
            }
            // 有小数点
            if (!eof && currentChar == '.') {
                lexeme.push_back(currentChar);
                advance();
                // 小数点后无数字 → 错误
                if (eof || !isdigit(currentChar)) {
                    token.type = ERROR_TOKEN;
                    token.lexeme = "Error";
                    token.line = tokenStartLine;
                    token.col = tokenStartCol;
                    return token;
                }
                // 读取小数部分
                while (!eof && isdigit(currentChar)) {
                    lexeme.push_back(currentChar);
                    advance();
                }
                // 合法浮点数
                token.type = FLOAT;
                token.lexeme = lexeme;
                token.line = tokenStartLine;
                token.col = tokenStartCol;
                return token;
            }
            // 无小数点 → 合法整数
            else {
                token.type = INTEGER;
                token.lexeme = lexeme;
                token.line = tokenStartLine;
                token.col = tokenStartCol;
                return token;
            }
        }
    }

    // 分隔符、运算符及其他符号
    else {
        char curr = currentChar;
        if (curr == '(' || curr == ')' || curr == ';' || curr == ',') {
            token.type = DELIMITER;
            token.lexeme = string(1, curr);
            token.line = tokenStartLine;
            token.col = tokenStartCol;
            advance();
            return token;
        }
        if (curr == '+' || curr == '-' || curr == '*') {
            token.type = ARITH_OP;
            token.lexeme = string(1, curr);
            token.line = tokenStartLine;
            token.col = tokenStartCol;
            advance();
            return token;
        }
        if (curr == '/') {
            char next = file.peek();
            if (next != '/' && next != '*') {
                token.type = ARITH_OP;
                token.lexeme = string(1, curr);
                token.line = tokenStartLine;
                token.col = tokenStartCol;
                advance();
                return token;
            } else {
                skipComment();
                return getNextToken();
            }
        }
        if (curr == '=') {
            advance();
            if (!eof && currentChar == '=') {
                token.type = COMP_OP;
                token.lexeme = "==";
                token.line = tokenStartLine;
                token.col = tokenStartCol;
                advance();
                return token;
            } else {
                token.type = ASSIGN_OP;
                token.lexeme = "=";
                token.line = tokenStartLine;
                token.col = tokenStartCol;
                return token;
            }
        }
        if (curr == '<') {
            advance();
            if (!eof && currentChar == '=') {
                token.type = COMP_OP;
                token.lexeme = "<=";
                token.line = tokenStartLine;
                token.col = tokenStartCol;
                advance();
                return token;
            } else if (!eof && currentChar == '>') {
                token.type = COMP_OP;
                token.lexeme = "<>";
                token.line = tokenStartLine;
                token.col = tokenStartCol;
                advance();
                return token;
            } else {
                token.type = COMP_OP;
                token.lexeme = "<";
                token.line = tokenStartLine;
                token.col = tokenStartCol;
                return token;
            }
        }
        if (curr == '>') {
            advance();
            if (!eof && currentChar == '=') {
                token.type = COMP_OP;
                token.lexeme = ">=";
                token.line = tokenStartLine;
                token.col = tokenStartCol;
                advance();
                return token;
            } else {
                token.type = COMP_OP;
                token.lexeme = ">";
                token.line = tokenStartLine;
                token.col = tokenStartCol;
                return token;
            }
        }
        // 无法识别的字符视为错误
        token.type = ERROR_TOKEN;
        token.lexeme = string(1, curr);
        token.line = tokenStartLine;
        token.col = tokenStartCol;
        advance();
        return token;
    }
}