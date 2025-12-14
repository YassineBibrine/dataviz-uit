#include "playback_controller.h"

PlaybackController::PlaybackController(QObject* parent)
    : QObject(parent),
    currentFrame(0),
    playbackSpeed(1.0f)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PlaybackController::onTimeout);
}

PlaybackController::~PlaybackController() = default;

void PlaybackController::loadFrames(const std::vector<AnimationFrame>& frames_)
{
    frames = frames_;
    currentFrame = 0;
    if (!frames.empty()) {
        emit frameReady(frames[currentFrame]);
    }
}

void PlaybackController::play()
{
    if (!frames.empty() && !timer->isActive()) {
        int interval = static_cast<int>(100 / playbackSpeed); // ms par frame
        timer->start(interval);
    }
}

void PlaybackController::pause()
{
    if (timer->isActive()) {
        timer->stop();
    }
}

void PlaybackController::stepForward()
{
    if (frames.empty()) return;
    currentFrame = (currentFrame + 1) % frames.size();
    emit frameReady(frames[currentFrame]);
}

void PlaybackController::stepBackward()
{
    if (frames.empty()) return;
    currentFrame = (currentFrame - 1 + frames.size()) % frames.size();
    emit frameReady(frames[currentFrame]);
}

void PlaybackController::setSpeed(float speed)
{
    playbackSpeed = speed;
    if (timer->isActive()) {
        timer->start(static_cast<int>(100 / playbackSpeed));
    }
}

AnimationFrame PlaybackController::interpolateBetween(const AnimationFrame& f1,
    const AnimationFrame& f2,
    double t)
{
    return interpolator.interpolate(f1, f2, t);
}

void PlaybackController::onTimeout()
{
    if (frames.empty()) return;

    int nextFrame = (currentFrame + 1) % frames.size();
    AnimationFrame interpolated = interpolateBetween(frames[currentFrame],
        frames[nextFrame],
        0.5); // interpolation à mi-chemin
    currentFrame = nextFrame;
    emit frameReady(interpolated);
}
