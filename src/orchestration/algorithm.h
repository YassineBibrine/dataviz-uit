// src/orchestration/algorithm.h
#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "../visualization/animation_frame.h"
#include <vector>

class Algorithm {
public:
    virtual ~Algorithm() = default;
    
    // Legacy method: execute without animation
    virtual void execute() = 0;
    
    // NEW: Execute with step-by-step animation frames
    virtual std::vector<AnimationFrame> executeWithFrames() {
        // Default implementation: just execute and return empty frames
        execute();
   return std::vector<AnimationFrame>();
    }
};

#endif // ALGORITHM_H