Updated upstream
#ifndef FRAME_RECORDER_H
#define FRAME_RECORDER_H


﻿#pragma once
>>>>>>> Stashed changes
#include <vector>
#include <string>
#include "../visualization/animation_frame.h"

class FrameRecorder {
public:
    // Attributes per UML
    std::vector<AnimationFrame> frameBuffer;
    bool interpolationEnabled{ false };
    int defaultDuration{ 16 }; // ~60 FPS

    // Methods per UML
    AnimationFrame generateComparisonFrame(const std::string& elem1, const std::string& elem2, bool result);
    AnimationFrame generateSwapFrame(const std::string& elem1, const std::string& elem2);
    AnimationFrame generateHighlightFrame(const std::vector<std::string>& elements, const std::string& color);
    AnimationFrame generateTraversalFrame(const std::string& currentNode, const std::vector<std::string>& visitedNodes);
    AnimationFrame generateInsertionFrame(const std::string& newNode, const std::string& position);
    std::vector<AnimationFrame> getAllFrames() const;
 Updated upstream

    // Clear all recorded frames
    void clear() { frameBuffer.clear(); }

    
    // Additional methods
    void clear();
    void reset();

private:
    int nextFrameNumber_{ 0 };
    AnimationFrame makeBaseFrame(const std::string& op);
 Stashed changes
};
