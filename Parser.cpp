#include "TuringMachine.h"

/*  input: arguments from command line
    return: 0 or -1, path of tm file, input string, and tm mode
    usage: parse the command line instruction */
int parse_cli(int argc,char *argv[], string &path, string& input, int &mode) {
    // turing [-h|--help]
    if(argc < 2 || argc > 4) {
        // TODO
        return -1;
    }
    if(argc == 2) {
        if(strcmp(argv[1],"-h")==0 || strcmp(argv[1],"--help")==0) {
            cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>";
        }
        else {
            // TODO
            cerr << "illegal order";
            return -1;
        }
    }
    else if(argc == 3) {
        // turing <tm> <input>
        path = string(argv[1]);
        input = string(argv[2]);
    }
    else if(argc == 4) {
        // turing [-v|--verbose] <tm> <input>
        if(strcmp(argv[1],"-v")==0 || strcmp(argv[1],"--verbose")==0) {
            mode = 1;
        }
        else {
            // TODO
            cerr << "illegal order";
            return -1;
        }
    }
    return 0;
}

/* usage: find the first c in str */
int find(char c, char* str) {
    for (int i = 0; i < strlen(str);i++){
        if(str[i]==c){
            return i;
        }
    }
    return -1;
} 

/* usage: spilit str by ch */
int split(char ch, char* str, vector<string> &vec) {
    int i = 0;
    int j = 0;
    char *temp = nullptr;
    for(; j < strlen(str); j++) {
        if(str[j] == ch) {
            str[j] = '\0';
            temp = new char[j - i + 1];
            strcpy(temp, str + i);
            vec.push_back(string(temp));
            str[j] = ch;
            i = j + 1;
        }
    }

    j--;
    if(i != j + 1) {
        temp = new char[j - i + 1];
        strcpy(temp, str + i);
        vec.push_back(string(temp));
    }
    return vec.size();
}

/*  input: path of tm file, TM
    return: 0 or -1
    usage: Parse tm file */
int parse_tm(string path, TuringMachine* tm, int mode) {
    // TODO:verbose模式
    ifstream file(path.c_str());
    if(!file) {
        cerr << path << " is not a legal tm file.";
        return -1;
    }
    char line[1024] = {0};
    regex r_states("#Q = \\{[a-zA-Z0-9_]+(,[a-zA-Z0-9_]+)*\\}"); //\\s*;[\\s\\S]*
    regex r_input_symbol("#S = \\{[^\\s,;\\*_\\{\\}](,[^\\s,;\\*_\\{\\}])*\\}");
    regex r_tape_symbol("#G = \\{[^\\s,;\\*\\{\\}](,[^\\s,;\\*\\{\\}])*\\}");
    regex r_start_state("#q0 = [a-zA-Z0-9_]+");
    regex r_blank("#B = _");
    regex r_final_states("#F = \\{[a-zA-Z0-9_]+(,[a-zA-Z0-9_]+)*\\}");
    regex r_tape_num("#N = [1-9]\\d*");
    //old state, old symbols, new symbols, direction, new state
    regex r_transition("[a-zA-Z0-9_]+ [^\\s,;\\*\\{\\}]+ [^\\s,;\\*\\{\\}]+ [rl\\*]+ [a-zA-Z0-9_]+");
    
    while(!file.eof()) {
        file.getline(line,1024);
        if(strlen(line) == 0 || line[0] == ';') {
            // TODO:其他无效行
            continue;
        }
        // 处理代码后的注释
        int k = find(';', line);
        if(k != -1){
            line[k] = '\0';
        }
        for (int j = strlen(line) - 1; j >= 0; j--) {
            // TODO: 不可见字符
            if(line[j] == ' '|| line[j] == '\t')
                line[j] = '\0';
            else
                break;
        }

        int st = -1;
        int i = -1;
        char *temp = nullptr;
        if (regex_match(line, r_states)) {
            cout << line << endl;
            st = find('{', line);
            line[strlen(line) - 1] = ',';
            split(',', line + st + 1, tm->states);
            line[strlen(line) - 1] = '}';
            /*for (auto it = tm->states.begin(); it != tm->states.end();it++){
                cout << it->c_str() << " ";
            }*/
        }
        else if(regex_match(line,r_input_symbol)) {
            cout << line << endl;
            st = find('{', line);
            i = st + 1;
            for(int j = st + 1; j < strlen(line); j++) {
                if(line[j] == ',' || line[j] == '}') {
                    tm->input_alphabet.push_back(line[j - 1]);
                }
            }
        }
        else if(regex_match(line, r_tape_symbol)) {
            cout << line << endl;
            st = find('{', line);
            i = st + 2;
            for(int j = st + 1; j < strlen(line); j++) {
                if(line[j] == ',' || line[j] == '}') {
                    tm->tape_alphabet.push_back(line[j - 1]);
                }
            }
        }
        else if(regex_match(line, r_start_state)) {
            cout << line << endl;
            st = find('=', line);
            temp = new char[strlen(line) - st];
            strcpy(temp, line + st + 2);
            tm->start_state = string(temp);
        }
        else if(regex_match(line, r_final_states)) {
            cout << line << endl;
            st = find('{', line);
            line[strlen(line) - 1] = ',';
            split(',', line + st + 1, tm->final_states);
            line[strlen(line) - 1] = '}';
        }
        else if(regex_match(line, r_blank)) {
            cout << line << endl;
            continue;
        }
        else if(regex_match(line, r_tape_num)) {
            cout << line << endl;
            st = find('=', line);
            temp = new char[strlen(line) - st];
            strcpy(temp, line + st + 2);
            int n = atoi(temp);
            if(n < 0) {
                cerr << "The number of tapes must be a positive integer.";
                return -1;
            }
            tm->tape_num = n;
        }
        else if(regex_match(line, r_transition)) {
            cout << line << endl;
            vector<string> vec;
            split(' ', line, vec);
            if(vec[1].size() != vec[2].size() || vec[2].size() != vec[3].size()) {
                cerr << "syntax error";
                cout << line << endl;
                return -1;
            }
            pair<string, string> key(vec[0], vec[1]);
            TransitionTuple val = TransitionTuple(vec[2], vec[3], vec[4]);
            auto it = tm->transition.find(key);
            if(it != tm->transition.end()) {
                if(it->second == val) {
                    cerr << "syntax error: redefinition of transition function";
                    return -1;
                }
            }
            else {
                tm->transition[key] = val;
            }
        }
        else{
            cerr << "syntax error";
            cout << line << endl;
            return -1;
        }
    }

    return 0;
}

/* usage: test if TM is valid */
int test_tm(TuringMachine* tm) {
    // TODO: 检验图灵机有效性
    return 0;
}