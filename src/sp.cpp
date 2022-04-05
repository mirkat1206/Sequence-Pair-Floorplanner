// sp.cpp
#include "sp.h"

SequencePair::SequencePair(ifstream &fin_blk, ifstream &fin_net) {
   this->ParseBlk(fin_blk);
   this->ParseNet(fin_net);
   DEBUG_MSG("Parse finished...");
}

SequencePair::~SequencePair() {
    for (int i = 0; i < num_blocks_; ++i)
        delete block_list_[i];
    for (int i = 0; i < num_terminals_; ++i)
        delete terminal_list_[i];
    for (int i = 0; i < num_nets_; ++i)
        delete net_list_[i];
}

/* -------------------- solver -------------------- */
void SequencePair::Solve() {
    
}

/* -------------------- input and output -------------------- */
void SequencePair::ParseBlk(ifstream &fin) {
    string s, name;
    int w, h, x, y;
    // Outline: <outline width> <outline height>
    fin >> s;
    if (s != "Outline:")    
        this->ParseError(0);
    fin >> W >> H;
    // NumBlocks: <number of blocks>
    fin >> s;
    if (s != "NumBlocks:")
        this->ParseError(1);
    fin >> num_blocks_;
    // NumTerminals: <number of terminals>
    fin >> s;
    if (s != "NumTerminals:")
        this->ParseError(2);
    fin >> num_terminals_;
    // <block name> <block width> <block height>
    for (int i = 0; i < num_blocks_; ++i) {
        fin >> name >> w >> h;
        Block* b = new Block(name, w, h);
        block_list_.push_back(b);
        name2terminal_.insert(pair<string, Terminal*>(name, b));
    }
    // <terminal name> terminal <terminal x coordinate> <terminal y coordinate>
    for (int i = 0; i < num_terminals_; ++i) {
        fin >> name >> s >> x >> y;
        if (s != "terminal")
            this->ParseError(3);
        Terminal* t = new Terminal(name, x, y);
        terminal_list_.push_back(t);
        name2terminal_.insert(pair<string, Terminal*>(name, t));
    }
}

void SequencePair::ParseNet(ifstream &fin) {
    string s, name;
    int num_degree;
    // NumNets: <number of nets>
    fin >> s;
    if (s != "NumNets:")
        this->ParseError(4);
    fin >> num_nets_;
    // NetDegree: <number of terminals in this net>
    // <terminal name>
    for (int i = 0; i < num_nets_; ++i) {
        fin >> s;
        if (s != "NetDegree:")
            this->ParseError(5);
        Net* n = new Net;
        fin >> num_degree;
        for (int j = 0; j < num_degree; ++j) {
            fin >> name;
            Terminal* t = name2terminal_[name];
            n->AddTerminal(t);
        }
        net_list_.push_back(n);
    }
}

void SequencePair::ParseError(int code) {
    if (code < 4)   cerr << "Input.block file format error("<< code << ")." << endl;
    else            cerr << "Input.net file format error(" << code << ")." << endl;
    cerr << "The program will be terminated..." << endl;
    exit(1);
}

void SequencePair::WriteReport(ofstream &fout) {
    // <final cost> 
    // <total wirelength>
    // <chip_area>
    // <chip_width> <chip_height>
    // <program_runtime>
    // <macro_name>    <x1>    <y1>    <x2>      <y2> 
}
