#include "interaction_manager.h"
#include <cmath>

static double dist(double x1,double y1,double x2,double y2){ double dx=x2-x1, dy=y2-y1; return std::sqrt(dx*dx+dy*dy);} 

void InteractionManager::handleMousePress(double x, double y) { (void)x; (void)y; }
void InteractionManager::handleMouseMove(double x, double y) { if (isDraggingFlag && !draggingId.empty()) updateNodePosition(draggingId, x, y); }
void InteractionManager::startNodeDrag(const std::string& id, double x, double y) { (void)x;(void)y; draggingId=id; isDraggingFlag=true; }
void InteractionManager::endDrag() { isDraggingFlag=false; draggingId.clear(); }
void InteractionManager::handleDrop(const std::string& type, double x, double y) { (void)type; nodePositions["n"+std::to_string(nodePositions.size()+1)] = {x,y}; }
void InteractionManager::updateNodePosition(const std::string& id, double x, double y) { nodePositions[id] = {x,y}; }
std::vector<NodePosition> InteractionManager::getAllNodePositions() const { std::vector<NodePosition> out; for (auto& kv : nodePositions) out.push_back({kv.first, kv.second.first, kv.second.second}); return out; }
std::string InteractionManager::getNodeAtPosition(double x, double y) const { for (auto& kv : nodePositions) if (dist(x,y,kv.second.first,kv.second.second) <12) return kv.first; return {}; }
