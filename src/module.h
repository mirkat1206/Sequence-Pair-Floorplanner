// module.h
#ifndef MODULE_H
#define MODULE_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Terminal { 
public:
    // constructor and destructor
    Terminal(string name, size_t x, size_t y):
        name_(name), x_(x), y_(y) {}
    ~Terminal() {}

    // get and set
    string GetName()    const { return name_; }
    size_t GetX()       const { return x_; }
    size_t GetY()       const { return y_; }
    size_t GetCenterX() const { return x_; }
    size_t GetCenterY() const { return y_; }
    void SetX(int x) { x_ = x; }
    void SetY(int y) { y_ = y; }

protected:
    string name_;
    size_t x_;
    size_t y_;
};

class Block: public Terminal {
public:
    // constructor and destructor
    Block(string name, size_t w, size_t h) :
        Terminal(name, 0, 0), width_(w), height_(h), rotate_(false) {}
    ~Block() {}

    // get and set
    size_t GetWidth()   const { return rotate_ ? height_ : width_; }
    size_t GetHeight()  const { return rotate_ ? width_ : height_; }
    size_t GetArea()    const { return width_ * height_; }
    double GetCenterX() const { return rotate_? x_ + height_ / 2.0 : x_ + width_ / 2.0; }
    double GetCenterY() const { return rotate_? y_ + width_ / 2.0 : y_ + height_ / 2.0; }
    void Rotate() { rotate_ = rotate_ ? false : true; }

private:
    bool rotate_;
    size_t width_;
    size_t height_;
};

class Net {
public:
    // constructor and destructor
    Net() {}
    ~Net() {}

    // get and set
    int GetNetDegree()      const { return terminal_list_.size() + block_list_.size(); }
    int GetTerminalDegree() const { return terminal_list_.size(); }
    int GetBlockDegree()    const { return block_list_.size(); }
    Terminal* GetTerminal(int i) const { return terminal_list_[i]; }
    Block* GetBlock(int i)       const { return block_list_[i]; }
    void AddTerminal(Terminal* t) { terminal_list_.push_back(t); }
    void AddBlock(Block* b)       { block_list_.push_back(b); }

private:
    vector<Terminal*> terminal_list_;
    vector<Block*> block_list_;
};

#endif
