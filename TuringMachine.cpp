#include "TuringMachine.h"


int main(int argc,char *argv[]) {
    int ret = 0;
    int mode = 0;
    string filename;
    string input;
    TuringMachine tm;

    // parse tm file
    ret = parse_cli(argc, argv, filename, input, mode);
    if(ret == -1)
        return -1;
    ret = parse_tm(filename, &tm, mode);
    if(ret == -1)
        return -1;
    ret = test_tm(&tm);
    if(ret == -1)
        return -1;

    // simulate the TM and decide the input
    ret = initialize_tm(&tm, input, mode);
    if(ret == -1)
        return -1;
    ret = simulate_tm(&tm, mode);
    if(ret == -1)
        return -1;


    return 0;
}