#pragma once

#include "data_structure.h"
#include "list_node.h"

/**
 * @class ListStructure
 * @brief Wrapper for linked list data structure
 */
class ListStructure : public DataStructure {
private:
    ListNode* head;
    
public:
    ListStructure();
    ~ListStructure() override;
    
    /**
     * @brief Clear all nodes from the list
     */
    void clearList();
    
    /**
     * @brief Get the head of the list
     * @return Pointer to head node
 */
    ListNode* getHead() { return head; }
    const ListNode* getHead() const { return head; }
    
    /**
     * @brief Generate a sequential list
     * @param size Number of nodes
     */
  void generateSequential(int size);
    
    /**
     * @brief Generate a list with random values
     * @param size Number of nodes
 */
    void generateRandom(int size);
  
    /**
     * @brief Append a value to the end of the list
     * @param value Value to append
     */
    void append(int value);
    
  // DataStructure interface implementation
    std::vector<DSNode> getNodes() const override;
    std::vector<DSEdge> getEdges() const override;
    std::string serializeToDOT() const override;
    void* getDataForRunner() override;
    // 🔥 Session
    QJsonObject serialize() const override;
    void deserialize(const QJsonObject& obj) override;
    std::string getType() const override;
};
