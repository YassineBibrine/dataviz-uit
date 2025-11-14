#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>
#include "algorithm_factory.h"

class AlgorithmManager {
 std::map<std::string, std::shared_ptr<AlgorithmFactory>> algorithms;
 std::map<std::string, std::vector<std::string>> categories;
 AlgorithmManager();
public:
 static AlgorithmManager& getInstance();
 void registerAlgorithm(const std::string& name, std::shared_ptr<AlgorithmFactory> factory);
 AlgorithmInfo getAlgorithmInfo(const std::string& name) const;
 std::vector<std::string> listAlgorithmsByCategory(const std::string& cat) const;
 std::vector<std::string> getCategories() const;
 bool hasAlgorithm(const std::string& name) const { return algorithms.count(name)>0; }
};
