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

/* usage: calculate digits of n*/
int digit_num(int n) {
    int ret = 1;
    if(n < 0)
        n = -n;
    n = n / 10;
    while(n >= 1) {
        ret++;
        n = n / 10;
    }
    return ret;
}

/*  return: <halt, accept>
    usage: decide if TM has halted */
pair<bool, bool> halt(TuringMachine* tm, TransitionTuple &result) {
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
    int digits = digit_num(tm->tape_num - 1);

    // TODO: What if TM never halts?
    while(true) {
        if(mode == 1) {
            // verbose mode
            int st = 0, ed = 0;
            printf("%-*s: %d\n", digits+6, "Step", step);
            for (int i = 0; i < tm->tape_num; i++) {
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
                    // if head points to blank
                    if(tm->heads[i] + tm->index0[i] < st) {
                        st = tm->heads[i] + tm->index0[i];
                    }
                    if(tm->heads[i] + tm->index0[i] > ed) {
                        ed = tm->heads[i] + tm->index0[i];
                    }
                }

                // st and ed are indexes in tm->tapes[i]
                printf("Index%-*d:", digits+1, i);
                for (j = st - tm->index0[i]; j <= ed - tm->index0[i]; j++) {
                    printf(" %d", abs(j));
                }
                int blank_num = 0;
                printf("\nTape%-*d:", digits+2, i);
                for (j = st; j <= ed; j++) {
                    int d = digit_num(j - tm->index0[i]);
                    printf(" %-*c", d, tm->tapes[i][j]);
                    if(j < tm->heads[i] - st + tm->index0[i])
                        blank_num += (1 + d);
                }
                blank_num += 1;
                printf("\nHead%-*d:", digits+2, i);
                printf("%-*s^\n", blank_num, "");
            }
            printf("%-*s: %s\n", digits+6, "State", tm->state.c_str());
            printf("---------------------------------------------\n");
        }
        
        step++;
        f_halt_acc = halt(tm, result);
        if(f_halt_acc.first) {
            int st = -1, ed = -1;
            int j;
            for (j = 0; j < tm->tapes[0].size(); j++) {
                if(tm->tapes[0][j] != tm->blank)
                    break;
            }
            if(j != tm->tapes[0].size()) {
                // if tape is all blank
                st = j;
                for (j = tm->tapes[0].size() - 1; j > st; j--) {
                    if(tm->tapes[0][j] != tm->blank)
                        break;
                }
                ed = j;
            }
            string temp = "";
            if(st >= 0 && ed < tm->tapes[0].size() && st <= ed) {
                for(j = st; j <= ed; j++)
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