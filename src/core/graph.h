#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

/**
 * @class Graph
 * @brief Graph data structure with nodes and edges
 */
class Graph {
public:
    /**
     * @struct Node
     * @brief Graph node
  */
    struct Node {
 std::string id;
        std::map<std::string, std::string> properties;
        int inDegree = 0;
 int outDegree = 0;
        
        Node() = default;
        Node(const std::string& id) : id(id) {}
    };
    
    /**
     * @struct Edge
  * @brief Graph edge
     */
    struct Edge {
    std::string from;
        std::string to;
   double weight = 1.0;
   std::map<std::string, std::string> properties;
        
        Edge() = default;
    Edge(const std::string& from, const std::string& to, double weight = 1.0)
     : from(from), to(to), weight(weight) {}
    };
    
    explicit Graph(bool directed = false);
    ~Graph();

    void addNode(const std::string& id, const std::map<std::string, std::string>& properties = {});
    void addEdge(const std::string& from, const std::string& to, double weight = 1.0, 
         const std::map<std::string, std::string>& properties = {});
    
  void removeNode(const std::string& id);
    void removeEdge(const std::string& from, const std::string& to);
    
    bool hasNode(const std::string& id) const;
 bool hasEdge(const std::string& from, const std::string& to) const;
    
  int getNodeCount() const;
    int getEdgeCount() const;
    
    std::vector<std::string> getNeighbors(const std::string& id) const;
    Node* getNode(const std::string& id);
    Edge* getEdge(const std::string& from, const std::string& to);
    
    void clear();
    Graph* clone() const;
    
    bool isDirected() const { return directed; }
    
private:
    bool directed;
    std::map<std::string, Node> nodes;
    std::vector<Edge> edges;
    std::map<std::string, std::vector<std::string>> adjacencyList;
    
    void updateAdjacencyList();
};
