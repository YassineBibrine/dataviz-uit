#include "data_model_manager.h"
#include <stdexcept>
#include <sstream>
#include <set>
#include <algorithm>

std::string DataModelManager::createDataStructure(
    const std::string& type, int size, const std::string& name) {
    
    std::string id = "struct_" + std::to_string(nextStructureId++);
    std::string structName = name.empty() ? (type + " " + std::to_string(nextStructureId - 1)) : name;
  
    std::unique_ptr<DataStructure> structure;

    if (type == "Array") {
        auto arr = std::make_unique<ArrayStructure>(size);
 arr->fillRandom(size);
        structure = std::move(arr);
    } else if (type == "LinkedList" || type == "List") {
        auto list = std::make_unique<ListStructure>();
     list->generateRandom(size);
        structure = std::move(list);
    } else if (type == "BinaryTree" || type == "Tree") {
 auto tree = std::make_unique<TreeStructure>();
 tree->generateRandom(size);
        structure = std::move(tree);
    } else if (type == "Graph") {
    structure = std::make_unique<GraphStructure>(size);
    } else {
        throw std::runtime_error("Unknown structure type: " + type);
    }
    
    structures[id] = std::move(structure);
    metadata[id] = StructureMetadata(id, structName, type, StructureCreationType::GENERATED);
    
    // Auto-select if it's the first structure
    if (structures.size() == 1) {
        selectStructure(id);
    }
    
    return id;
}

std::string DataModelManager::createInteractiveStructure(
    const std::string& type, const std::string& name) {
    
    std::string id = "interactive_" + std::to_string(nextStructureId++);
    std::string structName = name.empty() ? ("Custom " + type) : name;
    
    std::unique_ptr<DataStructure> structure;
    
 // Create empty structures that can be populated interactively
    if (type == "Graph") {
   structure = std::make_unique<GraphStructure>(0);
    } else if (type == "Tree" || type == "BinaryTree") {
        structure = std::make_unique<TreeStructure>();
    } else if (type == "List" || type == "LinkedList") {
        structure = std::make_unique<ListStructure>();
    } else {
   throw std::runtime_error("Cannot create interactive structure of type: " + type);
    }
    
    structures[id] = std::move(structure);
    metadata[id] = StructureMetadata(id, structName, type, StructureCreationType::INTERACTIVE);

    // Auto-select if it's the first structure
    if (structures.size() == 1) {
        selectStructure(id);
    }
    
    return id;
}

std::string DataModelManager::buildStructureFromNodes(
    const std::string& type,
    const std::map<std::string, int>& nodes,
    const std::vector<std::pair<std::string, std::string>>& edges,
    const std::string& name) {
    
    std::string detectedType = type;
    if (type == "Auto" || type.empty()) {
    detectedType = detectStructureType(nodes.size(), edges);
    }
    
    std::string id = "custom_" + std::to_string(nextStructureId++);
    std::string structName = name.empty() ? ("Custom " + detectedType) : name;
    
    std::unique_ptr<DataStructure> structure;
    
    if (detectedType == "Graph") {
        auto graph = std::make_unique<GraphStructure>(0);
        // Populate graph with nodes and edges
   if (auto* g = graph->getGraph()) {
for (const auto& [nodeId, value] : nodes) {
  g->addNode(nodeId);
      if (auto* node = g->getNode(nodeId)) {
    node->properties["value"] = std::to_string(value);
             }
        }
  for (const auto& [from, to] : edges) {
     g->addEdge(from, to);
            }
     }
        structure = std::move(graph);
    } 
    else if (detectedType == "Tree") {
   auto tree = std::make_unique<TreeStructure>();
  // For tree, build from nodes respecting tree constraints
     // This is simplified - you may need more sophisticated tree building
if (auto* root = tree->getRoot()) {
     // Build tree from nodes and edges
      // For now, just insert values in order
    for (const auto& [nodeId, value] : nodes) {
    tree->insert(value);
         }
        }
structure = std::move(tree);
  }
    else if (detectedType == "List") {
   auto list = std::make_unique<ListStructure>();
        // Build linked list from nodes in order
        // Determine order from edges (follow the chain)
   for (const auto& [nodeId, value] : nodes) {
     list->append(value);
   }
     structure = std::move(list);
    }
    else {
     // Default to graph for unknown/complex structures
        auto graph = std::make_unique<GraphStructure>(0);
  structure = std::move(graph);
  }
    
    structures[id] = std::move(structure);
    metadata[id] = StructureMetadata(id, structName, detectedType, StructureCreationType::INTERACTIVE);
    
  // Auto-select the newly created structure
    selectStructure(id);
    
    return id;
}

std::string DataModelManager::detectStructureType(
    int nodeCount,
    const std::vector<std::pair<std::string, std::string>>& edges) {
    
    if (nodeCount == 0) {
        return "Unknown";
    }
    
    if (edges.empty()) {
        return "Array"; // No connections = array-like
    }
    
    // Build adjacency info
    std::map<std::string, std::set<std::string>> outgoing;
    std::map<std::string, int> inDegree;
    std::set<std::string> allNodes;
    
    for (const auto& [from, to] : edges) {
        outgoing[from].insert(to);
        inDegree[to]++;
        allNodes.insert(from);
        allNodes.insert(to);
    }
    
    // Initialize in-degree for nodes with no incoming edges
    for (const auto& node : allNodes) {
        if (inDegree.find(node) == inDegree.end()) {
  inDegree[node] = 0;
        }
    }
    
  // Check for linear structure (LinkedList)
    // Each node has at most 1 outgoing edge and at most 1 incoming edge
    bool isLinear = true;
    for (const auto& [node, outs] : outgoing) {
        if (outs.size() > 1) {
         isLinear = false;
         break;
}
    }
    
  if (isLinear) {
        for (const auto& [node, inDeg] : inDegree) {
          if (inDeg > 1) {
       isLinear = false;
      break;
      }
        }
}
    
    if (isLinear && !edges.empty()) {
        return "List";
    }

    // Check for tree properties:
    // 1. Exactly one root (node with in-degree 0)
    // 2. All other nodes have exactly one parent (in-degree 1)
    // 3. No cycles
    int rootCount = 0;
    bool hasMultipleParents = false;
    
    for (const auto& [node, deg] : inDegree) {
        if (deg == 0) {
            rootCount++;
     } else if (deg > 1) {
hasMultipleParents = true;
          break;
        }
    }
    
    if (rootCount == 1 && !hasMultipleParents) {
// Additional check: number of edges should be nodeCount - 1 for a tree
        if (edges.size() == allNodes.size() - 1) {
    return "Tree";
        }
    }
    
  // Default to Graph for everything else
    return "Graph";
}

bool DataModelManager::selectStructure(const std::string& structureId) {
    if (structures.find(structureId) == structures.end()) {
        return false;
    }
    
  // Deselect all others
    for (auto& [id, meta] : metadata) {
 meta.isSelected = (id == structureId);
    }
    
    selectedStructureId = structureId;
 return true;
}

DataStructure* DataModelManager::getSelectedStructure() {
    if (selectedStructureId.empty()) return nullptr;
    auto it = structures.find(selectedStructureId);
    return (it != structures.end()) ? it->second.get() : nullptr;
}

const DataStructure* DataModelManager::getSelectedStructure() const {
    if (selectedStructureId.empty()) return nullptr;
    auto it = structures.find(selectedStructureId);
    return (it != structures.end()) ? it->second.get() : nullptr;
}

DataStructure* DataModelManager::getStructure(const std::string& id) {
    auto it = structures.find(id);
    return (it != structures.end()) ? it->second.get() : nullptr;
}

const DataStructure* DataModelManager::getStructure(const std::string& id) const {
    auto it = structures.find(id);
    return (it != structures.end()) ? it->second.get() : nullptr;
}

std::vector<StructureMetadata> DataModelManager::getAllStructures() const {
    std::vector<StructureMetadata> result;
    for (const auto& [id, meta] : metadata) {
        result.push_back(meta);
    }
    return result;
}

bool DataModelManager::removeStructure(const std::string& structureId) {
    auto it = structures.find(structureId);
    if (it == structures.end()) {
        return false;
    }
    
    structures.erase(it);
    metadata.erase(structureId);
    
    // If we removed the selected structure, select another one
    if (selectedStructureId == structureId) {
  selectedStructureId = "";
        // Auto-select first available structure
        if (!structures.empty()) {
            selectStructure(structures.begin()->first);
    }
    }
    
    return true;
}

void DataModelManager::clearAll() {
    structures.clear();
    metadata.clear();
    selectedStructureId = "";
}

bool DataModelManager::renameStructure(const std::string& structureId, const std::string& newName) {
  auto it = metadata.find(structureId);
    if (it != metadata.end()) {
        it->second.name = newName;
        return true;
    }
    return false;
}