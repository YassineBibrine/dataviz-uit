#include "interaction_manager.h"
#include <cmath>
#include <algorithm>
#include <string>
#include <queue>

#include "../core/data_model_manager.h"
#include "../core/graph_structure.h"
#include "../core/array_structure.h"
#include "../core/list_structure.h"
#include "../core/tree_structure.h"
#include "../core/tree_node.h"
#include "../core/graph.h"
#include <QDebug>

InteractionManager::InteractionManager() : backend(nullptr), syncWithBackend(false) {}
InteractionManager::~InteractionManager() {}

void InteractionManager::setBackend(DataModelManager* manager) {
    this->backend = manager;
}

void InteractionManager::setSyncWithBackend(bool sync) {
    syncWithBackend = sync;
}

void InteractionManager::setCurrentStructureId(const std::string& structureId) {
    currentStructureId = structureId;
}

std::string InteractionManager::getStructureType() const {
    if (!backend || currentStructureId.empty()) {
        return "";
    }
    
    auto allStructures = backend->getAllStructures();
    for (const auto& meta : allStructures) {
        if (meta.id == currentStructureId) {
            return meta.type;
        }
    }
    return "";
}

bool InteractionManager::saveToCurrentStructure() {
    if (!backend || currentStructureId.empty()) {
        qDebug() << "saveToCurrentStructure: No backend or no currentStructureId";
        return false;
  }
    
    std::string structureType = getStructureType();
    qDebug() << "saveToCurrentStructure: type=" << QString::fromStdString(structureType) 
           << "nodes=" << nodes.size();
    
    DataStructure* structure = backend->getStructure(currentStructureId);
    if (!structure) {
        qDebug() << "saveToCurrentStructure: structure not found";
        return false;
    }
    
    if (structureType == "Array") {
 if (auto* arr = dynamic_cast<ArrayStructure*>(structure)) {
          std::vector<size_t> sortedIndices(nodes.size());
        for (size_t i = 0; i < nodes.size(); ++i) sortedIndices[i] = i;
            std::sort(sortedIndices.begin(), sortedIndices.end(), [this](size_t a, size_t b) {
   return nodes[a].x < nodes[b].x;
          });
  
            auto& data = arr->getData();
  data.resize(nodes.size(), 0);
            
    for (size_t i = 0; i < sortedIndices.size(); ++i) {
           size_t nodeIdx = sortedIndices[i];
        auto it = nodeValues.find(nodes[nodeIdx].id);
   if (it != nodeValues.end()) {
     data[i] = it->second;
              }
     std::string arrNodeId = "arr_" + std::to_string(i);
                canvasToStructureNodeId[nodes[nodeIdx].id] = arrNodeId;
  }
            qDebug() << "saveToCurrentStructure: Array saved, size=" << data.size();
       return true;
        }
    }
    else if (structureType == "List") {
        if (auto* list = dynamic_cast<ListStructure*>(structure)) {
   std::vector<size_t> sortedIndices(nodes.size());
            for (size_t i = 0; i < nodes.size(); ++i) sortedIndices[i] = i;
       std::sort(sortedIndices.begin(), sortedIndices.end(), [this](size_t a, size_t b) {
              return nodes[a].x < nodes[b].x;
    });
  
 list->clearList();
       for (size_t i = 0; i < sortedIndices.size(); ++i) {
       size_t nodeIdx = sortedIndices[i];
     auto it = nodeValues.find(nodes[nodeIdx].id);
      int value = (it != nodeValues.end()) ? it->second : 0;
 list->append(value);
    
    std::string listNodeId = "list_" + std::to_string(i);
         canvasToStructureNodeId[nodes[nodeIdx].id] = listNodeId;
            }
            qDebug() << "saveToCurrentStructure: List saved, size=" << sortedIndices.size();
      return true;
      }
    }
    else if (structureType == "Binary Tree" || structureType == "Tree") {
        if (auto* tree = dynamic_cast<TreeStructure*>(structure)) {
     TreeNode* root = const_cast<TreeNode*>(tree->getRoot());
        
            // Count existing tree nodes
        int existingNodeCount = 0;
         std::map<int, TreeNode*> indexToNode;
       
     if (root) {
     std::queue<TreeNode*> bfsQueue;
         bfsQueue.push(root);
          
          while (!bfsQueue.empty()) {
      TreeNode* node = bfsQueue.front();
      bfsQueue.pop();
   if (node) {
             indexToNode[existingNodeCount++] = node;
 if (node->left) bfsQueue.push(node->left);
  if (node->right) bfsQueue.push(node->right);
  }
         }
            }

            // Update values for existing mapped nodes
        for (const auto& canvasNode : nodes) {
       auto mappingIt = canvasToStructureNodeId.find(canvasNode.id);
        if (mappingIt != canvasToStructureNodeId.end()) {
              const std::string& origId = mappingIt->second;
        if (origId.length() > 5 && origId.substr(0, 5) == "tree_") {
           try {
    int nodeIndex = std::stoi(origId.substr(5));
 auto nodeIt = indexToNode.find(nodeIndex);
       if (nodeIt != indexToNode.end()) {
         auto valIt = nodeValues.find(canvasNode.id);
         if (valIt != nodeValues.end()) {
    nodeIt->second->value = valIt->second;
        }
    }
      } catch (...) {
        // Invalid index, skip
             }
 }
     }
       }
            
            // Insert new nodes into the tree (nodes without existing mappings)
            for (const auto& canvasNode : nodes) {
            if (canvasToStructureNodeId.find(canvasNode.id) == canvasToStructureNodeId.end()) {
          auto valIt = nodeValues.find(canvasNode.id);
           int value = (valIt != nodeValues.end()) ? valIt->second : 0;
      tree->insert(value);
      qDebug() << "Inserted new node into tree with value:" << value;
           }
    }
    
       // Rebuild mappings after insertions
        root = const_cast<TreeNode*>(tree->getRoot());
        if (root) {
         std::queue<TreeNode*> bfsQueue;
     bfsQueue.push(root);
     int index = 0;
    std::map<int, int> valueToIndex;
       
      while (!bfsQueue.empty()) {
       TreeNode* node = bfsQueue.front();
    bfsQueue.pop();
      if (node) {
   valueToIndex[node->value] = index++;
          if (node->left) bfsQueue.push(node->left);
if (node->right) bfsQueue.push(node->right);
      }
    }
      
    // Update mappings for all canvas nodes based on their values
    for (const auto& canvasNode : nodes) {
        auto valIt = nodeValues.find(canvasNode.id);
       if (valIt != nodeValues.end()) {
         auto idxIt = valueToIndex.find(valIt->second);
   if (idxIt != valueToIndex.end()) {
      std::string treeNodeId = "tree_" + std::to_string(idxIt->second);
        canvasToStructureNodeId[canvasNode.id] = treeNodeId;
            }
           }
     }
     }

      // Save custom edges (user-drawn edges on canvas)
        structure->clearCustomEdges();
            for (const auto& edge : edges) {
     auto srcMapping = canvasToStructureNodeId.find(edge.source);
     auto tgtMapping = canvasToStructureNodeId.find(edge.target);
     
    std::string srcId = (srcMapping != canvasToStructureNodeId.end()) 
        ? srcMapping->second : edge.source;
 std::string tgtId = (tgtMapping != canvasToStructureNodeId.end()) 
      ? tgtMapping->second : edge.target;
    
       structure->addCustomEdge(srcId, tgtId);
        }
            
            qDebug() << "saveToCurrentStructure: Tree updated, custom edges saved:" << edges.size();
     return true;
     }
    }
    else if (structureType == "Graph") {
        if (auto* graphStruct = dynamic_cast<GraphStructure*>(structure)) {
            if (auto* g = graphStruct->getGraph()) {
      // Process all nodes
     for (const auto& node : nodes) {
      auto mappingIt = canvasToStructureNodeId.find(node.id);
              std::string origNodeId;

      if (mappingIt != canvasToStructureNodeId.end()) {
                   origNodeId = mappingIt->second;
             } else {
   int counter = g->getNodeCount();
         do {
          origNodeId = "n" + std::to_string(counter++);
          } while (g->hasNode(origNodeId));
       canvasToStructureNodeId[node.id] = origNodeId;
      }
         
    if (!g->hasNode(origNodeId)) {
      std::map<std::string, std::string> props;
           auto valIt = nodeValues.find(node.id);
           int value = (valIt != nodeValues.end()) ? valIt->second : 0;
           props["value"] = std::to_string(value);
     props["label"] = std::to_string(value);
         g->addNode(origNodeId, props);
                qDebug() << "Added new node to graph:" << QString::fromStdString(origNodeId);
      } else {
   Graph::Node* graphNode = g->getNode(origNodeId);
      if (graphNode) {
        auto valIt = nodeValues.find(node.id);
  if (valIt != nodeValues.end()) {
    graphNode->properties["value"] = std::to_string(valIt->second);
     graphNode->properties["label"] = std::to_string(valIt->second);
         }
   }
    }
       }
                
        // Process all edges
                for (const auto& edge : edges) {
         auto srcMapping = canvasToStructureNodeId.find(edge.source);
     auto tgtMapping = canvasToStructureNodeId.find(edge.target);
           
     std::string srcId = (srcMapping != canvasToStructureNodeId.end()) 
            ? srcMapping->second : edge.source;
       std::string tgtId = (tgtMapping != canvasToStructureNodeId.end()) 
        ? tgtMapping->second : edge.target;
   
         if (g->hasNode(srcId) && g->hasNode(tgtId) && !g->hasEdge(srcId, tgtId)) {
     g->addEdge(srcId, tgtId);
           qDebug() << "Added new edge to graph:" << QString::fromStdString(srcId) 
           << "->" << QString::fromStdString(tgtId);
      }
    }
             
     qDebug() << "saveToCurrentStructure: Graph saved, nodes=" << g->getNodeCount() 
        << "edges=" << g->getEdgeCount();
        return true;
   }
  }
    }

    return false;
}

void InteractionManager::saveNodePositionsToStructure() {
    if (!backend || currentStructureId.empty()) {
        return;
    }
    
    DataStructure* structure = backend->getStructure(currentStructureId);
    if (!structure) return;
    
    for (const auto& node : nodes) {
        auto it = canvasToStructureNodeId.find(node.id);
        if (it != canvasToStructureNodeId.end()) {
       structure->setNodePosition(it->second, node.x, node.y);
        } else {
            structure->setNodePosition(node.id, node.x, node.y);
        }
    }
}

std::string InteractionManager::addNode(double x, double y, const std::string& type) {
    std::string canvasId = "n" + std::to_string(nextId++);
    nodes.push_back({ canvasId, x, y, type });
    
    qDebug() << "addNode: canvasId=" << QString::fromStdString(canvasId) 
           << "syncWithBackend=" << syncWithBackend 
     << "currentStructureId=" << QString::fromStdString(currentStructureId);
    
    if (syncWithBackend && !currentStructureId.empty()) {
        saveToCurrentStructure();
        saveNodePositionsToStructure();
    }
    
    return canvasId;
}

std::string InteractionManager::addNodeWithMapping(double x, double y, const std::string& type, const std::string& originalNodeId) {
    std::string canvasId = "n" + std::to_string(nextId++);
    nodes.push_back({ canvasId, x, y, type });
    
    canvasToStructureNodeId[canvasId] = originalNodeId;

    if (syncWithBackend) {
        saveToCurrentStructure();
    saveNodePositionsToStructure();
    }
    return canvasId;
}

void InteractionManager::removeNode(const std::string& nodeId) {
    nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
        [&](const MockNode& n) { return n.id == nodeId; }), nodes.end());

    edges.erase(std::remove_if(edges.begin(), edges.end(),
        [&](const MockEdge& e) { return e.source == nodeId || e.target == nodeId; }), edges.end());

    nodeValues.erase(nodeId);
    canvasToStructureNodeId.erase(nodeId);

    if (syncWithBackend) {
      saveToCurrentStructure();
        saveNodePositionsToStructure();
    }
}

void InteractionManager::updateNodeValue(const std::string& nodeId, int value) {
    nodeValues[nodeId] = value;
    
    if (syncWithBackend) {
        saveToCurrentStructure();
        saveNodePositionsToStructure();
    }
}

void InteractionManager::addEdge(const std::string& sourceId, const std::string& targetId) {
    edges.push_back({ sourceId, targetId });

    qDebug() << "addEdge:" << QString::fromStdString(sourceId) << "->" << QString::fromStdString(targetId)
    << "syncWithBackend=" << syncWithBackend;

    if (syncWithBackend) {
        saveToCurrentStructure();
        saveNodePositionsToStructure();
    }
}

void InteractionManager::removeEdge(const std::string& sourceId, const std::string& targetId) {
edges.erase(std::remove_if(edges.begin(), edges.end(),
        [&](const MockEdge& e) {
   return (e.source == sourceId && e.target == targetId) ||
    (e.source == targetId && e.target == sourceId);
  }), edges.end());

    if (syncWithBackend) {
        saveToCurrentStructure();
    }
}

bool InteractionManager::startDragging(double x, double y) {
    std::string id = getNodeAtPosition(x, y);
    if (!id.empty()) {
        draggedNodeId = id;
  return true;
    }
    return false;
}

void InteractionManager::updateDragging(double x, double y) {
    if (!draggedNodeId.empty()) {
     for (auto& n : nodes) {
       if (n.id == draggedNodeId) {
      n.x = x;
              n.y = y;
                break;
    }
        }
    }
}

void InteractionManager::endDrag() {
    if (!draggedNodeId.empty()) {
     saveNodePositionsToStructure();
    }
    draggedNodeId = "";
}

std::vector<NodePosition> InteractionManager::getAllNodePositions() {
    std::vector<NodePosition> result;
  for (const auto& n : nodes) {
     result.push_back({ n.id, n.x, n.y, n.type });
    }
    return result;
}

std::vector<EdgeDisplay> InteractionManager::getAllEdges() {
    std::vector<EdgeDisplay> result;
    for (const auto& e : edges) {
        result.push_back({ e.source, e.target });
    }
    return result;
}

std::string InteractionManager::getNodeAtPosition(double x, double y) {
    for (auto it = nodes.rbegin(); it != nodes.rend(); ++it) {
double dx = x - it->x;
        double dy = y - it->y;
    if (std::sqrt(dx * dx + dy * dy) <= 20.0) {
      return it->id;
  }
    }
    return "";
}

std::pair<std::string, std::string> InteractionManager::getEdgeAtPosition(double mouseX, double mouseY) {
    const double threshold = 10.0;

    for (const auto& edge : edges) {
double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
        bool foundSource = false, foundTarget = false;

        for (const auto& node : nodes) {
            if (node.id == edge.source) {
          x1 = node.x; y1 = node.y; foundSource = true;
     }
       else if (node.id == edge.target) {
        x2 = node.x; y2 = node.y; foundTarget = true;
            }
            if (foundSource && foundTarget) break;
        }

     if (!foundSource || !foundTarget) continue;

  double A = mouseX - x1;
        double B = mouseY - y1;
        double C = x2 - x1;
        double D = y2 - y1;

        double dot = A * C + B * D;
        double len_sq = C * C + D * D;
        double param = -1.0;

        if (len_sq != 0)
            param = dot / len_sq;

        double xx, yy;

        if (param < 0) {
            xx = x1;
      yy = y1;
        }
   else if (param > 1) {
xx = x2;
     yy = y2;
  }
        else {
            xx = x1 + param * C;
   yy = y1 + param * D;
  }

        double dx = mouseX - xx;
        double dy = mouseY - yy;
        double distance = std::sqrt(dx * dx + dy * dy);

        if (distance < threshold) {
            return { edge.source, edge.target };
      }
    }

    return { "", "" };
}

std::string InteractionManager::finalizeStructure(const std::string& type, const std::string& name) {
    if (!backend) {
        return "";
    }
    
    if (nodes.empty()) {
     return "";
    }

    std::map<std::string, int> nodeData;
    for (const auto& node : nodes) {
        int value = (nodeValues.find(node.id) != nodeValues.end()) 
     ? nodeValues[node.id] : 0;
 nodeData[node.id] = value;
    }
    
    std::vector<std::pair<std::string, std::string>> edgeData;
    for (const auto& edge : edges) {
        edgeData.emplace_back(edge.source, edge.target);
    }

    std::string structId = backend->buildStructureFromNodes(type, nodeData, edgeData, name);

    return structId;
}

void InteractionManager::clearInteractive() {
    nodes.clear();
edges.clear();
    nodeValues.clear();
    canvasToStructureNodeId.clear();
 nextId = 1;
    currentStructureId = "";
}

std::map<std::string, int> InteractionManager::getNodeValues() const {
    return nodeValues;
}