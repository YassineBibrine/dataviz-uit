#include "animation_frame.h"

void AnimationFrame::setNodePosition(const std::string& nodeId, double x, double y) {
    nodePositions[nodeId] = { x, y };
}

void AnimationFrame::addHighlightedNode(const std::string& nodeId, const std::string& color) {
    highlightedNodes.push_back(nodeId);
    nodeColors[nodeId] = color;
}

void AnimationFrame::addHighlightedEdge(const std::string& from, const std::string& to, const std::string& color) {
<<<<<<< Updated upstream
    highlightedEdges.push_back(std::make_pair(from, to));
    edgeColors[from + "-" + to] = color;
}

// D�finit la position d�un n�ud dans le cadre d�animation
void AnimationFrame::setNodePosition(const std::string& nodeId, double x, double y) {
    nodePositions[nodeId] = { x, y };
=======
    highlightedEdges.push_back({from, to});
    edgeColors[from + "->" + to] = color;
>>>>>>> Stashed changes
}

// Ajoute une annotation au cadre d�animation
void AnimationFrame::addAnnotation(const std::string& text) {
<<<<<<< Updated upstream
    // Le diagramme UML indique addAnnotation(text: string)
    // mais ne pr�cise pas o� stocker l�annotation.
    // On utilise highlightedNodes comme support temporaire.
    highlightedNodes.push_back(text);
}
=======
    annotations.push_back(text);
}

void AnimationFrame::setDuration(int ms) {
    duration = ms;
}

void AnimationFrame::generateNodes(int count) {
    annotations.push_back("Generated " + std::to_string(count) + " nodes");
}
>>>>>>> Stashed changes
