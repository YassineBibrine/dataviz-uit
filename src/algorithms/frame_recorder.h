#ifndef FRAME_RECORDER_H
#define FRAME_RECORDER_H

#include <vector>
#include <string>
#include "../visualization/animation_frame.h"


// FrameRecorder collects AnimationFrame objects produced by algorithms.
// Algorithms use this helper to build a timeline of states that the
// visualization renderer will play back.
class FrameRecorder {
private:
 std::vector<AnimationFrame> frameBuffer;

public:
 bool interpolationEnabled = false;
 int defaultDuration =500; // default frame duration in milliseconds

 // Record a prepared frame into the internal buffer.
 void recordFrame(const AnimationFrame& frame) {
 frameBuffer.push_back(frame);
 }

 // Convenience helpers to create common frame types.
 AnimationFrame generateComparisonFrame(const std::string& elem1, const std::string& elem2, bool result);
 AnimationFrame generateSwapFrame(const std::string& elem1, const std::string& elem2);
 AnimationFrame generateHighlightFrame(const std::vector<std::string>& elements, const std::string& color);
 AnimationFrame generateTraversalFrame(const std::string& currentNode, const std::vector<std::string>& visitedNodes);
 AnimationFrame generateInsertionFrame(const std::string& newNode, const std::string& position);
 AnimationFrame generateNodesFrame(int count);
 std::vector<AnimationFrame> getAllFrames() const;

 // Reset/clear the recorded frames.
 void reset();
 void clear();
};

#endif // FRAME_RECORDER_H