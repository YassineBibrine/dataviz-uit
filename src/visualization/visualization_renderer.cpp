#include "visualization_renderer.h"
#include <QPainter>
#include <cmath>
#include <regex> // Used to extract numeric index from IDs like "arr_5"

VisualizationRenderer::VisualizationRenderer(QWidget* parent)
 : QWidget(parent) {
 setMinimumSize(600,400);
 setBackgroundRole(QPalette::Base);
 setAutoFillBackground(true);
}

void VisualizationRenderer::renderFrame(const AnimationFrame& frame) {
 currentFrame = frame;
 update();
}

void VisualizationRenderer::renderVisualization(const QString& dot) {
 (void)dot;
 update();
}

void VisualizationRenderer::setZoomFactor(float scale) {
 zoomLevel = scale;
 update();
}

void VisualizationRenderer::setNodeRadius(int radius) {
 (void)radius;
 update();
}

void VisualizationRenderer::panBy(double dx, double dy) {
 panOffset += QPointF(dx, dy);
 update();
}

void VisualizationRenderer::setPanOffset(const QPointF& offset) {
 panOffset = offset;
 update();
}

void VisualizationRenderer::paintEvent(QPaintEvent*) {
 QPainter p(this);
 p.setRenderHint(QPainter::Antialiasing);

 //1. Background: clear canvas to white
 p.fillRect(rect(), Qt::white);

 p.save();

 //2. Camera transform and grid
 // - Center the scene, apply zoom, then pan offset.
 p.translate(width() /2.0, height() /2.0);
 p.scale(zoomLevel, zoomLevel);
 p.translate(-width() /2.0 + panOffset.x(), -height() /2.0 + panOffset.y());

 // Draw a subtle grid to help positioning
 double visibleW = width() / zoomLevel;
 double visibleH = height() / zoomLevel;
 double startX = -visibleW *2;
 double startY = -visibleH *2;
 double endX = visibleW *2;
 double endY = visibleH *2;

 QPen gridPen(QColor("#F0F0F0"),1, Qt::SolidLine);
 gridPen.setCosmetic(true);
 p.setPen(gridPen);

 int gridSize =50;

 for (double i = startX; i < endX; i += gridSize) {
 double x = std::floor(i / gridSize) * gridSize;
 p.drawLine(QPointF(x, startY), QPointF(x, endY));
 }
 for (double i = startY; i < endY; i += gridSize) {
 double y = std::floor(i / gridSize) * gridSize;
 p.drawLine(QPointF(startX, y), QPointF(endX, y));
 }

 int r = baseNodeRadius;

 //3. Draw edges
 p.setPen(QPen(Qt::black,2));

 for (const auto& edge : currentFrame.edges) {
 std::string src = edge.first;
 std::string dst = edge.second;

 if (currentFrame.nodePositions.count(src) && currentFrame.nodePositions.count(dst)) {
 auto p1 = currentFrame.nodePositions.at(src);
 auto p2 = currentFrame.nodePositions.at(dst);

 std::string type1 = "CIRCLE";
 if (currentFrame.nodeShapes.count(src)) type1 = currentFrame.nodeShapes.at(src);

 // Do not draw edges for arrays (arrays are drawn as contiguous boxes)
 if (type1 != "ARRAY") {
 QPointF startPoint(p1.first, p1.second);
 QPointF endPoint(p2.first, p2.second);

 if (type1 == "RECT") {
 // List: draw a line and arrowhead between list nodes
 float w = r *3.0f;
 startPoint.setX(startPoint.x() + w /2 -10);
 if (endPoint.x() > startPoint.x()) endPoint.setX(endPoint.x() - w /2);
 p.drawLine(startPoint, endPoint);

 p.setBrush(Qt::black);
 QPolygonF arrow;
 arrow << endPoint << QPointF(endPoint.x() -8, endPoint.y() -4) << QPointF(endPoint.x() -8, endPoint.y() +4);
 p.drawPolygon(arrow);
 }
 else {
 // Circle: draw a line between circle edges (accounts for radius)
 double dx = p2.first - p1.first;
 double dy = p2.second - p1.second;
 double len = std::sqrt(dx * dx + dy * dy);
 if (len >0) {
 double uX = dx / len;
 double uY = dy / len;
 QPointF s(p1.first + uX * r, p1.second + uY * r);
 QPointF e(p2.first - uX * r, p2.second - uY * r);
 p.drawLine(s, e);
 }
 }
 }
 }
 }

 //4. Draw nodes
 for (const auto& kv : currentFrame.nodePositions) {
 std::string id = kv.first;
 double x = kv.second.first;
 double y = kv.second.second;

 std::string type = "CIRCLE";
 if (currentFrame.nodeShapes.count(id)) type = currentFrame.nodeShapes.at(id);

 // Determine fill color: prefer explicit per-frame color mapping
 QColor fillColor = QColor("#3498db"); // default blue
 if (currentFrame.nodeColors.count(id)) {
 QString cName = QString::fromStdString(currentFrame.nodeColors.at(id));
 if (QColor::isValidColor(cName)) fillColor = QColor(cName);
 else if (cName == "red") fillColor = QColor("#e74c3c");
 else if (cName == "green") fillColor = QColor("#2ecc71");
 else if (cName == "yellow") fillColor = QColor("#f39c12");
 else if (cName == "orange") fillColor = QColor("#e67e22");
 else if (cName == "blue") fillColor = QColor("#3498db");
 else if (cName == "cyan") fillColor = QColor("#1abc9c");
 else if (cName == "purple") fillColor = QColor("#9b59b6");
 else if (cName == "lightgray") fillColor = QColor("#bdc3c7");
 else if (cName == "#4CAF50") fillColor = QColor("#4CAF50");
 else if (cName == "#FF9800") fillColor = QColor("#FF9800");
 else if (cName == "#9E9E9E") fillColor = QColor("#9E9E9E");
 else if (cName == "#2196F3") fillColor = QColor("#2196F3");
 else if (cName == "#00FF00") fillColor = QColor("#00FF00");
 else if (cName == "#FF6B6B") fillColor = QColor("#FF6B6B");
 }

 if (type == "ARRAY") {
 float size =50.0f; // fixed cell size
 QRectF rect(x - size /2, y - size /2, size, size);

 // Slightly lighter background for memory cell look
 QColor memoryColor = (fillColor == QColor("#3498db")) ? QColor("#E3F2FD") : fillColor;
 p.setBrush(memoryColor);

 // Dark border for array cell
 p.setPen(QPen(QColor("#2C3E50"),2));
 p.drawRect(rect);

 // Draw value centered in the cell
 p.setPen(Qt::black);
 QFont valFont = p.font();
 valFont.setBold(true);
 p.setFont(valFont);

 QString textToDraw;
 if (currentFrame.nodeLabels.count(id)) textToDraw = QString::fromStdString(currentFrame.nodeLabels.at(id));
 else textToDraw = QString::fromStdString(id);
 p.drawText(rect, Qt::AlignCenter, textToDraw);

 // Draw array index in red below the cell by extracting digits from id (e.g. "arr_5" -> "5")
 std::regex re("(\\d+)");
 std::smatch match;
 if (std::regex_search(id, match, re) && match.size() >1) {
 QString indexStr = QString::fromStdString(match.str(1));

 QFont idxFont = p.font();
 idxFont.setPointSize(8);
 idxFont.setBold(false);
 p.setFont(idxFont);
 p.setPen(QColor("#E74C3C")); // red

 QRectF idxRect(x - size /2, y + size /2 +2, size,15);
 p.drawText(idxRect, Qt::AlignCenter, indexStr);
 }

 // Reset font
 p.setFont(valFont);
 }
 else if (type == "RECT") {
 // List node style: rectangular with a small pointer section
 float w = r *3.0f;
 float h = r *1.5f;
 QRectF rect(x - w /2, y - h /2, w, h);

 p.setBrush(fillColor);
 p.setPen(QPen(Qt::black,2));
 p.drawRect(rect);

 float barX = x + w /6;
 p.drawLine(barX, y - h /2, barX, y + h /2);

 // Draw label/value inside the left portion
 p.setPen(Qt::white);
 QString textToDraw;
 if (currentFrame.nodeLabels.count(id)) textToDraw = QString::fromStdString(currentFrame.nodeLabels.at(id));
 else textToDraw = QString::fromStdString(id);

 p.drawText(QRectF(x - w /2, y - h /2, w *0.66, h), Qt::AlignCenter, textToDraw);
 }
 else {
 // Default circle node
 p.setBrush(fillColor);
 p.setPen(QPen(Qt::black,2));
 p.drawEllipse(QPointF(x, y), r, r);

 p.setPen(Qt::white);
 QString textToDraw;
 if (currentFrame.nodeLabels.count(id)) textToDraw = QString::fromStdString(currentFrame.nodeLabels.at(id));
 else textToDraw = QString::fromStdString(id);

 p.drawText(QRectF(x - r, y - r,2 * r,2 * r), Qt::AlignCenter, textToDraw);
 }
 }

 //5. Highlights: draw an outer glow/outline for highlighted nodes
 p.setPen(QPen(QColor(255,215,0),3)); // gold outline
 p.setBrush(Qt::NoBrush);

 for (const auto& id : currentFrame.highlightedNodes) {
 if (currentFrame.nodePositions.count(id)) {
 auto pos = currentFrame.nodePositions.at(id);
 double x = pos.first;
 double y = pos.second;
 std::string type = "CIRCLE";
 if (currentFrame.nodeShapes.count(id)) type = currentFrame.nodeShapes.at(id);

 if (type == "ARRAY") {
 float size =50.0f;
 p.drawRect(QRectF(x - size /2 -2, y - size /2 -2, size +4, size +4));
 }
 else if (type == "RECT") {
 float w = r *3.0f +10;
 float h = r *1.5f +10;
 p.drawRect(QRectF(x - w /2, y - h /2, w, h));
 }
 else {
 p.drawEllipse(QPointF(x, y), r +5, r +5);
 }
 }
 }

 p.restore();
}