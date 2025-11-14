#include "tree_node.h"

TreeNode::TreeNode(int val) 
    : value(val), left(nullptr), right(nullptr), parent(nullptr), height(1), balanceFactor(0) {}

TreeNode::~TreeNode() {
    // Note: Caller is responsible for managing tree deletion
}

bool TreeNode::isLeaf() const {
    return left == nullptr && right == nullptr;
}

bool TreeNode::hasLeftChild() const {
    return left != nullptr;
}

bool TreeNode::hasRightChild() const {
    return right != nullptr;
}
