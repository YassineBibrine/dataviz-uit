#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>
#include "execution_state.h"
#include "frame_recorder.h"
#include "../visualization/animation_frame.h"
#include "../orchestration/algorithm.h"

class AlgorithmRunner : public Algorithm {
protected:
 ExecutionState executionState;
 FrameRecorder frameRecorder;
 bool isPaused{false};
 int currentStep{0};

 void recordStep(const std::string& operation, const std::map<std::string, std::string>& details) {
 // Record to execution state and frame recorder (placeholder)
 (void)operation; (void)details;
 }

public:
 virtual ~AlgorithmRunner() = default;

 virtual void execute() =0;
 virtual void stepForward() =0;
 virtual void stepBackward() =0;

 virtual void reset() {
 isPaused = false;
 currentStep =0;
 executionState.reset();
 frameRecorder.clear();
 }

 void pause() { isPaused = true; }
 void resume() { isPaused = false; }

 ExecutionState getExecutionState() const { return executionState; }
 std::vector<AnimationFrame> getAnimationFrames() const { return frameRecorder.getAllFrames(); }
};
