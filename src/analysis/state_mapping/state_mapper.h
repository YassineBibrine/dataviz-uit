#ifndef STATE_MAPPER_H
#define STATE_MAPPER_H

#include "execution_logger.h"
#include <string>
#include <vector>

struct VisualizationState {
    int stepNumber;
    std::string nodeData;   // JSON representation of nodes
    std::string edgeData;     // JSON representation of edges
    std::map<std::string, std::string> memoryState;  // Low-level memory view
  std::map<std::string, std::string> metrics;  // Performance metrics
};

/**
 * @class StateMapper
 * @brief Maps execution events to visualization states
 * 
 * Converts execution log events into visualization-ready state representations:
 * - Generates node/edge data for graphical rendering
 * - Creates memory views for low-level visualization
 * - Computes performance metrics
 * - Generates DOT format for Graphviz layout
 */
class StateMapper {
public:
    StateMapper();
    
    std::vector<VisualizationState> generateVisualizationStates(
const ExecutionLogger &logger,
        const std::string &dataStructureType);
    
    std::string generateDOTCode(const VisualizationState &state);
    std::string generateMemoryViewData(const VisualizationState &state);

private:
    std::string convertNodesToDOT(const std::string &nodeData);
    std::string convertEdgesToDOT(const std::string &edgeData);
};

#endif // STATE_MAPPER_H
