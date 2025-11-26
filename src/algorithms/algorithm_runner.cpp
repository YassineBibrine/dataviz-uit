#include "algorithm_runner.h"

// ===============================
// Constructeur / Destructeur
// ===============================

AlgorithmRunner::~AlgorithmRunner() = default;


// ===============================
// Record d'une étape
// ===============================
void AlgorithmRunner::recordStep(const std::string& operation,
    const std::map<std::string, std::string>& details)
{
    // Génère une frame d’animation basée sur cette opération
    frameRecorder.generateFrame(operation, details);

    // Sauvegarde l’état pour permettre le rewind
    executionState.saveState();
}


// ===============================
// Reset complet
// ===============================
void AlgorithmRunner::reset()
{
    isPaused = false;
    currentStep = 0;

    executionState.reset();
    frameRecorder.clear();
}


// ===============================
// Pause / Resume
// ===============================
void AlgorithmRunner::pause()
{
    isPaused = true;
}

void AlgorithmRunner::resume()
{
    isPaused = false;
}


// ===============================
// Navigation dans les étapes
// ===============================
void AlgorithmRunner::stepForward()
{
    const int total = static_cast<int>(frameRecorder.getAllFrames().size());
    if (currentStep + 1 < total)
        currentStep++;
}

void AlgorithmRunner::stepBackward()
{
    if (currentStep > 0) {
        currentStep--;
        executionState.restoreState(currentStep);
    }
}


// ===============================
// Getters
// ===============================
ExecutionState AlgorithmRunner::getExecutionState() const
{
    return executionState;
}

std::vector<AnimationFrame> AlgorithmRunner::getAnimationFrames() const
{
    return frameRecorder.getAllFrames();
}
