#include "list_structure.h"
#include <random>
#include <QJsonArray>
#include <sstream>

// ListStructure manages a doubly-linked list of ListNode instances and exposes
// a visualization-friendly representation (DSNode / DSEdge), plus session
// serialization support.

ListStructure::ListStructure() : head(nullptr) {}
ListStructure::~ListStructure() { clearList(); }

// Delete all nodes in the list.
void ListStructure::clearList() {
 while (head) {
 ListNode* temp = head;
 head = head->next;
 delete temp;
 }
}

// Generate a sequential list: values0..size-1
void ListStructure::generateSequential(int size) {
 clearList();
 for (int i =0; i < size; ++i) append(i);
}

// Generate a list of random values in range [1,100]
void ListStructure::generateRandom(int size) {
 clearList();
 std::random_device rd;
 std::mt19937 gen(rd());
 std::uniform_int_distribution<> dis(1,100);
 for (int i =0; i < size; ++i) append(dis(gen));
}

// Append a new value to the end of the list.
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

// Convert list nodes to DSNode objects for rendering. Each node gets an id
// like "list_i" and the display value comes from the node's integer value.
std::vector<DSNode> ListStructure::getNodes() const {
 std::vector<DSNode> nodes;
 ListNode* cur = head;
 int idx =0;
 while (cur) {
 // Include the value in the node representation
 nodes.emplace_back("list_" + std::to_string(idx), std::to_string(cur->value));
 cur = cur->next;
 idx++;
 }
 return nodes;
}

// Produce sequential edges between consecutive list elements for visualization.
std::vector<DSEdge> ListStructure::getEdges() const {
 std::vector<DSEdge> edges;
 ListNode* cur = head;
 int idx =0;
 while (cur && cur->next) {
 edges.emplace_back("list_" + std::to_string(idx), "list_" + std::to_string(idx +1));
 cur = cur->next;
 idx++;
 }
 return edges;
}

// DOT export helpful for debugging: produce left-to-right linked list layout.
std::string ListStructure::serializeToDOT() const {
 std::ostringstream oss;
 oss << "digraph LinkedList {\n rankdir=LR;\n node [shape=record];\n";
 ListNode* cur = head;
 int idx =0;
 while (cur) {
 oss << " node" << idx << " [label=\"" << cur->value << "\"];\n";
 cur = cur->next;
 idx++;
 }
 cur = head;
 idx =0;
 while (cur && cur->next) {
 oss << " node" << idx << " -> node" << (idx +1) << ";\n";
 cur = cur->next;
 idx++;
 }
 oss << "}\n";
 return oss.str();
}

// Provide pointer to head for algorithm runners that operate on linked lists.
void* ListStructure::getDataForRunner() { return static_cast<void*>(head); }

// Session serialization: save type, values array and node positions (if any).
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

 // Serialize node positions map if present
 QJsonObject positionsObj;
 for (const auto& pair : nodePositions) {
 QJsonObject posObj;
 posObj["x"] = pair.second.x;
 posObj["y"] = pair.second.y;
 positionsObj[QString::fromStdString(pair.first)] = posObj;
 }
 obj["nodePositions"] = positionsObj;

 return obj;
}

// Restore list values and node positions from JSON.
void ListStructure::deserialize(const QJsonObject& obj) {
 clearList();
 QJsonArray arr = obj["values"].toArray();
 for (auto v : arr) append(v.toInt());

 // Deserialize node positions if present
 nodePositions.clear();
 if (obj.contains("nodePositions")) {
 QJsonObject positionsObj = obj["nodePositions"].toObject();
 for (auto it = positionsObj.begin(); it != positionsObj.end(); ++it) {
 std::string nodeId = it.key().toStdString();
 QJsonObject posObj = it.value().toObject();
 double x = posObj["x"].toDouble();
 double y = posObj["y"].toDouble();
 nodePositions[nodeId] = DSNodePosition(x, y);
 }
 }
}

std::string ListStructure::getType() const { return "LinkedList"; }
