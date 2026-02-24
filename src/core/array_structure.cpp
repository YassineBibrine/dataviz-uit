#include "array_structure.h"
#include <random>
#include <sstream>
#include <QJsonArray>
#include <QJsonObject>

// ArrayStructure implementation
// Responsible for maintaining a vector<int> and exposing a visualization-friendly
// representation (nodes and edges), as well as session serialization helpers.

ArrayStructure::ArrayStructure(int size) {
    if (size > 0) {
        fillRandom(size);
    }
}

// Fill the internal vector with random values in the range [1,100].
void ArrayStructure::fillRandom(int size) {
    data.clear();
    data.reserve(size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    for (int i = 0; i < size; ++i) {
        data.push_back(dis(gen));
    }
}

// Return a list of DSNode objects used by the renderer. Each entry contains
// a unique id ("arr_i") and the display value for the array cell.
std::vector<DSNode> ArrayStructure::getNodes() const {
    std::vector<DSNode> nodes;
    nodes.reserve(data.size());

    for (size_t i = 0; i < data.size(); ++i) {
        std::string nodeId = "arr_" + std::to_string(i);
        std::string nodeValue = std::to_string(data[i]);
        nodes.emplace_back(nodeId, nodeValue);
    }

    return nodes;
}

// Arrays are visualized as adjacent boxes. No explicit edges are required so
// we return an empty vector (the VisualizationPane will arrange items side-by-side).
std::vector<DSEdge> ArrayStructure::getEdges() const {
    // MODIFICATION IMPORTANTE :
    // On renvoie un vecteur vide pour ne PAS dessiner de flèches.
    // Les cases seront juste dessinées côte à côte par le VisualizationPane.
    std::vector<DSEdge> edges;
    return edges;
}

// Produce a DOT-style record representation useful for debugging or export.
std::string ArrayStructure::serializeToDOT() const {
    std::ostringstream oss;

    oss << "digraph Array {\n";
    oss << "  rankdir=LR;\n";
    oss << "  node [shape=record];\n";

    // Create a single record-style node that contains all array entries.
    oss << "  array [label=\"";
    for (size_t i = 0; i < data.size(); ++i) {
        if (i > 0) oss << "|";
        oss << "<f" << i << "> " << data[i];
    }
    oss << "\"];\n";

    oss << "}\n";

    return oss.str();
}

// Provide a pointer to the underlying data for algorithm runners.
void* ArrayStructure::getDataForRunner() {
    return static_cast<void*>(&data);
}

// 🔥 Session methods
// Session serialization: write values as JSON array.
QJsonObject ArrayStructure::serialize() const {
    QJsonObject obj;
    obj["type"] = QString::fromStdString(getType());
    QJsonArray arr;
    for (int v : data) arr.append(v);
    obj["values"] = arr;
    return obj;
}

// Session deserialization: restore values from JSON array if present.
void ArrayStructure::deserialize(const QJsonObject& obj) {
    data.clear();
    if (obj.contains("values")) {
        QJsonArray arr = obj["values"].toArray();
        for (auto v : arr) data.push_back(v.toInt());
    }
}

std::string ArrayStructure::getType() const {
    return "Array";
}