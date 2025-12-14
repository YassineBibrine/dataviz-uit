#include "interaction_manager.h"
#include <cmath>
#include <algorithm>
#include <string>

// On inclut les fichiers du backend
#include "../core/data_model_manager.h"
#include "../core/graph_structure.h"
#include "../core/graph.h"

InteractionManager::InteractionManager() : backend(nullptr) {}
InteractionManager::~InteractionManager() {}

void InteractionManager::setBackend(DataModelManager* manager) {
    this->backend = manager;
}

std::string InteractionManager::addNode(double x, double y, const std::string& type) {
    std::string id = "n" + std::to_string(nextId++);
    nodes.push_back({ id, x, y, type });

    // ENVOI AU BACKEND
    if (backend) {
        if (auto* ds = backend->getCurrentStructure()) {
            if (auto* gs = dynamic_cast<GraphStructure*>(ds)) {
                if (auto* graph = gs->getGraph()) {
                    graph->addNode(id);
                }
            }
        }
    }
    return id;
}

void InteractionManager::removeNode(const std::string& nodeId) {
    nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
        [&](const MockNode& n) { return n.id == nodeId; }), nodes.end());

    edges.erase(std::remove_if(edges.begin(), edges.end(),
        [&](const MockEdge& e) { return e.source == nodeId || e.target == nodeId; }), edges.end());

    // ENVOI AU BACKEND
    if (backend) {
        if (auto* ds = backend->getCurrentStructure()) {
            if (auto* gs = dynamic_cast<GraphStructure*>(ds)) {
                if (auto* graph = gs->getGraph()) {
                    graph->removeNode(nodeId);
                }
            }
        }
    }
}

void InteractionManager::updateNodeValue(const std::string& nodeId, int value) {
    if (backend) {
        if (auto* ds = backend->getCurrentStructure()) {
            if (auto* gs = dynamic_cast<GraphStructure*>(ds)) {
                if (auto* graph = gs->getGraph()) {
                    if (auto* node = graph->getNode(nodeId)) {
                        node->properties["value"] = std::to_string(value);
                    }
                }
            }
        }
    }
}

void InteractionManager::addEdge(const std::string& sourceId, const std::string& targetId) {
    edges.push_back({ sourceId, targetId });

    // ENVOI AU BACKEND
    if (backend) {
        if (auto* ds = backend->getCurrentStructure()) {
            if (auto* gs = dynamic_cast<GraphStructure*>(ds)) {
                if (auto* graph = gs->getGraph()) {
                    graph->addEdge(sourceId, targetId);
                }
            }
        }
    }
}

void InteractionManager::removeEdge(const std::string& sourceId, const std::string& targetId) {
    edges.erase(std::remove_if(edges.begin(), edges.end(),
        [&](const MockEdge& e) {
            return (e.source == sourceId && e.target == targetId) ||
                (e.source == targetId && e.source == sourceId); // Petite correction ici aussi (inversion)
        }), edges.end());

    // ENVOI AU BACKEND
    if (backend) {
        if (auto* ds = backend->getCurrentStructure()) {
            if (auto* gs = dynamic_cast<GraphStructure*>(ds)) {
                if (auto* graph = gs->getGraph()) {
                    graph->removeEdge(sourceId, targetId);
                }
            }
        }
    }
}

bool InteractionManager::startDragging(double x, double y) {
    std::string id = getNodeAtPosition(x, y);
    if (!id.empty()) {
        draggedNodeId = id;
        return true;
    }
    return false;
}

void InteractionManager::updateDragging(double x, double y) {
    if (!draggedNodeId.empty()) {
        for (auto& n : nodes) {
            if (n.id == draggedNodeId) {
                n.x = x;
                n.y = y;
                break;
            }
        }
    }
}

void InteractionManager::endDrag() {
    draggedNodeId = "";
}

std::vector<NodePosition> InteractionManager::getAllNodePositions() {
    std::vector<NodePosition> result;
    for (const auto& n : nodes) {
        result.push_back({ n.id, n.x, n.y, n.type });
    }
    return result;
}

std::vector<EdgeDisplay> InteractionManager::getAllEdges() {
    std::vector<EdgeDisplay> result;
    for (const auto& e : edges) {
        result.push_back({ e.source, e.target });
    }
    return result;
}

std::string InteractionManager::getNodeAtPosition(double x, double y) {
    for (auto it = nodes.rbegin(); it != nodes.rend(); ++it) {
        double dx = x - it->x;
        double dy = y - it->y;
        if (std::sqrt(dx * dx + dy * dy) <= 20.0) {
            return it->id;
        }
    }
    return "";
}


std::pair<std::string, std::string> InteractionManager::getEdgeAtPosition(double mouseX, double mouseY) {
    const double threshold = 10.0; // Distance tolérée en pixels

    for (const auto& edge : edges) {
        // 1. Trouver les coordonnées des deux points
        double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
        bool foundSource = false, foundTarget = false;

        // On cherche les nœuds correspondants dans la liste
        for (const auto& node : nodes) {
            if (node.id == edge.source) {
                x1 = node.x; y1 = node.y; foundSource = true;
            }
            else if (node.id == edge.target) {
                x2 = node.x; y2 = node.y; foundTarget = true;
            }
            if (foundSource && foundTarget) break;
        }

        if (!foundSource || !foundTarget) continue;

        // 2. Calcul de la distance point-segment
        double A = mouseX - x1;
        double B = mouseY - y1;
        double C = x2 - x1;
        double D = y2 - y1;

        double dot = A * C + B * D;
        double len_sq = C * C + D * D;
        double param = -1.0;

        if (len_sq != 0) // Éviter division par zéro
            param = dot / len_sq;

        double xx, yy;

        if (param < 0) {
            xx = x1;
            yy = y1;
        }
        else if (param > 1) {
            xx = x2;
            yy = y2;
        }
        else {
            xx = x1 + param * C;
            yy = y1 + param * D;
        }

        double dx = mouseX - xx;
        double dy = mouseY - yy;
        double distance = std::sqrt(dx * dx + dy * dy);

        // 3. Si on est assez proche, on retourne l'arête
        if (distance < threshold) {
            return { edge.source, edge.target };
        }
    }

    return { "", "" };
}