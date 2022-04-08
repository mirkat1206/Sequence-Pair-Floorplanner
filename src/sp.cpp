// sp.cpp
#include "sp.h"
#include "veb.h"
#include <algorithm>
#include <cmath>
#include <random>
using namespace std;

SequencePair::SequencePair(ifstream &fin_blk, ifstream &fin_net, double alpha) {
    alpha_ = alpha;
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
double SequencePair::Area() {
    return max_width_ * max_height_;
}

double SequencePair::Wirelength() {
    return 1;
}

double SequencePair:: HPWL(Terminal* a, Terminal* b) {
    return 1;
}


double SequencePair::Cost(int w, int h) {
    if (has_legal_ == false) {
        int delta_w = (w - W_);
        int delta_h = (h - H_);
        int max_w = (max_width_ - W_);
        int max_h = (max_height_ - H_);
        if (delta_w > 0 && delta_h > 0)
            return w * h - max_width_ * max_height_;
        if (delta_w > 0)
            return delta_w - max_w;
        if (delta_h > 0)
            return delta_h - max_h;
        return -9999999;
    } else {
        return w * h - max_width_ * max_height_;
    }
}

void SequencePair::Solve() {
    this->RandomInitialize();
    DEBUG_MSG("RandomInitialize() finished...");

    // random
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> rand_op(0, 2);
    uniform_int_distribution<int> rand_blk(0, num_blocks_ - 1 - 1);
    uniform_real_distribution<double> rand_01(0.0, 1.0);

    // simulated annealing
    X_.assign(best_X_.begin(), best_X_.end());
    Y_.assign(best_Y_.begin(), best_Y_.end());

    double temparature = 99999999.0, delta;
    int cnt = 0;
    while (1) {
        if (temparature < 1) {
            if (has_legal_)
                break;
            temparature = 999.0;
        }
        for (int i = 0; i < NUM_STEPS; ++i) {
            int op = rand_op(mt);
            int a, b;
            if (op == 0) {
                // two module names in X
                a = rand_blk(mt);
                b = rand_blk(mt);
                int temp = X_[a];
                X_[a] = X_[b];
                X_[b] = temp;
                
            } else if (op == 1) {
                // two module names in Y                
                a = rand_blk(mt);
                b = rand_blk(mt);
                int temp = Y_[a];
                Y_[a] = Y_[b];
                Y_[b] = temp;
            } else if (op == 2) {
                // the width and the height of a module
                a = rand_blk(mt);
                block_list_[a]->Rotate();
            }

            int w = this->EvaluateSequence(0);
            int h = this->EvaluateSequence(1);

            if (has_legal_ == false) {
                if (w <= W_ && h <= H_) {
                    has_legal_ = true;
                    temparature = 99999.0;
                } else if (h <= W_ && w <= H_) {
                    has_legal_ = true;
                    this->Rotate90();
                    int temp = w;
                    w = h;
                    h = temp;
                    temparature = 99999.0;
                }       
            }
                        
            bool flag = false;
//            delta = w * h - max_width_ * max_height_;
            delta = this->Cost(w, h);

            // downhile move
            if (delta <= 0) {
                if (has_legal_ && (w > W_ || h > H_)) 
                    flag = false;
                else
                    flag = true; 
            }
            // uphill move
            else {
                if (has_legal_) 
                    flag = false;
                else if (rand_01(mt) < exp(-delta / temparature))
                    flag = true;            
            }

            if (has_legal_ == false && w <= W_ && h <= H_) {
                has_legal_ = true;
                flag = true;
            }

//            DEBUG_MSG(op << " --> " << flag);
            // keep the move
            if (flag) {
                max_width_ = w;
                max_height_ = h;
            } 
            // undo the move
            else {
                if (op == 0) {
                    int temp = X_[a];
                    X_[a] = X_[b];
                    X_[b] = temp;
                } else if (op == 1) {
                    int temp = Y_[a];
                    Y_[a] = Y_[b];
                    Y_[b] = temp;
                } else if (op == 2) {
                    block_list_[a]->Rotate();                
                }
            }
        }
        max_width_ = this->EvaluateSequence(0);
        max_height_ = this->EvaluateSequence(1);
        if (cnt % 1000 == 0) {
            cout << has_legal_ << ") Temparature = " << temparature
                 << ", probability = " << exp(-delta / temparature) << ":\t"
                 << "( " << max_width_ << " , " << max_height_ << " )"
                 << "\tarea = " << max_width_ * max_height_ << endl;
            cnt = 0;
        }
        cnt++;

        temparature = R * temparature; 
    }
    best_X_.assign(X_.begin(), X_.end());
    best_Y_.assign(Y_.begin(), Y_.end());
}

void SequencePair::Rotate90() {
    vector<int> temp;
    temp.resize(X_.size());
    temp.assign(X_.begin(), X_.end());
    X_.assign(Y_.begin(), Y_.end());
    reverse(temp.begin(), temp.end());
    Y_.assign(temp.begin(), temp.end());
    for (int i = 0; i < num_blocks_; ++i)
        block_list_[i]->Rotate();
}

void SequencePair::RandomInitialize() {
    int w, h;
    norm_area_ = 0;
    has_legal_ = false;
    // 1st: simply 1 to n, does not care about boundary constraint
    for (int i = 0; i < num_blocks_; ++i) {
        X_.push_back(i);
        Y_.push_back(i);
    }
    w = this->EvaluateSequence(0);
    h = this->EvaluateSequence(1);
    if (w <= W_ && h <= H_) {
        has_legal_ = true;
    } else if (h <= W_ && w <= H_) {
        has_legal_ = true;
        this->Rotate90();
        int temp = w;
        w = h;
        h = temp;
    }
    norm_area_ += w * h;
    max_width_ = w;
    max_height_ = h;
    best_X_.assign(X_.begin(), X_.end());
    best_Y_.assign(Y_.begin(), Y_.end());

    // 2nd ~ 100th: shuffle, does care about boundary constraint
    for (int i = 2; i <= 100; ++i) {
        random_shuffle(X_.begin(), X_.end());
        random_shuffle(Y_.begin(), Y_.end());
        w = this->EvaluateSequence(0);
        h = this->EvaluateSequence(1);
        norm_area_ += w * h;
        bool force_flag = false;
        if (has_legal_ == false) {
            if (w <= W_ && h <= H_) {
                has_legal_ = true;
                force_flag = true;
            } else if (h <= W_ && w <= H_) {
                this->Rotate90();
                int temp = w;
                w = h;
                h = temp;
                has_legal_ = true;
                force_flag = true;
            }       
        }
        if (w * h < max_width_ * max_height_ ||force_flag) {
            if (has_legal_ && (w > W_ || h > H_))
                continue;
            max_width_ = w;
            max_height_ = h;
            best_X_.assign(X_.begin(), X_.end());
            best_Y_.assign(Y_.begin(), Y_.end());
        }
    }

    //
    norm_area_ /= 100.0;
}

int SequencePair::EvaluateSequence(bool mode) {
    if (mode == 1)
        reverse(X_.begin(), X_.end());
    // initialize match array
    vector<int> match_x, match_y;
    match_x.resize(num_blocks_);
    match_y.resize(num_blocks_);
    for (int i = 0; i < num_blocks_; ++i) {
        match_x[X_[i]] = i;
        match_y[Y_[i]] = i;
    }
    // initialize H with initial index 0
    VanEmdeBoas* H = new VanEmdeBoas(num_blocks_ + 1);
    H->Insert(0);
    // initialize BUCKL with BUCKL[0] = 0
    vector<int> BUCKL(num_blocks_ + 1, 0);
    // for loop
    for (int i = 0; i < num_blocks_; ++i) {
        int b = X_[i];
        int p = match_y[b];
        H->Insert(p);
        if (mode == 0) {
            // x coordinates
            int pos = BUCKL[H->Predecessor(p)];
            if (block_list_[b]->IsRotate() == false)
                BUCKL[p] = pos + block_list_[b]->GetWidth();
            else 
                BUCKL[p] = pos + block_list_[b]->GetHeight();
            block_list_[b]->SetX(pos);
        } else {
            // y coordinates
            int pos = BUCKL[H->Predecessor(p)];
            if (block_list_[b]->IsRotate() == false)
                BUCKL[p] = pos + block_list_[b]->GetHeight();
            else
                BUCKL[p] = pos + block_list_[b]->GetWidth();
            block_list_[b]->SetY(pos);
        }
        //
        int k = p;
        while (H->Successor(k) != -1) {
            k = H->Successor(k);
            if (BUCKL[k] <= BUCKL[p])
                H->Delete(k);            
        }
    }
    //
    if (mode == 1) 
        reverse(X_.begin(), X_.end());

    return BUCKL[H->GetMax()]; 
}

/* -------------------- input and output -------------------- */
void SequencePair::ParseBlk(ifstream &fin) {
    string s, name;
    int w, h, x, y;
    // Outline: <outline width> <outline height>
    fin >> s;
    if (s != "Outline:")    
        this->ParseError(0);
    fin >> W_ >> H_;
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
    X_.assign(best_X_.begin(), best_X_.end());
    Y_.assign(best_Y_.begin(), best_Y_.end());
    max_width_ = this->EvaluateSequence(0);
    max_height_ = this->EvaluateSequence(1);

    // <final cost> 
    fout << this->Cost(max_width_, max_height_) << endl; 
    
    // <total wirelength>
    fout << this->Wirelength() << endl; 
    
    // <chip_area>
    fout << this->Area() << endl;
    
    // <chip_width> <chip_height>
    fout << max_width_ << " " << max_height_ << endl;
    
    // <program_runtime>

    // <macro_name> <x1> <y1> <x2> <y2> 
    for (int i = 0; i < num_blocks_; ++i) {
        Block* b = block_list_[i];
        fout << b->GetName() << " "
             << b->GetX() << " " << b->GetY() << " ";
        if (b->IsRotate() == false)
            fout << b->GetX() + b->GetWidth() << " " << b->GetY() + b->GetHeight() << " ";
        else
            fout << b->GetX() + b->GetHeight() << " " << b->GetY() + b->GetWidth() << " ";
        fout << endl;
    }
}
