#pragma once

#include <QObject>
#include <QTimer>
#include <vector>
#include "animation_frame.h"
#include "frame_interpolator.h"

class PlaybackController : public QObject {
    Q_OBJECT

public:
    explicit PlaybackController(QObject* parent = nullptr);
    ~PlaybackController() override;

    void loadFrames(const std::vector<AnimationFrame>& frames_);
    void play();
    void pause();
    void stepForward();
    void stepBackward();
    void setSpeed(float speed);
    AnimationFrame interpolateBetween(const AnimationFrame& f1,
        const AnimationFrame& f2,
        double t);

signals:
    void frameReady(const AnimationFrame& frame);
    void animationComplete();  // ? NEW: Signal when animation finishes

private slots:
    void onTimeout();

private:
    std::vector<AnimationFrame> frames;
    int currentFrame{0};
    float playbackSpeed{1.0f};
    QTimer* timer{nullptr};
    FrameInterpolator interpolator;
};
