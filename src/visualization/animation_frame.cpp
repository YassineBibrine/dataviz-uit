#include "animation_frame.h"
#include <chrono>

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
    nodePositions[nodeId] = std::make_pair(x, y);
}

void AnimationFrame::setNodeColor(const std::string& nodeId, const std::string& color) {
    nodeColors[nodeId] = color;
}

void AnimationFrame::addAnnotation(const std::string& text) {
    annotations.push_back(text);
}

void AnimationFrame::setDuration(int ms) {
    duration = ms;
}
