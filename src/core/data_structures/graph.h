#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <set>
#include <string>
#include <memory>
#include <vector>

/**
 * @class Graph
 * @brief Generic graph data structure for internal representation
 * 
 * Supports:
 * - Directed and undirected graphs
 * - Weighted and unweighted edges
 * - Node and edge properties
 * - Boost Graph Library integration
 */
class Graph {
public:
    Graph(bool directed = false);
    
    void addNode(const std::string &id, const std::map<std::string, std::string> &properties = {});
    void addEdge(const std::string &from, const std::string &to, 
   double weight = 1.0, const std::map<std::string, std::string> &properties = {});
    
    void removeNode(const std::string &id);
    void removeEdge(const std::string &from, const std::string &to);
    
 bool hasNode(const std::string &id) const;
    bool hasEdge(const std::string &from, const std::string &to) const;
    
    int getNodeCount() const;
    int getEdgeCount() const;
    
    const std::vector<std::string> &getNeighbors(const std::string &id) const;

private:
    struct Node {
        std::string id;
      std::map<std::string, std::string> properties;
    };
    
    struct Edge {
        std::string from, to;
        double weight;
        std::map<std::string, std::string> properties;
    };
    
    bool isDirected;
    std::map<std::string, Node> nodes;
 std::vector<Edge> edges;
};

#endif // GRAPH_H
