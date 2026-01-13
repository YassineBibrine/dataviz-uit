#include "graph_algorithms.h"

// BFS Implementation
BFSAlgorithm::BFSAlgorithm(GraphStructure* gs) : graphStruct(gs) {}

void BFSAlgorithm::execute() {
    if (!graphStruct) {
    qDebug() << "No graph structure provided";
        return;
 }
   
    Graph* graph = graphStruct->getGraph();
    if (!graph || graph->getNodeCount() == 0) {
 qDebug() << "Graph is empty";
        return;
    }
 
 // Start from first node
    auto nodeIds = graph->getAllNodeIds();
    if (nodeIds.empty()) return;
   
    std::string startNode = nodeIds[0];
    std::queue<std::string> q;
 std::set<std::string> visited;
    std::ostringstream oss;
        
    oss << "BFS Traversal starting from node " << startNode << ":\n";
 
    q.push(startNode);
    visited.insert(startNode);
 
    while (!q.empty()) {
        std::string current = q.front();
 q.pop();
     oss << current << " ";
    
        auto neighbors = graph->getNeighbors(current);
   for (const auto& neighbor : neighbors) {
   if (visited.find(neighbor) == visited.end()) {
              visited.insert(neighbor);
     q.push(neighbor);
      }
        }
  }
 
    qDebug() << QString::fromStdString(oss.str());
 QMessageBox::information(nullptr, "BFS Traversal", QString::fromStdString(oss.str()));
}

// DFS Implementation
DFSAlgorithm::DFSAlgorithm(GraphStructure* gs) : graphStruct(gs) {}

void DFSAlgorithm::execute() {
if (!graphStruct) {
        qDebug() << "No graph structure provided";
   return;
    }
  
  Graph* graph = graphStruct->getGraph();
    if (!graph || graph->getNodeCount() == 0) {
qDebug() << "Graph is empty";
   return;
    }
        
   auto nodeIds = graph->getAllNodeIds();
    if (nodeIds.empty()) return;
  
 std::string startNode = nodeIds[0];
    std::set<std::string> visited;
 std::ostringstream oss;
     
 oss << "DFS Traversal starting from node " << startNode << ":\n";
   
    std::function<void(const std::string&)> dfs = [&](const std::string& node) {
    visited.insert(node);
        oss << node << " ";
 
   auto neighbors = graph->getNeighbors(node);
  for (const auto& neighbor : neighbors) {
  if (visited.find(neighbor) == visited.end()) {
     dfs(neighbor);
}
   }
    };
  
    dfs(startNode);
        
    qDebug() << QString::fromStdString(oss.str());
 QMessageBox::information(nullptr, "DFS Traversal", QString::fromStdString(oss.str()));
}

// Dijkstra Implementation
DijkstraAlgorithm::DijkstraAlgorithm(GraphStructure* gs) : graphStruct(gs) {}

void DijkstraAlgorithm::execute() {
    if (!graphStruct) {
   qDebug() << "No graph structure provided";
     return;
  }
  
    Graph* graph = graphStruct->getGraph();
    if (!graph || graph->getNodeCount() == 0) {
  qDebug() << "Graph is empty";
   return;
  }
   
    auto nodeIds = graph->getAllNodeIds();
 if (nodeIds.size() < 2) {
  QMessageBox::information(nullptr, "Dijkstra", "Need at least 2 nodes for shortest path");
        return;
    }
     
    // For now, just show that it would calculate shortest paths
    QString message = QString("Dijkstra's Algorithm:\n\n")
        + QString("Would calculate shortest paths from node %1 to all other nodes.")
   .arg(QString::fromStdString(nodeIds[0]));
      
  qDebug() << message;
    QMessageBox::information(nullptr, "Dijkstra's Algorithm", message);
}
