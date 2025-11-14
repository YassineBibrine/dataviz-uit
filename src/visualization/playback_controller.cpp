#include "playback_controller.h"

PlaybackController::PlaybackController(QObject* parent) : QObject(parent) {
 timer = new QTimer(this);
 connect(timer, &QTimer::timeout, this, &PlaybackController::onTick);
 timer->setInterval(16);
}

void PlaybackController::loadFrames(const std::vector<AnimationFrame>& f) { frames = f; currentFrame =0; }
void PlaybackController::play() { if (!timer->isActive()) timer->start(); }
void PlaybackController::pause() { if (timer->isActive()) timer->stop(); }
void PlaybackController::stepForward() { if (currentFrame +1 < (int)frames.size()) emit frameReady(frames[++currentFrame]); }
void PlaybackController::stepBackward() { if (currentFrame -1 >=0) emit frameReady(frames[--currentFrame]); }
void PlaybackController::setSpeed(float s) { playbackSpeed = s; int interval = (int)(16 / (s <=0 ?1.0f : s)); timer->setInterval(std::max(1, interval)); }
void PlaybackController::onTick() { if (frames.empty()) return; if (currentFrame < (int)frames.size()) emit frameReady(frames[currentFrame++]); else pause(); }
