#ifndef GRAPH_DETECT_CYCLE_H
#define GRAPH_DETECT_CYCLE_H

#include "../orchestration/algorithm.h"
#include "../core/graph_structure.h"
#include "../core/graph.h"
#include <QDebug>
#include <QMessageBox>
#include <set>
#include <functional>

class GraphDetectCycle : public Algorithm {
private:
  GraphStructure* graphStruct;
    
public:
    GraphDetectCycle(GraphStructure* gs = nullptr) : graphStruct(gs) {}
 ~GraphDetectCycle() override = default;
    
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
  
  // For undirected graph, detect cycle using DFS
  auto nodeIds = graph->getAllNodeIds();
std::set<std::string> visited;
  std::set<std::string> recStack;
 bool hasCycle = false;
  
 std::function<bool(const std::string&, const std::string&)> dfs = 
  [&](const std::string& node, const std::string& parent) -> bool {
  visited.insert(node);
   recStack.insert(node);
      
  auto neighbors = graph->getNeighbors(node);
   for (const auto& neighbor : neighbors) {
    if (neighbor == parent) continue;  // Skip parent in undirected graph
       
  if (recStack.find(neighbor) != recStack.end()) {
      return true;  // Cycle detected
    }
 
  if (visited.find(neighbor) == visited.end()) {
      if (dfs(neighbor, node)) {
  return true;
}
          }
  }
 
     recStack.erase(node);
    return false;
   };
        
  for (const auto& nodeId : nodeIds) {
   if (visited.find(nodeId) == visited.end()) {
       if (dfs(nodeId, "")) {
  hasCycle = true;
      break;
 }
        }
     }
  
  QString message = hasCycle ? 
     "Cycle Detected!\n\nThe graph contains at least one cycle." :
     "No Cycle Detected\n\nThe graph is acyclic.";
 
    qDebug() << message;
      QMessageBox::information(nullptr, "Cycle Detection", message);
    }
};

#endif // GRAPH_DETECT_CYCLE_H
