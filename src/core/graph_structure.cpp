#include "graph_structure.h"
#include <random>
#include <sstream>
#include <map>

GraphStructure::GraphStructure() 
    : graph(std::make_unique<Graph>(false)) {
}

GraphStructure::GraphStructure(bool directed)
    : graph(std::make_unique<Graph>(directed)) {
}

GraphStructure::GraphStructure(int nodeCount, bool directed)
    : graph(std::make_unique<Graph>(directed)) {
    if (nodeCount > 0) {
        generateRandom(nodeCount, 0);  // 0 means fully connected
    }
}

void GraphStructure::generateRandom(int nodeCount, int edgeAttempts) {
    (void)edgeAttempts;  // Unused - we create fully connected graph
    
    if (!graph) {
        graph = std::make_unique<Graph>(false);
    }
    
 graph->clear();
 
    if (nodeCount <= 0) return;
 
    // Random value generator for node values
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> valueDis(1, 100);
    std::uniform_real_distribution<> weightDis(1.0, 10.0);
    
 // Add nodes with random values
    for (int i = 0; i < nodeCount; ++i) {
        std::string nodeId = "n" + std::to_string(i);
        int randomValue = valueDis(gen);
      std::map<std::string, std::string> props;
        props["label"] = std::to_string(randomValue);
  props["value"] = std::to_string(randomValue);
     graph->addNode(nodeId, props);
    }
  
    // Create a fully connected graph (all nodes linked to each other)
    for (int i = 0; i < nodeCount; ++i) {
        for (int j = i + 1; j < nodeCount; ++j) {
     std::string fromId = "n" + std::to_string(i);
   std::string toId = "n" + std::to_string(j);
       double weight = weightDis(gen);
            graph->addEdge(fromId, toId, weight);
        }
    }
}

std::vector<DSNode> GraphStructure::getNodes() const {
    std::vector<DSNode> nodes;
    
    if (graph) {
        auto nodeIds = graph->getAllNodeIds();
        nodes.reserve(nodeIds.size());
      
        for (const auto& id : nodeIds) {
    const Graph::Node* node = graph->getNode(id);
  std::string displayValue = id;  // Default to id
            
    // Use the node value from properties if available
            if (node) {
 auto it = node->properties.find("value");
     if (it != node->properties.end()) {
    displayValue = it->second;
   }
            }
            nodes.emplace_back(id, displayValue);
        }
  }
    
    return nodes;
}

std::vector<DSEdge> GraphStructure::getEdges() const {
    std::vector<DSEdge> edges;
    
    if (graph) {
   auto graphEdges = graph->getAllEdges();
        edges.reserve(graphEdges.size());
   
        for (const auto& edge : graphEdges) {
     edges.emplace_back(edge.from, edge.to);
        }
    }
    
    return edges;
}

std::string GraphStructure::serializeToDOT() const {
    std::ostringstream oss;
    
    if (!graph) {
        return "";
    }
    
    if (graph->isDirected()) {
oss << "digraph G {\n";
    } else {
        oss << "graph G {\n";
    }
    
    oss << "  node [shape=circle];\n";
    
    // Add nodes
    auto nodeIds = graph->getAllNodeIds();
    for (const auto& id : nodeIds) {
        auto* node = graph->getNode(id);
        if (node) {
            oss << "  " << id;
            if (!node->properties.empty()) {
        oss << " [";
         bool first = true;
                for (const auto& [key, value] : node->properties) {
      if (!first) oss << ",";
 oss << key << "=\"" << value << "\"";
   first = false;
                }
   oss << "]";
          }
            oss << ";\n";
        }
    }
    
    // Add edges
    auto edges = graph->getAllEdges();
  std::string connector = graph->isDirected() ? " -> " : " -- ";
    
  for (const auto& edge : edges) {
   oss << "  " << edge.from << connector << edge.to;
        oss << " [label=\"" << edge.weight << "\"]";
        oss << ";\n";
    }
    
    oss << "}\n";
    
    return oss.str();
}

void* GraphStructure::getDataForRunner() {
    return static_cast<void*>(graph.get());
}
