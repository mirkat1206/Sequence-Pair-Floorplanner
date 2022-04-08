#include "veb.h"

VanEmdeBoas::VanEmdeBoas(int u) {
    universe_size_ = u;
    min_ = -1;
    max_ = -1;
    
    if (u <= 2) {
        summary_ = nullptr;
        clusters_ = vector<VanEmdeBoas*>(0, nullptr);
    } else {
        int sub_size = ceil(sqrt(u));
        summary_ = new VanEmdeBoas(sub_size);
        clusters_ = vector<VanEmdeBoas*>(sub_size, nullptr);
        for (int i = 0; i < sub_size; ++i) {
            clusters_[i] = new VanEmdeBoas(sub_size);
        }
    }
}

VanEmdeBoas::~VanEmdeBoas() {
    if (universe_size_ > 2) {
        int sub_size = ceil(sqrt(universe_size_));
        delete summary_;
        for (int i = 0; i < sub_size; ++i)
            delete clusters_[i];
    }
}

void VanEmdeBoas::Insert(int key) {
    if (min_ == -1) {
        min_ = key;
        max_ = key;
    } else {
        if (key < min_)
            swap(min_, key);

        if (universe_size_ > 2) {
            int sub_size = ceil(sqrt(universe_size_));
            int high = key / sub_size, low = key % sub_size;
            if (clusters_[high]->GetMin() == -1) {
                summary_->Insert(high);
                clusters_[high]->SetMin(low);
                clusters_[high]->SetMax(low);
            } else {
                clusters_[high]->Insert(low);
            }
        }

        if (key > max_)
            max_ = key;
    }
}

void VanEmdeBoas::Delete(int key) {
    if (max_ == min_) {
        max_ = -1;
        min_ = -1;
    } else if (universe_size_ == 2) {
        if (key == 0)
            min_ = 1;
        else
            min_ = 0;
        max_ = min_;
    } else {
        if (key == min_) {
            int first_cluster = summary_->GetMin();
            key = this->GenerateIndex(first_cluster, 
                                      clusters_[first_cluster]->GetMin());
            min_ = key;
        }
        
        int sub_size = ceil(sqrt(universe_size_));
        int high = key / sub_size, low = key % sub_size;
        clusters_[high]->Delete(low);

        if (clusters_[high]->GetMin() == -1) {
            summary_->Delete(high);

            if (key == max_) {
                int max_in_summary = summary_->GetMax();
                if (max_in_summary == -1) {
                    max_ = min_;
                } else {                   
                    max_ = this->GenerateIndex(max_in_summary,
                                               clusters_[max_in_summary]->GetMax());
                }
            }
        } else if (key == max_) {
            max_ = this->GenerateIndex(high, 
                                       clusters_[high]->GetMax());
        }
    }
}

int VanEmdeBoas::Predecessor(int key) {
    if (universe_size_ == 2) {
        if (key == 1 && min_ == 0)
            return 0;
        else
            return -1;
    } else if (max_ != -1 && key > max_) {
        return max_;
    } else {
        int sub_size = ceil(sqrt(universe_size_));
        int high = key / sub_size, low = key % sub_size; 
        int min_in_cluster = clusters_[high]->GetMin();
        
        int offset = 0, pred_cluster = 0;
        if (min_in_cluster != -1 && low > min_in_cluster) {
            offset = clusters_[high]->Predecessor(low);
            return this->GenerateIndex(high, offset);
        } else {
            pred_cluster = summary_->Predecessor(high);
            if (pred_cluster == -1) {
                if (min_ != -1 && key > min_)
                    return min_;
                else
                    return -1;
            } else {
                offset = clusters_[pred_cluster]->GetMax();
                return this->GenerateIndex(pred_cluster, offset);
            }
        }
    }
}

int VanEmdeBoas::Successor(int key) {
    if (universe_size_ == 2) {
        if (key == 0 && max_ == 1) 
            return 1;
        else
            return -1;
    } else if (min_ != -1 && key < min_) {
        return min_;
    } else {
        int sub_size = ceil(sqrt(universe_size_));
        int high = key / sub_size, low = key % sub_size; 
        int max_in_cluster = clusters_[high]->GetMax();
        
        int offset = 0, succ_cluster = 0;
        if (max_in_cluster != -1 && low < max_in_cluster) {
            offset = clusters_[high]->Successor(low);
            return this->GenerateIndex(high, offset);
        } else {
            succ_cluster = summary_->Successor(high);
            if (succ_cluster == -1) {
                return -1;
            } else {
                offset = clusters_[succ_cluster]->GetMin();
                return this->GenerateIndex(succ_cluster, offset);
            }
        }
    }
}

bool VanEmdeBoas::IsMember(int key) {
    if (universe_size_ < key)
        return false;
    if (min_ == key || max_ == key) {
        return true;
    } else {
        if (universe_size_ == 2)
            return false;
        else {
            int sub_size = ceil(sqrt(universe_size_));
            int high = key / sub_size, low = key % sub_size; 
            return clusters_[high]->IsMember(low);
        }
    }
}
