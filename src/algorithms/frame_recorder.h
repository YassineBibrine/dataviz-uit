#pragma once

#include <vector>
#include "../visualization/animation_frame.h"

class FrameRecorder {
 std::vector<AnimationFrame> frames;
 bool interpolationEnabled{false};
 int defaultDuration{16};
public:
 AnimationFrame generateComparisonFrame(const std::string& a, const std::string& b, bool result) {
 (void)a; (void)b; (void)result; return AnimationFrame(); }
 AnimationFrame generateSwapFrame(const std::string& a, const std::string& b) {
 (void)a; (void)b; return AnimationFrame(); }
 AnimationFrame generateHighlightFrame(const std::vector<std::string>& elems, const std::string& color) {
 (void)elems; (void)color; return AnimationFrame(); }
 AnimationFrame generateTraversalFrame(const std::string& node, const std::vector<std::string>& visited) {
 (void)node; (void)visited; return AnimationFrame(); }
 AnimationFrame generateInsertionFrame(const std::string& n, const std::string& pos) {
 (void)n; (void)pos; return AnimationFrame(); }
 std::vector<AnimationFrame> getAllFrames() const { return frames; }
 void clear() { frames.clear(); }
};
