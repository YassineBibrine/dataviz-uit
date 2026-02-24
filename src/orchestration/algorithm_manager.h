/* encoding: utf-8 */
#pragma once

#include "algorithm_factory.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "../core/algorithm_info.h"

/**
 * @class AlgorithmManager
 * @brief Global registry and factory manager for algorithm creation.
 *
 * This singleton holds a collection of AlgorithmFactory instances, one per
 * category (Sorting, Graph, Tree, etc.). Callers request algorithm instances
 * by category and type. The manager also provides convenience helpers to
 * enumerate available algorithms and map an algorithm name to its category.
 */
class AlgorithmManager {
public:
    static AlgorithmManager& getInstance();
    AlgorithmManager(const AlgorithmManager&) = delete;
    AlgorithmManager& operator=(const AlgorithmManager&) = delete;

    // Register a concrete factory implementation for a category
    void registerFactory(const std::string& category, std::unique_ptr<AlgorithmFactory> factory);

    // Create an algorithm instance by category and type. Returns nullptr if not found.
    std::unique_ptr<Algorithm> createAlgorithm(const std::string& category, const std::string& type);

    // Optional enumeration helpers used by the UI
    std::vector<std::string> listAlgorithmsByCategory(const std::string& category) const;
    std::vector<std::string> getCategories() const;
    std::vector<std::string> getAlgorithmNames(const std::string& category) const;
    
    /**
     * @brief Resolve the category that contains a given algorithm name.
     * @param algorithm The algorithm name (e.g., "BubbleSort")
     * @return The category name (e.g., "Sorting") or a sensible default
     */
    std::string getCategoryForAlgorithm(const std::string& algorithm) const;
    
private:
    AlgorithmManager(); // private ctor for singleton

    std::map<std::string, std::unique_ptr<AlgorithmFactory>> factories_;
    std::map<std::string, std::vector<std::string>> m_categories; // optional cached categories
};