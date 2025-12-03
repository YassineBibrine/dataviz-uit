#include "graph.h"
#include <algorithm>

Graph::Graph(bool directed) : directed(directed) {}

Graph::~Graph() {
    clear();
}

void Graph::addNode(const std::string& id, const std::map<std::string, std::string>& properties) {
    if (nodes.find(id) != nodes.end()) {
  return;
    }
    
    Node node(id);
    node.properties = properties;
    nodes[id] = node;
    adjacencyList[id] = std::vector<std::string>();
}

void Graph::addEdge(const std::string& from, const std::string& to, double weight,
         const std::map<std::string, std::string>& properties) {
    if (!hasNode(from) || !hasNode(to)) {
        return;
 }
    
    if (hasEdge(from, to)) {
      return;
    }
    
    Edge edge(from, to, weight);
    edge.properties = properties;
    edges.push_back(edge);
    
    nodes[from].outDegree++;
    nodes[to].inDegree++;
    
    adjacencyList[from].push_back(to);
    if (!directed) {
   adjacencyList[to].push_back(from);
    }
}

void Graph::removeNode(const std::string& id) {
    auto it = nodes.find(id);
    if (it == nodes.end()) {
      return;
 }
    
    // Remove all edges connected to this node
    edges.erase(
        std::remove_if(edges.begin(), edges.end(),
            [&id](const Edge& e) { return e.from == id || e.to == id; }),
        edges.end()
    );
    
    nodes.erase(it);
    adjacencyList.erase(id);
    updateAdjacencyList();
}

void Graph::removeEdge(const std::string& from, const std::string& to) {
    auto it = std::find_if(edges.begin(), edges.end(),
        [&from, &to](const Edge& e) { return e.from == from && e.to == to; });
    
    if (it != edges.end()) {
edges.erase(it);
  nodes[from].outDegree--;
     nodes[to].inDegree--;
        updateAdjacencyList();
    }
}

bool Graph::hasNode(const std::string& id) const {
    return nodes.find(id) != nodes.end();
}

bool Graph::hasEdge(const std::string& from, const std::string& to) const {
    return std::find_if(edges.begin(), edges.end(),
        [&from, &to](const Edge& e) { return e.from == from && e.to == to; }) != edges.end();
}

int Graph::getNodeCount() const {
    return nodes.size();
}

int Graph::getEdgeCount() const {
    return edges.size();
}

std::vector<std::string> Graph::getNeighbors(const std::string& id) const {
    auto it = adjacencyList.find(id);
    if (it != adjacencyList.end()) {
        return it->second;
    }
    return std::vector<std::string>();
}

Graph::Node* Graph::getNode(const std::string& id) {
    auto it = nodes.find(id);
  if (it != nodes.end()) {
  return &it->second;
    }
    return nullptr;
}

const Graph::Node* Graph::getNode(const std::string& id) const {
    auto it = nodes.find(id);
    if (it != nodes.end()) {
        return &it->second;
    }
    return nullptr;
}

Graph::Edge* Graph::getEdge(const std::string& from, const std::string& to) {
    auto it = std::find_if(edges.begin(), edges.end(),
      [&from, &to](Edge& e) { return e.from == from && e.to == to; });
    
    if (it != edges.end()) {
        return &(*it);
    }
    return nullptr;
}

std::vector<std::string> Graph::getAllNodeIds() const {
    std::vector<std::string> ids;
    ids.reserve(nodes.size());
    
    for (const auto& pair : nodes) {
    ids.push_back(pair.first);
    }
    
    return ids;
}

const std::vector<Graph::Edge>& Graph::getAllEdges() const {
    return edges;
}

void Graph::clear() {
    nodes.clear();
    edges.clear();
    adjacencyList.clear();
}

Graph* Graph::clone() const {
    Graph* newGraph = new Graph(directed);
    
    for (const auto& pair : nodes) {
        newGraph->addNode(pair.first, pair.second.properties);
    }
    
    for (const auto& edge : edges) {
        newGraph->addEdge(edge.from, edge.to, edge.weight, edge.properties);
    }
    
    return newGraph;
}

void Graph::updateAdjacencyList() {
    // Rebuild adjacency list from edges
    for (auto& pair : adjacencyList) {
  pair.second.clear();
  }
    
    for (const auto& edge : edges) {
        adjacencyList[edge.from].push_back(edge.to);
        if (!directed) {
adjacencyList[edge.to].push_back(edge.from);
        }
    }
}
