#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <string>

/**
 * @class Timer
 * @brief Performance timing utility
 * 
 * Measures:
 * - Execution time of code sections
 * - Algorithm performance
 * - Benchmark data
 */
class Timer {
public:
    Timer();
    
    void start();
  void stop();
    void reset();
    
    double getElapsedSeconds() const;
    double getElapsedMilliseconds() const;
    double getElapsedMicroseconds() const;
    
    std::string getElapsedTimeString() const;

private:
   std::chrono::high_resolution_clock::time_point startTime;
std::chrono::high_resolution_clock::time_point endTime;
    bool isRunning = false;
};

#endif // TIMER_H
