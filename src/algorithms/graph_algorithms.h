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

// BFS Implementation
class BFSAlgorithm : public Algorithm {
private:
    GraphStructure* graphStruct;
    std::vector<AnimationFrame> frames;

    void createFrame(const std::string& operation,
        const std::vector<std::string>& highlighted,
        const std::map<std::string, std::string>& colors,
        const std::string& annotation);

public:
    BFSAlgorithm(GraphStructure* gs = nullptr);
    void execute() override;
    std::vector<AnimationFrame> executeWithFrames() override;
};

// DFS Implementation
class DFSAlgorithm : public Algorithm {
private:
    GraphStructure* graphStruct;
    std::vector<AnimationFrame> frames;

    void createFrame(const std::string& operation,
        const std::vector<std::string>& highlighted,
        const std::map<std::string, std::string>& colors,
        const std::string& annotation);

    void dfsVisit(const std::string& node,
        std::set<std::string>& visited,
        Graph* graph,
        std::ostringstream& oss);

public:
    DFSAlgorithm(GraphStructure* gs = nullptr);
    void execute() override;
    std::vector<AnimationFrame> executeWithFrames() override;
};

// Dijkstra Implementation
class DijkstraAlgorithm : public Algorithm {
private:
    GraphStructure* graphStruct;
    std::vector<AnimationFrame> frames;

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