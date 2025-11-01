#ifndef EXECUTION_LOGGER_H
#define EXECUTION_LOGGER_H

#include <string>
#include <vector>
#include <map>
#include <memory>

struct ExecutionEvent {
    int lineNumber;
 std::string operation;    // "assign", "add_node", "add_edge", etc.
    std::map<std::string, std::string> variables;  // Variable name -> value
    std::map<std::string, std::string> addresses;  // Variable name -> memory address
 long long timestamp;      // Nanoseconds since execution start
};

/**
 * @class ExecutionLogger
 * @brief Manages runtime execution logs from instrumented code
 * 
 * Captures and stores:
 * - Variable state changes
 * - Memory allocations/deallocations
 * - Pointer assignments
 * - Function call sequences
 * - Timestamps for animation playback
 */
class ExecutionLogger {
public:
    ExecutionLogger();
    
    void recordEvent(const ExecutionEvent &event);
    void loadFromFile(const std::string &logFilePath);
    void saveToFile(const std::string &logFilePath) const;
    
    const std::vector<ExecutionEvent> &getEvents() const;
    const ExecutionEvent *getEventAt(size_t index) const;
    size_t getEventCount() const;
    void clear();

private:
    std::vector<ExecutionEvent> executionEvents;
};

#endif // EXECUTION_LOGGER_H
