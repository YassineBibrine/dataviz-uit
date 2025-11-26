#include "animation_frame.h"

// Définit la position d’un nœud dans le cadre d’animation
void AnimationFrame::setNodePosition(const std::string& nodeId, double x, double y) {
    nodePositions[nodeId] = { x, y };
}

// Ajoute une annotation au cadre d’animation
void AnimationFrame::addAnnotation(const std::string& text) {
    // Le diagramme UML indique addAnnotation(text: string)
    // mais ne précise pas où stocker l’annotation.
    // On utilise highlightedNodes comme support temporaire.
    highlightedNodes.push_back(text);
}