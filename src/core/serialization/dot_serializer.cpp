#include "dot_serializer.h"
#include <sstream>

DOTSerializer::DOTSerializer() = default;

std::string DOTSerializer::serializeGraph(const Graph &graph)
{
 // TODO: Implement graph serialization to DOT format
    // Example output:
    // digraph {
    //     A [label="A"];
    //  B [label="B"];
    //     A -> B [label="1.0"];
    // }
    std::stringstream ss;
    ss << "digraph {\n";
    // Add nodes and edges
    ss << "}\n";
    return ss.str();
}

std::string DOTSerializer::serializeTree(const std::string &treeData)
{
    // TODO: Implement tree serialization to DOT format
    return "";
}

std::string DOTSerializer::formatNodeDOT(const std::string &nodeId,
    const std::map<std::string, std::string> &properties)
{
    std::stringstream ss;
    ss << "    \"" << nodeId << "\" [label=\"" << nodeId << "\"";
    // Add properties as attributes
    ss << "];\n";
    return ss.str();
}

std::string DOTSerializer::formatEdgeDOT(const std::string &from, 
    const std::string &to, double weight)
{
    std::stringstream ss;
    ss << "    \"" << from << "\" -> \"" << to << "\"";
    if (weight != 1.0) {
        ss << " [label=\"" << weight << "\"]";
    }
    ss << ";\n";
    return ss.str();
}
