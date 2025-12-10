#include "frame_recorder.h"

AnimationFrame FrameRecorder::generateComparisonFrame(const std::string& elem1, const std::string& elem2, bool result) {
    AnimationFrame frame;
    frame.operationType = "comparison";
    frame.highlightedNodes = { elem1, elem2 };
    frame.duration = defaultDuration;
    frame.nodeColors[elem1] = result ? "green" : "red";
    frame.nodeColors[elem2] = result ? "green" : "red";
    frameBuffer.push_back(frame);
    return frame;
}

AnimationFrame FrameRecorder::generateSwapFrame(const std::string& elem1, const std::string& elem2) {
    AnimationFrame frame;
    frame.operationType = "swap";
    frame.highlightedNodes = { elem1, elem2 };
    frame.duration = defaultDuration;
    frameBuffer.push_back(frame);
    return frame;
}

AnimationFrame FrameRecorder::generateHighlightFrame(const std::vector<std::string>& elements, const std::string& color) {
    AnimationFrame frame;
    frame.operationType = "highlight";
    frame.highlightedNodes = elements;
    frame.duration = defaultDuration;
    for (const auto& elem : elements) {
        frame.nodeColors[elem] = color;
    }
    frameBuffer.push_back(frame);
    return frame;
}

AnimationFrame FrameRecorder::generateTraversalFrame(const std::string& currentNode, const std::vector<std::string>& visitedNodes) {
    AnimationFrame frame;
    frame.operationType = "traversal";
    frame.highlightedNodes = visitedNodes;
    frame.duration = defaultDuration;
    frame.nodeColors[currentNode] = "blue";
    frameBuffer.push_back(frame);
    return frame;
}

AnimationFrame FrameRecorder::generateInsertionFrame(const std::string& newNode, const std::string& position) {
    AnimationFrame frame;
    frame.operationType = "insertion";
    frame.highlightedNodes = { newNode };
    frame.duration = defaultDuration;
    frame.nodeColors[newNode] = "yellow";
    frameBuffer.push_back(frame);
    return frame;
}
AnimationFrame FrameRecorder::generateNodesFrame(int count) {
    AnimationFrame frame;
    frame.operationType = "generateNodes";
    frame.duration = defaultDuration;

    // Créer les identifiants des nœuds
    for (int i = 1; i <= count; ++i) {
        std::string nodeId = "N" + std::to_string(i);
        frame.highlightedNodes.push_back(nodeId);
        frame.nodeColors[nodeId] = "blue"; // couleur par défaut
    }

    // 👉 Ajouter une annotation explicative
    frame.annotations.push_back("Generated " + std::to_string(count) + " nodes");

    // Ajouter au buffer
    frameBuffer.push_back(frame);
    return frame;
}

std::vector<AnimationFrame> FrameRecorder::getAllFrames() const {
    return frameBuffer;
}