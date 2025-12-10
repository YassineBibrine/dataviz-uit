#include "interaction_manager.h"
#include <cmath>
#include <algorithm>

static double dist(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

static double distToSegment(double px, double py, double x1, double y1, double x2, double y2) {
    double l2 = dist(x1, y1, x2, y2); l2 *= l2;
    if (l2 == 0) return dist(px, py, x1, y1);
    double t = ((px - x1) * (x2 - x1) + (py - y1) * (y2 - y1)) / l2;
    t = std::max(0.0, std::min(1.0, t));
    return dist(px, py, x1 + t * (x2 - x1), y1 + t * (y2 - y1));
}

InteractionManager::InteractionManager() {}

void InteractionManager::addNode(double x, double y, const std::string& type) {
    std::string newId = "n" + std::to_string(positions.size() + 1);
    while (positions.count(newId)) newId += "_new";
    positions[newId] = { newId, x, y, type };
}

void InteractionManager::updateNodePosition(const std::string& id, double x, double y) {
    if (positions.count(id)) {
        positions[id].x = x;
        positions[id].y = y;
    }
}

void InteractionManager::removeNode(const std::string& nodeId) {
    positions.erase(nodeId);
    auto newEnd = std::remove_if(edges.begin(), edges.end(),
        [&](const Edge& e) { return e.source == nodeId || e.target == nodeId; });
    edges.erase(newEnd, edges.end());
}

void InteractionManager::removeEdge(const std::string& src, const std::string& dest) {
    auto newEnd = std::remove_if(edges.begin(), edges.end(),
        [&](const Edge& e) { return e.source == src && e.target == dest; });
    edges.erase(newEnd, edges.end());
}

NodePosition InteractionManager::getNodePosition(const std::string& nodeId) const {
    if (positions.count(nodeId)) return positions.at(nodeId);
    return {};
}

std::vector<NodePosition> InteractionManager::getAllNodePositions() const {
    std::vector<NodePosition> out;
    for (auto const& [key, val] : positions) out.push_back(val);
    return out;
}

std::string InteractionManager::getNodeAtPosition(double x, double y, double radius) const {
    for (auto const& [key, val] : positions) {
        if (dist(x, y, val.x, val.y) < radius) return key;
    }
    return "";
}

std::pair<std::string, std::string> InteractionManager::getEdgeAtPosition(double x, double y, double threshold) const {
    for (const auto& edge : edges) {
        if (positions.count(edge.source) && positions.count(edge.target)) {
            auto p1 = positions.at(edge.source);
            auto p2 = positions.at(edge.target);
            if (distToSegment(x, y, p1.x, p1.y, p2.x, p2.y) < threshold) {
                return { edge.source, edge.target };
            }
        }
    }
    return { "", "" };
}

void InteractionManager::addEdge(const std::string& src, const std::string& target) {
    if (src != target) edges.push_back({ src, target });
}

std::vector<Edge> InteractionManager::getAllEdges() const {
    return edges;
}

bool InteractionManager::startDragging(double mouseX, double mouseY) {
    std::string id = getNodeAtPosition(mouseX, mouseY);
    if (!id.empty()) {
        draggedNodeId = id;
        isDragging = true;
        return true;
    }
    return false;
}

void InteractionManager::updateDragging(double mouseX, double mouseY) {
    if (isDragging && !draggedNodeId.empty()) {
        updateNodePosition(draggedNodeId, mouseX, mouseY);
    }
}

void InteractionManager::endDragging() {
    isDragging = false;
    draggedNodeId.clear();
}

void InteractionManager::endDrag() {
    endDragging();
}