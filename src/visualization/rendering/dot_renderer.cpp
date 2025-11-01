#include "dot_renderer.h"

DOTRenderer::DOTRenderer() = default;

void DOTRenderer::renderVisualization(const std::string &dotCode)
{
    // TODO: Implement rendering of DOT visualization
    // Parse DOT code and convert to renderable elements
}

const std::vector<RenderableNode> &DOTRenderer::getNodes() const
{
    return nodes;
}

const std::vector<RenderableEdge> &DOTRenderer::getEdges() const
{
    return edges;
}
