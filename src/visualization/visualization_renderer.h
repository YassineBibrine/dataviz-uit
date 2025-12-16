#pragma once

#include <QWidget>
#include <vector>
#include <map>
#include <string>
#include <QPointF> // Récupéré du main (utile pour les coordonnées)
#include "animation_frame.h" 

class VisualizationRenderer : public QWidget {
    Q_OBJECT

public:
    explicit VisualizationRenderer(QWidget* parent = nullptr);
    ~VisualizationRenderer() override = default;

    void renderFrame(const AnimationFrame& frame);
    void renderVisualization(const QString& dot);

    // --- ZOOM ---
    // Indispensable car appelé par VisualizationPane
    void setZoomFactor(float scale);
    float getZoomFactor() const { return zoomLevel; }

    void setNodeRadius(int radius);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    float zoomLevel{ 1.0f };
    int baseNodeRadius = 20; // Ta variable de taille

    // On garde cette variable du main pour le futur (Déplacement/Panning)
    // Même si on ne l'utilise pas tout de suite, c'est bien de l'avoir.
    QPointF panOffset{0, 0};

    AnimationFrame currentFrame;
};