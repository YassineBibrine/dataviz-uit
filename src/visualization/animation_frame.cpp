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

// D�finit la position d�un n�ud dans le cadre d�animation
void AnimationFrame::setNodePosition(const std::string& nodeId, double x, double y) {
    nodePositions[nodeId] = { x, y };
}

// Ajoute une annotation au cadre d�animation
void AnimationFrame::addAnnotation(const std::string& text) {
    // Le diagramme UML indique addAnnotation(text: string)
    // mais ne pr�cise pas o� stocker l�annotation.
    // On utilise highlightedNodes comme support temporaire.
    highlightedNodes.push_back(text);
}