#include "AlgorithmManager.h"
#include <stdexcept>
#include <algorithm>

AlgorithmManager& AlgorithmManager::getInstance() {
    static AlgorithmManager instance;
    return instance;
}

void AlgorithmManager::registerAlgorithm(const std::string& name, AlgorithmFactory* factory) {
    if (!factory) {
        throw std::invalid_argument("Factory cannot be null");
    }

    m_factories[name] = factory;

    // Ajout à la catégorie (par défaut "General" si non spécifié)
    std::string category = "General"; // On pourrait ajouter une méthode getCategory() plus tard
    m_categories[category].push_back(name);
}

AlgorithmInfo AlgorithmManager::getAlgorithmInfo(const std::string& name) const {
    auto it = m_factories.find(name);
    if (it == m_factories.end()) {
        throw std::runtime_error("Algorithm not found: " + name);
    }

    AlgorithmInfo info;
    info.name = name;
    info.description = it->second->getDescription();
    info.category = "General"; // À modifier si tu ajoutes getCategory() plus tard
    return info;
}

std::vector<std::string> AlgorithmManager::listAlgorithmsByCategory(const std::string& category) const {
    auto it = m_categories.find(category);
    if (it == m_categories.end()) {
        return {};
    }
    return it->second;
}

std::vector<std::string> AlgorithmManager::getCategories() const {
    std::vector<std::string> categories;
    for (const auto& pair : m_categories) {
        categories.push_back(pair.first);
    }
    return categories;
}