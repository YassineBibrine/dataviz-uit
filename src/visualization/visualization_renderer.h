#pragma once

#include <QWidget>
#include <QPointF>
#include <vector>
#include <map>
#include <string>
#include "animation_frame.h" // Indispensable pour la structure de données

class VisualizationRenderer : public QWidget {
    Q_OBJECT

public:
    explicit VisualizationRenderer(QWidget* parent = nullptr);

    // La signature reste la même, mais on utilise la frame complète
    void renderFrame(const AnimationFrame& frame);
    void renderVisualization(const QString& dot);

    void zoomIn();
    void zoomOut();
    void setNodeRadius(int radius);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    float zoomLevel{ 1.0f };
    int nodeRadius = 20;
    QPointF panOffset{ 0,0 };

    // --- NOUVEAU : On stocke la frame complète (positions, formes, couleurs) ---
    AnimationFrame currentFrame;
};