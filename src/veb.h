// veb.h
#ifndef VEB_H
#define VEB_H

#include <cmath>
#include <vector>
using namespace std;

class VanEmdeBoas {
public:
    // constructor and destructor 
    VanEmdeBoas(int u);
    ~VanEmdeBoas();
    
    // get and set
    int GetMin()   const { return min_; }
    int GetMax()   const { return max_; }
    void SetMin(int m)   { min_ = m; }
    void SetMax(int m)   { max_ = m; }

    // Van Emde Boas
    int GenerateIndex(int high, int low) {
        int sub_size = ceil(sqrt(universe_size_));
        return high * sub_size + low;
    }
    void Insert(int key);
    void Delete(int key);
    int Successor(int key);
    int Predecessor(int key);
    bool IsMember(int key);

private:
    int universe_size_;
    int min_;
    int max_;
    VanEmdeBoas* summary_;
    vector<VanEmdeBoas*> clusters_;
};

#endif
