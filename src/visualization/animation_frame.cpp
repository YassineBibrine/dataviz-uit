#include "animation_frame.h"
#include <chrono>

// AnimationFrame implementation: simple helpers to build frame contents
AnimationFrame::AnimationFrame()
    : frameNumber(0), operationType(""), duration(100), timestamp(0) {
    timestamp = std::chrono::system_clock::now().time_since_epoch().count();
}

void AnimationFrame::addHighlightedNode(const std::string& nodeId, const std::string& color) {
    highlightedNodes.push_back(nodeId);
    nodeColors[nodeId] = color;
}

void AnimationFrame::addHighlightedEdge(const std::string& from, const std::string& to, const std::string& color) {
    highlightedEdges.push_back(std::make_pair(from, to));
    edgeColors[from + "-" + to] = color;
}

void AnimationFrame::setNodePosition(const std::string& nodeId, double x, double y) {
    nodePositions[nodeId] = { x, y };
}

void AnimationFrame::addAnnotation(const std::string& text) {
    // Use the annotations vector for user-facing messages
    annotations.push_back(text);
}

void AnimationFrame::generateNodes(int count) {
    // Prepare a basic generated nodes frame where nodes are named N1..Ncount
    highlightedNodes.clear();
    nodePositions.clear();
    nodeColors.clear();

    for (int i = 1; i <= count; ++i) {
        std::string nodeId = "N" + std::to_string(i);
        highlightedNodes.push_back(nodeId);
        nodeColors[nodeId] = "blue"; // default color
    }

    operationType = "GenerateNodes";
    annotations.push_back("Generated " + std::to_string(count) + " nodes");
}
