#pragma once
#include <string>
#include <map>
#include <vector>
#include <utility>

struct AnimationFrame {
    // Basic frame properties
    std::string description;
    int duration = 0;
    int frameNumber = 0;

    // Visualization properties
    std::string operationType;
    std::vector<std::string> highlightedNodes;
    std::vector<std::pair<std::string, std::string>> highlightedEdges;

    // Node and Edge data
    std::map<std::string, std::pair<double, double>> nodePositions;
    std::map<std::string, std::string> nodeColors;
    std::map<std::string, std::string> nodeShapes;
    std::map<std::string, std::string> nodeLabels;
    std::map<std::string, std::string> edgeColors;
    std::vector<std::pair<std::string, std::string>> edges;

    // Annotations and metadata
    std::vector<std::string> annotations;
    std::string dotCode;
    long long timestamp = 0;

    // Constructor
    AnimationFrame() = default;

    // Methods
    void setNodePosition(const std::string& nodeId, double x, double y);
    void addHighlightedNode(const std::string& nodeId, const std::string& color);
    void addHighlightedEdge(const std::string& from, const std::string& to, const std::string& color);
    void addAnnotation(const std::string& text);
    void setDuration(int ms);
<<<<<<< Updated upstream
=======
    void generateNodes(int count);
>>>>>>> Stashed changes
};
