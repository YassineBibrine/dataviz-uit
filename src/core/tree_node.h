#pragma once

/**
 * @struct TreeNode
 * @brief Binary search tree node
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
