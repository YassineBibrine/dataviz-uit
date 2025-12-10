#include "visualization_renderer.h"

VisualizationRenderer::VisualizationRenderer(QObject* parent)
    : QObject(parent),
    zoomLevel(1.0f),
    panOffset(0.0, 0.0)
{
}

VisualizationRenderer::~VisualizationRenderer() = default;

void VisualizationRenderer::renderFrame(const AnimationFrame& frame)
{
    // Mettre à jour les nodes à partir de la frame
    nodes = frame.getRenderableNodes(); // Supposons que AnimationFrame ait cette méthode
}

void VisualizationRenderer::renderVisualization(const QString& dotCode)
{
    GraphvizLayout layout = generateLayout(dotCode.toStdString());
    nodes = layout.getNodes(); // Supposons que GraphvizLayout ait getNodes()
}

void VisualizationRenderer::renderNodes(QPainter& painter)
{
    painter.save();
    painter.translate(panOffset);
    painter.scale(zoomLevel, zoomLevel);

    for (auto& node : nodes) {
        node.render(painter); // Supposons que RenderableNode a render(QPainter&)
    }

    painter.restore();
}

void VisualizationRenderer::zoomIn()
{
    zoomLevel *= 1.2f;
}

void VisualizationRenderer::zoomOut()
{
    zoomLevel /= 1.2f;
}

GraphvizLayout VisualizationRenderer::generateLayout(const std::string& dotCode)
{
    return layoutEngine.generate(dotCode); // GraphvizLayoutEngine génère le layout à partir du DOT
}
