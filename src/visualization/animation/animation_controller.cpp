#include "animation_controller.h"

AnimationController::AnimationController() = default;

void AnimationController::loadFrames(const std::vector<std::string> &frames)
{
    this->frames = frames;
    currentFrame = 0;
}

void AnimationController::play()
{
    playing = true;
}

void AnimationController::pause()
{
    playing = false;
}

void AnimationController::stop()
{
 playing = false;
    currentFrame = 0;
}

void AnimationController::stepForward()
{
    if (currentFrame < static_cast<int>(frames.size()) - 1) {
        ++currentFrame;
    }
}

void AnimationController::stepBackward()
{
    if (currentFrame > 0) {
        --currentFrame;
    }
}

void AnimationController::setSpeed(float speed)
{
    playbackSpeed = (speed > 0) ? speed : 1.0f;
}

void AnimationController::seekToFrame(int frameIndex)
{
    if (frameIndex >= 0 && frameIndex < static_cast<int>(frames.size())) {
  currentFrame = frameIndex;
    }
}

int AnimationController::getCurrentFrame() const
{
    return currentFrame;
}

int AnimationController::getTotalFrames() const
{
    return frames.size();
}

bool AnimationController::isPlaying() const
{
    return playing;
}

float AnimationController::getSpeed() const
{
    return playbackSpeed;
}
