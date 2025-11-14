#include "visualization_renderer.h"
#include <QPainter>

VisualizationRenderer::VisualizationRenderer(QWidget* parent): QWidget(parent) {
 setMinimumSize(600,400);
}

void VisualizationRenderer::renderFrame(const AnimationFrame& frame) {
 positions = frame.nodePositions;
 highlights = frame.highlightedNodes;
 update();
}

void VisualizationRenderer::renderVisualization(const QString& dot) {
 (void)dot; // placeholder for Graphviz
 update();
}

void VisualizationRenderer::zoomIn() { zoomLevel *=1.1f; update(); }
void VisualizationRenderer::zoomOut() { zoomLevel *=0.9f; update(); }

void VisualizationRenderer::paintEvent(QPaintEvent*) {
 QPainter p(this);
 p.fillRect(rect(), Qt::white);
 p.save();
 p.translate(width()/2.0, height()/2.0);
 p.scale(zoomLevel, zoomLevel);
 p.translate(-width()/2.0, -height()/2.0);
 p.setPen(Qt::black);
 for (auto& kv : positions) {
 const auto& pos = kv.second;
 p.drawEllipse(QPointF(pos.first, pos.second),8,8);
 }
 p.setPen(QPen(Qt::red,2));
 for (auto& id : highlights) {
 auto it = positions.find(id);
 if (it != positions.end()) p.drawEllipse(QPointF(it->second.first, it->second.second),12,12);
 }
 p.restore();
}
