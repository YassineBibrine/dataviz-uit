#pragma once

#include <string>
#include <map>
#include <utility>

/**
 * @class GraphvizLayoutEngine
 * @brief Converts DOT format to node positions using Graphviz when available.
 *
 * This class attempts to use the Graphviz layout engines (dot, neato, etc.)
 * to compute node coordinates from a DOT description. When Graphviz is not
 * available it falls back to a simple circular layout computed locally.
 */
class GraphvizLayoutEngine {
public:
    GraphvizLayoutEngine();
    ~GraphvizLayoutEngine();

    /**
     * @brief Compute node positions from a DOT string
     * @param dotString DOT format graph description
     * @return Map of node ID to (x, y) position
     */
    std::map<std::string, std::pair<double, double>> computeLayout(const std::string& dotString);

    /**
     * @brief Returns true if Graphviz libraries are available and initialized
     */
    bool isAvailable() const;

    /**
     * @brief Select the layout algorithm to use when Graphviz is active
     * @param algorithm Layout name ("dot", "neato", "fdp", "circo", "twopi")
     */
    void setLayoutAlgorithm(const std::string& algorithm);

private:
    std::string layoutAlgorithm; ///< Selected layout algorithm name
    bool graphvizAvailable; ///< True when Graphviz context is initialized

    void* gvc; ///< Opaque Graphviz context pointer (GVC_t*)

    /**
     * @brief Fallback circular layout used when Graphviz is not available
     * @param dotString DOT format (used to extract node names)
     * @return Map of node ID to (x, y) position arranged in a circle
     */
    std::map<std::string, std::pair<double, double>> computeFallbackLayout(const std::string& dotString);
};
