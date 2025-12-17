#include "algorithm_manager.h"
#include "sorting_algorithm_factory.h"
#include "filtering_algorithm_factory.h"
#include "transform_algorithm_factory.h"
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
}

void AlgorithmManager::registerFactory(const std::string& category, std::unique_ptr<AlgorithmFactory> factory) {
    factories_[category] = std::move(factory);
}

std::unique_ptr<Algorithm> AlgorithmManager::createAlgorithm(const std::string& category, const std::string& type) {
    auto it = factories_.find(category);
    if (it == factories_.end()) {
        return nullptr;
    }
    return it->second->createAlgorithm(type);
}

std::vector<std::string> AlgorithmManager::listAlgorithmsByCategory(const std::string& category) const {
    // Tu peux étendre ça si tu stockes la liste des types par catégorie
    // Pour l'instant, retour vide ou logique spécifique
    return {};
}

std::vector<std::string> AlgorithmManager::getCategories() const {
    std::vector<std::string> categories;
    for (const auto& pair : factories_) {
        categories.push_back(pair.first);
    }
    return categories;
}