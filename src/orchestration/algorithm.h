// src/orchestration/algorithm.h
// Base interface for algorithm implementations used by the system.
#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "../visualization/animation_frame.h"
#include <vector>

/**
 * @class Algorithm
 * @brief Abstract base class for all algorithms that operate on data structures.
 *
 * Implementations may perform in-place transformations of the underlying
 * DataStructure and can optionally provide an animation timeline by
 * implementing executeWithFrames(). For legacy algorithms that only
 * implement execute(), the default executeWithFrames() simply runs the
 * algorithm and returns an empty frame list.
 */
class Algorithm {
public:
    virtual ~Algorithm() = default;
    
    // Legacy method: execute without animation. Implementations must provide
    // this if they do not emit animation frames.
    virtual void execute() = 0;
    
    // NEW: Execute with step-by-step animation frames. Algorithms that
    // support visualization should override this and return a sequence of
    // AnimationFrame objects representing the timeline of the algorithm.
    virtual std::vector<AnimationFrame> executeWithFrames() {
        // Default implementation: run the non-animated execution and return
        // an empty frame list to indicate no animation is available.
        execute();
        return std::vector<AnimationFrame>();
    }
};

#endif // ALGORITHM_H