// src/algorithms/bubble_sort.h
#ifndef BUBBLE_SORT_H
#define BUBBLE_SORT_H

#include "../orchestration/algorithm.h"
#include <vector>

class BubbleSort : public Algorithm {
public:
    BubbleSort();
    void execute() override;
};

#endif // BUBBLE_SORT_H