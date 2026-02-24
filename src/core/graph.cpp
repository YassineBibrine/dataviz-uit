#include "graph.h"
#include <algorithm>

// Basic Graph implementation: maintain nodes, edges and adjacency list.
// Supports directed or undirected graphs; for undirected graphs we add
// symmetric edges to the internal edges vector so algorithms that iterate
// edges see both directions.

Graph::Graph(bool directed) : directed(directed) {}

Graph::~Graph() {
 clear();
}

void Graph::addNode(const std::string& id, const std::map<std::string, std::string>& properties) {
 if (nodes.find(id) != nodes.end()) {
 return; // Node already exists
 }
 
 Node node(id);
 node.properties = properties;
 nodes[id] = node;
 adjacencyList[id] = std::vector<std::string>();
}

void Graph::addEdge(const std::string& from, const std::string& to, double weight,
 const std::map<std::string, std::string>& properties) {
 if (!hasNode(from) || !hasNode(to)) {
 return; // ignore edges referencing unknown nodes
 }
 
 // Prevent duplicate logical edges. For undirected graphs treat (a,b) and (b,a) as the same.
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
 // For undirected graphs also add the reverse logical edge so both directions appear
 Edge rev(to, from, weight);
 rev.properties = properties;
 edges.push_back(rev);

 nodes[to].outDegree++;
 nodes[from].inDegree++;

 adjacencyList[to].push_back(from);
 }
}

void Graph::removeNode(const std::string& id) {
 auto it = nodes.find(id);
 if (it == nodes.end()) {
 return; // not present
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
 // Remove any matching edge(s). For undirected graphs remove both directions.
 auto it = std::remove_if(edges.begin(), edges.end(),
 [&from, &to, this](const Edge& e) {
 if (directed) return e.from == from && e.to == to;
 return (e.from == from && e.to == to) || (e.from == to && e.to == from);
 });

 if (it != edges.end()) {
 // Adjust degrees conservatively: recalculate after erase to avoid mismatches
 edges.erase(it, edges.end());
 // Recompute degrees from edges
 for (auto& p : nodes) {
 p.second.inDegree =0;
 p.second.outDegree =0;
 }
 for (const auto& e : edges) {
 if (nodes.find(e.from) != nodes.end()) nodes[e.from].outDegree++;
 if (nodes.find(e.to) != nodes.end()) nodes[e.to].inDegree++;
 }
 updateAdjacencyList();
 }
}

bool Graph::hasNode(const std::string& id) const {
 return nodes.find(id) != nodes.end();
}

bool Graph::hasEdge(const std::string& from, const std::string& to) const {
 if (directed) {
 return std::find_if(edges.begin(), edges.end(),
 [&from, &to](const Edge& e) { return e.from == from && e.to == to; }) != edges.end();
 } else {
 // For undirected graphs consider either direction as the same logical edge
 return std::find_if(edges.begin(), edges.end(),
 [&from, &to](const Edge& e) { return (e.from == from && e.to == to) || (e.from == to && e.to == from); }) != edges.end();
 }
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
 // Rebuild adjacency list from edges and make sure all nodes are present
 adjacencyList.clear();
 for (const auto& pair : nodes) {
 adjacencyList[pair.first] = std::vector<std::string>();
 }
 
 for (const auto& edge : edges) {
 // only add neighbors for nodes that still exist
 if (adjacencyList.find(edge.from) != adjacencyList.end())
 adjacencyList[edge.from].push_back(edge.to);
 if (!directed) {
 if (adjacencyList.find(edge.to) != adjacencyList.end())
 adjacencyList[edge.to].push_back(edge.from);
 }
 }
}
