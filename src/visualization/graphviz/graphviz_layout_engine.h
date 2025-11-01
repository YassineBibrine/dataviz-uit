#ifndef GRAPHVIZ_LAYOUT_ENGINE_H
#define GRAPHVIZ_LAYOUT_ENGINE_H

#include <string>
#include <map>
#include <vector>

struct LayoutNode {
    std::string id;
    double x, y;
    double width, height;
};

struct LayoutEdge {
    std::string from, to;
    std::vector<std::pair<double, double>> controlPoints;
};

struct GraphvizLayout {
    std::vector<LayoutNode> nodes;
    std::vector<LayoutEdge> edges;
    double canvasWidth, canvasHeight;
};

/**
 * @class GraphvizLayoutEngine
 * @brief Executes Graphviz for algorithmic graph layout
 * 
 * Responsibilities:
 * - Execute Graphviz dot command
 * - Parse layout output
 * - Generate node positions and edge paths
 * - Support various layout algorithms (dot, neato, fdp, circo, twopi)
 */
class GraphvizLayoutEngine {
public:
  GraphvizLayoutEngine();
    ~GraphvizLayoutEngine();
    
    bool setGraphvizPath(const std::string &path);
    bool generateLayout(const std::string &dotCode, GraphvizLayout &output);
    std::string getLastError() const;

private:
    std::string graphvizPath;
    std::string lastError;
    
    bool executeGraphviz(const std::string &dotCode, std::string &output);
    bool parseGraphvizOutput(const std::string &output, GraphvizLayout &layout);
};

#endif // GRAPHVIZ_LAYOUT_ENGINE_H
