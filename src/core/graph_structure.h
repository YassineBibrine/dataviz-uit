#pragma once

#include "data_structure.h"
#include "graph.h"
#include <memory>

/**
 * @class GraphStructure
 * @brief Wrapper for Graph data structure
 */
class GraphStructure : public DataStructure {
private:
    std::unique_ptr<Graph> graph;
    
public:
    GraphStructure();
    explicit GraphStructure(bool directed);
    explicit GraphStructure(int nodeCount, bool directed = false);
    
    /**
     * @brief Get the underlying graph
     * @return Pointer to the graph
     */
    Graph* getGraph() { return graph.get(); }
    const Graph* getGraph() const { return graph.get(); }
    
    /**
     * @brief Generate a random graph
     * @param nodeCount Number of nodes to generate
     * @param edgeAttempts Number of edges to attempt to create (default: nodeCount * 1.5)
     */
    void generateRandom(int nodeCount, int edgeAttempts = 0);
    
    // DataStructure interface implementation
    std::vector<DSNode> getNodes() const override;
    std::vector<DSEdge> getEdges() const override;
    std::string serializeToDOT() const override;
    void* getDataForRunner() override;
    // 🔥 Session
    QJsonObject serialize() const override;
    void deserialize(const QJsonObject& obj) override;
    std::string getType() const override;
};
