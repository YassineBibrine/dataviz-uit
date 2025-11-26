#pragma once

#include <string>
#include <memory>
#include "data_structure.h"
#include "array_structure.h"
#include "graph_structure.h"
#include "list_structure.h"
#include "tree_structure.h"

/**
 * @class DataModelManager
 * @brief Manages creation and lifecycle of data structures
 */
class DataModelManager {
private:
    std::unique_ptr<DataStructure> currentStructure;
    
public:
    DataModelManager() = default;
    ~DataModelManager() = default;
    
    /**
     * @brief Create a data structure of the specified type
     * @param type Type of data structure ("Array", "Graph", "LinkedList", "Tree")
     * @param size Size parameter (interpretation depends on type)
  * @return Pointer to the created data structure
     */
    DataStructure* createDataStructure(const std::string& type, int size);
    
    /**
     * @brief Get the current data structure
     * @return Pointer to current structure, or nullptr if none exists
     */
    DataStructure* getCurrentStructure() { return currentStructure.get(); }
    const DataStructure* getCurrentStructure() const { return currentStructure.get(); }
};
