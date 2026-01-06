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
        // Default edge attempts to create a reasonably connected graph
    int edgeAttempts = static_cast<int>(nodeCount * 1.5);
        generateRandom(nodeCount, edgeAttempts);
    }
}

void GraphStructure::generateRandom(int nodeCount, int edgeAttempts) {
    if (!graph) {
     graph = std::make_unique<Graph>(false);
    }
    
    graph->clear();
 
    if (nodeCount <= 0) return;
    
    // Default edge attempts if not specified
    if (edgeAttempts <= 0) {
 edgeAttempts = static_cast<int>(nodeCount * 1.5);
    }
    
    // Add nodes
    for (int i = 0; i < nodeCount; ++i) {
        std::string nodeId = "n" + std::to_string(i);
        std::map<std::string, std::string> props;
    props["label"] = std::to_string(i);
     props["value"] = std::to_string(i);
        graph->addNode(nodeId, props);
    }
  
    // Add random edges to make a connected graph
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> nodeDis(0, nodeCount - 1);
    std::uniform_real_distribution<> weightDis(1.0, 10.0);

 // First, ensure the graph is connected by creating a spanning tree
    for (int i = 1; i < nodeCount; ++i) {
    std::uniform_int_distribution<> parentDis(0, i - 1);
        int parent = parentDis(gen);
   std::string fromId = "n" + std::to_string(parent);
        std::string toId = "n" + std::to_string(i);
        double weight = weightDis(gen);
        graph->addEdge(fromId, toId, weight);
    }
    
    // Add additional random edges
    int additionalEdges = edgeAttempts - (nodeCount - 1);
    int edgesAdded = 0;
  int attempts = 0;
    
    while (edgesAdded < additionalEdges && attempts < additionalEdges * 3) {
        int from = nodeDis(gen);
        int to = nodeDis(gen);
        
        if (from != to) {
     std::string fromId = "n" + std::to_string(from);
        std::string toId = "n" + std::to_string(to);
      
            if (!graph->hasEdge(fromId, toId)) {
      double weight = weightDis(gen);
            graph->addEdge(fromId, toId, weight);
     edgesAdded++;
            }
        }
     attempts++;
    }
}

std::vector<DSNode> GraphStructure::getNodes() const {
    std::vector<DSNode> nodes;
    
    if (graph) {
      auto nodeIds = graph->getAllNodeIds();
        nodes.reserve(nodeIds.size());
      
        for (const auto& id : nodeIds) {
      nodes.emplace_back(id);
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
