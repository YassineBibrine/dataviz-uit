#pragma once

#include <QWidget>
#include <QPointF>
#include <map>
#include <string>
#include "animation_frame.h"

class VisualizationRenderer : public QWidget {
    Q_OBJECT

public:
    explicit VisualizationRenderer(QWidget* parent = nullptr);
    ~VisualizationRenderer() override = default;

    void renderFrame(const AnimationFrame& frame);
    void renderVisualization(const QString& dot);
    void zoomIn();
    void zoomOut();
    void setNodeRadius(int radius);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    float zoomLevel{1.0f};
    int nodeRadius = 20;
    QPointF panOffset{0, 0};
    AnimationFrame currentFrame;
};
