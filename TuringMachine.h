#ifndef TURINGMACHINE_H
#define TURINGMACHINE_H

#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <regex>
using namespace std;


struct TransitionTuple {
public:
    string state;
    string symbols;
    string directions;

    TransitionTuple(string st, string sym, string dir) {
        state = st;
        symbols = sym;
        directions = dir;
    }

    bool operator==(const TransitionTuple& a) {
        if(a.state == this->state && a.symbols == this->symbols && a.directions == this->directions)
            return true;
        else
            return false;
    }
};

class TuringMachine {
public:
    char blank;
    int tape_num;
    string state;
    string start_state;
    vector<string> states;
    vector<string> final_states;
    vector<char> input_alphabet;
    vector<char> tape_alphabet;
    vector<string> tapes;
    vector<int> heads;
    map<pair<string, string>, TransitionTuple> transition;

    TuringMachine() {
        blank = '_';
        tape_num = 1;
    }
};



// Parser
int parse_cli(int argc, char *argv[], string &path, string &input, int &mode);
int parse_tm(string path, TuringMachine *tm, int mode);
int test_tm(TuringMachine *tm);

// Simulator
int initialize_tm(TuringMachine *tm, string input, int mode);
bool halt(TuringMachine *tm);
int simulate_tm(TuringMachine *tm, int mode);



#endif