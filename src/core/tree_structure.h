#pragma once

#include "data_structure.h"
#include "tree_node.h"
#include <vector>
#include <map>

/**
 * @class TreeStructure
 * @brief Binary Search Tree data structure
 */
class TreeStructure : public DataStructure {
private:
    TreeNode* root;
    
    void insertNode(TreeNode*& node, int value, TreeNode* parent);
    void collectNodes(TreeNode* node, std::vector<DSNode>& nodes) const;
    void collectEdges(TreeNode* node, std::vector<DSEdge>& edges) const;
    void serializeNode(TreeNode* node, std::ostringstream& oss) const;
    void serializeEdges(TreeNode* node, std::ostringstream& oss) const;
    
    // Helper to build balanced BST from sorted array
    TreeNode* buildBalancedTree(const std::vector<int>& sortedValues, int start, int end, TreeNode* parent);
    
    // Helper for index-based node IDs
    void buildNodeIndexMap(std::map<const TreeNode*, int>& nodeToIndex) const;
    
public:
    TreeStructure();
    ~TreeStructure();
    
    /**
     * @brief Clear a subtree (recursive delete)
     * @param node Root of subtree to clear
     */
    void clear(TreeNode* node);
    
    /**
 * @brief Clear the entire tree
     */
    void clearTree();
    
    /**
     * @brief Set the root node
     * @param newRoot New root node (can be nullptr)
   */
    void setRoot(TreeNode* newRoot) { root = newRoot; }

    /**
     * @brief Insert a value into the tree
     * @param value Value to insert
     */
    void insert(int value);
    
    /**
  * @brief Generate a balanced random tree
     * @param count Number of nodes
   */
 void generateRandom(int count);

    /**
   * @brief Generate a balanced tree (alias for generateRandom)
     * @param count Number of nodes
     */
    void generateBalanced(int count);
    
  /**
     * @brief Get the root node
     */
TreeNode* getRoot() { return root; }
    const TreeNode* getRoot() const { return root; }
    
 // DataStructure interface
    std::vector<DSNode> getNodes() const override;
    std::vector<DSEdge> getEdges() const override;
    std::string serializeToDOT() const override;
    void* getDataForRunner() override;
    // 🔥 Session
    QJsonObject serialize() const override;
    void deserialize(const QJsonObject& obj) override;
    std::string getType() const override;
};
