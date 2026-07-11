#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <string>
#include <cctype>
using namespace std;

// 去除字符串两端空白
static string trim(const string &s) {
    size_t i = 0, j = s.size();
    while (i < s.size() && isspace((unsigned char)s[i])) i++;
    while (j > 0 && isspace((unsigned char)s[j - 1])) j--;
    return s.substr(i, j - i);
}

// 判断 token 是否为运算符
static bool isOperator(const string &tok) {
    return (tok == "+" || tok == "-" || tok == "*" || tok == "/");
}

// 返回运算符优先级（数字越大优先级越高）
static int precedence(const string &op) {
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}

// 将表达式字符串拆分成 token 列表
static vector<string> tokenize(const string &expr) {
    vector<string> tokens;
    int n = expr.size();
    for (int i = 0; i < n; ) {
        if (isspace((unsigned char)expr[i])) {
            i++;
            continue;
        }
        // 如果是字母或数字，读取完整标识符（变量名或常数）
        if (isalnum((unsigned char)expr[i])) {
            int j = i;
            while (j < n && isalnum((unsigned char)expr[j])) j++;
            tokens.push_back(expr.substr(i, j - i));
            i = j;
        }
        else {
            // 否则当作单字符运算符 + - * /
            string op(1, expr[i]);
            tokens.push_back(op);
            i++;
        }
    }
    return tokens;
}

// 中序转后序 (Shunting‐Yard)，不处理括号
static vector<string> infixToPostfix(const vector<string> &infix) {
    vector<string> output;
    stack<string> opStack;
    for (auto &tok : infix) {
        if (isOperator(tok)) {
            // 如果栈顶也是运算符且优先级 >= 当前 tok，就弹出
            while (!opStack.empty() && isOperator(opStack.top())
                   && precedence(opStack.top()) >= precedence(tok)) {
                output.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(tok);
        }
        else {
            // 操作数直接加入输出
            output.push_back(tok);
        }
    }
    // 弹出剩余运算符
    while (!opStack.empty()) {
        output.push_back(opStack.top());
        opStack.pop();
    }
    return output;
}

// 根据后序序列生成四元组并输出
static void generateQuadruples(const vector<string> &postfix,
                               const string &lhsVar,
                               int &tempCnt) {
    stack<string> st;
    for (auto &tok : postfix) {
        if (isOperator(tok)) {
            string right = st.top(); st.pop();
            string left  = st.top(); st.pop();
            tempCnt++;
            string tvar = "t" + to_string(tempCnt);
            cout << "(" << tok << ", " << left << ", " << right << ", " << tvar << ")\n";
            st.push(tvar);
        }
        else {
            st.push(tok);
        }
    }
    // 最后栈顶是表达式结果的临时变量
    if (!st.empty()) {
        string finalTemp = st.top();
        cout << "(=, " << finalTemp << ", 0, " << lhsVar << ")\n";
    }
}

// 主函数增加 argc/argv 命令行参数接收
int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int selectNum = 0;
    // 1. 判断是否传入命令行参数
    if (argc == 2) {
        try {
            selectNum = stoi(argv[1]);
        } catch (...) {
            cerr << "错误：命令行参数必须是数字 1~10！\n";
            return 1;
        }
        // 校验数字范围
        if (selectNum < 1 || selectNum > 10) {
            cerr << "错误：文件编号范围仅支持 1 ~ 10\n";
            return 1;
        }
    } else {
        // 2. 无参数时，交互式手动输入
        cout << "===== 中间代码生成程序 =====\n";
        cout << "使用方式1(推荐)：./ex5 [1-10]  例如 ./ex5 10\n";
        cout << "使用方式2：直接输入文件编号 1~10\n";
        while (true) {
            cout << "请输入要读取的txt文件编号：";
            cin >> selectNum;
            if (selectNum >= 1 && selectNum <= 10) break;
            cout << "输入非法！只能输入 1~10 的整数，请重新输入\n\n";
        }
    }

    // 拼接文件路径
    string filePath = "examples/" + to_string(selectNum) + ".txt";
    cout << "\n正在读取输入文件：" << filePath << "\n四元组输出：\n\n";

    // 打开目标文件
    ifstream fin(filePath);
    if (!fin.is_open()) {
        cerr << "致命错误：无法打开文件 " << filePath << "，检查目录是否存在！\n";
        return 1;
    }

    // 读取全部文件内容
    string rawInput;
    {
        ostringstream oss;
        string line;
        while (getline(fin, line)) {
            oss << line;
        }
        rawInput = oss.str();
    }
    fin.close();

    // 按分号分割多条赋值语句
    vector<string> statements;
    {
        int n = rawInput.size();
        string buf;
        for (int i = 0; i < n; i++) {
            if (rawInput[i] == ';') {
                string stmt = trim(buf);
                if (!stmt.empty()) statements.push_back(stmt);
                buf.clear();
            } else {
                buf.push_back(rawInput[i]);
            }
        }
        // 处理末尾无分号的剩余语句
        string tail = trim(buf);
        if (!tail.empty()) statements.push_back(tail);
    }

    // 全局临时变量计数器
    int tempCnt = 0;
    // 逐条翻译语句生成四元组
    for (auto &stmt : statements) {
        size_t posEq = stmt.find('=');
        if (posEq == string::npos) continue;
        string lhs = trim(stmt.substr(0, posEq));
        string rhs = trim(stmt.substr(posEq + 1));

        vector<string> tokens = tokenize(rhs);
        vector<string> postfix = infixToPostfix(tokens);
        generateQuadruples(postfix, lhs, tempCnt);
    }

    return 0;
}