#pragma once

#include "data_structure.h"
#include "tree_node.h"

/**
 * @class TreeStructure
 * @brief Wrapper for binary tree data structure
 */
class TreeStructure : public DataStructure {
private:
    TreeNode* root;
    
  void clear(TreeNode* node);
    void insertNode(TreeNode*& node, int value, TreeNode* parent);
    void collectNodes(TreeNode* node, std::vector<DSNode>& nodes) const;
    void collectEdges(TreeNode* node, std::vector<DSEdge>& edges) const;
    void serializeNode(TreeNode* node, std::ostringstream& oss) const;
    void serializeEdges(TreeNode* node, std::ostringstream& oss) const;
    
public:
    TreeStructure();
    ~TreeStructure() override;
    
    /**
     * @brief Get the root of the tree
     * @return Pointer to root node
     */
    TreeNode* getRoot() { return root; }
 const TreeNode* getRoot() const { return root; }
    
    /**
     * @brief Insert a value into the tree (BST insertion)
     * @param value Value to insert
     */
    void insert(int value);
    
    /**
     * @brief Generate a random tree
     * @param count Number of nodes
     */
    void generateRandom(int count);
    
    // DataStructure interface implementation
    std::vector<DSNode> getNodes() const override;
    std::vector<DSEdge> getEdges() const override;
    std::string serializeToDOT() const override;
    void* getDataForRunner() override;
};
