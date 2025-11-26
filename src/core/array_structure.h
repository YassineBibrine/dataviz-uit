#pragma once

#include "data_structure.h"
#include <vector>
#include <memory>

/**
 * @class ArrayStructure
 * @brief Wrapper for array/vector data structure
 */
class ArrayStructure : public DataStructure {
private:
    std::vector<int> data;
    
public:
    explicit ArrayStructure(int size = 0);
    
    /**
     * @brief Fill the array with random values
     * @param size Number of elements
   */
    void fillRandom(int size);
    
    /**
 * @brief Get the underlying vector data
     * @return Reference to the vector
     */
    std::vector<int>& getData() { return data; }
 const std::vector<int>& getData() const { return data; }
    
    // DataStructure interface implementation
    std::vector<DSNode> getNodes() const override;
    std::vector<DSEdge> getEdges() const override;
    std::string serializeToDOT() const override;
    void* getDataForRunner() override;
};
