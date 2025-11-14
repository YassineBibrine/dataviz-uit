#include "algorithm_manager.h"
#include <stdexcept>

AlgorithmManager::AlgorithmManager() {}

AlgorithmManager& AlgorithmManager::getInstance() { static AlgorithmManager inst; return inst; }

void AlgorithmManager::registerAlgorithm(const std::string& name, std::shared_ptr<AlgorithmFactory> factory) {
 if (algorithms.count(name)) throw std::runtime_error("Algorithm already registered: "+name);
 algorithms[name]=factory;
 categories[factory->getCategory()].push_back(name);
}

AlgorithmInfo AlgorithmManager::getAlgorithmInfo(const std::string& name) const {
 auto it = algorithms.find(name);
 if (it==algorithms.end()) throw std::runtime_error("Algorithm not found: "+name);
 AlgorithmInfo info; info.name = it->second->getName(); info.description = it->second->getDescription(); info.category = it->second->getCategory(); return info;
}

std::vector<std::string> AlgorithmManager::listAlgorithmsByCategory(const std::string& cat) const {
 auto it = categories.find(cat);
 if (it==categories.end()) return {}; return it->second;
}

std::vector<std::string> AlgorithmManager::getCategories() const {
 std::vector<std::string> out; out.reserve(categories.size());
 for (auto& kv : categories) out.push_back(kv.first); return out;
}
