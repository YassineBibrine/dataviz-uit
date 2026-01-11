#include "graph_structure.h"
#include <random>
#include <QJsonArray>

GraphStructure::GraphStructure() : graph(std::make_unique<Graph>(false)) {}
GraphStructure::GraphStructure(bool directed) : graph(std::make_unique<Graph>(directed)) {}

void GraphStructure::generateRandom(int nodeCount, int edgeAttempts) {
    if (!graph) graph = std::make_unique<Graph>(false);
    graph->clear();

    for (int i = 0; i < nodeCount; ++i) {
        graph->addNode("n" + std::to_string(i), { {"label", std::to_string(i)} });
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> nodeDis(0, nodeCount - 1);
    std::uniform_real_distribution<> weightDis(1.0, 10.0);

    int edgesAdded = 0, attempts = 0;
    while (edgesAdded < edgeAttempts && attempts < edgeAttempts * 3) {
        int from = nodeDis(gen);
        int to = nodeDis(gen);
        if (from != to) {
            std::string fromId = "n" + std::to_string(from);
            std::string toId = "n" + std::to_string(to);
            if (!graph->hasEdge(fromId, toId)) {
                graph->addEdge(fromId, toId, weightDis(gen), {});
                edgesAdded++;
            }
        }
        attempts++;
    }
}

std::vector<DSNode> GraphStructure::getNodes() const {
    std::vector<DSNode> nodes;
    if (!graph) return nodes;
    for (const auto& id : graph->getAllNodeIds())
        nodes.emplace_back(id);
    return nodes;
}

std::vector<DSEdge> GraphStructure::getEdges() const {
    std::vector<DSEdge> edges;
    if (!graph) return edges;
    for (const auto& e : graph->getAllEdges())
        edges.emplace_back(e.from, e.to);
    return edges;
}

std::string GraphStructure::serializeToDOT() const {
    if (!graph) return "";
    std::ostringstream oss;
    oss << (graph->isDirected() ? "digraph G {\n" : "graph G {\n");
    oss << "  node [shape=circle];\n";
    for (const auto& id : graph->getAllNodeIds()) oss << "  " << id << ";\n";
    std::string connector = graph->isDirected() ? " -> " : " -- ";
    for (const auto& e : graph->getAllEdges())
        oss << "  " << e.from << connector << e.to << " [label=\"" << e.weight << "\"];\n";
    oss << "}\n";
    return oss.str();
}

void* GraphStructure::getDataForRunner() { return static_cast<void*>(graph.get()); }

// 🔥 Session
QJsonObject GraphStructure::serialize() const {
    QJsonObject obj;
    obj["type"] = QString::fromStdString(getType());
    QJsonArray nodes;
    for (const auto& id : graph->getAllNodeIds()) nodes.append(QString::fromStdString(id));
    obj["nodes"] = nodes;

    QJsonArray edges;
    for (auto& e : graph->getAllEdges()) {
        QJsonObject edgeObj;
        edgeObj["from"] = QString::fromStdString(e.from);
        edgeObj["to"] = QString::fromStdString(e.to);
        edgeObj["weight"] = e.weight;
        edges.append(edgeObj);
    }
    obj["edges"] = edges;
    return obj;
}

void GraphStructure::deserialize(const QJsonObject& obj) {
    if (!graph) graph = std::make_unique<Graph>(false);
    graph->clear();

    QJsonArray nodes = obj["nodes"].toArray();
    for (auto n : nodes) graph->addNode(n.toString().toStdString(), {});

    QJsonArray edges = obj["edges"].toArray();
    for (auto e : edges) {
        QJsonObject eo = e.toObject();
        graph->addEdge(eo["from"].toString().toStdString(),
            eo["to"].toString().toStdString(),
            eo["weight"].toDouble(), {});
    }
}

std::string GraphStructure::getType() const { return "Graph"; }
