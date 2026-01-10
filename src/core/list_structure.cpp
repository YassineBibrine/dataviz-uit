#include "list_structure.h"
#include <random>
#include <sstream>

ListStructure::ListStructure() : head(nullptr) {
}

ListStructure::~ListStructure() {
    clearList();
}

void ListStructure::clearList() {
    while (head != nullptr) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

void ListStructure::generateSequential(int size) {
    clearList();
    
    for (int i = 0; i < size; ++i) {
      append(i);
    }
}

void ListStructure::generateRandom(int size) {
    clearList();
    
    std::random_device rd;
 std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    
    for (int i = 0; i < size; ++i) {
        append(dis(gen));
    }
}

void ListStructure::append(int value) {
    ListNode* newNode = new ListNode(value);
    
    if (head == nullptr) {
  head = newNode;
    } else {
        ListNode* current = head;
   while (current->next != nullptr) {
          current = current->next;
        }
        current->next = newNode;
        newNode->prev = current;
    }
}

std::vector<DSNode> ListStructure::getNodes() const {
    std::vector<DSNode> nodes;
    
    ListNode* current = head;
    int index = 0;
    
    while (current != nullptr) {
   std::string nodeId = "list_" + std::to_string(index);
        std::string nodeValue = std::to_string(current->value);
     nodes.emplace_back(nodeId, nodeValue);
        current = current->next;
        index++;
    }
    
    return nodes;
}

std::vector<DSEdge> ListStructure::getEdges() const {
    std::vector<DSEdge> edges;
    
    ListNode* current = head;
    int index = 0;
  
    while (current != nullptr && current->next != nullptr) {
   edges.emplace_back("list_" + std::to_string(index),
          "list_" + std::to_string(index + 1));
 current = current->next;
        index++;
    }
    
    return edges;
}

std::string ListStructure::serializeToDOT() const {
  std::ostringstream oss;
    
    oss << "digraph LinkedList {\n";
    oss << "  rankdir=LR;\n";
    oss << "  node [shape=record];\n";
    
    ListNode* current = head;
    int index = 0;
    
    // Create nodes
  while (current != nullptr) {
        oss << "  node" << index 
     << " [label=\"{" << current->value << " | <ptr>}\"];\n";
      current = current->next;
        index++;
}
    
    // Create edges
    current = head;
    index = 0;
    while (current != nullptr && current->next != nullptr) {
        oss << "  node" << index << ":ptr -> node" << (index + 1) << ";\n";
        current = current->next;
        index++;
    }
    
    // Add null pointer at the end
    if (index > 0) {
        oss << "  null [shape=plaintext];\n";
    oss << "  node" << (index - 1) << ":ptr -> null;\n";
 }
    
    oss << "}\n";
    
    return oss.str();
}

void* ListStructure::getDataForRunner() {
    return static_cast<void*>(head);
}
