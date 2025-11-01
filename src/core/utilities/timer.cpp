#include "timer.h"
#include <sstream>
#include <iomanip>

Timer::Timer() = default;

void Timer::start()
{
    startTime = std::chrono::high_resolution_clock::now();
 isRunning = true;
}

void Timer::stop()
{
    endTime = std::chrono::high_resolution_clock::now();
    isRunning = false;
}

void Timer::reset()
{
    isRunning = false;
}

double Timer::getElapsedSeconds() const
{
    auto end = isRunning ? std::chrono::high_resolution_clock::now() : endTime;
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - startTime);
    return duration.count();
}

double Timer::getElapsedMilliseconds() const
{
    auto end = isRunning ? std::chrono::high_resolution_clock::now() : endTime;
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - startTime);
    return duration.count();
}

double Timer::getElapsedMicroseconds() const
{
    auto end = isRunning ? std::chrono::high_resolution_clock::now() : endTime;
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - startTime);
    return duration.count();
}

std::string Timer::getElapsedTimeString() const
{
    double ms = getElapsedMilliseconds();
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << ms << " ms";
    return oss.str();
}
