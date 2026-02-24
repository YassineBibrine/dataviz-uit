#pragma once

/**
 * @struct TreeNode
 * @brief Binary tree node used by TreeStructure.
 *
 * Fields include pointers to left/right children and parent as well as
 * auxiliary data for balancing/layout algorithms (height, balanceFactor).
 */
struct TreeNode {
  int value;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;
    int height;
    int balanceFactor;
    
    explicit TreeNode(int val);
    ~TreeNode();
    
    bool isLeaf() const;
    bool hasLeftChild() const;
    bool hasRightChild() const;
};
