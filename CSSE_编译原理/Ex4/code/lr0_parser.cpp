#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace std;

// --- Data Structures ---

// Represents a production rule: Lhs -> Rhs
struct Production {
    string lhs;
    vector<string> rhs;
    int id; // Unique ID for the production

    bool operator<(const Production& other) const {
        if (lhs != other.lhs) return lhs < other.lhs;
        if (rhs != other.rhs) return rhs < other.rhs;
        return id < other.id;
    }

    bool operator==(const Production& other) const {
        return lhs == other.lhs && rhs == other.rhs && id == other.id;
    }

    void print() const {
        cout << id << ". " << lhs << " -> ";
        if (rhs.empty() || (rhs.size() == 1 && rhs[0] == "epsilon")) { // epsilon often represented by empty or "epsilon"
            cout << "epsilon";
        } else {
            for (const auto& s : rhs) {
                cout << s << " ";
            }
        }
    }
};

// Represents an LR(0) item: Production with a dot
struct Item {
    Production prod;
    int dot_pos;

    Item(Production p, int d) : prod(p), dot_pos(d) {}

    bool operator<(const Item& other) const {
        if (prod.id != other.prod.id) return prod.id < other.prod.id;
        return dot_pos < other.dot_pos;
    }

    bool operator==(const Item& other) const {
        return prod.id == other.prod.id && dot_pos == other.dot_pos;
    }

    void print() const {
        cout << "[" << prod.lhs << " -> ";
        for (int i = 0; i < prod.rhs.size(); ++i) {
            if (i == dot_pos) cout << ". ";
            cout << prod.rhs[i] << " ";
        }
        if (dot_pos == prod.rhs.size()) cout << ".";
        if (prod.rhs.empty() && dot_pos == 0) cout << "."; // For A -> .
        cout << "]";
    }

    string next_symbol() const {
        if (dot_pos < prod.rhs.size()) {
            return prod.rhs[dot_pos];
        }
        return ""; // Empty string if dot is at the end
    }
};

using ItemSet = set<Item>;

// Action in the parsing table
struct Action {
    enum Type { SHIFT, REDUCE, ACCEPT, ERROR, CONFLICT } type;
    int value; // State number for SHIFT, Production ID for REDUCE

    Action() : type(ERROR), value(-1) {}
    Action(Type t, int v) : type(t), value(v) {}

    void print() const {
        switch (type) {
            case SHIFT: cout << "s" << value; break;
            case REDUCE: cout << "r" << value; break;
            case ACCEPT: cout << "acc"; break;
            case ERROR: cout << "err"; break;
            case CONFLICT: cout << "conf"; break; // Or print specific conflict
        }
    }
};


// --- Global Variables for Grammar ---
vector<Production> productions;
string augmented_start_symbol = "S'"; // Default, can be changed if needed
string original_start_symbol;
set<string> non_terminals;
set<string> terminals;
set<string> all_symbols; // All unique symbols in the grammar

map<ItemSet, int> state_to_id;
vector<ItemSet> canonical_collection; // Stores all states (ItemSets)

// Parsing table
map<pair<int, string>, Action> action_table; // {state_id, terminal} -> Action
map<pair<int, string>, int> goto_table;     // {state_id, non_terminal} -> state_id

// Added to support input strings directly from grammar files or args
string input_string = ""; 


// --- Helper Functions ---

// Tokenizes the RHS of a production. Handles "id" as a single token.
vector<string> tokenize_rhs(const string& rhs_str) {
    vector<string> tokens;
    if (rhs_str.empty() || rhs_str == "epsilon") { // Handle epsilon productions
        // tokens.push_back("epsilon"); // Conventionally, empty RHS vector means epsilon
        return tokens;
    }

    for (int i = 0; i < rhs_str.length(); ++i) {
        if (i + 1 < rhs_str.length() && rhs_str.substr(i, 2) == "id") {
            tokens.push_back("id");
            i++; // Skip 'd'
        } else {
            tokens.push_back(string(1, rhs_str[i]));
        }
    }
    return tokens;
}

void load_grammar(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open grammar file " << filename << endl;
        exit(1);
    }

    string line;
    int prod_id_counter = 0; // Start with 0 for augmented production

    // Temporarily store original productions to find the first LHS for augmentation
    vector<pair<string, string>> original_rules;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // Skip empty lines or comments

        size_t arrow_pos = line.find("->");
        if (arrow_pos == string::npos) {
            // If there's a non-comment line without "->", we treat it as our target input string to analyze
            string temp = line;
            temp.erase(0, temp.find_first_not_of(" \t\n\r"));
            temp.erase(temp.find_last_not_of(" \t\n\r") + 1);
            if (!temp.empty()) {
                input_string = temp;
            }
            continue;
        }
        string lhs = line.substr(0, arrow_pos);
        string rhs_str = line.substr(arrow_pos + 2);

        // Trim whitespace
        lhs.erase(0, lhs.find_first_not_of(" \t\n\r"));
        lhs.erase(lhs.find_last_not_of(" \t\n\r") + 1);
        rhs_str.erase(0, rhs_str.find_first_not_of(" \t\n\r"));
        rhs_str.erase(rhs_str.find_last_not_of(" \t\n\r") + 1);

        original_rules.push_back({lhs, rhs_str});
        non_terminals.insert(lhs);
        all_symbols.insert(lhs);
    }
    file.close();

    if (original_rules.empty()) {
        cerr << "Error: No productions found in grammar file." << endl;
        exit(1);
    }

    // Augment grammar: S' -> original_start_symbol
    original_start_symbol = original_rules[0].first;
    Production augmented_prod;
    augmented_prod.id = prod_id_counter++;
    augmented_prod.lhs = augmented_start_symbol;
    augmented_prod.rhs = {original_start_symbol};
    productions.push_back(augmented_prod);
    non_terminals.insert(augmented_start_symbol);
    all_symbols.insert(augmented_start_symbol);
    all_symbols.insert(original_start_symbol);


    for (const auto& rule : original_rules) {
        Production p;
        p.id = prod_id_counter++;
        p.lhs = rule.first;
        p.rhs = tokenize_rhs(rule.second);
        productions.push_back(p);
        for (const auto& sym : p.rhs) {
            all_symbols.insert(sym);
        }
    }

    // Identify terminals
    for (const auto& sym : all_symbols) {
        if (non_terminals.find(sym) == non_terminals.end()) {
            terminals.insert(sym);
        }
    }
    terminals.insert("$"); // Add EOF marker
    all_symbols.insert("$");

    cout << "--- Grammar Productions ---" << endl;
    for (const auto& p : productions) {
        p.print();
        cout << endl;
    }
    cout << "Non-terminals: ";
    for (const auto& nt : non_terminals) cout << nt << " ";
    cout << endl;
    cout << "Terminals: ";
    for (const auto& t : terminals) cout << t << " ";
    cout << endl << endl;
}

ItemSet closure(ItemSet I) {
    ItemSet current_closure = I;
    bool changed;
    do {
        changed = false;
        ItemSet to_add;
        for (const Item& item : current_closure) {
            string B = item.next_symbol();
            if (!B.empty() && non_terminals.count(B)) {
                for (const Production& p : productions) {
                    if (p.lhs == B) {
                        Item new_item(p, 0);
                        if (current_closure.find(new_item) == current_closure.end() &&
                            to_add.find(new_item) == to_add.end()) { // Avoid re-adding if already in current or to_add
                            to_add.insert(new_item);
                            changed = true;
                        }
                    }
                }
            }
        }
        current_closure.insert(to_add.begin(), to_add.end());
    } while (changed);
    return current_closure;
}

ItemSet goTo(const ItemSet& I, const string& symbol) {
    ItemSet J;
    for (const Item& item : I) {
        if (item.next_symbol() == symbol) {
            J.insert(Item(item.prod, item.dot_pos + 1));
        }
    }
    return closure(J);
}

void build_canonical_collection() {
    cout << "--- Building Canonical Collection of LR(0) Items ---" << endl;
    // Initial state: Closure of [S' -> .S]
    ItemSet I0 = closure({Item(productions[0], 0)});
    state_to_id[I0] = 0;
    canonical_collection.push_back(I0);

    vector<ItemSet> work_list = {I0};
    int head = 0;

    while(head < work_list.size()){
        ItemSet current_I = work_list[head];
        int current_state_id = state_to_id[current_I];
        head++;

        cout << "Processing State I" << current_state_id << ":" << endl;
        for(const auto& item : current_I) {
            cout << "  "; item.print(); cout << endl;
        }


        for (const string& symbol : all_symbols) {
            if (symbol == "$") continue; // No goto on $

            ItemSet next_set = goTo(current_I, symbol);
            if (!next_set.empty()) {
                int next_state_id;
                if (state_to_id.find(next_set) == state_to_id.end()) {
                    next_state_id = canonical_collection.size();
                    state_to_id[next_set] = next_state_id;
                    canonical_collection.push_back(next_set);
                    work_list.push_back(next_set);
                    cout << "  Goto(I" << current_state_id << ", " << symbol << ") = I" << next_state_id << " (New)" << endl;
                } else {
                    next_state_id = state_to_id[next_set];
                    cout << "  Goto(I" << current_state_id << ", " << symbol << ") = I" << next_state_id << endl;
                }
                // Store transition for table construction later
                if (terminals.count(symbol)) { // Shift or part of GOTO for terminals
                     // This transition will be used for SHIFT action
                } else if (non_terminals.count(symbol)) { // GOTO for non-terminals
                    goto_table[{current_state_id, symbol}] = next_state_id;
                }
            }
        }
        cout << endl;
    }
}


void build_parsing_table() {
    cout << "--- Building LR(0) Parsing Table ---" << endl;
    for (int i = 0; i < canonical_collection.size(); ++i) {
        const ItemSet& state_items = canonical_collection[i];
        for (const Item& item : state_items) {
            // Case 1: Shift action [A -> α . a β] where a is a terminal
            string next_sym = item.next_symbol();
            if (!next_sym.empty() && terminals.count(next_sym)) {
                ItemSet next_state_candidate = goTo(state_items, next_sym);
                if(state_to_id.count(next_state_candidate)) {
                    int j = state_to_id[next_state_candidate];
                    if (action_table.count({i, next_sym}) &&
                        (action_table[{i, next_sym}].type != Action::SHIFT || action_table[{i, next_sym}].value != j)) {
                        // Conflict!
                        cerr << "Conflict at ACTION[" << i << ", " << next_sym << "]: existing ";
                        action_table[{i, next_sym}].print();
                        cerr << ", new s" << j << endl;
                        action_table[{i, next_sym}].type = Action::CONFLICT;
                    } else {
                        action_table[{i, next_sym}] = Action(Action::SHIFT, j);
                    }
                }
            }
            // Case 2: Reduce action [A -> α .]
            else if (item.dot_pos == item.prod.rhs.size() || (item.prod.rhs.empty() && item.dot_pos == 0) ) { // Dot at the end
                if (item.prod.lhs == augmented_start_symbol) { // Accept: [S' -> S .]
                    if (action_table.count({i, "$"}) && action_table[{i, "$"}].type != Action::ACCEPT) {
                         cerr << "Conflict at ACTION[" << i << ", $]: existing ";
                         action_table[{i,"$"}].print();
                         cerr << ", new acc" << endl;
                         action_table[{i, "$"}].type = Action::CONFLICT;
                    } else {
                        action_table[{i, "$"}] = Action(Action::ACCEPT, 0); // Value for accept is not crucial
                    }
                } else { // Reduce for A -> α
                    // For LR(0), reduce on ALL terminals if the item is complete.
                    for (const string& term_sym : terminals) { // Includes $
                         if (action_table.count({i, term_sym})) {
                             // Conflict!
                             cerr << "Conflict at ACTION[" << i << ", " << term_sym << "]: existing ";
                             action_table[{i, term_sym}].print();
                             cerr << ", new r" << item.prod.id << endl;
                             action_table[{i, term_sym}].type = Action::CONFLICT;
                         } else {
                            action_table[{i, term_sym}] = Action(Action::REDUCE, item.prod.id);
                         }
                    }
                }
            }
        }
    }

    // Fill GOTO table (some might have been set during canonical collection building, ensure all are set)
    for (int i = 0; i < canonical_collection.size(); ++i) {
        const ItemSet& current_I = canonical_collection[i];
        for (const string& symbol : non_terminals) {
            if (symbol == augmented_start_symbol && symbol != original_start_symbol) continue; // Don't compute goto on S' unless it's the original
            ItemSet next_set = goTo(current_I, symbol);
            if (!next_set.empty() && state_to_id.count(next_set)) {
                goto_table[{i, symbol}] = state_to_id[next_set];
            }
        }
    }
}

void print_parsing_table() {
    cout << "\n--- LR(0) Parsing Table ---" << endl;

    // Collect all unique terminal and non-terminal symbols for header
    vector<string> term_header;
    for(const auto& t : terminals) term_header.push_back(t);
    sort(term_header.begin(), term_header.end());
    // remove first "$" if it exists
    auto it = find(term_header.begin(), term_header.end(), "$");
    if (it != term_header.end()) {
        term_header.erase(it);
    }
    term_header.push_back("$"); // Add it back to the end

    vector<string> non_term_header;
     for(const auto& nt : non_terminals) {
        if (nt != augmented_start_symbol) {
            non_term_header.push_back(nt);
        }
    }
    sort(non_term_header.begin(), non_term_header.end());


    cout << "State | ";
    for (const auto& t : term_header) cout << setw(5) << left << t << " | ";
    cout << " | "; // Separator for GOTO
    for (const auto& nt : non_term_header) cout << setw(5) << left << nt << " | ";
    cout << endl;

    string line_sep = string(6, '-');
    for (size_t k=0; k < term_header.size(); ++k) line_sep += "+-------"; // Use size_t for loop counter with .size()
    line_sep += "+--"; // Separator for GOTO part
    for (size_t k=0; k < non_term_header.size(); ++k) line_sep += "+-------";
    line_sep += "-";
    cout << line_sep << endl;


    for (int i = 0; i < canonical_collection.size(); ++i) {
        cout << setw(5) << left << i << " | ";
        // ACTION part
        for (const auto& t : term_header) {
            if (action_table.count({i, t})) {
                string out_str;
                switch(action_table[{i, t}].type){
                    case Action::SHIFT: out_str = "s" + to_string(action_table[{i,t}].value); break;
                    case Action::REDUCE: out_str = "r" + to_string(action_table[{i,t}].value); break;
                    case Action::ACCEPT: out_str = "acc"; break;
                    case Action::ERROR: out_str = ""; break; // Blank for error
                    case Action::CONFLICT: out_str = "conf"; break;
                }
                cout << setw(5) << left << out_str << " | ";
            } else {
                cout << setw(5) << left << "" << " | "; // Empty cell
            }
        }
        cout << " | "; // Separator

        // GOTO part
        for (const auto& nt : non_term_header) {
            if (goto_table.count({i, nt})) {
                cout << setw(5) << left << goto_table[{i, nt}] << " | ";
            } else {
                cout << setw(5) << left << "" << " | "; // Empty cell
            }
        }
        cout << endl;
    }
    cout << endl;

    cout << "States (Item Sets):" << endl;
    for(int i=0; i < canonical_collection.size(); ++i) {
        cout << "I" << i << ":" << endl;
        for(const auto& item : canonical_collection[i]) {
            cout << "  "; item.print(); cout << endl;
        }
    }
}

// --- New Functions to Support Parsing Simulation ---

// Separates input string into discrete tokens and appends $ terminal marker
vector<string> tokenize_input(const string& input) {
    vector<string> tokens;
    stringstream ss(input);
    string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    if (tokens.empty() || tokens.back() != "$") {
        tokens.push_back("$");
    }
    return tokens;
}

// Parsing simulator driver utilizing LR(0) transition tables
void parse_input(const vector<string>& input_tokens) {
    cout << "\n--- Parsing Process ---" << endl;
    cout << setw(15) << left << "State Stack" 
         << setw(20) << left << "Symbol Stack" 
         << setw(20) << left << "Input" 
         << "Action" << endl;
    cout << string(75, '-') << endl;

    vector<int> state_stack = {0};
    vector<string> symbol_stack = {"$"};
    int ip = 0; // Input Pointer

    while (true) {
        int s = state_stack.back();
        string a = (ip < input_tokens.size()) ? input_tokens[ip] : "$";

        // Display state stack contents
        string state_str = "";
        for (int state : state_stack) {
            state_str += to_string(state) + " ";
        }

        // Display symbol stack contents
        string symbol_str = "";
        for (const string& sym : symbol_stack) {
            symbol_str += sym + " ";
        }

        // Display remaining input stream
        string input_str = "";
        for (int i = ip; i < input_tokens.size(); ++i) {
            input_str += input_tokens[i] + " ";
        }

        cout << setw(15) << left << state_str
             << setw(20) << left << symbol_str
             << setw(20) << left << input_str;

        if (!action_table.count({s, a})) {
            cout << "Error: No action for state " << s << " and input \"" << a << "\"" << endl;
            break;
        }

        Action act = action_table[{s, a}];

        if (act.type == Action::SHIFT) {
            cout << "Shift " << act.value << endl;
            state_stack.push_back(act.value);
            symbol_stack.push_back(a);
            ip++;
        } else if (act.type == Action::REDUCE) {
            Production prod = productions[act.value];
            cout << "Reduce using production " << prod.id << ": " << prod.lhs << " -> ";
            if (prod.rhs.empty() || (prod.rhs.size() == 1 && prod.rhs[0] == "epsilon")) {
                cout << "epsilon";
            } else {
                for (const auto& sym : prod.rhs) {
                    cout << sym << " ";
                }
            }
            cout << endl;

            // Determine RHS pop length
            int pop_len = prod.rhs.size();
            if (prod.rhs.size() == 1 && prod.rhs[0] == "epsilon") {
                pop_len = 0;
            }

            // Pop stacks
            for (int k = 0; k < pop_len; ++k) {
                if (!state_stack.empty()) state_stack.pop_back();
                if (!symbol_stack.empty()) symbol_stack.pop_back();
            }

            // GOTO step
            int prev_state = state_stack.back();
            if (!goto_table.count({prev_state, prod.lhs})) {
                cout << "Error: GOTO table entry not found for state " << prev_state << " and LHS " << prod.lhs << endl;
                break;
            }
            int next_state = goto_table[{prev_state, prod.lhs}];
            state_stack.push_back(next_state);
            symbol_stack.push_back(prod.lhs);
        } else if (act.type == Action::ACCEPT) {
            cout << "Accept" << endl;
            break;
        } else if (act.type == Action::CONFLICT) {
            cout << "Error: Shift/Reduce or Reduce/Reduce conflict encountered." << endl;
            break;
        } else {
            cout << "Error: Invalid parse table action." << endl;
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "用法: " << argv[0] << " <文法文件名> [待分析字符串]" << endl;
        return 1;
    }
    
    load_grammar(argv[1]);
    
    // Command line argument overrides file parsing parameter if available
    if (argc >= 3) {
        input_string = "";
        for (int i = 2; i < argc; ++i) {
            if (i > 2) input_string += " ";
            input_string += argv[i];
        }
    }
    
    build_canonical_collection();
    build_parsing_table();
    print_parsing_table();

    if (!input_string.empty()) {
        cout << "Input String: " << input_string << endl;
        vector<string> tokens = tokenize_input(input_string);
        parse_input(tokens);
    } else {
        cout << "提示: 尚未配置待分析字符串。您可以通过参数传入或在文法文件尾部独立写入一行。" << endl;
    }

    return 0;
}