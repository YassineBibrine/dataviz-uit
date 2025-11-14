#pragma once

#include <map>
#include <string>
#include <vector>
#include <utility>

struct NodePosition { std::string id; double x; double y; };

class InteractionManager {
 std::map<std::string, std::pair<double,double>> nodePositions;
 bool isDraggingFlag{false};
 std::pair<double,double> pan{0,0};
 std::string draggingId;
public:
 void handleMousePress(double x, double y);
 void handleMouseMove(double x, double y);
 void startNodeDrag(const std::string& id, double x, double y);
 void endDrag();
 void handleDrop(const std::string& type, double x, double y);
 void updateNodePosition(const std::string& id, double x, double y);
 std::vector<NodePosition> getAllNodePositions() const;
 std::string getNodeAtPosition(double x, double y) const;
 void setPanOffset(double ox, double oy) { pan = {ox, oy}; }
};
