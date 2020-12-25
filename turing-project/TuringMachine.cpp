#include "TuringMachine.h"


int main(int argc,char *argv[]) {
    int mode = 0;
    string filename;
    string input;
    TuringMachine tm;

    // parse tm file
    if(parse_cli(argc, argv, filename, input, mode) == -1)
        return -1;
    else if(argc == 2)  // [-h|--help]
        return 0;
    if(parse_tm(filename, &tm, mode) == -1)
        return -1;
    if(test_tm(&tm, 1, mode) == -1)
        return -1;

    // simulate the TM and decide the input
    if(initialize_tm(&tm, input, mode) == -1)
        return -1;
    if(simulate_tm(&tm, input, mode) == -1)
        return -1;

    return 0;
}