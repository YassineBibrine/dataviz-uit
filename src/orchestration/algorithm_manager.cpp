#include "algorithm_manager.h"
#include "sorting_algorithm_factory.h"
#include "filtering_algorithm_factory.h"
#include "transform_algorithm_factory.h"
#include "tree_algorithm_factory.h"
#include "graph_algorithm_factory.h"
#include <stdexcept>
#include <memory>

// Implémentation du singleton
AlgorithmManager& AlgorithmManager::getInstance() {
    static AlgorithmManager instance;
    return instance;
}

// Constructeur privé — UNIQUE définition
AlgorithmManager::AlgorithmManager() {
    // Enregistre les factories concrètes au démarrage
    registerFactory("Sorting", std::make_unique<SortingAlgorithmFactory>());
    registerFactory("Filtering", std::make_unique<FilteringAlgorithmFactory>());
    registerFactory("Transform", std::make_unique<TransformAlgorithmFactory>());
    registerFactory("Tree", std::make_unique<TreeAlgorithmFactory>());
    registerFactory("Graph", std::make_unique<GraphAlgorithmFactory>());
}

void AlgorithmManager::registerFactory(const std::string& category, std::unique_ptr<AlgorithmFactory> factory) {
    factories_[category] = std::move(factory);
}

std::unique_ptr<Algorithm> AlgorithmManager::createAlgorithm(const std::string& category, const std::string& type) {
    auto it = factories_.find(category);
    if (it == factories_.end()) {
        return nullptr;
    }
    try {
        return it->second->createAlgorithm(type);
    } catch (const std::exception&) {
        return nullptr;
    }
}

std::vector<std::string> AlgorithmManager::listAlgorithmsByCategory(const std::string& category) const {
    return getAlgorithmNames(category);
}

std::vector<std::string> AlgorithmManager::getCategories() const {
    std::vector<std::string> categories;
    for (const auto& pair : factories_) {
        categories.push_back(pair.first);
    }
    return categories;
}

std::vector<std::string> AlgorithmManager::getAlgorithmNames(const std::string& category) const {
    if (category == "Sorting") return { "BubbleSort", "QuickSort", "MergeSort", "InsertionSort", "SelectionSort" };
    if (category == "Filtering") return { "RangeFilter", "RemoveDuplicates" };
    if (category == "Transform") return { "Normalize", "Reverse", "Map" };
    if (category == "Graph") return { "BFS", "DFS", "Dijkstra", "DetectCycle", "TopologicalSort" };
    if (category == "Tree") return { "InOrder", "PreOrder", "PostOrder", "LevelOrder", "FindHeight", "FindMin", "FindMax" };
    return {};
}

std::string AlgorithmManager::getCategoryForAlgorithm(const std::string& algorithm) const {
    // Sorting algorithms
    if (algorithm == "BubbleSort" || algorithm == "QuickSort" || algorithm == "MergeSort" ||
     algorithm == "InsertionSort" || algorithm == "SelectionSort" || algorithm == "HeapSort") {
        return "Sorting";
    }
    // Filtering algorithms
    if (algorithm == "RangeFilter" || algorithm == "Filter") {
 return "Filtering";
    }
    // Transform algorithms
    if (algorithm == "Normalize" || algorithm == "Transform") {
        return "Transform";
    }
    // Graph algorithms
    if (algorithm == "BFS" || algorithm == "DFS" || algorithm == "Dijkstra" || 
  algorithm == "Kruskal" || algorithm == "Prim") {
        return "Graph";
    }
    // Tree algorithms
    if (algorithm == "InOrder" || algorithm == "PreOrder" || algorithm == "PostOrder" ||
        algorithm == "LevelOrder") {
        return "Tree";
    }
    return "Sorting";  // Default fallback
}