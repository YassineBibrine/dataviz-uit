#pragma once

#include <QWidget>
#include <QPointF>
#include <vector>
#include <map>
#include <string>
#include "animation_frame.h"

class VisualizationRenderer : public QWidget {
 Q_OBJECT
 float zoomLevel{1.0f};
 QPointF panOffset{0,0};
 std::map<std::string, std::pair<double,double>> positions;
 std::vector<std::string> highlights;
public:
 explicit VisualizationRenderer(QWidget* parent=nullptr);
 void renderFrame(const AnimationFrame& frame);
 void renderVisualization(const QString& dot);
 void zoomIn();
 void zoomOut();
protected:
 void paintEvent(QPaintEvent*) override;
};
