#include "graph_structure.h"
#include <random>
#include <sstream>
#include <QJsonArray>

GraphStructure::GraphStructure() : graph(std::make_unique<Graph>(false)) {}
GraphStructure::GraphStructure(bool directed) : graph(std::make_unique<Graph>(directed)) {}

void GraphStructure::generateRandom(int nodeCount, int edgeAttempts) {
    if (!graph) graph = std::make_unique<Graph>(false);
    graph->clear();

    // Create all nodes first with values
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> valueDis(1, 100);
    
    for (int i = 0; i < nodeCount; ++i) {
   int nodeValue = valueDis(gen);
        std::map<std::string, std::string> props;
        props["value"] = std::to_string(nodeValue);
 props["label"] = std::to_string(nodeValue);
        graph->addNode("n" + std::to_string(i), props);
    }

    std::uniform_real_distribution<> weightDis(1.0, 10.0);

    // Create a fully connected graph (complete graph)
  // Connect every node to every other node
    for (int i = 0; i < nodeCount; ++i) {
        for (int j = i + 1; j < nodeCount; ++j) {
    std::string fromId = "n" + std::to_string(i);
 std::string toId = "n" + std::to_string(j);
          graph->addEdge(fromId, toId, weightDis(gen), {});
    }
    }
}

std::vector<DSNode> GraphStructure::getNodes() const {
    std::vector<DSNode> nodes;
    if (!graph) return nodes;
    
    for (const auto& id : graph->getAllNodeIds()) {
        const Graph::Node* node = graph->getNode(id);
      std::string value = "0"; // Default value
    
        if (node) {
  // Try to get value from properties
   auto it = node->properties.find("value");
      if (it != node->properties.end()) {
    value = it->second;
            } else {
   // Fallback to label property
    it = node->properties.find("label");
            if (it != node->properties.end()) {
          value = it->second;
          } else {
     // Last resort: use node ID
      value = id;
         }
 }
    }
        
        nodes.emplace_back(id, value);
    }
    
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
    
    // Serialize nodes with their values/properties
    QJsonArray nodesArray;
    for (const auto& id : graph->getAllNodeIds()) {
     QJsonObject nodeObj;
   nodeObj["id"] = QString::fromStdString(id);
       
        // Save node properties (especially value)
 const Graph::Node* node = graph->getNode(id);
    if (node) {
      QJsonObject propsObj;
    for (const auto& prop : node->properties) {
          propsObj[QString::fromStdString(prop.first)] = QString::fromStdString(prop.second);
      }
      nodeObj["properties"] = propsObj;
     }
      
        nodesArray.append(nodeObj);
    }
    obj["nodes"] = nodesArray;

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

 // Deserialize nodes with properties
    QJsonArray nodesArray = obj["nodes"].toArray();
  for (const auto& nodeValue : nodesArray) {
   if (nodeValue.isObject()) {
 QJsonObject nodeObj = nodeValue.toObject();
  std::string nodeId = nodeObj["id"].toString().toStdString();
   
    // Restore properties
std::map<std::string, std::string> properties;
  if (nodeObj.contains("properties")) {
       QJsonObject propsObj = nodeObj["properties"].toObject();
     for (auto it = propsObj.begin(); it != propsObj.end(); ++it) {
                properties[it.key().toStdString()] = it.value().toString().toStdString();
  }
      }
       
          graph->addNode(nodeId, properties);
    } else {
   // Legacy format: just node ID
     graph->addNode(nodeValue.toString().toStdString(), {});
        }
    }

    QJsonArray edges = obj["edges"].toArray();
    for (auto e : edges) {
        QJsonObject eo = e.toObject();
        graph->addEdge(eo["from"].toString().toStdString(),
  eo["to"].toString().toStdString(),
       eo["weight"].toDouble(), {});
    }
}

std::string GraphStructure::getType() const { return "Graph"; }
