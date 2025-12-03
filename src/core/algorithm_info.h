#pragma once

#include <string>

/**
 * @class AlgorithmInfo
 * @brief Stores metadata about an algorithm
 */
class AlgorithmInfo {
private:
    std::string name;
    std::string description;
    std::string timeComplexity;
    std::string spaceComplexity;
    
public:
    AlgorithmInfo() = default;
    
    AlgorithmInfo(const std::string& n, 
        const std::string& d,
       const std::string& t,
const std::string& s)
        : name(n), description(d), timeComplexity(t), spaceComplexity(s) {}
    
    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
    const std::string& getTimeComplexity() const { return timeComplexity; }
    const std::string& getSpaceComplexity() const { return spaceComplexity; }
    
    std::string getComplexity() const {
     return "Time: " + timeComplexity + ", Space: " + spaceComplexity;
    }
};
