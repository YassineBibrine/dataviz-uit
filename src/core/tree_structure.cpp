#include "tree_structure.h"
#include <random>
#include <QJsonArray>
#include <set>
#include <sstream>
#include <algorithm>
#include <queue>
#include <map>
#include <unordered_map>

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
 // For simple binary tree, just add the node without BST logic
 // The structure will be determined by user-drawn edges, not value comparison
 TreeNode* newNode = new TreeNode(value);

 if (!root) {
 root = newNode;
 root->parent = nullptr;
 return;
 }

 // For non-BST tree, we'll just attach it as a temporary orphan
 // The actual parent-child relationships will be established through user-drawn edges
 // This allows manual tree construction without value-based positioning

 // Note: In a manual tree, nodes don't auto-insert based on values
 // This method is kept for compatibility but doesn't enforce BST properties
 delete newNode; // Don't auto-insert, user will create structure manually
}

TreeNode* TreeStructure::addChild(TreeNode* parentNode, int value, bool isLeft) {
 if (!parentNode) return nullptr;

 TreeNode** childSlot = isLeft ? &(parentNode->left) : &(parentNode->right);

 // Check if slot is already occupied
 if (*childSlot != nullptr) {
 return nullptr; // Slot occupied
 }

 // Create new child node
 TreeNode* newChild = new TreeNode(value);
 newChild->parent = parentNode;
 *childSlot = newChild;

 return newChild;
}

void TreeStructure::insertNode(TreeNode*& node, int value, TreeNode* parent) {
 // Removed BST insertion logic
 // This is now a manual tree structure where users control the topology
 if (!node) {
 node = new TreeNode(value);
 node->parent = parent;
 return;
 }
 // Don't auto-insert based on value comparison
}

void TreeStructure::generateRandom(int count) {
 clear(root);
 root = nullptr;

 if (count <=0) return;

 // Generate unique random values (1 to count*2) for better readability
 std::vector<int> values;
 values.reserve(count);

 std::random_device rd;
 std::mt19937 gen(rd());
 std::uniform_int_distribution<> dis(1, count *2);

 std::set<int> used;
 while (values.size() < static_cast<size_t>(count)) {
 int val = dis(gen);
 if (used.insert(val).second) {
 values.push_back(val);
 }
 }

 // Build balanced tree structure (balanced shape) using the vector order
 root = buildBalancedTreeHelper(values,0, static_cast<int>(values.size()) -1, nullptr);
}

void TreeStructure::generateBalanced(int count) {
 // Same as generateRandom - always creates balanced tree
 generateRandom(count);
}

// Helper function to build a balanced tree from values (by index)
TreeNode* TreeStructure::buildBalancedTreeHelper(const std::vector<int>& values, int start, int end, TreeNode* parent) {
 if (start > end) return nullptr;

 int mid = start + (end - start) /2;
 TreeNode* node = new TreeNode(values[mid]);
 node->parent = parent;

 node->left = buildBalancedTreeHelper(values, start, mid -1, node);
 node->right = buildBalancedTreeHelper(values, mid +1, end, node);

 return node;
}

void TreeStructure::collectNodes(TreeNode* node, std::vector<DSNode>& nodes) const {
 if (!node) return;

 // BFS order to ensure stable 'tree_i' ids
 std::queue<const TreeNode*> q;
 std::vector<const TreeNode*> list;
 std::unordered_map<const TreeNode*, int> idxMap;

 q.push(node);
 while (!q.empty()) {
 const TreeNode* cur = q.front(); q.pop();
 if (!cur) continue;
 idxMap[cur] = static_cast<int>(list.size());
 list.push_back(cur);
 if (cur->left) q.push(cur->left);
 if (cur->right) q.push(cur->right);
 }

 for (size_t i =0; i < list.size(); ++i) {
 std::string nodeId = "tree_" + std::to_string(static_cast<int>(i));
 std::string nodeValue = std::to_string(list[i]->value);
 nodes.emplace_back(nodeId, nodeValue);
 }
}

void TreeStructure::collectEdges(TreeNode* node, std::vector<DSEdge>& edges) const {
 if (!node) return;

 std::queue<const TreeNode*> q;
 std::vector<const TreeNode*> list;
 std::unordered_map<const TreeNode*, int> idxMap;

 q.push(node);
 while (!q.empty()) {
 const TreeNode* cur = q.front(); q.pop();
 if (!cur) continue;
 idxMap[cur] = static_cast<int>(list.size());
 list.push_back(cur);
 if (cur->left) q.push(cur->left);
 if (cur->right) q.push(cur->right);
 }

 for (size_t i =0; i < list.size(); ++i) {
 const TreeNode* cur = list[i];
 std::string parentId = "tree_" + std::to_string(static_cast<int>(i));
 if (cur->left) {
 int li = idxMap[cur->left];
 std::string leftId = "tree_" + std::to_string(li);
 edges.emplace_back(parentId, leftId);
 }
 if (cur->right) {
 int ri = idxMap[cur->right];
 std::string rightId = "tree_" + std::to_string(ri);
 edges.emplace_back(parentId, rightId);
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
 oss << "digraph BinaryTree {\n node [shape=circle];\n";
 for (auto& n : getNodes()) oss << " " << n.id << ";\n";
 for (auto& e : getEdges()) oss << " " << e.from << " -> " << e.to << ";\n";
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
 int index =0;
 root = build(index, nullptr);
}

std::string TreeStructure::getType() const { return "Tree"; }
