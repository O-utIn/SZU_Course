#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <set>
#include <vector>
#include <map>
#include <stack>
#include <iomanip>
using namespace std;

char start;
map<string, vector<string>> pro;
map<string, set<string>> FIRST, FOLLOW;
map<pair<char, char>, string> M;
set<string> alpha, Vn, Vt;
stack<char> stk, stk_tmp;

// === 已实现 ===
bool isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')';
}

// === 已实现 ===
bool check(string vn, string pstr) {
    return pstr.substr(0, vn.size()) == vn;
}

// === 已实现 ===
void disp(bool exist) {
    cout << "\n非终结符(Vn): ";
    for (auto &it : Vn) cout << it << ' ';
    cout << "\n终结符(Vt): ";
    for (auto &it : Vt) cout << it << ' ';
    cout << endl;
    if (exist) {
        cout << "存在左递归，消除后如下：" << endl;
        for (auto &it : pro) {
            for (auto &i : it.second) {
                cout << it.first << " -> " << i << endl;
            }
        }
    }
}

// 计算FIRST集
void Sol_First() {
    // 初始化：终结符的FIRST集是自身
    for (auto &vt : Vt) {
        if (vt != "e")
            FIRST[vt] = {vt};
    }
    // 初始化非终结符FIRST集为空
    for (auto &vn : Vn) {
        FIRST[vn].clear();
    }

    bool updated;
    // 迭代计算直到不再变化
    do {
        updated = false;
        for (auto &vn : Vn) {
            for (auto &prod : pro[vn]) {
                set<string> cur_first;
                bool can_empty = true;

                // 遍历产生式右部每个符号
                for (int i = 0; i < prod.size(); ++i) {
                    string s(1, prod[i]);
                    // 遇到终结符，直接加入并终止
                    if (Vt.count(s)) {
                        cur_first.insert(s);
                        can_empty = false;
                        break;
                    }
                    // 遇到非终结符，加入其FIRST集
                    if (Vn.count(s)) {
                        cur_first.insert(FIRST[s].begin(), FIRST[s].end());
                        // 不能推出空，终止
                        if (!FIRST[s].count("e")) {
                            can_empty = false;
                            break;
                        }
                    }
                }

                // 全部符号都能推出空，加入e
                if (can_empty || prod.empty()) {
                    cur_first.insert("e");
                }

                // 更新FIRST集
                for (auto &x : cur_first) {
                    if (!FIRST[vn].count(x)) {
                        FIRST[vn].insert(x);
                        updated = true;
                    }
                }
            }
        }
    } while (updated);

    // 输出FIRST集
    cout << "\nFIRST集：" << endl;
    for (auto &p : FIRST) {
        if (Vn.count(p.first)) {
            cout << "FIRST(" << p.first << ")={";
            bool first = true;
            for (auto &x : p.second) {
                if (!first) cout << ",";
                cout << x;
                first = false;
            }
            cout << "}" << endl;
        }
    }
}

// 计算FOLLOW集
void Sol_Follow() {
    // 初始化所有非终结符FOLLOW集为空
    for (auto &vn : Vn)
        FOLLOW[vn].clear();
    // 开始符号FOLLOW加入#
    FOLLOW[string(1, start)].insert("#");

    bool updated;
    // 迭代计算直到不再变化
    do {
        updated = false;
        for (auto &A : Vn) {
            for (auto &alpha_str : pro[A]) {
                int len = alpha_str.size();
                // 遍历产生式右部每个符号B
                for (int i = 0; i < len; ++i) {
                    string B(1, alpha_str[i]);
                    if (!Vn.count(B)) continue;

                    string beta = alpha_str.substr(i + 1);
                    set<string> first_beta;
                    bool beta_empty = true;

                    // 计算FIRST(beta)
                    for (int j = 0; j < beta.size(); ++j) {
                        string s(1, beta[j]);
                        if (Vt.count(s)) {
                            first_beta.insert(s);
                            beta_empty = false;
                            break;
                        }
                        if (Vn.count(s)) {
                            first_beta.insert(FIRST[s].begin(), FIRST[s].end());
                            if (!FIRST[s].count("e")) {
                                beta_empty = false;
                                break;
                            }
                        }
                    }
                    if (beta.empty() || beta_empty)
                        first_beta.insert("e");

                    // 将FIRST(beta)-{e}加入FOLLOW(B)
                    set<string> to_add;
                    for (auto &x : first_beta) {
                        if (x != "e")
                            to_add.insert(x);
                    }
                    // 若beta可空，将FOLLOW(A)加入FOLLOW(B)
                    if (first_beta.count("e")) {
                        to_add.insert(FOLLOW[A].begin(), FOLLOW[A].end());
                    }

                    // 更新FOLLOW集
                    for (auto &x : to_add) {
                        if (!FOLLOW[B].count(x)) {
                            FOLLOW[B].insert(x);
                            updated = true;
                        }
                    }
                }
            }
        }
    } while (updated);

    // 输出FOLLOW集
    cout << "\nFOLLOW集：" << endl;
    for (auto &p : FOLLOW) {
        cout << "FOLLOW(" << p.first << ")={";
        bool first = true;
        for (auto &x : p.second) {
            if (!first) cout << ",";
            cout << x;
            first = false;
        }
        cout << "}" << endl;
    }
}

// 构造LL(1)分析表
void Build_LL1() {
    M.clear();
    for (auto &A : Vn) {
        for (auto &prod : pro[A]) {
            set<string> first_p;
            bool p_empty = true;

            // 计算当前产生式右部的FIRST集
            for (int i = 0; i < prod.size(); ++i) {
                string s(1, prod[i]);
                if (Vt.count(s)) {
                    first_p.insert(s);
                    p_empty = false;
                    break;
                }
                if (Vn.count(s)) {
                    first_p.insert(FIRST[s].begin(), FIRST[s].end());
                    if (!FIRST[s].count("e")) {
                        p_empty = false;
                        break;
                    }
                }
            }
            if (prod.empty() || p_empty)
                first_p.insert("e");

            // 填充分析表
            for (auto &a : first_p) {
                if (a == "e") {
                    // 可空，用FOLLOW(A)填充
                    for (auto &b : FOLLOW[A]) {
                        if (b == "#" || Vt.count(b)) {
                            M[{A[0], b[0]}] = prod;
                        }
                    }
                } else {
                    // 不可空，直接用FILL
                    if (Vt.count(a) || a == "#") {
                        M[{A[0], a[0]}] = prod;
                    }
                }
            }
        }
    }
}

// === 已实现 ===
void Print_LL1_Table() {
    cout << "\nLL(1) 分析表 (表格形式):" << endl;
    
      const int colWidth = 8;          // 每列统一宽度
    
      // 计算可显示宽度，中文字符按 2 宽处理
      auto getDisplayWidth = [](const std::string &s) {
          int width = 0;
          for (size_t i = 0; i < s.size();) {
              unsigned char c = static_cast<unsigned char>(s[i]);
              if (c < 0x80) { ++width; ++i; }
              else {
                  width += 2;
                  if      ((c & 0xF0) == 0xF0) i += 4;
                  else if ((c & 0xE0) == 0xE0) i += 3;
                  else if ((c & 0xC0) == 0xC0) i += 2;
                  else                         ++i;
              }
          }
          return width;
      };
    
      // 按显示宽度补足空格
      auto pad = [&](const std::string &s, int width) -> std::string {
          int w = getDisplayWidth(s);
          if (w >= width) return s;
          return s + std::string(width - w, ' ');
      };
    
      // 输出一个单元格（前置竖线 + 填充字符串）
      auto printCell = [&](const std::string &s) {
          std::cout << "|" << pad(s, colWidth);
      };

    // 获取列头（终结符 + #）
    set<char> terminalCols;
    for (const auto &vt : Vt) {
        if (!vt.empty())
            terminalCols.insert(vt[0]);
    }
    terminalCols.insert('#'); // 加入结束符

    // 构造横向分隔线（行分隔）
      std::string separator;
      for (size_t i = 0; i <= terminalCols.size(); ++i) {
          separator += "+" + std::string(colWidth, '-');
      }
      separator += "+";
    
      // 打印表头
      std::cout << separator << std::endl;
      printCell(" ");                        // 左上角空单元
      for (char t : terminalCols)
          printCell(std::string(1, t));
      std::cout << "|" << std::endl;
      std::cout << separator << std::endl;

    for (const auto &vn : Vn) {
          if (vn.empty()) continue;
          char row = vn[0];
          printCell(std::string(1, row));      // 行首单元
 
          for (char col : terminalCols) {
              auto it = M.find({row, col});
              if (it != M.end())
                  printCell(std::string(1, row) + "->" + it->second);
              else
                  printCell(" ");
          }
          std::cout << "|" << std::endl;
          std::cout << separator << std::endl;
      }

}

// === 已实现 ===
void Sol_LL1(string in) {
    cout << "\n分析过程：" << endl;

    const int colWidth = 15;

    // 计算可显示宽度，中文字符按 2 宽处理
    auto getDisplayWidth = [](const std::string &s) {
        int width = 0;
        for (size_t i = 0; i < s.size();) {
            unsigned char c = static_cast<unsigned char>(s[i]);
            if (c < 0x80) {                // ASCII
                ++width;
                ++i;
            } else {                       // 简单按双宽字符处理
                width += 2;
                // 跳过当前 UTF‑8 字节序列
                if ((c & 0xF0) == 0xF0)      i += 4;
                else if ((c & 0xE0) == 0xE0) i += 3;
                else if ((c & 0xC0) == 0xC0) i += 2;
                else                         ++i;
            }
        }
        return width;
    };

    // 按显示宽度补足空格
    auto pad = [&](const std::string &s, int width) -> std::string {
        int w = getDisplayWidth(s);
        if (w >= width) return s;          // 太长就不裁剪，直接返回
        return s + std::string(width - w, ' ');
    };

    // 输出一个单元格（前置竖线 + 填充字符串）
    auto printCell = [&](const std::string &s) {
        std::cout << "|" << pad(s, colWidth);
    };

    const string separator = "+" + string(colWidth, '-') + "+" + string(colWidth, '-') + "+" +
                             string(colWidth, '-') + "+" + string(colWidth, '-') + "+";

    cout << separator << endl;
    printCell("步骤");
    printCell("分析栈");
    printCell("输入串");
    printCell("所用产生式");
    std::cout << "|" << std::endl;
    cout << separator << endl;

    stk.push('#');
    stk.push(start);
    int turn = 0;

    while (++turn) {
        string show_exc;
        if (stk.top() == '#' && in[0] == '#') show_exc = "Success";
        else if (M.find({ stk.top(), in[0] }) == M.end() && stk.top() != in[0]) show_exc = "Error";
        else if (stk.top() == in[0]) show_exc = " ";
        else show_exc = string(1, stk.top()) + "->" + M[{stk.top(), in[0]}];

        // 获取栈内容
        stack<char> stk_copy = stk;
        string stackStr;
        while (!stk_copy.empty()) {
            stk_tmp.push(stk_copy.top());
            stk_copy.pop();
        }
        while (!stk_tmp.empty()) {
            stackStr += stk_tmp.top();
            stk_tmp.pop();
        }

        // 输出当前步骤
        std::cout << "|" << pad(std::to_string(turn), colWidth)
                  << "|" << pad(stackStr, colWidth)
                  << "|" << pad(in, colWidth)
                  << "|" << pad(show_exc, colWidth)
                  << "|" << std::endl;
        std::cout << separator << std::endl;

        // 分析逻辑
        if (stk.top() == '#' && in[0] == '#') break;
        if (stk.top() == in[0]) {
            stk.pop();
            in.erase(in.begin());
            continue;
        }
        if (M.find({ stk.top(), in[0] }) == M.end()) {
            cout << "分析失败！" << endl;
            return;
        }
        string exc = M[{stk.top(), in[0]}];
        stk.pop();
        for (int i = exc.size() - 1; i >= 0; --i)
            if (exc[i] != 'e') stk.push(exc[i]);
    }
}

// === 已实现 ===
int main() {
    string filename;
    cout << "请输入文法文件名（例如 grammar.txt）: ";
    cin >> filename;
    ifstream fin(filename);
    if (!fin.is_open()) {
        cerr << "无法打开文件 " << filename << endl;
        return 1;
    }

    bool startFlag = true;
    string production;
    while (getline(fin, production)) {
        while (!production.empty() && production.back() == ' ') production.pop_back();
        if (production.empty()) continue;

        int idx = 0;
        string vn;
        while (production[idx] != '-') {
            if (production[idx] != ' ') vn += production[idx];
            ++idx;
        }
        Vn.insert(vn), alpha.insert(vn);
        if (startFlag) FOLLOW[vn].insert("#"), start = vn[0], startFlag = false;
        while (production[idx] != '>') ++idx;
        ++idx;

        string str, alp;
        for (; idx < production.size(); ++idx) {
            if (production[idx] == ' ') continue;
            if (production[idx] == '|') {
                if (!str.empty()) pro[vn].push_back(str), str = "";
                if (!alp.empty()) alpha.insert(alp), alp = "";
                continue;
            }
            str += production[idx];
            if (isOperator(production[idx])) {
                if (!alp.empty()) alpha.insert(alp), alp = "";
                alp += production[idx];
                alpha.insert(alp);
                alp.clear();
            } else alp += production[idx];
        }
        if (!str.empty()) pro[vn].push_back(str);
        if (!alp.empty()) alpha.insert(alp);
    }

    for (auto &a : alpha) {
        if (!Vn.count(a)) Vt.insert(a);
    }

    string str;
    cout << "请输入要分析的输入串: ";
    cin >> str;

    // 去除左递归（简化版）
    bool exist = false;
    for (auto &vn : Vn) {
        bool flag = false;
        string str1, subpstr;
        for (auto it = pro[vn].begin(); it != pro[vn].end();) {
            if (check(vn, *it)) {
                exist = flag = true;
                subpstr = it->substr(vn.size());
                it = pro[vn].erase(it);
            } else ++it;
        }
        if (flag) {
            for (auto it = pro[vn].begin(); it != pro[vn].end();) {
                if (!check(vn, *it)) {
                    str1 = *it;
                    it = pro[vn].erase(it);
                    break;
                } else ++it;
            }
            string newvn;
            for (char ch = 'A'; ch <= 'Z'; ++ch) {
                string temp(1, ch);
                if (!Vn.count(temp)) {
                    newvn = temp;
                    Vn.insert(newvn);
                    break;
                }
            }
            pro[vn].push_back(str1 + newvn);
            pro[newvn].push_back(subpstr + newvn);
            Vt.insert("e");
            pro[newvn].push_back("e");
        }
    }

    disp(exist);
    Sol_First();
    Sol_Follow();
    Build_LL1();
    Print_LL1_Table();

    // 处理输入串
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    string formatted;
    for (char ch : str) {
        if (isdigit(ch)) {
            if (formatted.empty() || formatted.back() != 'i') formatted.push_back('i');
        } else formatted.push_back(ch);
    }

    cout << "转换后输入串为: " << formatted << endl;
    Sol_LL1(formatted + "#");
    return 0;
}