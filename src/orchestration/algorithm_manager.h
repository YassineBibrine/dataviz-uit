#pragma once

#include "algorithm_factory.h"
#include <string>
#include <vector>
#include <map>

class AlgorithmManager {
public:
    static AlgorithmManager& getInstance();
    AlgorithmManager(const AlgorithmManager&) = delete;
    AlgorithmManager& operator=(const AlgorithmManager&) = delete;

    void registerAlgorithm(const std::string& name, AlgorithmFactory* factory);
    AlgorithmInfo getAlgorithmInfo(const std::string& name) const;
    std::vector<std::string> listAlgorithmsByCategory(const std::string& category) const;
    std::vector<std::string> getCategories() const;

private:
    AlgorithmManager() = default;

    std::map<std::string, AlgorithmFactory*> m_factories;
    std::map<std::string, std::vector<std::string>> m_categories;
};