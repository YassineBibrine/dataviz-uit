#include "tree_structure.h"
#include <random>
#include <sstream>
#include <queue>
#include <set>
#include <algorithm>

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

void TreeStructure::clearTree() {
  clear(root);
    root = nullptr;
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
    
    int leftHeight = node->left ? node->left->height : 0;
    int rightHeight = node->right ? node->right->height : 0;
    node->height = 1 + std::max(leftHeight, rightHeight);
    node->balanceFactor = rightHeight - leftHeight;
}

TreeNode* TreeStructure::buildBalancedTree(const std::vector<int>& values, int start, int end, TreeNode* parent) {
    if (start > end) {
        return nullptr;
    }
    
    int mid = start + (end - start) / 2;
    TreeNode* node = new TreeNode(values[mid]);
    node->parent = parent;
    
    node->left = buildBalancedTree(values, start, mid - 1, node);
    node->right = buildBalancedTree(values, mid + 1, end, node);
    
    int leftHeight = node->left ? node->left->height : 0;
    int rightHeight = node->right ? node->right->height : 0;
    node->height = 1 + std::max(leftHeight, rightHeight);
    node->balanceFactor = rightHeight - leftHeight;
    
    return node;
}

void TreeStructure::generateRandom(int count) {
    clear(root);
    root = nullptr;
    
    if (count <= 0) return;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    
    std::vector<int> randomValues;
    randomValues.reserve(count);
    for (int i = 0; i < count; ++i) {
        randomValues.push_back(dis(gen));
    }
    
    std::shuffle(randomValues.begin(), randomValues.end(), gen);
    
    root = buildBalancedTree(randomValues, 0, static_cast<int>(randomValues.size()) - 1, nullptr);
}

void TreeStructure::generateBalanced(int count) {
    generateRandom(count);
}

// Use BFS to collect nodes with stable index-based IDs
std::vector<DSNode> TreeStructure::getNodes() const {
  std::vector<DSNode> nodes;
    if (!root) return nodes;
    
    // BFS traversal to assign consistent index-based IDs
  std::queue<const TreeNode*> bfsQueue;
    bfsQueue.push(root);
    int index = 0;
    
    while (!bfsQueue.empty()) {
        const TreeNode* node = bfsQueue.front();
        bfsQueue.pop();
        
      if (node) {
  // Use index-based ID for stability (doesn't change when value changes)
            std::string nodeId = "tree_" + std::to_string(index);
            std::string nodeValue = std::to_string(node->value);
nodes.emplace_back(nodeId, nodeValue);
        index++;
      
            // Add children (even if null, we skip them in the loop)
            if (node->left) bfsQueue.push(node->left);
     if (node->right) bfsQueue.push(node->right);
        }
    }
    
    return nodes;
}

// Helper to build a map from TreeNode* to index
void TreeStructure::buildNodeIndexMap(std::map<const TreeNode*, int>& nodeToIndex) const {
    if (!root) return;
    
    std::queue<const TreeNode*> bfsQueue;
    bfsQueue.push(root);
    int index = 0;
    
    while (!bfsQueue.empty()) {
    const TreeNode* node = bfsQueue.front();
        bfsQueue.pop();
        
     if (node) {
       nodeToIndex[node] = index;
            index++;
            
 if (node->left) bfsQueue.push(node->left);
    if (node->right) bfsQueue.push(node->right);
     }
    }
}

std::vector<DSEdge> TreeStructure::getEdges() const {
    std::vector<DSEdge> edges;
    if (!root) return edges;
    
    // Build node to index map
    std::map<const TreeNode*, int> nodeToIndex;
    buildNodeIndexMap(nodeToIndex);
    
    // BFS to collect edges using index-based IDs
    std::queue<const TreeNode*> bfsQueue;
    bfsQueue.push(root);
    
    while (!bfsQueue.empty()) {
      const TreeNode* node = bfsQueue.front();
   bfsQueue.pop();
     
        if (node) {
            std::string parentId = "tree_" + std::to_string(nodeToIndex[node]);
        
  if (node->left) {
 std::string leftId = "tree_" + std::to_string(nodeToIndex[node->left]);
   edges.emplace_back(parentId, leftId);
         bfsQueue.push(node->left);
     }
    if (node->right) {
    std::string rightId = "tree_" + std::to_string(nodeToIndex[node->right]);
  edges.emplace_back(parentId, rightId);
     bfsQueue.push(node->right);
            }
        }
    }
    
    return edges;
}

void TreeStructure::collectNodes(TreeNode* node, std::vector<DSNode>& nodes) const {
    // Deprecated - kept for compatibility, but getNodes() now uses BFS
    if (node == nullptr) {
        return;
    }

    std::string nodeId = "tree_" + std::to_string(node->value);
    std::string nodeValue = std::to_string(node->value);
    nodes.emplace_back(nodeId, nodeValue);
    
    collectNodes(node->left, nodes);
    collectNodes(node->right, nodes);
}

void TreeStructure::collectEdges(TreeNode* node, std::vector<DSEdge>& edges) const {
    // Deprecated - kept for compatibility, but getEdges() now uses BFS
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
