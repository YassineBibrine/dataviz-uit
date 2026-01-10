#pragma once

#include <string>
#include <vector>
#include <map>

/**
 * @struct DSNode
 * @brief Represents a node in a data structure visualization
 */
struct DSNode {
    std::string id;
    std::string value;  // Display value (may differ from id)
    
    DSNode() = default;
    explicit DSNode(const std::string& nodeId) : id(nodeId), value(nodeId) {}
    DSNode(const std::string& nodeId, const std::string& nodeValue) : id(nodeId), value(nodeValue) {}
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
 * @struct NodePosition
 * @brief Stores the visual position of a node
 */
struct DSNodePosition {
    double x = 0.0;
    double y = 0.0;
    
    DSNodePosition() = default;
    DSNodePosition(double px, double py) : x(px), y(py) {}
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
    
    /**
     * @brief Set the position of a node for visualization
     * @param nodeId The node identifier
     * @param x X coordinate
     * @param y Y coordinate
     */
    void setNodePosition(const std::string& nodeId, double x, double y) {
    nodePositions[nodeId] = DSNodePosition(x, y);
    }
    
    /**
     * @brief Get the position of a node
     * @param nodeId The node identifier
     * @param x Output X coordinate
     * @param y Output Y coordinate
     * @return True if position was found, false otherwise
     */
    bool getNodePosition(const std::string& nodeId, double& x, double& y) const {
        auto it = nodePositions.find(nodeId);
        if (it != nodePositions.end()) {
            x = it->second.x;
   y = it->second.y;
          return true;
   }
 return false;
    }
    
    /**
     * @brief Check if node has a saved position
     */
    bool hasNodePosition(const std::string& nodeId) const {
        return nodePositions.find(nodeId) != nodePositions.end();
    }
    
    /**
     * @brief Get all saved node positions
     */
    const std::map<std::string, DSNodePosition>& getAllNodePositions() const {
        return nodePositions;
    }
    
    /**
     * @brief Clear all saved node positions
 */
    void clearNodePositions() {
        nodePositions.clear();
    }
    
    /**
     * @brief Check if any positions have been saved
     */
    bool hasAnyPositions() const {
        return !nodePositions.empty();
 }
    
    // === Custom Edge Storage (for structures that need to preserve user-drawn edges) ===
    
    /**
     * @brief Add a custom edge
     */
    void addCustomEdge(const std::string& from, const std::string& to) {
        // Avoid duplicates
        for (const auto& edge : customEdges) {
        if (edge.from == from && edge.to == to) return;
        }
        customEdges.emplace_back(from, to);
    }
    
 /**
     * @brief Clear all custom edges
     */
    void clearCustomEdges() {
        customEdges.clear();
  }
    
    /**
     * @brief Get custom edges
 */
    const std::vector<DSEdge>& getCustomEdges() const {
        return customEdges;
    }
    
  /**
     * @brief Check if custom edges have been set
     */
    bool hasCustomEdges() const {
        return !customEdges.empty();
    }
    
    /**
     * @brief Set all custom edges at once
     */
    void setCustomEdges(const std::vector<DSEdge>& edges) {
        customEdges = edges;
    }

protected:
    std::map<std::string, DSNodePosition> nodePositions;
    std::vector<DSEdge> customEdges;
};
