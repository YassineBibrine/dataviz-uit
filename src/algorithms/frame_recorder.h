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
    int defaultDuration = 500;  // 500ms per frame by default

    // NEW: Record a frame
    void recordFrame(const AnimationFrame& frame) {
 frameBuffer.push_back(frame);
    }

    AnimationFrame generateComparisonFrame(const std::string& elem1, const std::string& elem2, bool result);
    AnimationFrame generateSwapFrame(const std::string& elem1, const std::string& elem2);
    AnimationFrame generateHighlightFrame(const std::vector<std::string>& elements, const std::string& color);
    AnimationFrame generateTraversalFrame(const std::string& currentNode, const std::vector<std::string>& visitedNodes);
    AnimationFrame generateInsertionFrame(const std::string& newNode, const std::string& position);
    AnimationFrame generateNodesFrame(int count);
    std::vector<AnimationFrame> getAllFrames() const;

    // ✅ reset du FrameRecorder
    void reset();
    void clear();
};

#endif // FRAME_RECORDER_H