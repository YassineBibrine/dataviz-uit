#ifndef DOT_SERIALIZER_H
#define DOT_SERIALIZER_H

#include <string>
#include "../data_structures/graph.h"

/**
 * @class DOTSerializer
 * @brief Serializes data structures to Graphviz DOT format
 * 
 * Converts internal graph/tree representations to DOT language
 * for layout and visualization by Graphviz
 */
class DOTSerializer {
public:
    DOTSerializer();
    
    std::string serializeGraph(const Graph &graph);
    std::string serializeTree(const std::string &treeData);
    
private:
    std::string formatNodeDOT(const std::string &nodeId, 
        const std::map<std::string, std::string> &properties);
    std::string formatEdgeDOT(const std::string &from, const std::string &to,
        double weight = 1.0);
};

#endif // DOT_SERIALIZER_H
