#pragma once

#include <string>
#include <vector>

/**
 * @struct DSNode
 * @brief Represents a node in a data structure visualization
 */
struct DSNode {
    std::string id;
    
    DSNode() = default;
    explicit DSNode(const std::string& nodeId) : id(nodeId) {}
};

/**
 * @struct DSEdge
 * @brief Represents an edge between two nodes
 */
struct DSEdge {
    std::string from;
    std::string to;
    
    DSEdge() = default;
    DSEdge(const std::string& fromNode, const std::string& toNode)
        : from(fromNode), to(toNode) {}
};

/**
 * @interface DataStructure
 * @brief Base interface for all data structures in the system
 */
class DataStructure {
public:
    virtual ~DataStructure() = default;
    
    /**
     * @brief Get all nodes in the data structure
     * @return Vector of DSNode objects
  */
    virtual std::vector<DSNode> getNodes() const = 0;
    
    /**
     * @brief Get all edges in the data structure
   * @return Vector of DSEdge objects
     */
    virtual std::vector<DSEdge> getEdges() const = 0;
    
    /**
     * @brief Serialize the data structure to DOT format (Graphviz)
     * @return DOT format string representation
   */
    virtual std::string serializeToDOT() const = 0;
    
    /**
     * @brief Get the underlying data for algorithm execution
     * @return Void pointer to the raw data structure
     */
 virtual void* getDataForRunner() = 0;
};
