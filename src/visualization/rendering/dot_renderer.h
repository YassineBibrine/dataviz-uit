#ifndef DOT_RENDERER_H
#define DOT_RENDERER_H

#include <string>
#include <vector>

struct RenderableNode {
    std::string id;
    double x, y;
    double radius;
    std::string label;
    std::string color;
    std::string shape;
};

struct RenderableEdge {
    std::string from, to;
    std::vector<std::pair<double, double>> path;
    std::string label;
    std::string color;
    double width;
};

/**
 * @class DOTRenderer
 * @brief Converts layout data to renderable graphical elements
 * 
 * Converts Graphviz layout output into:
 * - Node rendering parameters
 * - Edge path information
 * - Visual styling information
 * - Interaction metadata
 */
class DOTRenderer {
public:
    DOTRenderer();
    
    void renderVisualization(const std::string &dotCode);
    const std::vector<RenderableNode> &getNodes() const;
    const std::vector<RenderableEdge> &getEdges() const;

private:
    std::vector<RenderableNode> nodes;
    std::vector<RenderableEdge> edges;
};

#endif // DOT_RENDERER_H
