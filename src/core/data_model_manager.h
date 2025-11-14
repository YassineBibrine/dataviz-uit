#pragma once

#include <string>
#include <utility>
#include <vector>
#include "graph.h"
#include "list_node.h"
#include "tree_node.h"

class DataModelManager {
 Graph graph;
 TreeNode* rootNode{nullptr};
 ListNode* head{nullptr};
public:
 DataModelManager() = default;
 ~DataModelManager() = default;

 void* createDataStructure(const std::string& type, int size);
 void* generateRandomData(const std::string& type, int size);
 std::string serializeToDOT(void* data);
};
