#pragma once

#include <string>
#include <map>
#include <vector>

class AlgorithmRunner; // fwd

struct AlgorithmInfo {
 std::string name;
 std::string description;
 std::string category;
 std::vector<std::string> supportedStructures;
};

class AlgorithmFactory {
public:
 virtual ~AlgorithmFactory() = default;
 virtual AlgorithmRunner* create(const std::map<std::string,std::string>& params) =0;
 virtual std::string getName() const =0;
 virtual std::string getDescription() const =0;
 virtual std::string getCategory() const { return "General"; }
};
