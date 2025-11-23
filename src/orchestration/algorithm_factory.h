#pragma once

#include <string>
#include <map>

class AlgorithmRunner; // fwd

struct AlgorithmInfo {
    std::string name;
    std::string description;
    std::string category; // Utile pour AlgorithmManager, même si non dans l'interface
};

class AlgorithmFactory {
public:
    virtual ~AlgorithmFactory() = default;

    virtual AlgorithmRunner* create(const std::map<std::string, std::string>& params) = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
};