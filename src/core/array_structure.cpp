#include "array_structure.h"
#include <random>
#include <sstream>

ArrayStructure::ArrayStructure(int size) {
    if (size > 0) {
        fillRandom(size);
    }
}

void ArrayStructure::fillRandom(int size) {
  data.clear();
    data.reserve(size);
    
    std::random_device rd;
    std::mt19937 gen(rd());
 std::uniform_int_distribution<> dis(1, 100);
  
    for (int i = 0; i < size; ++i) {
        data.push_back(dis(gen));
    }
}

std::vector<DSNode> ArrayStructure::getNodes() const {
    std::vector<DSNode> nodes;
    nodes.reserve(data.size());
    
    for (size_t i = 0; i < data.size(); ++i) {
        std::string nodeId = "arr_" + std::to_string(i);
        std::string nodeValue = std::to_string(data[i]);
        nodes.emplace_back(nodeId, nodeValue);
    }
    
    return nodes;
}

std::vector<DSEdge> ArrayStructure::getEdges() const {
    std::vector<DSEdge> edges;
    
    // Arrays typically don't have edges in their visualization
    // but we can show sequential connections
    for (size_t i = 0; i + 1 < data.size(); ++i) {
        edges.emplace_back("arr_" + std::to_string(i), 
         "arr_" + std::to_string(i + 1));
 }
    
    return edges;
}

std::string ArrayStructure::serializeToDOT() const {
    std::ostringstream oss;
    
    oss << "digraph Array {\n";
    oss << "  rankdir=LR;\n";
    oss << "  node [shape=record];\n";
    
    // Create record-style node for array
    oss << "  array [label=\"";
    for (size_t i = 0; i < data.size(); ++i) {
        if (i > 0) oss << "|";
oss << "<f" << i << "> " << data[i];
    }
    oss << "\"];\n";
    
    oss << "}\n";
  
    return oss.str();
}

void* ArrayStructure::getDataForRunner() {
    return static_cast<void*>(&data);
}
