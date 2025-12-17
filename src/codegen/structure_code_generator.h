#pragma once

#include <string>
#include <map>
#include <vector>
#include "../core/data_structure.h"
#include "../core/array_structure.h"
#include "../core/list_structure.h"
#include "../core/tree_structure.h"
#include "../core/graph_structure.h"

/**
 * @class StructureCodeGenerator
 * @brief Generates C++ code from data structures
 */
class StructureCodeGenerator {
public:
    /**
 * @brief Generate C++ code from a data structure
     * @param structure The structure to convert
  * @param includeComments Whether to include explanatory comments
     * @return Generated C++ code
     */
    static std::string generateCode(const DataStructure* structure, bool includeComments = true);
    
    /**
     * @brief Generate code for an array structure
     */
    static std::string generateArrayCode(const ArrayStructure* arr, bool includeComments);
    
    /**
  * @brief Generate code for a linked list structure
     */
    static std::string generateListCode(const ListStructure* list, bool includeComments);
    
    /**
     * @brief Generate code for a tree structure
     */
    static std::string generateTreeCode(const TreeStructure* tree, bool includeComments);
    
    /**
     * @brief Generate code for a graph structure
     */
    static std::string generateGraphCode(const GraphStructure* graph, bool includeComments);
    
private:
    static std::string generateHeader(const std::string& structureType, bool includeComments);
    static std::string generateFooter();
};
