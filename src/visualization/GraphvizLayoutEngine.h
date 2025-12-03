#pragma once

#include <string>
#include <map>
#include <utility>

/**
 * @class GraphvizLayoutEngine
 * @brief Converts DOT format to node positions using Graphviz library
 */
class GraphvizLayoutEngine {
public:
    GraphvizLayoutEngine();
    ~GraphvizLayoutEngine();

    /**
     * @brief Process DOT string and compute node positions
     * @param dotString DOT format graph description
     * @return Map of node ID to (x, y) position
     */
    std::map<std::string, std::pair<double, double>> computeLayout(const std::string& dotString);

    /**
     * @brief Check if Graphviz is available
     * @return true if Graphviz libraries are loaded and functional
     */
    bool isAvailable() const;

    /**
     * @brief Set the layout algorithm
 * @param algorithm Layout algorithm name ("dot", "neato", "fdp", "circo", "twopi")
     */
    void setLayoutAlgorithm(const std::string& algorithm);

private:
    std::string layoutAlgorithm;
    bool graphvizAvailable;
    
    void* gvc; // Graphviz context (opaque pointer to avoid including gvc.h in header)
};
