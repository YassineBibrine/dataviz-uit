#ifndef VISUALIZATION_BUILDER_H
#define VISUALIZATION_BUILDER_H

#include <string>
#include <vector>
#include <memory>

/**
 * @class VisualizationBuilder
 * @brief Builds complete visualization from execution states
 * 
 * Orchestrates the pipeline:
 * 1. Receives execution states
 * 2. Serializes to DOT format
 * 3. Generates layout via Graphviz
 * 4. Converts to renderable elements
 * 5. Prepares for animation
 */
class VisualizationBuilder {
public:
    VisualizationBuilder();
    
    std::string buildVisualization(const std::string &executionState);
    std::vector<std::string> buildAnimationSequence(
      const std::vector<std::string> &executionStates);

private:
    std::string convertToGraphicElements(const std::string &dotLayout);
};

#endif // VISUALIZATION_BUILDER_H
