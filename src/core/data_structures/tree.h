#ifndef TREE_H
#define TREE_H

#include <string>
#include <memory>
#include <vector>
#include <map>

/**
 * @class Tree
 * @brief Generic tree data structure for internal representation
 * 
 * Supports:
 * - Binary trees, BSTs, AVL, Red-Black, B-Trees
 * - Node with arbitrary properties
 * - Tree traversal operations
 */
template<typename T>
class TreeNode {
public:
    T value;
  std::string id;
    std::map<std::string, std::string> properties;
    
    std::shared_ptr<TreeNode<T>> left;
    std::shared_ptr<TreeNode<T>> right;
    std::shared_ptr<TreeNode<T>> parent;
    
    TreeNode(const T &val, const std::string &nodeId)
        : value(val), id(nodeId), left(nullptr), right(nullptr), parent(nullptr)
    {
    }
};

class Tree {
public:
    Tree();
    
    void insertNode(const std::string &id, const std::string &parentId = "");
    void removeNode(const std::string &id);
    void updateNodeProperty(const std::string &id, const std::string &key, 
       const std::string &value);
    
    bool hasNode(const std::string &id) const;
 int getNodeCount() const;

private:
    struct Node {
        std::string id;
        std::string parentId;
        std::map<std::string, std::string> properties;
        std::vector<std::string> childrenIds;
    };
    
    std::map<std::string, Node> nodes;
    std::string rootId;
};

#endif // TREE_H
