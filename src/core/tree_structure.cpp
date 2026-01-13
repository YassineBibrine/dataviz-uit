#include "tree_structure.h"
#include <random>
#include <QJsonArray>
#include <set>
#include <sstream>
#include <algorithm>
#include <queue>
#include <map>

TreeStructure::TreeStructure() : root(nullptr) {}
TreeStructure::~TreeStructure() { clear(root); }

void TreeStructure::clear(TreeNode* node) {
    if (!node) return;
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
    if (!node) {
        node = new TreeNode(value);
        node->parent = parent;
        return;
    }
    if (value < node->value) insertNode(node->left, value, node);
    else insertNode(node->right, value, node);
}

void TreeStructure::generateRandom(int count) {
    clear(root);
    root = nullptr;
    
  if (count <= 0) return;
    
    // Generate unique small values (1 to count*2) for better readability
    std::vector<int> values;
    values.reserve(count);
    
    std::random_device rd;
  std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, count * 2);
    
    std::set<int> used;
    while (values.size() < static_cast<size_t>(count)) {
int val = dis(gen);
        if (used.insert(val).second) {
          values.push_back(val);
        }
    }
  
    // Sort values to create a balanced BST
    std::sort(values.begin(), values.end());
    
    // Build balanced tree using the sorted values
    buildBalancedTree(values, 0, values.size() - 1, nullptr);
}

// Helper function to build a balanced tree from sorted values
void TreeStructure::buildBalancedTree(const std::vector<int>& values, int start, int end, TreeNode* parent) {
  if (start > end) return;
    
    // Find middle element
    int mid = start + (end - start) / 2;
    
 // Create node with middle value
    TreeNode* node = new TreeNode(values[mid]);
    node->parent = parent;
 
    // Set as root if no parent
    if (parent == nullptr) {
     root = node;
    } else if (values[mid] < parent->value) {
        parent->left = node;
    } else {
        parent->right = node;
    }
    
    // Recursively build left and right subtrees
    buildBalancedTree(values, start, mid - 1, node);
    buildBalancedTree(values, mid + 1, end, node);
}

void TreeStructure::collectNodes(TreeNode* node, std::vector<DSNode>& nodes) const {
    if (!node) return;
    
    // Use BFS to assign indices consistently (matches visualization loading)
    std::queue<const TreeNode*> bfsQueue;
    std::map<const TreeNode*, int> nodeToIndex;
    
    bfsQueue.push(node);
    int index = 0;
  
    while (!bfsQueue.empty()) {
 const TreeNode* current = bfsQueue.front();
        bfsQueue.pop();
        
   if (current) {
            nodeToIndex[current] = index++;
         if (current->left) bfsQueue.push(current->left);
            if (current->right) bfsQueue.push(current->right);
        }
    }
    
    // Now collect nodes with index-based IDs
    for (const auto& pair : nodeToIndex) {
        std::string nodeId = "tree_" + std::to_string(pair.second);
        std::string nodeValue = std::to_string(pair.first->value);
        nodes.emplace_back(nodeId, nodeValue);
    }
}

void TreeStructure::collectEdges(TreeNode* node, std::vector<DSEdge>& edges) const {
    if (!node) return;
    
    // Use BFS to assign indices consistently
    std::queue<const TreeNode*> bfsQueue;
    std::map<const TreeNode*, int> nodeToIndex;
    
    bfsQueue.push(node);
    int index = 0;
    
    while (!bfsQueue.empty()) {
     const TreeNode* current = bfsQueue.front();
   bfsQueue.pop();
 
        if (current) {
      nodeToIndex[current] = index++;
            if (current->left) bfsQueue.push(current->left);
  if (current->right) bfsQueue.push(current->right);
        }
    }
    
    // Collect edges using index-based IDs
    for (const auto& pair : nodeToIndex) {
        const TreeNode* current = pair.first;
        std::string parentId = "tree_" + std::to_string(pair.second);
        
     if (current->left) {
            auto it = nodeToIndex.find(current->left);
    if (it != nodeToIndex.end()) {
            std::string leftId = "tree_" + std::to_string(it->second);
   edges.emplace_back(parentId, leftId);
            }
        }
    
        if (current->right) {
            auto it = nodeToIndex.find(current->right);
            if (it != nodeToIndex.end()) {
  std::string rightId = "tree_" + std::to_string(it->second);
      edges.emplace_back(parentId, rightId);
            }
        }
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

std::string TreeStructure::serializeToDOT() const {
    std::ostringstream oss;
    oss << "digraph BinaryTree {\n  node [shape=circle];\n";
    for (auto& n : getNodes()) oss << "  " << n.id << ";\n";
    for (auto& e : getEdges()) oss << "  " << e.from << " -> " << e.to << ";\n";
    oss << "}\n";
    return oss.str();
}

void* TreeStructure::getDataForRunner() { return static_cast<void*>(root); }

// 🔥 Session
QJsonObject TreeStructure::serialize() const {
    QJsonObject obj;
    obj["type"] = QString::fromStdString(getType());
    QJsonArray arr;
    std::function<void(TreeNode*)> preorder = [&](TreeNode* n) {
        if (!n) { arr.append(QJsonValue()); return; }
        arr.append(n->value);
        preorder(n->left);
        preorder(n->right);
        };
    preorder(root);
    obj["values"] = arr;
    return obj;
}

void TreeStructure::deserialize(const QJsonObject& obj) {
    clear(root);
    root = nullptr;
    QJsonArray arr = obj["values"].toArray();
    std::function<TreeNode* (int&, TreeNode*)> build = [&](int& idx, TreeNode* parent) -> TreeNode* {
        if (idx >= arr.size() || arr[idx].isNull()) { idx++; return nullptr; }
        TreeNode* n = new TreeNode(arr[idx].toInt());
        n->parent = parent;
        idx++;
        n->left = build(idx, n);
        n->right = build(idx, n);
        return n;
        };
    int index = 0;
    root = build(index, nullptr);
}

std::string TreeStructure::getType() const { return "Tree"; }
