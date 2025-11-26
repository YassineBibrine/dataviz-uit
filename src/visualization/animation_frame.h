#ifndef ANIMATION_FRAME_H
#define ANIMATION_FRAME_H

#include <string>
#include <map>
#include <vector>
#include <utility>

class AnimationFrame {
public:
    int frameNumber = 0;
    std::string operationType;
    std::map<std::string, std::pair<double, double>> nodePositions;
    std::map<std::string, std::string> nodeColors;
    std::vector<std::string> highlightedNodes;
    int duration = 5;

    AnimationFrame() = default;

    void setNodePosition(const std::string& nodeId, double x, double y);
    void addAnnotation(const std::string& text);
};

#endif // ANIMATION_FRAME_H