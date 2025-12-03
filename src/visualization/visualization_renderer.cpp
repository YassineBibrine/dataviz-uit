#include "visualization_renderer.h"
#include <QPainter>
#include <cmath>

VisualizationRenderer::VisualizationRenderer(QWidget* parent)
    : QWidget(parent) {
    setMinimumSize(600, 400);
    // Fond blanc par défaut
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void VisualizationRenderer::renderFrame(const AnimationFrame& frame) {
    currentFrame = frame;
    update(); // Déclenche le paintEvent
}

void VisualizationRenderer::renderVisualization(const QString& dot) {
    (void)dot;
    update();
}

void VisualizationRenderer::zoomIn() { zoomLevel *= 1.1f; update(); }
void VisualizationRenderer::zoomOut() { zoomLevel *= 0.9f; update(); }

void VisualizationRenderer::setNodeRadius(int radius) {
    nodeRadius = radius;
    update();
}

void VisualizationRenderer::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // 1. Fond blanc
    p.fillRect(rect(), Qt::white);

    p.save();

    // 2. Zoom et Pan (Centré)
    p.translate(width() / 2.0, height() / 2.0);
    p.scale(zoomLevel, zoomLevel);
    p.translate(-width() / 2.0, -height() / 2.0);

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
                // LISTE : Part de la droite vers la gauche du suivant
                float w = nodeRadius * 3.0f;

                // Ajustement départ (Sortie de la case pointeur)
                startPoint.setX(startPoint.x() + w / 2 - 10);

                // Ajustement arrivée (Rentre à gauche)
                if (endPoint.x() > startPoint.x()) {
                    endPoint.setX(endPoint.x() - w / 2);
                }

                p.drawLine(startPoint, endPoint);

                // Flèche au bout
                p.setBrush(Qt::black);
                QPolygonF arrow;
                arrow << endPoint << QPointF(endPoint.x() - 8, endPoint.y() - 4) << QPointF(endPoint.x() - 8, endPoint.y() + 4);
                p.drawPolygon(arrow);
            }
            else {
                // GRAPHE : S'arrête au bord du cercle
                double dx = p2.first - p1.first;
                double dy = p2.second - p1.second;
                double len = std::sqrt(dx * dx + dy * dy);

                if (len > 0) {
                    double uX = dx / len;
                    double uY = dy / len;
                    // Départ et Arrivée décalés du rayon pour ne pas dessiner DANS le cercle
                    QPointF s(p1.first + uX * nodeRadius, p1.second + uY * nodeRadius);
                    QPointF e(p2.first - uX * nodeRadius, p2.second - uY * nodeRadius);
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
            // --- Rectangle (Liste) ---
            p.setBrush(QColor("#e67e22"));
            p.setPen(QPen(Qt::black, 2));

            float w = nodeRadius * 3.0f;
            float h = nodeRadius * 1.5f;

            QRectF rect(x - w / 2, y - h / 2, w, h);
            p.drawRect(rect);

            // Barre verticale de séparation
            float barX = x + w / 6;
            p.drawLine(barX, y - h / 2, barX, y + h / 2);
        }
        else {
            // --- Cercle (Graphe) ---
            p.setBrush(QColor("#3498db"));
            p.setPen(QPen(Qt::black, 2));
            p.drawEllipse(QPointF(x, y), nodeRadius, nodeRadius);
        }

        // Texte ID
        p.setPen(Qt::white);
        QRectF textRect = (type == "RECT") ? QRectF(x - nodeRadius * 1.5, y - nodeRadius * 0.75, nodeRadius * 2, nodeRadius * 1.5)
            : QRectF(x - nodeRadius, y - nodeRadius, nodeRadius * 2, nodeRadius * 2);
        p.drawText(textRect, Qt::AlignCenter, QString::fromStdString(id));
    }

    // ==========================
    // 5. DESSIN DES HIGHLIGHTS (Sélection Rouge)
    // ==========================
    p.setPen(QPen(Qt::red, 3));
    p.setBrush(Qt::NoBrush);

    for (const auto& id : currentFrame.highlightedNodes) {
        if (currentFrame.nodePositions.count(id)) {
            auto pos = currentFrame.nodePositions.at(id);
            double x = pos.first;
            double y = pos.second;

            // On vérifie le type pour adapter la forme rouge
            std::string type = "CIRCLE";
            if (currentFrame.nodeShapes.count(id)) type = currentFrame.nodeShapes.at(id);

            if (type == "RECT") {
                // Highlight RECTANGLE (un peu plus grand que le noeud)
                float w = nodeRadius * 3.0f + 10; // +10 pixels de marge
                float h = nodeRadius * 1.5f + 10;
                p.drawRect(QRectF(x - w / 2, y - h / 2, w, h));
            }
            else {
                // Highlight CERCLE
                p.drawEllipse(QPointF(x, y), nodeRadius + 5, nodeRadius + 5);
            }
        }
    }

    p.restore();
} 