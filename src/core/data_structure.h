#pragma once

#include <string>
#include <vector>
#include <map>

#include <QJsonObject>

/**
 * @struct DSNode
 * @brief Represents a node in a data structure visualization
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
 * @brief Represents an edge between two nodes
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

    // ===== Visualisation =====
    virtual std::vector<DSNode> getNodes() const = 0;
    virtual std::vector<DSEdge> getEdges() const = 0;

    // ===== Export =====
    virtual std::string serializeToDOT() const = 0;

    // ===== Algorithm runner =====
    virtual void* getDataForRunner() = 0;

    // ===== Session persistence (CRITICAL) =====
    virtual std::string getType() const = 0;
    virtual QJsonObject serialize() const = 0;
    virtual void deserialize(const QJsonObject& obj) = 0;

    // ===== Node positions =====
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

    // ===== Custom Edge Storage =====
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
