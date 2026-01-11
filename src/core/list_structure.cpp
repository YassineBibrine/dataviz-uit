#include "list_structure.h"
#include <random>
#include <QJsonArray>
#include <sstream>

ListStructure::ListStructure() : head(nullptr) {}
ListStructure::~ListStructure() { clearList(); }

void ListStructure::clearList() {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

void ListStructure::generateSequential(int size) {
    clearList();
    for (int i = 0; i < size; ++i) append(i);
}

void ListStructure::generateRandom(int size) {
    clearList();
    std::random_device rd;
 std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    for (int i = 0; i < size; ++i) append(dis(gen));
}

void ListStructure::append(int value) {
    ListNode* newNode = new ListNode(value);
    if (!head) head = newNode;
    else {
        ListNode* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = newNode;
        newNode->prev = cur;
    }
}

std::vector<DSNode> ListStructure::getNodes() const {
    std::vector<DSNode> nodes;
    ListNode* cur = head;
    int idx = 0;
    while (cur) {
        nodes.emplace_back("list_" + std::to_string(idx));
        cur = cur->next;
        idx++;
    }
    return nodes;
}

std::vector<DSEdge> ListStructure::getEdges() const {
    std::vector<DSEdge> edges;
    ListNode* cur = head;
    int idx = 0;
    while (cur && cur->next) {
        edges.emplace_back("list_" + std::to_string(idx), "list_" + std::to_string(idx + 1));
        cur = cur->next;
        idx++;
    }
    return edges;
}

std::string ListStructure::serializeToDOT() const {
    std::ostringstream oss;
    oss << "digraph LinkedList {\n  rankdir=LR;\n  node [shape=record];\n";
    ListNode* cur = head;
    int idx = 0;
    while (cur) {
        oss << "  node" << idx << " [label=\"" << cur->value << "\"];\n";
        cur = cur->next;
        idx++;
    }
    cur = head;
    idx = 0;
    while (cur && cur->next) {
        oss << "  node" << idx << " -> node" << (idx + 1) << ";\n";
        cur = cur->next;
        idx++;
    }
    oss << "}\n";
    return oss.str();
}

void* ListStructure::getDataForRunner() { return static_cast<void*>(head); }

// 🔥 Session
QJsonObject ListStructure::serialize() const {
    QJsonObject obj;
    obj["type"] = QString::fromStdString(getType());
    QJsonArray arr;
    ListNode* cur = head;
    while (cur) {
        arr.append(cur->value);
        cur = cur->next;
    }
    obj["values"] = arr;
    return obj;
}

void ListStructure::deserialize(const QJsonObject& obj) {
    clearList();
    QJsonArray arr = obj["values"].toArray();
    for (auto v : arr) append(v.toInt());
}

std::string ListStructure::getType() const { return "LinkedList"; }
