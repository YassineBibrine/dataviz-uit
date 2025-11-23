#include "data_model_manager.h"
#include <stdexcept>

DataStructure* DataModelManager::createDataStructure(const std::string& type, int size) {
    if (type == "Array") {
        auto arrayStruct = std::make_unique<ArrayStructure>(size);
        currentStructure = std::move(arrayStruct);
        return currentStructure.get();
    }
    else if (type == "Graph") {
 auto graphStruct = std::make_unique<GraphStructure>(false);
        // Size is interpreted as node count, edges = size * 1.5
        graphStruct->generateRandom(size, static_cast<int>(size * 1.5));
        currentStructure = std::move(graphStruct);
        return currentStructure.get();
    }
    else if (type == "LinkedList") {
        auto listStruct = std::make_unique<ListStructure>();
        listStruct->generateRandom(size);
        currentStructure = std::move(listStruct);
        return currentStructure.get();
    }
  else if (type == "Tree") {
        auto treeStruct = std::make_unique<TreeStructure>();
     treeStruct->generateRandom(size);
        currentStructure = std::move(treeStruct);
      return currentStructure.get();
    }
    
    throw std::invalid_argument("Unknown data structure type: " + type);
}
