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

// BFS Implementation
class BFSAlgorithm : public Algorithm {
private:
    GraphStructure* graphStruct;

public:
    BFSAlgorithm(GraphStructure* gs = nullptr);
    void execute() override;
};

// DFS Implementation
class DFSAlgorithm : public Algorithm {
private:
    GraphStructure* graphStruct;

public:
    DFSAlgorithm(GraphStructure* gs = nullptr);
    void execute() override;
};

// Dijkstra Implementation
class DijkstraAlgorithm : public Algorithm {
private:
    GraphStructure* graphStruct;

public:
 DijkstraAlgorithm(GraphStructure* gs = nullptr);
    void execute() override;
};

#endif // GRAPH_ALGORITHMS_H
