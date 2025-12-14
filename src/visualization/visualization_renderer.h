#pragma once

#include <QWidget>
#include <vector>
#include <map>
#include <string>
#include "animation_frame.h" 

class VisualizationRenderer : public QWidget {
    Q_OBJECT

public:
    explicit VisualizationRenderer(QWidget* parent = nullptr);

    void renderFrame(const AnimationFrame& frame);
    void renderVisualization(const QString& dot);

    // --- ZOOM ---
    // Ces méthodes sont appelées par la VisualizationPane lors du scroll souris
    void setZoomFactor(float scale);
    float getZoomFactor() const { return zoomLevel; }

    // Gardé pour compatibilité
    void setNodeRadius(int radius);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    float zoomLevel{ 1.0f };
    int baseNodeRadius = 20; // Taille de base fixe (le zoom s'occupe de l'agrandir)

    AnimationFrame currentFrame;
};