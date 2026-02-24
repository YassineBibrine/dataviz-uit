#pragma once

#include "data_structure.h"
#include <vector>
#include <memory>

/**
 * @class ArrayStructure
 * @brief Lightweight wrapper around a std::vector<int> exposing the
 * data as a visualizable DataStructure.
 *
 * Provides helpers to generate random content, access the underlying
 * data vector, and implement the DataStructure interface for session
 * serialization and visualization.
 */
class ArrayStructure : public DataStructure {
private:
    std::vector<int> data;
    
public:
    explicit ArrayStructure(int size =0);
    
    /**
     * @brief Fill the array with random integer values.
     * @param size Number of elements to generate
     */
    void fillRandom(int size);
    
    /**
     * @brief Access the underlying vector of values.
     * @return Reference to the internal vector<int>
     */
    std::vector<int>& getData() { return data; }
    const std::vector<int>& getData() const { return data; }
    
    // DataStructure interface implementation
    std::vector<DSNode> getNodes() const override;
    std::vector<DSEdge> getEdges() const override;
    std::string serializeToDOT() const override;
    void* getDataForRunner() override;
    // Session persistence
    QJsonObject serialize() const override;
    void deserialize(const QJsonObject& obj) override;
    std::string getType() const override;
};
