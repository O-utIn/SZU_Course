#include "Lexer.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "用法: " << argv[0] << " <源文件名>" << endl;
        return 1;
    }
    Lexer lexer(argv[1]);
    Token token;
    while (true) {
        token = lexer.getNextToken();
        if (token.type == END_OF_FILE)
            break;
        cout << "(" << token.type << ", " << token.lexeme << ", "
             << token.line << ", " << token.col << ")" << endl;
    }
    return 0;
}