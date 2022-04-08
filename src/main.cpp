// main.cpp
#include "sp.h"
#include "veb.h"
#include <fstream>
#include <iostream>
#include <iomanip>
using namespace std;

void debug() {
    VanEmdeBoas* v = new VanEmdeBoas(10);
    v->Insert(6);
    v->Insert(1);
    v->Insert(3);

    for (int i = 0; i < 10; ++i) {
        cout << i << "(" << v->IsMember(i) << "): "
             << v->Predecessor(i) << ", "
             << v->Successor(i) << endl;
    }
    cout << "max = " << v->GetMax() << ", min = " << v->GetMin() << endl;

    v->Insert(9);
    v->Delete(3);
    v->Insert(0);

    for (int i = 0; i < 10; ++i) {
        cout << i << "(" << v->IsMember(i) << "): "
             << v->Predecessor(i) << ", "
             << v->Successor(i) << endl;
    }
    cout << "max = " << v->GetMax() << ", min = " << v->GetMin() << endl;
}

int main(int argc, char* argv[])
{
//    debug();    
  
    ifstream fin_blk, fin_net;
    ofstream fout;
    if (argc == 5) {
        fin_blk.open(argv[2]);
        fin_net.open(argv[3]);
        fout.open(argv[4]);
        if (!fin_blk) {
            cerr << "Cannot open the input file \"" << argv[2] << "\"." << endl
                 << "The program will be terminated..." << endl;
            exit(1);
        }
        if (!fin_net) {
            cerr << "Cannot open the input file \"" << argv[3] << "\"." << endl
                 << "The program will be terminated..." << endl;
            exit(1);
       }
       if (!fout) {
            cerr << "Cannot open the input file \"" << argv[4] << "\"." << endl
                 << "The program will be terminated..." << endl;
            exit(1);
       }
    } else {
        cerr << "Usage: ./fp <alpha value> <input.block file name> <input.net file name> <output file name>" << endl;
        exit(1);
    }

    SequencePair sp(fin_blk, fin_net);

    clock_t start, end;
    start = clock();

     
    sp.Solve();
    sp.WriteReport(fout);

    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);       
    cout << "Time taken by fp is : " << fixed << setprecision(5)
         << time_taken << " secends." << endl;
    
    return 0;
}
