#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

vector<string> rules;

void read_grammar(const string &filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "无法打开文件" << endl;
        exit(EXIT_FAILURE);
    }
    string line;
    while (getline(file, line)) {
        if (!line.empty()) rules.push_back(line);
    }
}

bool is_non_terminal(char c) { return c >= 'A' && c <= 'Z'; }
bool is_terminal(char c) { return islower(c) || isdigit(c); }

// 检查是否所有产生式符合3型文法且方向一致
bool is_type_3() {
    // 在此处书写你的代码

    return true;
}

// 2型文法：左部必须是单个非终结符
bool is_type_2() {
    // 在此处书写你的代码

    return true;
}

// 1型文法：左部长度 ≤ 右部长度，且左部至少含一个非终结符，允许 A -> ε
bool is_type_1() {
    // 在此处书写你的代码

    return true;
}

int classify_grammar() {    
    if (is_type_3()) return 3;
    if (is_type_2()) return 2;
    if (is_type_1()) return 1;
    return 0;
}

int main() {
    // 输出结果
    for (char i = '0'; i <= '9'; i++) {
        string filename = "examples/test" + string(1, i) + ".in";
        read_grammar(filename);
        cout << "case" << i << ": " << classify_grammar() << endl;
        rules.clear();
    }
        
    return 0;
}
