#include "graph.h"

Graph::Graph(bool directed)
    : isDirected(directed)
{
}

void Graph::addNode(const std::string &id, const std::map<std::string, std::string> &properties)
{
    Node node;
    node.id = id;
    node.properties = properties;
    nodes[id] = node;
}

void Graph::addEdge(const std::string &from, const std::string &to, 
    double weight, const std::map<std::string, std::string> &properties)
{
    if (!hasNode(from) || !hasNode(to)) {
        return; // Cannot add edge between non-existent nodes
    }
    
Edge edge;
    edge.from = from;
    edge.to = to;
    edge.weight = weight;
    edge.properties = properties;
    
    edges.push_back(edge);
    
    // Add reverse edge for undirected graphs
    if (!isDirected) {
    Edge reverseEdge;
        reverseEdge.from = to;
        reverseEdge.to = from;
        reverseEdge.weight = weight;
        reverseEdge.properties = properties;
        edges.push_back(reverseEdge);
    }
}

void Graph::removeNode(const std::string &id)
{
    nodes.erase(id);
    // TODO: Remove all edges connected to this node
}

void Graph::removeEdge(const std::string &from, const std::string &to)
{
    // TODO: Remove edge from edges vector
}

bool Graph::hasNode(const std::string &id) const
{
    return nodes.find(id) != nodes.end();
}

bool Graph::hasEdge(const std::string &from, const std::string &to) const
{
  for (const auto &edge : edges) {
      if (edge.from == from && edge.to == to) {
            return true;
      }
    }
    return false;
}

int Graph::getNodeCount() const
{
    return nodes.size();
}

int Graph::getEdgeCount() const
{
    return edges.size();
}

const std::vector<std::string> &Graph::getNeighbors(const std::string &id) const
{
    static std::vector<std::string> empty;
    // TODO: Return neighbors for the given node
    return empty;
}
