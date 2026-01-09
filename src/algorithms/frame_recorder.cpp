#include "frame_recorder.h"

AnimationFrame FrameRecorder::makeBaseFrame(const std::string& op) {
    AnimationFrame f;
    f.frameNumber = nextFrameNumber_++;
    f.operationType = op;
    f.duration = defaultDuration;
    return f;
}

AnimationFrame FrameRecorder::generateComparisonFrame(const std::string& elem1, const std::string& elem2, bool result) {
    auto f = makeBaseFrame("comparison");
    f.highlightedNodes = { elem1, elem2 };
    f.nodeColors[elem1] = result ? "green" : "red";
    f.nodeColors[elem2] = result ? "green" : "red";
    f.addAnnotation("Compare " + elem1 + " vs " + elem2);
    frameBuffer.push_back(f);
    return f;
}

AnimationFrame FrameRecorder::generateSwapFrame(const std::string& elem1, const std::string& elem2) {
    auto f = makeBaseFrame("swap");
    f.highlightedNodes = { elem1, elem2 };
    f.nodeColors[elem1] = "orange";
    f.nodeColors[elem2] = "orange";
    f.addAnnotation("Swap " + elem1 + " <-> " + elem2);
    frameBuffer.push_back(f);
    return f;
}

AnimationFrame FrameRecorder::generateHighlightFrame(const std::vector<std::string>& elements, const std::string& color) {
    auto f = makeBaseFrame("highlight");
    f.highlightedNodes = elements;
    for (const auto& e : elements) {
        f.nodeColors[e] = color;
    }
    f.addAnnotation("Highlight segment");
    frameBuffer.push_back(f);
    return f;
}

AnimationFrame FrameRecorder::generateTraversalFrame(const std::string& currentNode, const std::vector<std::string>& visitedNodes) {
    auto f = makeBaseFrame("traversal");
    f.highlightedNodes = visitedNodes;
    f.nodeColors[currentNode] = "blue";
    f.addAnnotation("Visit " + currentNode);
    frameBuffer.push_back(f);
    return f;
}

AnimationFrame FrameRecorder::generateInsertionFrame(const std::string& newNode, const std::string& position) {
 Updated upstream
    AnimationFrame frame;
    frame.operationType = "insertion";
    frame.highlightedNodes = { newNode };
    frame.duration = defaultDuration;
    frame.nodeColors[newNode] = "yellow";
    frameBuffer.push_back(frame);
    return frame;

    auto f = makeBaseFrame("insertion");
    f.highlightedNodes = { newNode };
    f.nodeColors[newNode] = "purple";
    f.addAnnotation("Insert " + newNode + " at " + position);
    frameBuffer.push_back(f);
    return f;
 Stashed changes
}

std::vector<AnimationFrame> FrameRecorder::getAllFrames() const {
    return frameBuffer;
 Updated upstream
}

}

void FrameRecorder::clear() {
    frameBuffer.clear();
    nextFrameNumber_ = 0;
}

void FrameRecorder::reset() {
    frameBuffer.clear();
    nextFrameNumber_ = 0;
}
 Stashed changes
