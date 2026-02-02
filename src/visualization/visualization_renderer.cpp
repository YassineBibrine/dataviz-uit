#include "visualization_renderer.h"
#include <QPainter>
#include <cmath>
#include <regex> // NÉCESSAIRE pour extraire l'indice "0", "1"...

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

    // 1. Fond BLANC
    p.fillRect(rect(), Qt::white);

    p.save();

    // ==========================
    // 2. CAMÉRA ET GRILLE
    // ==========================
    p.translate(width() / 2.0, height() / 2.0);
    p.scale(zoomLevel, zoomLevel);
    p.translate(-width() / 2.0 + panOffset.x(), -height() / 2.0 + panOffset.y());

    // --- DESSIN DE LA GRILLE ---
    double visibleW = width() / zoomLevel;
    double visibleH = height() / zoomLevel;
    double startX = -visibleW * 2;
    double startY = -visibleH * 2;
    double endX = visibleW * 2;
    double endY = visibleH * 2;

    QPen gridPen(QColor("#F0F0F0"), 1, Qt::SolidLine); // Gris très clair pour ne pas gêner
    gridPen.setCosmetic(true);
    p.setPen(gridPen);

    int gridSize = 50;

    for (double i = startX; i < endX; i += gridSize) {
        double x = std::floor(i / gridSize) * gridSize;
        p.drawLine(QPointF(x, startY), QPointF(x, endY));
    }
    for (double i = startY; i < endY; i += gridSize) {
        double y = std::floor(i / gridSize) * gridSize;
        p.drawLine(QPointF(startX, y), QPointF(endX, y));
    }

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

            // --- IMPORTANT : On ne dessine PAS d'arêtes pour les ARRAYS (contigus) ---
            if (type1 != "ARRAY") {
                QPointF startPoint(p1.first, p1.second);
                QPointF endPoint(p2.first, p2.second);

                if (type1 == "RECT") {
                    // Logique LISTE (Flèches)
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
                    // Logique CERCLE (Arbres/Graphes)
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

        // Couleur de base
        QColor fillColor = QColor("#3498db"); // Bleu par défaut
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
        }

        // --- DESSIN ARRAY (CARRÉ MÉMOIRE) ---
        if (type == "ARRAY") {
            float size = 50.0f; // Taille fixe pour que l'aimant marche bien
            QRectF rect(x - size / 2, y - size / 2, size, size);

            // Fond un peu plus clair pour faire "case mémoire"
            QColor memoryColor = (fillColor == QColor("#3498db")) ? QColor("#E3F2FD") : fillColor;
            p.setBrush(memoryColor);

            // Bordure "Technique" (Gris foncé au lieu de noir pur)
            p.setPen(QPen(QColor("#2C3E50"), 2));
            p.drawRect(rect);

            // Valeur (Centrée)
            p.setPen(Qt::black);
            QFont valFont = p.font();
            valFont.setBold(true);
            p.setFont(valFont);

            QString textToDraw;
            if (currentFrame.nodeLabels.count(id)) textToDraw = QString::fromStdString(currentFrame.nodeLabels.at(id));
            else textToDraw = QString::fromStdString(id);
            p.drawText(rect, Qt::AlignCenter, textToDraw);

            // --- INDICE EN ROUGE (Dessous) ---
            // On extrait le chiffre de l'ID (ex: "arr_5" -> "5")
            std::regex re("(\\d+)");
            std::smatch match;
            if (std::regex_search(id, match, re) && match.size() > 1) {
                QString indexStr = QString::fromStdString(match.str(1));

                QFont idxFont = p.font();
                idxFont.setPointSize(8); // Plus petit
                idxFont.setBold(false);
                p.setFont(idxFont);
                p.setPen(QColor("#E74C3C")); // Rouge

                QRectF idxRect(x - size / 2, y + size / 2 + 2, size, 15);
                p.drawText(idxRect, Qt::AlignCenter, indexStr);
            }

            // Reset font
            p.setFont(valFont);
        }
        // --- DESSIN LISTE (RECTANGLE + POINTEUR) ---
        else if (type == "RECT") {
            float w = r * 3.0f;
            float h = r * 1.5f;
            QRectF rect(x - w / 2, y - h / 2, w, h);

            p.setBrush(fillColor);
            p.setPen(QPen(Qt::black, 2));
            p.drawRect(rect);

            float barX = x + w / 6;
            p.drawLine(barX, y - h / 2, barX, y + h / 2);

            // Texte
            p.setPen(Qt::white);
            QString textToDraw;
            if (currentFrame.nodeLabels.count(id)) textToDraw = QString::fromStdString(currentFrame.nodeLabels.at(id));
            else textToDraw = QString::fromStdString(id);

            p.drawText(QRectF(x - w / 2, y - h / 2, w * 0.66, h), Qt::AlignCenter, textToDraw);
        }
        // --- DESSIN STANDARD (CERCLE) ---
        else {
            p.setBrush(fillColor);
            p.setPen(QPen(Qt::black, 2));
            p.drawEllipse(QPointF(x, y), r, r);

            p.setPen(Qt::white);
            QString textToDraw;
            if (currentFrame.nodeLabels.count(id)) textToDraw = QString::fromStdString(currentFrame.nodeLabels.at(id));
            else textToDraw = QString::fromStdString(id);

            p.drawText(QRectF(x - r, y - r, 2 * r, 2 * r), Qt::AlignCenter, textToDraw);
        }
    }

    // ==========================
    // 5. HIGHLIGHTS (Cadre brillant)
    // ==========================
    p.setPen(QPen(QColor(255, 215, 0), 3)); // Couleur Or
    p.setBrush(Qt::NoBrush);

    for (const auto& id : currentFrame.highlightedNodes) {
        if (currentFrame.nodePositions.count(id)) {
            auto pos = currentFrame.nodePositions.at(id);
            double x = pos.first;
            double y = pos.second;
            std::string type = "CIRCLE";
            if (currentFrame.nodeShapes.count(id)) type = currentFrame.nodeShapes.at(id);

            if (type == "ARRAY") {
                float size = 50.0f;
                // Highlight carré ajusté
                p.drawRect(QRectF(x - size / 2 - 2, y - size / 2 - 2, size + 4, size + 4));
            }
            else if (type == "RECT") {
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