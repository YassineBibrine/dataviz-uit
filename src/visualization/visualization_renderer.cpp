#include "visualization_renderer.h"
#include <QPainter>
#include <cmath>

VisualizationRenderer::VisualizationRenderer(QWidget* parent)
    : QWidget(parent) {
    setMinimumSize(600, 400);
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

void VisualizationRenderer::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // 1. Fond BLANC
    p.fillRect(rect(), Qt::white);

    p.save();

    // ==========================
    // 2. CAMÉRA ET GRILLE
    // ==========================
    p.translate(width() / 2.0, height() / 2.0);
    p.scale(zoomLevel, zoomLevel);

    // --- DESSIN DE LA GRILLE  ---
    double visibleW = width() / zoomLevel;
    double visibleH = height() / zoomLevel;
    double startX = -visibleW * 2;
    double startY = -visibleH * 2;
    double endX = visibleW * 2;
    double endY = visibleH * 2;

    QPen gridPen(QColor("#E0E0E0"), 1, Qt::SolidLine); // Gris clair
    gridPen.setCosmetic(true);
    p.setPen(gridPen);

    int gridSize = 50;

    // Verticales
    for (double i = startX; i < endX; i += gridSize) {
        double x = std::floor(i / gridSize) * gridSize;
        p.drawLine(QPointF(x, startY), QPointF(x, endY));
    }
    // Horizontales
    for (double i = startY; i < endY; i += gridSize) {
        double y = std::floor(i / gridSize) * gridSize;
        p.drawLine(QPointF(startX, y), QPointF(endX, y));
    }
    // -----------------------------------------------

    // On remet l'origine pour dessiner les objets
    p.translate(-width() / 2.0, -height() / 2.0);

    int r = baseNodeRadius;

    // ==========================
    // 3. DESSIN DES ARÊTES
    // ==========================
    p.setPen(QPen(Qt::black, 2));

    for (const auto& edge : currentFrame.edges) {
        std::string src = edge.first;
        std::string dst = edge.second;

        if (currentFrame.nodePositions.count(src) && currentFrame.nodePositions.count(dst)) {
            auto p1 = currentFrame.nodePositions.at(src);
            auto p2 = currentFrame.nodePositions.at(dst);

            std::string type1 = "CIRCLE";
            if (currentFrame.nodeShapes.count(src)) type1 = currentFrame.nodeShapes.at(src);

            QPointF startPoint(p1.first, p1.second);
            QPointF endPoint(p2.first, p2.second);

            if (type1 == "RECT") {
                float w = r * 3.0f;
                startPoint.setX(startPoint.x() + w / 2 - 10);
                if (endPoint.x() > startPoint.x()) endPoint.setX(endPoint.x() - w / 2);
                p.drawLine(startPoint, endPoint);

                p.setBrush(Qt::black);
                QPolygonF arrow;
                arrow << endPoint << QPointF(endPoint.x() - 8, endPoint.y() - 4) << QPointF(endPoint.x() - 8, endPoint.y() + 4);
                p.drawPolygon(arrow);
            }
            else {
                double dx = p2.first - p1.first;
                double dy = p2.second - p1.second;
                double len = std::sqrt(dx * dx + dy * dy);
                if (len > 0) {
                    double uX = dx / len;
                    double uY = dy / len;
                    QPointF s(p1.first + uX * r, p1.second + uY * r);
                    QPointF e(p2.first - uX * r, p2.second - uY * r);
                    p.drawLine(s, e);
                }
            }
        }
    }

    // ==========================
    // 4. DESSIN DES NOEUDS
    // ==========================
    for (const auto& kv : currentFrame.nodePositions) {
        std::string id = kv.first;
        double x = kv.second.first;
        double y = kv.second.second;

        std::string type = "CIRCLE";
        if (currentFrame.nodeShapes.count(id)) type = currentFrame.nodeShapes.at(id);

        if (type == "RECT") {
            p.setBrush(QColor("#e67e22"));
            p.setPen(QPen(Qt::black, 2));
            float w = r * 3.0f;
            float h = r * 1.5f;
            QRectF rect(x - w / 2, y - h / 2, w, h);
            p.drawRect(rect);
            float barX = x + w / 6;
            p.drawLine(barX, y - h / 2, barX, y + h / 2);
        }
        else {
            p.setBrush(QColor("#3498db"));
            p.setPen(QPen(Qt::black, 2));
            p.drawEllipse(QPointF(x, y), r, r);
        }

        p.setPen(Qt::white);
        QRectF textRect = (type == "RECT")
            ? QRectF(x - r * 1.5, y - r * 0.75, r * 2, r * 1.5)
            : QRectF(x - r, y - r, r * 2, r * 2);

        QString textToDraw;
        if (currentFrame.nodeLabels.count(id)) textToDraw = QString::fromStdString(currentFrame.nodeLabels.at(id));
        else textToDraw = QString::fromStdString(id);

        p.drawText(textRect, Qt::AlignCenter, textToDraw);
    }

    // ==========================
    // 5. HIGHLIGHTS
    // ==========================
    p.setPen(QPen(Qt::red, 3));
    p.setBrush(Qt::NoBrush);

    for (const auto& id : currentFrame.highlightedNodes) {
        if (currentFrame.nodePositions.count(id)) {
            auto pos = currentFrame.nodePositions.at(id);
            double x = pos.first;
            double y = pos.second;
            std::string type = "CIRCLE";
            if (currentFrame.nodeShapes.count(id)) type = currentFrame.nodeShapes.at(id);

            if (type == "RECT") {
                float w = r * 3.0f + 10;
                float h = r * 1.5f + 10;
                p.drawRect(QRectF(x - w / 2, y - h / 2, w, h));
            }
            else {
                p.drawEllipse(QPointF(x, y), r + 5, r + 5);
            }
        }
    }

    p.restore();
}