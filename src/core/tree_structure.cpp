#include "tree_structure.h"
#include <random>
#include <sstream>
#include <queue>
#include <set>

TreeStructure::TreeStructure() : root(nullptr) {
}

TreeStructure::~TreeStructure() {
clear(root);
}

void TreeStructure::clear(TreeNode* node) {
    if (node == nullptr) {
        return;
    }
    
    clear(node->left);
    clear(node->right);
    delete node;
}

void TreeStructure::insert(int value) {
    insertNode(root, value, nullptr);
}

void TreeStructure::insertNode(TreeNode*& node, int value, TreeNode* parent) {
  if (node == nullptr) {
    node = new TreeNode(value);
        node->parent = parent;
        return;
    }

    if (value < node->value) {
        insertNode(node->left, value, node);
    } else {
        insertNode(node->right, value, node);
    }
    
    // Update height (simplified, for AVL would need balancing)
    int leftHeight = node->left ? node->left->height : 0;
    int rightHeight = node->right ? node->right->height : 0;
    node->height = 1 + std::max(leftHeight, rightHeight);
    node->balanceFactor = rightHeight - leftHeight;
}

void TreeStructure::generateRandom(int count) {
    clear(root);
    root = nullptr;
    
  std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);
    std::set<int> usedValues;
    
    while (usedValues.size() < static_cast<size_t>(count)) {
        int value = dis(gen);
    if (usedValues.insert(value).second) {
       insert(value);
        }
    }
}

void TreeStructure::collectNodes(TreeNode* node, std::vector<DSNode>& nodes) const {
    if (node == nullptr) {
        return;
    }
    
    nodes.emplace_back("tree_" + std::to_string(node->value));
    collectNodes(node->left, nodes);
    collectNodes(node->right, nodes);
}

void TreeStructure::collectEdges(TreeNode* node, std::vector<DSEdge>& edges) const {
    if (node == nullptr) {
        return;
 }
    
    std::string nodeId = "tree_" + std::to_string(node->value);
    
    if (node->left) {
        std::string leftId = "tree_" + std::to_string(node->left->value);
      edges.emplace_back(nodeId, leftId);
   collectEdges(node->left, edges);
    }
    
    if (node->right) {
        std::string rightId = "tree_" + std::to_string(node->right->value);
        edges.emplace_back(nodeId, rightId);
        collectEdges(node->right, edges);
    }
}

std::vector<DSNode> TreeStructure::getNodes() const {
    std::vector<DSNode> nodes;
collectNodes(root, nodes);
    return nodes;
}

std::vector<DSEdge> TreeStructure::getEdges() const {
    std::vector<DSEdge> edges;
    collectEdges(root, edges);
    return edges;
}

void TreeStructure::serializeNode(TreeNode* node, std::ostringstream& oss) const {
    if (node == nullptr) {
        return;
    }
    
    oss << "  node" << node->value 
  << " [label=\"" << node->value << "\"];\n";
    
    serializeNode(node->left, oss);
    serializeNode(node->right, oss);
}

void TreeStructure::serializeEdges(TreeNode* node, std::ostringstream& oss) const {
    if (node == nullptr) {
        return;
    }
    
    if (node->left) {
        oss << "  node" << node->value << " -> node" << node->left->value 
            << " [label=\"L\"];\n";
     serializeEdges(node->left, oss);
    }
    
    if (node->right) {
   oss << "  node" << node->value << " -> node" << node->right->value 
       << " [label=\"R\"];\n";
      serializeEdges(node->right, oss);
    }
}

std::string TreeStructure::serializeToDOT() const {
    std::ostringstream oss;
    
    oss << "digraph BinaryTree {\n";
    oss << "  node [shape=circle];\n";
    
    if (root != nullptr) {
        serializeNode(root, oss);
        serializeEdges(root, oss);
    }
    
    oss << "}\n";
    
    return oss.str();
}

void* TreeStructure::getDataForRunner() {
    return static_cast<void*>(root);
}
