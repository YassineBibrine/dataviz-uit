// src/orchestration/algorithm.h
#ifndef ALGORITHM_H
#define ALGORITHM_H

class Algorithm {
public:
    virtual ~Algorithm() = default;
    virtual void execute() = 0;
};

#endif // ALGORITHM_H