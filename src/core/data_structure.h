#pragma once

#include <string>
#include <vector>
#include <map>

#include <QJsonObject>

/**
 * @struct DSNode
 * @brief Represents a visual node in a data structure.
 *
 * Contains an identifier and a display value used by the renderer.
 */
struct DSNode {
    std::string id;
    std::string value;  // Display value (may differ from id)

    DSNode() = default;
    explicit DSNode(const std::string& nodeId) : id(nodeId), value(nodeId) {}
    DSNode(const std::string& nodeId, const std::string& nodeValue)
        : id(nodeId), value(nodeValue) {
    }
};

/**
 * @struct DSEdge
 * @brief Represents a directed edge between two visual nodes.
 */
struct DSEdge {
    std::string from;
    std::string to;

    DSEdge() = default;
    DSEdge(const std::string& fromNode, const std::string& toNode)
        : from(fromNode), to(toNode) {
    }
};

/**
 * @struct DSNodePosition
 * @brief Stores the x/y screen or canvas position for a node.
 */
struct DSNodePosition {
    double x = 0.0;
    double y = 0.0;

    DSNodePosition() = default;
    DSNodePosition(double px, double py) : x(px), y(py) {}
};

/**
 * @interface DataStructure
 * @brief Abstract base class for all visualizable data structures.
 *
 * Implementations provide node/edge lists, serialization for DOT and
 * session persistence, and a way to expose runtime data to algorithm
 * runners.
 */
class DataStructure {
public:
    virtual ~DataStructure() = default;

    // ===== Visualization =====
    // Return the nodes/edges that the renderer will draw.
    virtual std::vector<DSNode> getNodes() const = 0;
    virtual std::vector<DSEdge> getEdges() const = 0;

    // ===== Export/Debug =====
    // Export a DOT representation suitable for graph tools or debugging.
    virtual std::string serializeToDOT() const = 0;

    // ===== Algorithm runner integration =====
    // Returns a pointer to internal data used by algorithm runner code.
    virtual void* getDataForRunner() = 0;

    // ===== Session persistence (CRITICAL) =====
    // Identify structure type and provide JSON serialize/deserialize.
    virtual std::string getType() const = 0;
    virtual QJsonObject serialize() const = 0;
    virtual void deserialize(const QJsonObject& obj) = 0;

    // ===== Node positions =====
    // Store and query per-node canvas positions used by the visualization.
    void setNodePosition(const std::string& nodeId, double x, double y) {
        nodePositions[nodeId] = DSNodePosition(x, y);
    }

    bool getNodePosition(const std::string& nodeId, double& x, double& y) const {
        auto it = nodePositions.find(nodeId);
        if (it != nodePositions.end()) {
            x = it->second.x;
            y = it->second.y;
            return true;
        }
        return false;
    }

    bool hasNodePosition(const std::string& nodeId) const {
        return nodePositions.find(nodeId) != nodePositions.end();
    }

    const std::map<std::string, DSNodePosition>& getAllNodePositions() const {
        return nodePositions;
    }

    void clearNodePositions() {
        nodePositions.clear();
    }

    bool hasAnyPositions() const {
        return !nodePositions.empty();
    }

    // ===== Custom edges =====
    // Manage user-added edges that supplement the structure's topology.
    void addCustomEdge(const std::string& from, const std::string& to) {
        for (const auto& edge : customEdges) {
            if (edge.from == from && edge.to == to)
                return;
        }
        customEdges.emplace_back(from, to);
    }

    void clearCustomEdges() {
        customEdges.clear();
    }

    const std::vector<DSEdge>& getCustomEdges() const {
        return customEdges;
    }

    bool hasCustomEdges() const {
        return !customEdges.empty();
    }

    void setCustomEdges(const std::vector<DSEdge>& edges) {
        customEdges = edges;
    }

protected:
    std::map<std::string, DSNodePosition> nodePositions;
    std::vector<DSEdge> customEdges;
};
