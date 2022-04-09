// sp.h
#ifndef SP_H
#define SP_H

#define DEBUG

#ifdef DEBUG
#define DEBUG_MSG(str) do { cout << str << endl; } while (false)
#else
#define DEBUG_MSG(str) do { } while (false)
#endif

#include "module.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

class SequencePair {
public:
    // constructor and destructor
    SequencePair(ifstream &fin_blk, ifstream &fin_net, double alpha);
    ~SequencePair();

    // input and output
    void ParseBlk(ifstream &fin);
    void ParseNet(ifstream &fin);
    void ParseError(int code);
    void WriteReport(ofstream &fout, double time_taken);

    // solver
    void Solve();
    void Rotate90();    // 90 degree clockwise rotation
    void RandomInitialize();
    int EvaluateSequence(bool mode);    // 0: compute x coordinates, 1: comput y coordinates
    double Cost(int w, int h);
    size_t Area();
    double Wirelength();
    double HPWL(Net* net);

private:
    // basic information
    double alpha_;
    int W_, H_;   // boundary constraint
    int num_blocks_;
    int num_terminals_;
    int num_nets_;
    vector<Block*> block_list_;
    vector<Terminal*> terminal_list_;
    vector<Net*> net_list_;
    map<string, Terminal*> name2terminal_;
    map<string, Block*> name2block_;

    // sequence pair    
    bool has_legal_;
    vector<size_t> X_, Y_;
    int max_width_, max_height_;
};

#endif
