#ifndef ANIMATION_CONTROLLER_H
#define ANIMATION_CONTROLLER_H

#include <string>
#include <vector>

/**
 * @class AnimationController
 * @brief Manages visualization animation and playback
 * 
 * Controls:
 * - Animation playback (play, pause, stop)
 * - Frame stepping (forward, backward)
 * - Playback speed
 * - Timeline management
 */
class AnimationController {
public:
 AnimationController();
    
    void loadFrames(const std::vector<std::string> &frames);
    void play();
    void pause();
    void stop();
    void stepForward();
    void stepBackward();
    void setSpeed(float speed);  // Speed multiplier (1.0 = normal)
    void seekToFrame(int frameIndex);
    
    int getCurrentFrame() const;
    int getTotalFrames() const;
    bool isPlaying() const;
    float getSpeed() const;

private:
    std::vector<std::string> frames;
    int currentFrame = 0;
    bool playing = false;
  float playbackSpeed = 1.0f;
};

#endif // ANIMATION_CONTROLLER_H
