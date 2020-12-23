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
            cerr << "error: '" << s << "' was not declared in the set of input symbols";
            return -1;
        }
    }
    int n = tm->tape_num;
    tm->state = tm->start_state;
    tm->heads.resize(n, 0);
    tm->tapes.resize(n, "_");
    tm->tapes[0] = input;

    return 0;
}

bool halt(TuringMachine* tm) {
    
    return true;
}

/* usage: simulate TM and decide the input */
int simulate_tm(TuringMachine* tm, int mode) {
    if(mode == 0) {
        // ordinary mode
        while(!halt(tm)) {

        }
    }
    else {
        // verbose mode
    }
    return 0;
}