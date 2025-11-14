#pragma once

#include <vector>
#include <QObject>
#include <QTimer>
#include "animation_frame.h"

class PlaybackController : public QObject {
 Q_OBJECT
 std::vector<AnimationFrame> frames;
 int currentFrame{0};
 float playbackSpeed{1.0f};
 QTimer* timer{nullptr};
public:
 explicit PlaybackController(QObject* parent=nullptr);
 void loadFrames(const std::vector<AnimationFrame>& f);
 void play();
 void pause();
 void stepForward();
 void stepBackward();
 void setSpeed(float s);

signals:
 void frameReady(const AnimationFrame& frame);

private slots:
 void onTick();
};
