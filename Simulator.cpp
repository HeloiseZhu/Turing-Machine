#include "TuringMachine.h"

/* usage: get TM ready to decide the input */
int initialize_tm(TuringMachine* tm, string input, int mode) {
    char s;
    int i, j;

    for (i = 0; i < input.size(); i++) {
        s = input[i];
        for (j = 0; j < tm->input_alphabet.size(); j++) {
            if(s == tm->input_alphabet[j])
                break;
        }
        if(j == tm->input_alphabet.size()) {
            if(mode == 1) {
                cerr << "Input: " << input << endl
                    << "==================== ERR ====================" << endl
                    << "error: '" << s << "' was not declared in the set of input symbols" << endl
                    << "Input: " << input << endl;
                for (int k = 0; k < 7 + i; k++) {
                    cerr << ' ';
                }
                cerr << '^' << endl
                    << "==================== END ====================" << endl;
            }
            else {
                cerr << "illegal input" << endl;
            }
            return -1;
        }
    }
    int n = tm->tape_num;
    tm->state = tm->start_state;
    tm->heads.resize(n, 0);
    tm->index0.resize(n, 0);
    tm->tapes.resize(n, "_");
    if(input == "")
        tm->tapes[0] = "_";
    else
        tm->tapes[0] = input;

    if(mode == 1) {
        cout << "Input: " << input << endl
            << "==================== RUN ====================" << endl;
    }

    return 0;
}

/*  return: <halt, accept>
    usage: decide if TM has halted */
pair<bool, bool> halt(TuringMachine* tm, TransitionTuple &result) {
    // TODO: halt & accpt ???
    bool halt = false;
    bool accept = false;
    string symbols;

    for(int i = 0; i < tm->tape_num; i++) {
        int vec_idx = tm->heads[i] + tm->index0[i];
        if(vec_idx < 0 || vec_idx >= tm->tapes[i].size())
            symbols.push_back('_');
        else
            symbols.push_back(tm->tapes[i][vec_idx]);
    }
    pair<string, string> key(tm->state, symbols);
    //cout << "key: " << key.first << " " << key.second << endl;
    auto it = tm->transition.find(key);
    if(it != tm->transition.end()) {
        result.state = it->second.state;
        result.symbols = it->second.symbols;
        result.directions = it->second.directions;
        return pair<bool, bool>(false, false);
    }
    else {
        halt = true;
        for (int i = 0; i < tm->final_states.size(); i++) {
            if(tm->state == tm->final_states[i]) {
                accept = true;
                break;
            }
        }    
    }
    return pair<bool, bool>(halt, accept);
}

/* usage: simulate TM and decide the input */
int simulate_tm(TuringMachine* tm, string input, int mode) {
    int step = 0;
    pair<bool, bool> f_halt_acc(false, false);
    TransitionTuple result;
    // TODO: 不停机怎么办？
    while(true) {
        if(mode == 1) {
            // verbose mode
            int st = 0, ed = 0;
            cout << "Step   : " << step << endl;
            for (int i = 0; i < tm->tape_num; i++) {
                //cout << "tape" << i << ": " << tm->tapes[i] << endl;
                st = -1;
                ed = -1;
                int j;
                for (j = 0; j < tm->tapes[i].size(); j++) {
                    if(tm->tapes[i][j] != tm->blank)
                        break;
                }
                if(j == tm->tapes[i].size()) {
                    // tape is all blank
                    st = tm->heads[i] + tm->index0[i];
                    ed = tm->heads[i] + tm->index0[i];
                }
                else {
                    st = j;
                    for (j = tm->tapes[i].size() - 1; j > st; j--) {
                        if(tm->tapes[i][j] != tm->blank)
                            break;
                    }
                    ed = j;
                    // TODO: 考虑读写头指向左右空格的情况
                    // 实际上会确保读写头指向的单元一定在数组中
                    // st和ed都是数组中的下标
                    if(tm->heads[i] + tm->index0[i] < st) {
                        st = tm->heads[i] + tm->index0[i];
                    }
                    if(tm->heads[i] + tm->index0[i] > ed) {
                        ed = tm->heads[i] + tm->index0[i];
                    }
                }
                
                //cout << "st & ed: " << st << " " << ed << endl;
                cout << "Index" << i << " :";
                for (j = st - tm->index0[i]; j <= ed - tm->index0[i]; j++) {
                    if(j < 0)
                        cout << " " << -j;
                    else
                        cout << " " << j;
                }
                cout << endl
                     << "Tape" << i << "  : ";
                for (j = st; j <= ed; j++) {
                    if(j == ed)
                        cout <<  tm->tapes[i][j];
                    else {
                        cout << tm->tapes[i][j] << " ";
                        int a = abs(j - tm->index0[i]);
                        a = a / 10;
                        while(a >= 1) {
                            cout << " ";
                            a = a / 10;
                        }
                    }
                }
                cout << endl
                     << "Head" << i << "  :";
                for(j = 0; j < tm->heads[i] - st + tm->index0[i];j++) {
                    cout << "  ";
                }
                cout << " ^" << endl;
            }
            cout << "State  : " << tm->state << endl;
            cout << "---------------------------------------------" << endl;    
        }
        
        step++;
        f_halt_acc = halt(tm, result);
        //cout << "transition: " << result.state << " " << result.symbols << " " << result.directions << endl;
        if(f_halt_acc.first) {
            int st = -1, ed = -1;
            int j;
            for (j = 0; j < tm->tapes[0].size(); j++) {
                if(tm->tapes[0][j] != tm->blank)
                    break;
            }
            if(j != tm->tapes[0].size()) {
                // TODO: if tape is all blank
                st = j;
                for (j = tm->tapes[0].size() - 1; j > st; j--) {
                    if(tm->tapes[0][j] != tm->blank)
                        break;
                }
                ed = j;
            }
            string temp;
            for(j = st; j <= ed; j++) {
                temp.push_back(tm->tapes[0][j]);
            }
            if(mode == 1) {
                cout << "Result: " << temp << endl
                     << "==================== END ====================" << endl;
            }
            else {
                cout << temp << endl;
            }
            break;
        }
        
        // make transition(state, rewrite, heads)
        tm->state = result.state;
        for(int i = 0; i < tm->tape_num; i++) {
            tm->tapes[i][tm->heads[i] + tm->index0[i]] = result.symbols[i];
            if(result.directions[i] == 'l') {
                tm->heads[i]--;
                if(tm->heads[i] + tm->index0[i] < 0) {
                    tm->tapes[i] = "_" + tm->tapes[i];
                    tm->index0[i]++;
                }
            }
            else if(result.directions[i] == 'r') {
                tm->heads[i]++;
                if(tm->heads[i] + tm->index0[i] >= tm->tapes[i].size()) {
                    tm->tapes[i] = tm->tapes[i] + "_";
                }
            }
        }
    }
    
    return 0;
}