/* encoding: utf-8 */
#pragma once

#include "algorithm_factory.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "../core/algorithm_info.h"

class AlgorithmManager {
public:
    static AlgorithmManager& getInstance();
    AlgorithmManager(const AlgorithmManager&) = delete;
    AlgorithmManager& operator=(const AlgorithmManager&) = delete;

    void registerFactory(const std::string& category, std::unique_ptr<AlgorithmFactory> factory);
    std::unique_ptr<Algorithm> createAlgorithm(const std::string& category, const std::string& type);

    // Methodes optionnelles
    std::vector<std::string> listAlgorithmsByCategory(const std::string& category) const;
    std::vector<std::string> getCategories() const;
    std::vector<std::string> getAlgorithmNames(const std::string& category) const;
private:
    AlgorithmManager(); // declaration seulement

    std::map<std::string, std::unique_ptr<AlgorithmFactory>> factories_;
    std::map<std::string, std::vector<std::string>> m_categories; // si tu en as besoin
};