#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include "../orchestration/algorithm.h"
#include "../core/graph_structure.h"
#include "../core/graph.h"
#include <QDebug>
#include <QMessageBox>
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <functional>
#include <map>
#include <limits>

// Breadth-first search algorithm implementation that emits animation frames.
class BFSAlgorithm : public Algorithm {
private:
    GraphStructure* graphStruct;
    std::vector<AnimationFrame> frames;

    // Helper: construct and append a frame describing 'operation'.
    void createFrame(const std::string& operation,
        const std::vector<std::string>& highlighted,
        const std::map<std::string, std::string>& colors,
        const std::string& annotation);

public:
    BFSAlgorithm(GraphStructure* gs = nullptr);
    void execute() override;
    std::vector<AnimationFrame> executeWithFrames() override;
};

// Depth-first search algorithm implementation that emits frames and supports
// optional search/trace reporting.
class DFSAlgorithm : public Algorithm {
private:
    GraphStructure* graphStruct;
    std::vector<AnimationFrame> frames;

    // Helper to build a frame from provided parameters.
    void createFrame(const std::string& operation,
        const std::vector<std::string>& highlighted,
        const std::map<std::string, std::string>& colors,
        const std::string& annotation);

    // Internal recursive visit routine used by DFS.
    void dfsVisit(const std::string& node,
        std::set<std::string>& visited,
        Graph* graph,
        std::ostringstream& oss,
        bool searchMode,
        int targetValue,
        bool& found,
        std::string& foundNode,
        std::map<std::string, std::string>& parent,
        int& nodesExplored);

public:
    DFSAlgorithm(GraphStructure* gs = nullptr);
    void execute() override;
    std::vector<AnimationFrame> executeWithFrames() override;
};

// Dijkstra's shortest path algorithm implementation producing frames.
class DijkstraAlgorithm : public Algorithm {
private:
    GraphStructure* graphStruct;
    std::vector<AnimationFrame> frames;

    // Helper to create a descriptive frame for the algorithm steps.
    void createFrame(const std::string& operation,
        const std::vector<std::string>& highlighted,
        const std::map<std::string, std::string>& colors,
        const std::string& annotation);

public:
    DijkstraAlgorithm(GraphStructure* gs = nullptr);
    void execute() override;
    std::vector<AnimationFrame> executeWithFrames() override;
};

#endif // GRAPH_ALGORITHMS_H