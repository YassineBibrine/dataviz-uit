// algorithm_manager.cpp
// Central registry for algorithm factories and a simple adapter for UI helpers.
//
// This module constructs concrete AlgorithmFactory instances for each
// supported category and exposes creation and enumeration helpers used by
// the application (UI and execution code). Changes are documentation only.

#include "algorithm_manager.h"
#include "sorting_algorithm_factory.h"
#include "filtering_algorithm_factory.h"
#include "transform_algorithm_factory.h"
#include "tree_algorithm_factory.h"
#include "graph_algorithm_factory.h"
#include <stdexcept>
#include <memory>

// Implementation of the singleton
AlgorithmManager& AlgorithmManager::getInstance() {
 static AlgorithmManager instance;
 return instance;
}

// Private constructor registers concrete factories used by the app
AlgorithmManager::AlgorithmManager() {
 // Register concrete factories at startup
 registerFactory("Sorting", std::make_unique<SortingAlgorithmFactory>());
 registerFactory("Filtering", std::make_unique<FilteringAlgorithmFactory>());
 registerFactory("Transform", std::make_unique<TransformAlgorithmFactory>());
 registerFactory("Tree", std::make_unique<TreeAlgorithmFactory>());
 registerFactory("Graph", std::make_unique<GraphAlgorithmFactory>());
}

void AlgorithmManager::registerFactory(const std::string& category, std::unique_ptr<AlgorithmFactory> factory) {
 factories_[category] = std::move(factory);
}

// Create an algorithm instance by category. Returns nullptr when not found.
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

// Convenience wrapper for UI code
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

// Return known algorithm names for a given category. This is used by the UI
// to populate dropdowns. The list is intentionally small and maintained here
// to keep the UI decoupled from concrete factories.
std::vector<std::string> AlgorithmManager::getAlgorithmNames(const std::string& category) const {
 if (category == "Sorting") return { "InsertionSort", "SelectionSort", "BubbleSort" };
 if (category == "Filtering") return { "RemoveDuplicates" };
 if (category == "Transform") return { "Reverse", "MapTransform" };
 if (category == "Graph") return { "BFS", "DFS", "Dijkstra" };
 if (category == "Tree") return { "InOrder", "PreOrder", "PostOrder" };
 return {};
}

// Map an algorithm name to its category. The mapping is a simple series of
// checks and returns a sensible default if the algorithm name is unknown.
std::string AlgorithmManager::getCategoryForAlgorithm(const std::string& algorithm) const {
 // Sorting algorithms
 if (algorithm == "InsertionSort" || algorithm == "SelectionSort" || algorithm == "BubbleSort") {
 return "Sorting";
 }
 // Transform algorithms
 if (algorithm == "Reverse" || algorithm == "MapTransform") {
 return "Transform";
 }
 // Filtering algorithms
 if (algorithm == "RemoveDuplicates") {
 return "Filtering";
 }
 // Graph algorithms
 if (algorithm == "BFS" || algorithm == "DFS" || algorithm == "Dijkstra") {
 return "Graph";
 }
 // Tree algorithms
 if (algorithm == "InOrder" || algorithm == "PreOrder" || algorithm == "PostOrder") {
 return "Tree";
 }
 return "Sorting"; // Default fallback
}