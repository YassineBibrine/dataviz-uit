#pragma once

#include <QObject>
#include <QTimer>
#include <vector>
#include "AnimationFrame.h"
#include "FrameInterpolator.h"

class PlaybackController : public QObject
{
    Q_OBJECT

public:
    explicit PlaybackController(QObject* parent = nullptr);
    ~PlaybackController();

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

private slots:
    void onTimeout();

private:
    std::vector<AnimationFrame> frames;
    int currentFrame;
    float playbackSpeed;
    QTimer* timer;
    FrameInterpolator interpolator;
};
