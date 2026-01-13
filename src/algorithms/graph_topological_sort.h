#ifndef GRAPH_TOPOLOGICAL_SORT_H
#define GRAPH_TOPOLOGICAL_SORT_H

#include "../orchestration/algorithm.h"
#include "../core/graph_structure.h"
#include "../core/graph.h"
#include <QDebug>
#include <QMessageBox>
#include <set>
#include <stack>
#include <sstream>
#include <functional>

class GraphTopologicalSort : public Algorithm {
private:
    GraphStructure* graphStruct;
    
public:
    GraphTopologicalSort(GraphStructure* gs = nullptr) : graphStruct(gs) {}
 ~GraphTopologicalSort() override = default;
    
  void execute() override {
      if (!graphStruct) {
 qDebug() << "No graph structure provided";
     return;
   }
 
   Graph* graph = graphStruct->getGraph();
  if (!graph || graph->getNodeCount() == 0) {
    qDebug() << "Graph is empty";
    return;
    }
        
  if (!graph->isDirected()) {
    QMessageBox::warning(nullptr, "Topological Sort", 
     "Topological sort only works on directed graphs!");
  return;
 }
  
   auto nodeIds = graph->getAllNodeIds();
   std::set<std::string> visited;
  std::stack<std::string> result;
  
   std::function<void(const std::string&)> dfs = [&](const std::string& node) {
    visited.insert(node);
  
    auto neighbors = graph->getNeighbors(node);
    for (const auto& neighbor : neighbors) {
if (visited.find(neighbor) == visited.end()) {
    dfs(neighbor);
  }
        }
  
  result.push(node);
  };
     
  for (const auto& nodeId : nodeIds) {
  if (visited.find(nodeId) == visited.end()) {
  dfs(nodeId);
    }
 }
     
  std::ostringstream oss;
oss << "Topological Sort Order:\n";
 while (!result.empty()) {
   oss << result.top() << " ";
     result.pop();
        }
        
   qDebug() << QString::fromStdString(oss.str());
  QMessageBox::information(nullptr, "Topological Sort", QString::fromStdString(oss.str()));
    }
};

#endif // GRAPH_TOPOLOGICAL_SORT_H
