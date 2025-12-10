#pragma once
#include <string>
#include <map>
#include <vector>
#include <utility>

struct AnimationFrame {
    int frameNumber;
    std::string operationType;

    std::vector<std::string> highlightedNodes;
    std::vector<std::pair<std::string, std::string>> highlightedEdges;

    std::map<std::string, std::pair<double, double>> nodePositions;
    std::map<std::string, std::string> nodeColors;
    std::map<std::string, std::string> edgeColors;

    // --- NOS AJOUTS ---
    std::map<std::string, std::string> nodeShapes;
    std::vector<std::pair<std::string, std::string>> edges;
    // ------------------

    std::map<std::string, std::string> nodeLabels;
    std::map<std::string, std::string> edgeLabels;
    std::vector<std::string> annotations;
    std::string dotCode;
    int duration;
    long long timestamp;

    // Juste les signatures (le point-virgule à la fin), pas de {}
    AnimationFrame();

    void addHighlightedNode(const std::string& nodeId, const std::string& color);
    void addHighlightedEdge(const std::string& from, const std::string& to, const std::string& color);
    void setNodePosition(const std::string& nodeId, double x, double y);
    void addAnnotation(const std::string& text);
    void setDuration(int ms);
    void generateNodes(int count);

};
