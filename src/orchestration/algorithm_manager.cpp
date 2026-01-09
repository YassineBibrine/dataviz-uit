#include "algorithm_manager.h"
#include "sorting_algorithm_factory.h"
<<<<<<< Updated upstream
=======
#include "filtering_algorithm_factory.h"
#include "transform_algorithm_factory.h"
#include "../core/data_structure.h"
#include "../core/array_structure.h"
>>>>>>> Stashed changes
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
}

void AlgorithmManager::registerFactory(const std::string& category, std::unique_ptr<AlgorithmFactory> factory) {
    factories_[category] = std::move(factory);
    // Optionnel : remplir m_categories si tu l'utilises
}

std::unique_ptr<Algorithm> AlgorithmManager::createAlgorithm(const std::string& category, const std::string& type) {
    auto it = factories_.find(category);
    if (it == factories_.end()) {
        return nullptr;
    }
    return it->second->createAlgorithm(type);
}

std::unique_ptr<Algorithm> AlgorithmManager::createAlgorithmWithData(
    const std::string& category, 
    const std::string& type, 
    DataStructure* data) {
    
    if (!data) {
        throw std::invalid_argument("DataStructure pointer is null");
    }
    
    auto it = factories_.find(category);
    if (it == factories_.end()) {
        throw std::invalid_argument("Factory not found for category: " + category);
    }
    
    try {
        // Cast to specific structure type based on category
        if (category == "Sorting" || category == "Filtering" || category == "Transform") {
            ArrayStructure* arrayData = dynamic_cast<ArrayStructure*>(data);
            if (!arrayData) {
                throw std::invalid_argument(category + " algorithms require an ArrayStructure");
            }
            
            SortingAlgorithmFactory* sortFactory = dynamic_cast<SortingAlgorithmFactory*>(it->second.get());
            if (sortFactory && category == "Sorting") {
                return sortFactory->createAlgorithmWithData(type, arrayData);
            }
        }
        
        return it->second->createAlgorithm(type);
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to create algorithm: ") + e.what());
    }
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