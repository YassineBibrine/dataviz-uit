#pragma once
#include <string>
#include <map>
#include <vector>

struct NodePosition {
    std::string id;
    double x;
    double y;
    std::string type;
};

struct Edge {
    std::string source;
    std::string target;
};

class InteractionManager {
public:
    InteractionManager();

    void addNode(double x, double y, const std::string& type = "CIRCLE");
    void updateNodePosition(const std::string& nodeId, double x, double y);
    void removeNode(const std::string& nodeId); // Ajouté
    void removeEdge(const std::string& src, const std::string& dest); // Ajouté

    NodePosition getNodePosition(const std::string& nodeId) const;
    std::vector<NodePosition> getAllNodePositions() const;
    std::string getNodeAtPosition(double x, double y, double radius = 25.0) const;
    std::pair<std::string, std::string> getEdgeAtPosition(double x, double y, double threshold = 10.0) const; // Ajouté

    void addEdge(const std::string& src, const std::string& target);
    std::vector<Edge> getAllEdges() const;

    // Dragging logic
    bool startDragging(double mouseX, double mouseY);
    void updateDragging(double mouseX, double mouseY);
    void endDragging();
    void endDrag();

private:
    std::map<std::string, NodePosition> positions;
    std::vector<Edge> edges;

    bool isDragging = false;
    std::string draggedNodeId = "";
};