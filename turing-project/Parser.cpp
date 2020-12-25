#include "TuringMachine.h"

/*  input: arguments from command line
    return: 0 or -1, path of tm file, input string, and tm mode
    usage: parse the command line instruction */
int parse_cli(int argc,char *argv[], string &path, string& input, int &mode) {
    // turing [-h|--help]
    if(argc < 2 || argc > 5) {
        cerr << "illegal order" << endl;
        return -1;
    }

    if(argc == 2) {
        // turing [-h|--help]
        if(strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"--help") == 0) {
            cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
        }
        else {
            cerr << "illegal order" << endl;
            return -1;
        }
    }
    else if(argc == 3) {
        // turing <tm> <input>
        path = string(argv[1]);
        input = string(argv[2]);
    }
    else if(argc == 4) {
        // turing [-v|--verbose] <tm> <input> or turing [-h|--help] <tm> <input>
        if(strcmp(argv[1],"-v") == 0 || strcmp(argv[1],"--verbose") == 0) {
            mode = 1;
            path = string(argv[2]);
            input = string(argv[3]);
        }
        else if(strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"--help") == 0) {
            path = string(argv[2]);
            input = string(argv[3]);
            cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
        }
        else {
            cerr << "illegal order" << endl;
            return -1;
        }
    }
    else if(argc == 5) {
        if((strcmp(argv[1],"-v") == 0 || strcmp(argv[1],"--verbose") == 0) && (strcmp(argv[2],"-h") == 0 || strcmp(argv[2],"--help") == 0)) {
            mode = 1;
            path = string(argv[3]);
            input = string(argv[4]);
            cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
        }
        else if((strcmp(argv[2],"-v") == 0 || strcmp(argv[2],"--verbose") == 0) && (strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"--help") == 0)) {
            mode = 1;
            path = string(argv[3]);
            input = string(argv[4]);
            cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
        }
        else {
            cerr << "illegal order" << endl;
            return -1;
        }
    }
    
    return 0;
}

/*  input: char c, string str
    return: index of the first c in str
    usage: find the first c in str */
int find(char c, char* str) {
    for(int i = 0; i < strlen(str);i++) {
        if(str[i]==c) {
            return i;
        }
    }
    return -1;
} 

/*  input: char ch, string str
    return: vector of strings split by ch
    usage: spilit str by ch */
int split(char ch, char* str, vector<string> &vec) {
    int i = 0;
    int j = 0;
    char *temp = nullptr;
    for(; j < strlen(str); j++) {
        if(str[j] == ch) {
            str[j] = '\0';
            temp = new char[j - i + 1];
            strcpy(temp, str + i);
            if(temp[0] != '\0')
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

/* usage: output syntax error information */
void syntax_err(string path, int line_num, string err, char* line, int mode) {
    if(line_num == -1) {
        if(mode) {
            cerr << "syntax error: " << err << endl;
        }
        else {
            cerr << "syntax error" << endl;
        }
    }
    else {
        if(mode) {
            cerr << path << ": line " << line_num << ": "
                << "syntax error: " << err << endl
                << "\t" << line << endl;
        }
        else {
            cerr << "syntax error" << endl;
        }
    }
    
}

/*  input: path of tm file, TM
    return: 0 or -1
    usage: parse tm file */
int parse_tm(string path, TuringMachine* tm, int mode) {
    ifstream file(path.c_str());
    if(!file) {
        cerr << path << " is not a legal tm file." << endl;
        return -1;
    }
    char line[1024] = {0};
    regex r_blank_line("[\\s]*");
    regex r_states("#Q = \\{[a-zA-Z0-9_]+(,[a-zA-Z0-9_]+)*\\}");
    regex r_input_symbol("#S = \\{[^\\s,;\\*_\\{\\}](,[^\\s,;\\*_\\{\\}])*\\}");
    regex r_tape_symbol("#G = \\{[^\\s,;\\*\\{\\}](,[^\\s,;\\*\\{\\}])*\\}");
    regex r_start_state("#q0 = [a-zA-Z0-9_]+");
    regex r_blank("#B = _");
    regex r_final_states_0("#F = \\{\\}");
    regex r_final_states("#F = \\{[a-zA-Z0-9_]+(,[a-zA-Z0-9_]+)*\\}");
    regex r_tape_num("#N = [1-9]\\d*");
    //old state, old symbols, new symbols, direction, new state
    regex r_transition("[a-zA-Z0-9_]+ [^\\s,;\\*\\{\\}]+ [^\\s,;\\*\\{\\}]+ [rl\\*]+ [a-zA-Z0-9_]+");

    int line_num = 0;
    while(!file.eof()) {
        file.getline(line,1024);
        line_num++;
        if(regex_match(line, r_blank_line) || line[0] == ';')
            continue;

        // delete comment at the end of line
        int k = find(';', line);
        if(k != -1)
            line[k] = '\0';
        // delete invible characters at the end of line
        for (int j = strlen(line) - 1; j >= 0; j--) {
            if(line[j] == ' '|| line[j] == '\t' || line[j] == '\n' || line[j] == '\r' || line[j] == '\f' || line[j] == '\v')
                line[j] = '\0';
            else
                break;
        }

        int st = -1;
        int i = -1;
        char *temp = nullptr;
        if (regex_match(line, r_states)) {
            if(tm->states.size() != 0) {
                syntax_err(path, line_num, "redefinition of states", line, mode);
                return -1;
            }
            st = find('{', line);
            line[strlen(line) - 1] = ',';
            split(',', line + st + 1, tm->states);
            line[strlen(line) - 1] = '}';
            if(tm->start_state != "" && test_tm(tm, 3)) {
                syntax_err(path, line_num, "start state is not in the set of states", line, mode);
                return -1;
            }
            if(tm->final_states.size() != 0 && test_tm(tm, 4)) {
                syntax_err(path, line_num, "set of final states is not a subset of set of states", line, mode);
                return -1;
            }
        }
        else if(regex_match(line,r_input_symbol)) {
            if(tm->input_alphabet.size() != 0) {
                syntax_err(path, line_num, "redefinition of input symbols", line, mode);
                return -1;
            }
            st = find('{', line);
            i = st + 1;
            for(int j = st + 1; j < strlen(line); j++) {
                if(line[j] == ',' || line[j] == '}') {
                    tm->input_alphabet.push_back(line[j - 1]);
                }
            }
            if(tm->tape_alphabet.size() != 0 && test_tm(tm, 2)) {
                syntax_err(path, line_num, "set of input symbols is not a subset of set of tape symbols", line, mode);
                return -1;
            }
        }
        else if(regex_match(line, r_tape_symbol)) {
            if(tm->tape_alphabet.size() != 0) {
                syntax_err(path, line_num, "redefinition of tape symbols", line, mode);
                return -1;
            }
            st = find('{', line);
            i = st + 2;
            for(int j = st + 1; j < strlen(line); j++) {
                if(line[j] == ',' || line[j] == '}') {
                    tm->tape_alphabet.push_back(line[j - 1]);
                }
            }
            if(tm->input_alphabet.size() != 0 && test_tm(tm, 2)) {
                syntax_err(path, line_num, "set of input symbols is not a subset of set of tape symbols", line, mode);
                return -1;
            }
        }
        else if(regex_match(line, r_start_state)) {
            st = find('=', line);
            temp = new char[strlen(line) - st];
            strcpy(temp, line + st + 2);
            if(tm->start_state != "" && tm->start_state != string(temp)) {
                syntax_err(path, line_num, "redefinition of start state", line, mode);
                return -1;
            }
            tm->start_state = string(temp);
            if(tm->states.size() != 0 && test_tm(tm, 3)) {
                syntax_err(path, line_num, "start state is not in the set of states", line, mode);
                return -1;
            }
        }
        else if(regex_match(line, r_final_states) || regex_match(line, r_final_states_0)) {
            if(tm->final_states.size() != 0) {
                syntax_err(path, line_num, "redefinition of final states", line, mode);
                return -1;
            }
            st = find('{', line);
            line[strlen(line) - 1] = ',';
            split(',', line + st + 1, tm->final_states);
            line[strlen(line) - 1] = '}';
            if(tm->states.size() != 0 && test_tm(tm, 4)) {
                syntax_err(path, line_num, "set of final states is a subset of set of states", line, mode);
                return -1;
            }
        }
        else if(regex_match(line, r_blank)) {
            continue;
        }
        else if(regex_match(line, r_tape_num)) {
            st = find('=', line);
            temp = new char[strlen(line) - st];
            strcpy(temp, line + st + 2);
            int n = atoi(temp);
            if(n < 0) {
                syntax_err(path, line_num, "The number of tapes is not a positive integer", line, mode);
                return -1;
            }
            if(tm->tape_num > 0 and tm->tape_num != n) {
                syntax_err(path, line_num, "redefinition of the number of tapes", line, mode);
                return -1;
            }
            tm->tape_num = n;
        }
        else if(regex_match(line, r_transition)) {
            vector<string> vec;
            split(' ', line, vec);
            if(tm->states.size() != 0) {
                bool f1 = false, f2 = false;
                for(auto it = tm->states.begin(); it != tm->states.end(); it++) {
                    if(*it == vec[0])
                        f1 = true;
                    if(*it == vec[4])
                        f2 = true;
                }
                if(!f1 || !f2){
                    syntax_err(path, line_num, "current state or new state is not in the set of states", line, mode);
                    return -1;
                }
            }
            if(tm->tape_num > 0 && (vec[1].size() != tm->tape_num || vec[2].size() != tm->tape_num || vec[3].size() != tm->tape_num)) {
                syntax_err(path, line_num, "number of symbols under heads, new symbols or directions is not the same as number of tapes", line, mode);
                return -1;
            }
            pair<string, string> key(vec[0], vec[1]);
            TransitionTuple val = TransitionTuple(vec[2], vec[3], vec[4]);
            auto it = tm->transition.find(key);
            if(it != tm->transition.end()) {
                if(!(it->second == val)) {
                    syntax_err(path, line_num, "redefinition of transition function", line, mode);
                    return -1;
                }
            }
            else {
                tm->transition[key] = val;
            }
        }
        else{
            syntax_err(path, line_num, "illegal line", line, mode);
            return -1;
        }
    }

    return 0;
}

/*  input: TM, label, tm mode
    return: 0 or -1
    usage: test if TM is valid */
int test_tm(TuringMachine* tm, int label, int mode) {
    if(label == 1) {
        // test all
        // test tape_num
        if(tm->tape_num < 1) {
            syntax_err("", -1, "number of tapes is not a postive integer", nullptr, mode);
            return -1;
        }
        // test blank
        bool f = false;
        for(int j = 0; j < tm->tape_alphabet.size(); j++) {
            if(tm->tape_alphabet[j] == tm->blank) {
                f = true;
                break;
            }
        }
        if(!f) {
            syntax_err("", -1, "blank symbol is not in the set of tape symbols", nullptr, mode);
            return -1;
        }
        // test input_symbols and tape_symbols
        if(tm->input_alphabet.size() == 0) {
            syntax_err("", -1, "set of input symbols is undefined", nullptr, mode);
            return -1;
        }
        if(tm->tape_alphabet.size() == 0) {
            syntax_err("", -1, "set of tape symbols is undefined", nullptr, mode);
            return -1;
        }
        // test start_state
        if(tm->start_state == "") {
            syntax_err("", -1, "start state is undefined", nullptr, mode);
            return -1;
        }
        if(tm->states.size() == 0) {
            syntax_err("", -1, "set of states is undefined", nullptr, mode);
            return -1;
        }
        // test states and final_states
        /*if(tm->final_states.size() == 0) {
            syntax_err("", -1, "set of final states is undefined", nullptr, mode);
            return -1;
        }*/
    }
    else if(label == 2) {
        // test input_symbols and tape_symbols
        for(int i = 0; i < tm->input_alphabet.size(); i++) {
            bool f1 = false;
            char ch = tm->input_alphabet[i];
            for(int j = 0; j < tm->tape_alphabet.size(); j++) {
                if(ch == tm->tape_alphabet[j]) {
                    f1 = true;
                    break;
                }
            }
            if(!f1)
                return -1;
        }
    }
    else if(label == 3) {
        // test start_state
        bool f2 = false;
        for(int j = 0; j < tm->states.size(); j++) {
            if(tm->start_state == tm->states[j]) {
                f2 = true;
                break;
            }
        }
        if(!f2)
            return -1;
    }
    else if(label == 4) {
        // test states and final_states
        for(int i = 0; i < tm->final_states.size(); i++) {
            bool f3 = false;
            for(int j = 0; j < tm->states.size(); j++) {
                if(tm->final_states[i] == tm->states[j]) {
                    f3 = true;
                    break;
                }
            }
            if(!f3)
                return -1;
        }
    }

    return 0;
}