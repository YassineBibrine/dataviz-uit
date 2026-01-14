#include "playback_controller.h"
#include <QDebug>

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
    qDebug() << "PlaybackController: Loaded" << frames.size() << "frames";
    
    if (!frames.empty()) {
        qDebug() << "PlaybackController: Emitting first frame";
        emit frameReady(frames[currentFrame]);
    }
}

void PlaybackController::play()
{
    if (!frames.empty() && !timer->isActive()) {
        // Default: 500ms per frame, adjusted by playback speed
        // playbackSpeed of 1.0 = normal, 2.0 = 2x faster, 0.5 = 2x slower
        int interval = static_cast<int>(500 / playbackSpeed);  // ms per frame
        qDebug() << "PlaybackController: Starting playback with interval" << interval << "ms (speed:" << playbackSpeed << "x)";
   timer->start(interval);
    }
}

void PlaybackController::pause()
{
    if (timer->isActive()) {
     timer->stop();
        qDebug() << "PlaybackController: Paused at frame" << currentFrame;
    }
}

void PlaybackController::stepForward()
{
    if (frames.empty()) return;
    currentFrame = (currentFrame + 1) % frames.size();
    qDebug() << "PlaybackController: Stepped forward to frame" << currentFrame;
    emit frameReady(frames[currentFrame]);
}

void PlaybackController::stepBackward()
{
    if (frames.empty()) return;
    currentFrame = (currentFrame - 1 + static_cast<int>(frames.size())) % static_cast<int>(frames.size());
    qDebug() << "PlaybackController: Stepped backward to frame" << currentFrame;
    emit frameReady(frames[currentFrame]);
}

void PlaybackController::setSpeed(float speed)
{
    playbackSpeed = speed;
    qDebug() << "PlaybackController: Speed set to" << speed << "x";
 
    if (timer->isActive()) {
        int interval = static_cast<int>(500 / playbackSpeed);
        qDebug() << "PlaybackController: Restarting timer with new interval" << interval << "ms";
        timer->start(interval);
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

    // Simply move to next frame
    currentFrame = (currentFrame + 1) % static_cast<int>(frames.size());
    
    // Emit the frame (no interpolation for now - direct frame playback)
emit frameReady(frames[currentFrame]);
    
    // When we reach the end, pause automatically
    if (currentFrame == 0 && frames.size() > 1) {
        pause();
        qDebug() << "PlaybackController: Animation complete";
    }
}
