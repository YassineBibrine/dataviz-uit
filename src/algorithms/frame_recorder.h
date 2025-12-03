#ifndef FRAME_RECORDER_H
#define FRAME_RECORDER_H

#include <vector>
#include <string>
#include "../visualization/animation_frame.h"


class FrameRecorder {
private:
    std::vector<AnimationFrame> frameBuffer;

public:
    bool interpolationEnabled = false;
    int defaultDuration = 5;

    AnimationFrame generateComparisonFrame(const std::string& elem1, const std::string& elem2, bool result);
    AnimationFrame generateSwapFrame(const std::string& elem1, const std::string& elem2);
    AnimationFrame generateHighlightFrame(const std::vector<std::string>& elements, const std::string& color);
    AnimationFrame generateTraversalFrame(const std::string& currentNode, const std::vector<std::string>& visitedNodes);
    AnimationFrame generateInsertionFrame(const std::string& newNode, const std::string& position);
    std::vector<AnimationFrame> getAllFrames() const;
};

#endif // FRAME_RECORDER_H