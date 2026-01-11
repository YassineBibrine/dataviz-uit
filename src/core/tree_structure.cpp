#include "tree_structure.h"
#include <random>
#include <QJsonArray>
#include <set>
#include <sstream>
#include <algorithm>

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
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);
    std::set<int> used;
    while (used.size() < static_cast<size_t>(count)) {
        int val = dis(gen);
        if (used.insert(val).second) insert(val);
    }
    
    return edges;
}

void TreeStructure::collectNodes(TreeNode* node, std::vector<DSNode>& nodes) const {
    if (!node) return;
    nodes.emplace_back("tree_" + std::to_string(node->value));
    collectNodes(node->left, nodes);
    collectNodes(node->right, nodes);
}

void TreeStructure::collectEdges(TreeNode* node, std::vector<DSEdge>& edges) const {
    if (!node) return;
    std::string id = "tree_" + std::to_string(node->value);
    if (node->left) {
        edges.emplace_back(id, "tree_" + std::to_string(node->left->value));
        collectEdges(node->left, edges);
    }
    if (node->right) {
        edges.emplace_back(id, "tree_" + std::to_string(node->right->value));
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
