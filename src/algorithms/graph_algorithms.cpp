#include "graph_algorithms.h"
#include "../ui/algorithm_input_dialog.h"
#include <chrono>

// ============================================================================
// BFS ALGORITHM
// ============================================================================

BFSAlgorithm::BFSAlgorithm(GraphStructure* gs) : graphStruct(gs) {}

void BFSAlgorithm::createFrame(const std::string& operation,
    const std::vector<std::string>& highlighted,
    const std::map<std::string, std::string>& colors,
    const std::string& annotation) {
    AnimationFrame frame;
    frame.frameNumber = frames.size();
    frame.operationType = operation;
    frame.highlightedNodes = highlighted;
    frame.nodeColors = colors;
    frame.annotations.push_back(annotation);
    frame.duration = 800; // 800ms per frame
    frame.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    frames.push_back(frame);
}

void BFSAlgorithm::execute() {
    // Execute with frames and show final result
    auto resultFrames = executeWithFrames();

    if (!resultFrames.empty()) {
        // Get the last frame's annotation for the summary
        const auto& lastFrame = resultFrames.back();
        QString summary;
        for (const auto& annotation : lastFrame.annotations) {
            summary += QString::fromStdString(annotation) + "\n";
        }

        qDebug() << "BFS completed with" << resultFrames.size() << "frames";
        QMessageBox::information(nullptr, "BFS Traversal", summary);
    }
}

std::vector<AnimationFrame> BFSAlgorithm::executeWithFrames() {
    frames.clear();

  if (!graphStruct) {
        qDebug() << "No graph structure provided";
        return frames;
    }

    Graph* graph = graphStruct->getGraph();
    if (!graph || graph->getNodeCount() == 0) {
        qDebug() << "Graph is empty";
        return frames;
    }

    // Get all node IDs
    auto nodeIds = graph->getAllNodeIds();
    if (nodeIds.empty()) return frames;

    qDebug() << "========================================";
    qDebug() << "BFS DEBUG - Total nodes in graph:" << nodeIds.size();
    for (const auto& id : nodeIds) {
        const Graph::Node* node = graph->getNode(id);
std::string value = "N/A";
   if (node && node->properties.find("value") != node->properties.end()) {
  value = node->properties.at("value");
        }
        qDebug() << "  Node:" << QString::fromStdString(id) << "Value:" << QString::fromStdString(value);
    }

  // ⭐ NEW: Check adjacency list
    qDebug() << "\n=== ADJACENCY LIST ===";
    auto allEdges = graph->getAllEdges();
    qDebug() << "Total edges stored:" << allEdges.size();
    
    int totalAdjEntries = 0;
    for (const auto& nodeId : nodeIds) {
        auto neighbors = graph->getNeighbors(nodeId);
        totalAdjEntries += neighbors.size();
        qDebug() << "Node" << QString::fromStdString(nodeId) << "has" << neighbors.size() << "neighbors:";
        for (const auto& neighbor : neighbors) {
   qDebug() << "  ->" << QString::fromStdString(neighbor);
        }
        if (neighbors.empty()) {
         qDebug() << "  ⚠️  WARNING: This node is ISOLATED!";
        }
    }
    qDebug() << "Total adjacency entries:" << totalAdjEntries;
qDebug() << "Expected for undirected:" << (allEdges.size() * 2);
    qDebug() << "========================================\n";

    // ⭐ NEW: Prepare node data with values for dialog
  std::vector<std::pair<std::string, std::string>> nodeData;
    for (const auto& nodeId : nodeIds) {
        const Graph::Node* node = graph->getNode(nodeId);
   std::string value = "N/A";
  if (node && node->properties.find("value") != node->properties.end()) {
            value = node->properties.at("value");
        }
        nodeData.push_back({nodeId, value});
    }

    // ⭐ NEW: Enhanced dialog - Ask for start node AND search target
    AlgorithmInputDialog dialog("BFS Search");
    dialog.addNodeSelectionWithValues("Select start node:", nodeData);
    
    // Add option to search for specific value or traverse all
    std::vector<std::pair<std::string, std::string>> searchOptions = {
        {"(Traverse All)", ""},
     {"(Search by Value)", "search"}
    };
    dialog.addNodeSelectionWithValues("Search mode:", searchOptions);
    dialog.addIntInput("Target value (if searching):", 0, 0, 100);
    
    std::string startNode;
    bool searchMode = false;
    int targetValue = 0;
 
    if (dialog.exec() == QDialog::Accepted) {
  startNode = dialog.getStringValue(0).toStdString();
        QString mode = dialog.getStringValue(1);
        if (mode == "(Search by Value)") {
            searchMode = true;
  targetValue = dialog.getIntValue(2);
        }
    } else {
        // User cancelled - use first node as default
        startNode = nodeIds[0];
    }

    qDebug() << "BFS: Starting from node:" << QString::fromStdString(startNode);
    if (searchMode) {
        qDebug() << "BFS: SEARCH MODE - Looking for value:" << targetValue;
    } else {
        qDebug() << "BFS: TRAVERSAL MODE - Visiting all nodes";
    }

    std::queue<std::string> q;
    std::set<std::string> visited;
    std::vector<std::string> traversalOrder;
    std::map<std::string, std::string> parent; // Track path
    
    bool found = false;
    std::string foundNode = "";
    int nodesExplored = 0;

  // Initial frame
    std::string initialMsg = searchMode ? 
        "BFS Search starting from node " + startNode + "\nSearching for value: " + std::to_string(targetValue) :
    "BFS Traversal starting from node " + startNode;
    
    createFrame("START", { startNode }, { {startNode, "#4CAF50"} }, initialMsg);

    q.push(startNode);
    visited.insert(startNode);
    traversalOrder.push_back(startNode);
    parent[startNode] = "";

  // ⭐ Check if start node is the target
    if (searchMode) {
        const Graph::Node* startNodeObj = graph->getNode(startNode);
        if (startNodeObj && startNodeObj->properties.find("value") != startNodeObj->properties.end()) {
            int startValue = std::stoi(startNodeObj->properties.at("value"));
     if (startValue == targetValue) {
      found = true;
         foundNode = startNode;
            }
    }
    }

    // Process queue
    while (!q.empty() && (!searchMode || !found)) {
        std::string current = q.front();
        q.pop();
     nodesExplored++;

        qDebug() << "BFS: Processing node:" << QString::fromStdString(current);

     // Frame: Processing current node
        std::map<std::string, std::string> colors;
        colors[current] = "#FF9800"; // Orange for current
        for (const auto& v : visited) {
            if (v != current) {
        colors[v] = "#9E9E9E"; // Gray for visited
    }
        }

        std::string processMsg = "Processing node: " + current;
        if (searchMode) {
   const Graph::Node* node = graph->getNode(current);
  if (node && node->properties.find("value") != node->properties.end()) {
         processMsg += " (value: " + node->properties.at("value") + ")";
            }
         processMsg += "\nStill searching for: " + std::to_string(targetValue);
        }
        
        createFrame("PROCESS", { current }, colors, processMsg);

        // Get neighbors
        auto neighbors = graph->getNeighbors(current);
    qDebug() << "BFS: Node" << QString::fromStdString(current) << "has" << neighbors.size() << "neighbors";

        for (const auto& neighbor : neighbors) {
          qDebug() << "BFS:   Neighbor:" << QString::fromStdString(neighbor);
      if (visited.find(neighbor) == visited.end()) {
 visited.insert(neighbor);
       q.push(neighbor);
      traversalOrder.push_back(neighbor);
     parent[neighbor] = current; // Track parent for path reconstruction

qDebug() << "BFS:   -> Added to queue, total visited:" << visited.size();

     // ⭐ Check if this is the target
    if (searchMode) {
      const Graph::Node* node = graph->getNode(neighbor);
           if (node && node->properties.find("value") != node->properties.end()) {
             int nodeValue = std::stoi(node->properties.at("value"));
          if (nodeValue == targetValue) {
           found = true;
             foundNode = neighbor;
                }
                 }
   }

    // Frame: Discovered new node
         std::string discoverColor = found && foundNode == neighbor ? "#00FF00" : "#2196F3";
          colors[neighbor] = discoverColor;
   std::vector<std::string> highlighted = { current, neighbor };

        AnimationFrame frame;
        frame.frameNumber = frames.size();
    frame.operationType = found && foundNode == neighbor ? "FOUND" : "DISCOVER";
       frame.highlightedNodes = highlighted;
         frame.highlightedEdges.push_back({ current, neighbor });
          frame.nodeColors = colors;
         
     std::string annotation = found && foundNode == neighbor ?
       "✓ FOUND! Node: " + neighbor + " has value " + std::to_string(targetValue) :
 "Discovered node: " + neighbor + " from " + current;
  
          frame.annotations.push_back(annotation);
        frame.duration = 800;
  frame.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch()
              ).count();
        frames.push_back(frame);
    
                if (found) {
        qDebug() << "BFS: ✓ TARGET FOUND at node:" << QString::fromStdString(foundNode);
          break; // Stop exploring from this node
       }
    } else {
     qDebug() << "BFS:   -> Already visited";
            }
        }

     // Mark current as completely visited
        colors[current] = "#9E9E9E"; // Gray
        createFrame("VISITED", { current }, colors, "Finished processing node: " + current);
        
        if (found) break; // Stop BFS
    }

    qDebug() << "========================================";
    qDebug() << "BFS: Search complete. Visited" << visited.size() << "nodes out of" << nodeIds.size();
    qDebug() << "BFS: Nodes explored:" << nodesExplored;
    if (searchMode) {
      qDebug() << "BFS: Target" << targetValue << (found ? "FOUND" : "NOT FOUND");
    }
    qDebug() << "BFS: Traversal order:";
 for (const auto& node : traversalOrder) {
        qDebug() << "  " << QString::fromStdString(node);
 }
    
    // ⭐ Check for missing nodes
    std::vector<std::string> missingNodes;
    for (const auto& nodeId : nodeIds) {
        if (visited.find(nodeId) == visited.end()) {
        missingNodes.push_back(nodeId);
  qDebug() << "⚠️  MISSING NODE:" << QString::fromStdString(nodeId);
  }
    }
    qDebug() << "========================================\n";

 // ⭐ NEW: Final frame with search results
    std::map<std::string, std::string> finalColors;
    std::ostringstream oss;
    
    if (searchMode) {
        if (found) {
          // Reconstruct path from start to found node
        std::vector<std::string> path;
            std::string current = foundNode;
      while (!current.empty()) {
            path.push_back(current);
         current = parent[current];
            }
   std::reverse(path.begin(), path.end());
    
          // Highlight the path
     for (const auto& node : path) {
       finalColors[node] = "#00FF00"; // Bright green for successful path
  }
        for (const auto& node : visited) {
      if (finalColors.find(node) == finalColors.end()) {
     finalColors[node] = "#9E9E9E"; // Gray for explored but not on path
    }
   }
       
  oss << "✓ BFS SEARCH SUCCESSFUL!\n\n";
          oss << "Target value: " << targetValue << "\n";
     oss << "Found in node: " << foundNode << "\n";
        oss << "Nodes explored: " << nodesExplored << " out of " << nodeIds.size() << "\n\n";
       oss << "Path from " << startNode << " to " << foundNode << ":\n";
       for (size_t i = 0; i < path.size(); ++i) {
      if (i > 0) oss << " → ";
       oss << path[i];
     }
     oss << "\n\nPath length: " << (path.size() - 1) << " edges";
       
        createFrame("SUCCESS", path, finalColors, oss.str());
      } else {
            // Target not found
         for (const auto& node : visited) {
  finalColors[node] = "#FF6B6B"; // Red for failed search
   }
  
            oss << "✗ BFS SEARCH FAILED\n\n";
oss << "Target value: " << targetValue << "\n";
       oss << "NOT FOUND in reachable nodes\n";
            oss << "Nodes explored: " << nodesExplored << " out of " << nodeIds.size() << "\n\n";
    oss << "Searched through: ";
      for (size_t i = 0; i < traversalOrder.size(); ++i) {
        if (i > 0) oss << " → ";
      oss << traversalOrder[i];
    }
    
    if (!missingNodes.empty()) {
                oss << "\n\n⚠️ Note: " << missingNodes.size() << " node(s) were unreachable";
            }

            createFrame("FAILURE", traversalOrder, finalColors, oss.str());
 }
    } else {
      // Traversal mode - original behavior
   for (const auto& node : visited) {
       finalColors[node] = "#4CAF50"; // Green for all visited
        }

        oss << "BFS Traversal Complete!\n\nOrder: ";
        for (size_t i = 0; i < traversalOrder.size(); ++i) {
    if (i > 0) oss << " → ";
    oss << traversalOrder[i];
        }
 oss << "\n\nVisited " << visited.size() << " out of " << nodeIds.size() << " nodes";
        
        // ⭐ Add warning if nodes are missing
   if (!missingNodes.empty()) {
     oss << "\n\n⚠️  WARNING: " << missingNodes.size() << " node(s) not reached:\n";
            for (const auto& missing : missingNodes) {
       oss << "  - " << missing << "\n";
   }
    oss << "\nThis usually means the graph is disconnected or has isolated nodes.";
}

        createFrame("COMPLETE", traversalOrder, finalColors, oss.str());
    }

    qDebug() << "BFS generated" << frames.size() << "animation frames";
    return frames;
}

// ============================================================================
// DFS ALGORITHM
// ============================================================================

DFSAlgorithm::DFSAlgorithm(GraphStructure* gs) : graphStruct(gs) {}

void DFSAlgorithm::createFrame(const std::string& operation,
    const std::vector<std::string>& highlighted,
    const std::map<std::string, std::string>& colors,
    const std::string& annotation) {
    AnimationFrame frame;
    frame.frameNumber = frames.size();
    frame.operationType = operation;
    frame.highlightedNodes = highlighted;
    frame.nodeColors = colors;
    frame.annotations.push_back(annotation);
    frame.duration = 800;
    frame.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    frames.push_back(frame);
}

void DFSAlgorithm::dfsVisit(const std::string& node,
    std::set<std::string>& visited,
    Graph* graph,
    std::ostringstream& oss,
    bool searchMode,
    int targetValue,
    bool& found,
    std::string& foundNode,
    std::map<std::string, std::string>& parent,
    int& nodesExplored) {
    visited.insert(node);
    oss << node << " ";
  nodesExplored++;

  // ⭐ Check if this is the target
    if (searchMode && !found) {
        const Graph::Node* nodeObj = graph->getNode(node);
     if (nodeObj && nodeObj->properties.find("value") != nodeObj->properties.end()) {
          int nodeValue = std::stoi(nodeObj->properties.at("value"));
    if (nodeValue == targetValue) {
       found = true;
         foundNode = node;
        return; // Stop searching
    }
   }
    }

    // Frame: Visiting node
 std::map<std::string, std::string> colors;
    std::string nodeColor = (found && foundNode == node) ? "#00FF00" : "#FF9800";
    colors[node] = nodeColor;
    for (const auto& v : visited) {
        if (v != node) {
      colors[v] = "#9E9E9E"; // Gray for visited
}
    }

    std::string visitMsg = "Visiting node: " + node;
    if (searchMode) {
     const Graph::Node* nodeObj = graph->getNode(node);
        if (nodeObj && nodeObj->properties.find("value") != nodeObj->properties.end()) {
      visitMsg += " (value: " + nodeObj->properties.at("value") + ")";
    }
        if (!found) {
       visitMsg += "\nSearching for: " + std::to_string(targetValue);
        } else {
  visitMsg += "\n✓ TARGET FOUND!";
        }
    }

    createFrame(found && foundNode == node ? "FOUND" : "VISIT", { node }, colors, visitMsg);

    if (found) return; // Stop exploring if found

    auto neighbors = graph->getNeighbors(node);
    for (const auto& neighbor : neighbors) {
        if (visited.find(neighbor) == visited.end()) {
        parent[neighbor] = node; // Track parent
       
         // Frame: Exploring edge
   AnimationFrame frame;
      frame.frameNumber = frames.size();
        frame.operationType = "EXPLORE";
    frame.highlightedNodes = { node, neighbor };
   frame.highlightedEdges.push_back({ node, neighbor });
     frame.nodeColors = colors;
  frame.nodeColors[neighbor] = "#2196F3"; // Blue for next
      frame.annotations.push_back("Exploring edge: " + node + " → " + neighbor);
 frame.duration = 800;
            frame.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch()
   ).count();
     frames.push_back(frame);

   // Recursive call
      dfsVisit(neighbor, visited, graph, oss, searchMode, targetValue, found, foundNode, parent, nodesExplored);
  
       if (found) return; // Stop if target found in recursion
  }
    }

    // Frame: Backtracking (only if not found yet)
    if (!found) {
        colors[node] = "#9E9E9E"; // Gray
  createFrame("BACKTRACK", { node }, colors, "Backtracking from node: " + node);
    }
}

void DFSAlgorithm::execute() {
    // Execute with frames and show final result
    auto resultFrames = executeWithFrames();

    if (!resultFrames.empty()) {
        const auto& lastFrame = resultFrames.back();
        QString summary;
        for (const auto& annotation : lastFrame.annotations) {
            summary += QString::fromStdString(annotation) + "\n";
        }

        qDebug() << "DFS completed with" << resultFrames.size() << "frames";
        QMessageBox::information(nullptr, "DFS Traversal", summary);
    }
}

std::vector<AnimationFrame> DFSAlgorithm::executeWithFrames() {
    frames.clear();

    if (!graphStruct) {
        qDebug() << "No graph structure provided";
        return frames;
    }

    Graph* graph = graphStruct->getGraph();
    if (!graph || graph->getNodeCount() == 0) {
        qDebug() << "Graph is empty";
        return frames;
    }

    auto nodeIds = graph->getAllNodeIds();
    if (nodeIds.empty()) return frames;

    qDebug() << "DFS: Total nodes in graph:" << nodeIds.size();

    // ⭐ NEW: Prepare node data with values for dialog
    std::vector<std::pair<std::string, std::string>> nodeData;
    for (const auto& nodeId : nodeIds) {
      const Graph::Node* node = graph->getNode(nodeId);
  std::string value = "N/A";
   if (node && node->properties.find("value") != node->properties.end()) {
   value = node->properties.at("value");
  }
   nodeData.push_back({nodeId, value});
    }

    // ⭐ NEW: Enhanced dialog - Ask for start node AND search target
    AlgorithmInputDialog dialog("DFS Search");
 dialog.addNodeSelectionWithValues("Select start node:", nodeData);
    
    // Add option to search for specific value or traverse all
    std::vector<std::pair<std::string, std::string>> searchOptions = {
   {"(Traverse All)", ""},
  {"(Search by Value)", "search"}
    };
 dialog.addNodeSelectionWithValues("Search mode:", searchOptions);
    dialog.addIntInput("Target value (if searching):", 0, 0, 100);
    
    std::string startNode;
    bool searchMode = false;
    int targetValue = 0;
 
    if (dialog.exec() == QDialog::Accepted) {
        startNode = dialog.getStringValue(0).toStdString();
      QString mode = dialog.getStringValue(1);
 if (mode == "(Search by Value)") {
    searchMode = true;
   targetValue = dialog.getIntValue(2);
  }
  } else {
      // User cancelled - use first node as default
        startNode = nodeIds[0];
  }

    qDebug() << "DFS: Starting from node:" << QString::fromStdString(startNode);
    if (searchMode) {
  qDebug() << "DFS: SEARCH MODE - Looking for value:" << targetValue;
    } else {
        qDebug() << "DFS: TRAVERSAL MODE - Visiting all nodes";
    }

    std::set<std::string> visited;
std::ostringstream oss;
std::map<std::string, std::string> parent;
    bool found = false;
    std::string foundNode = "";
    int nodesExplored = 0;

    // Initial frame
 std::string initialMsg = searchMode ? 
    "DFS Search starting from node " + startNode + "\nSearching for value: " + std::to_string(targetValue) :
  "DFS Traversal starting from node " + startNode;
    
    createFrame("START", { startNode }, { {startNode, "#4CAF50"} }, initialMsg);

    parent[startNode] = "";
    oss << "DFS Order: ";
    dfsVisit(startNode, visited, graph, oss, searchMode, targetValue, found, foundNode, parent, nodesExplored);

    qDebug() << "DFS: Traversal complete. Visited" << visited.size() << "nodes out of" << nodeIds.size();
    qDebug() << "DFS: Nodes explored:" << nodesExplored;
    if (searchMode) {
        qDebug() << "DFS: Target" << targetValue << (found ? "FOUND" : "NOT FOUND");
}

    // ⭐ NEW: Final frame with search results
    std::map<std::string, std::string> finalColors;
    std::ostringstream summary;
    
    if (searchMode) {
        if (found) {
       // Reconstruct path from start to found node
            std::vector<std::string> path;
 std::string current = foundNode;
         while (!current.empty()) {
          path.push_back(current);
   current = parent[current];
    }
   std::reverse(path.begin(), path.end());
      
      // Highlight the path
         for (const auto& node : path) {
       finalColors[node] = "#00FF00"; // Bright green for successful path
   }
      for (const auto& node : visited) {
if (finalColors.find(node) == finalColors.end()) {
    finalColors[node] = "#9E9E9E"; // Gray for explored but not on path
        }
    }
      
    summary << "✓ DFS SEARCH SUCCESSFUL!\n\n";
  summary << "Target value: " << targetValue << "\n";
  summary << "Found in node: " << foundNode << "\n";
  summary << "Nodes explored: " << nodesExplored << " out of " << nodeIds.size() << "\n\n";
     summary << "Path from " << startNode << " to " << foundNode << ":\n";
       for (size_t i = 0; i < path.size(); ++i) {
   if (i > 0) summary << " → ";
  summary << path[i];
   }
    summary << "\n\nPath length: " << (path.size() - 1) << " edges";
     
      createFrame("SUCCESS", path, finalColors, summary.str());
  } else {
    // Target not found
     for (const auto& node : visited) {
     finalColors[node] = "#FF6B6B"; // Red for failed search
            }
   
 summary << "✗ DFS SEARCH FAILED\n\n";
        summary << "Target value: " << targetValue << "\n";
         summary << "NOT FOUND in reachable nodes\n";
      summary << "Nodes explored: " << nodesExplored << " out of " << nodeIds.size() << "\n\n";
            summary << "Explored: " << oss.str();
     
   std::vector<std::string> allVisited(visited.begin(), visited.end());
            createFrame("FAILURE", allVisited, finalColors, summary.str());
}
    } else {
 // Traversal mode - original behavior
        for (const auto& node : visited) {
   finalColors[node] = "#4CAF50"; // Green for all visited
   }

        std::vector<std::string> allVisited(visited.begin(), visited.end());

  std::string completeMsg = "DFS Traversal Complete!\n\n" + oss.str();
    completeMsg += "\n\nVisited " + std::to_string(visited.size()) + " out of " + std::to_string(nodeIds.size()) + " nodes";
 
        createFrame("COMPLETE", allVisited, finalColors, completeMsg);
    }

    qDebug() << "DFS generated" << frames.size() << "animation frames";
    return frames;
}

// ============================================================================
// DIJKSTRA ALGORITHM
// ============================================================================

DijkstraAlgorithm::DijkstraAlgorithm(GraphStructure* gs) : graphStruct(gs) {}

void DijkstraAlgorithm::createFrame(const std::string& operation,
    const std::vector<std::string>& highlighted,
    const std::map<std::string, std::string>& colors,
    const std::string& annotation) {
    AnimationFrame frame;
    frame.frameNumber = frames.size();
    frame.operationType = operation;
    frame.highlightedNodes = highlighted;
    frame.nodeColors = colors;
    frame.annotations.push_back(annotation);
    frame.duration = 1000; // Longer duration for Dijkstra
    frame.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    frames.push_back(frame);
}

void DijkstraAlgorithm::execute() {
    // Execute with frames and show final result
    auto resultFrames = executeWithFrames();

    if (!resultFrames.empty()) {
        const auto& lastFrame = resultFrames.back();
        QString summary;
        for (const auto& annotation : lastFrame.annotations) {
            summary += QString::fromStdString(annotation) + "\n";
        }

        qDebug() << "Dijkstra completed with" << resultFrames.size() << "frames";
        QMessageBox::information(nullptr, "Dijkstra's Algorithm", summary);
    }
}

std::vector<AnimationFrame> DijkstraAlgorithm::executeWithFrames() {
    frames.clear();

  if (!graphStruct) {
   qDebug() << "No graph structure provided";
        return frames;
    }

    Graph* graph = graphStruct->getGraph();
 if (!graph || graph->getNodeCount() == 0) {
  qDebug() << "Graph is empty";
  return frames;
    }

 auto nodeIds = graph->getAllNodeIds();
    if (nodeIds.empty()) return frames;

    qDebug() << "Dijkstra: Total nodes in graph:" << nodeIds.size();

// ⭐ NEW: Prepare node data with values for dialog
    std::vector<std::pair<std::string, std::string>> nodeData;
    for (const auto& nodeId : nodeIds) {
        const Graph::Node* node = graph->getNode(nodeId);
   std::string value = "N/A";
    if (node && node->properties.find("value") != node->properties.end()) {
     value = node->properties.at("value");
      }
     nodeData.push_back({nodeId, value});
    }

    // ⭐ NEW: Ask user to select start node and optionally destination with values
 AlgorithmInputDialog dialog("Dijkstra's Shortest Path");
  dialog.addNodeSelectionWithValues("Select start node:", nodeData);
    
    // Add "All nodes" option for destination
    std::vector<std::pair<std::string, std::string>> destData = {{"(All nodes)", ""}};
  destData.insert(destData.end(), nodeData.begin(), nodeData.end());
    dialog.addNodeSelectionWithValues("Select destination (optional):", destData);
    
    std::string startNode;
    std::string destNode = "";  // Empty means find paths to all nodes
 
    if (dialog.exec() == QDialog::Accepted) {
        startNode = dialog.getStringValue(0).toStdString();
      QString destChoice = dialog.getStringValue(1);
        if (destChoice != "(All nodes)") {
      destNode = destChoice.toStdString();
        }
    } else {
   // User cancelled - use first node as default
   startNode = nodeIds[0];
    }

    const double INF = std::numeric_limits<double>::infinity();

    // Initialize distances
    std::map<std::string, double> dist;
    std::map<std::string, std::string> parent;
    std::set<std::string> visited;

    for (const auto& nodeId : nodeIds) {
    dist[nodeId] = INF;
   parent[nodeId] = "";
    }
    dist[startNode] = 0;

    // Initial frame
    std::string initialMsg = "Dijkstra's Algorithm starting from node " + startNode;
    if (!destNode.empty()) {
        initialMsg += "\nFinding shortest path to: " + destNode;
    } else {
   initialMsg += "\nFinding shortest paths to all nodes";
    }
    initialMsg += "\nInitial distance: 0";
  
    createFrame("START", { startNode }, { {startNode, "#4CAF50"} }, initialMsg);

    // Priority queue: (distance, node)
    using PII = std::pair<double, std::string>;
    std::priority_queue<PII, std::vector<PII>, std::greater<PII>> pq;
    pq.push({ 0, startNode });

    // Early termination flag if we only want path to specific destination
    bool foundDestination = false;

    while (!pq.empty()) {
    double d = pq.top().first;
   std::string u = pq.top().second;
        pq.pop();

        if (visited.find(u) != visited.end()) continue;
     visited.insert(u);

        qDebug() << "Dijkstra: Visited node:" << QString::fromStdString(u) << "Total visited:" << visited.size();

 // ⭐ NEW: Early termination if we found the destination
 if (!destNode.empty() && u == destNode) {
        foundDestination = true;
     
            // Frame: Found destination
 std::map<std::string, std::string> colors;
            colors[u] = "#4CAF50"; // Green for destination
            
    std::ostringstream oss;
            oss << "Found destination: " << u << "\nShortest distance: " << dist[u];
      createFrame("DESTINATION_FOUND", { u }, colors, oss.str());
      
   break;  // Stop searching once we found the specific destination
}

        // Frame: Processing node
        std::map<std::string, std::string> colors;
        colors[u] = "#FF9800"; // Orange for current
     for (const auto& v : visited) {
          if (v != u) colors[v] = "#9E9E9E"; // Gray for visited
        }
        
        // Highlight destination if specified
        if (!destNode.empty() && destNode != u) {
colors[destNode] = "#2196F3"; // Blue for target
        }

     std::ostringstream oss;
    oss << "Processing node: " << u << "\nDistance: ";
        if (d == INF) oss << "∞";
        else oss << d;

        createFrame("PROCESS", { u }, colors, oss.str());

        // Check all edges from u
        auto edges = graph->getAllEdges();
        for (const auto& edge : edges) {
    if (edge.from == u) {
       std::string v = edge.to;
             double weight = edge.weight;

         if (visited.find(v) == visited.end() &&
         dist[u] != INF &&
   dist[u] + weight < dist[v]) {

       dist[v] = dist[u] + weight;
         parent[v] = u;
         pq.push({ dist[v], v });

         // Frame: Relaxation
            colors[v] = "#2196F3"; // Blue for updated

     AnimationFrame frame;
            frame.frameNumber = frames.size();
            frame.operationType = "RELAX";
      frame.highlightedNodes = { u, v };
          frame.highlightedEdges.push_back({ u, v });
 frame.nodeColors = colors;

  std::ostringstream annotation;
              annotation << "Relaxing edge: " << u << " → " << v
 << "\nNew distance to " << v << ": " << dist[v]
            << " (via " << u << ")";
  frame.annotations.push_back(annotation.str());
         frame.duration = 1000;
          frame.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
       std::chrono::system_clock::now().time_since_epoch()
   ).count();
         frames.push_back(frame);
     }
       }
    }

        // Mark as completely processed
        colors[u] = "#9E9E9E";
    createFrame("VISITED", { u }, colors, "Finished processing node: " + u);
    }

    qDebug() << "Dijkstra: Traversal complete. Visited" << visited.size() << "nodes out of" << nodeIds.size();

    // Final frame with results
    std::map<std::string, std::string> finalColors;
  
    // ⭐ NEW: If specific destination was requested, highlight the path
    if (!destNode.empty() && foundDestination) {
        // Reconstruct path from start to destination
     std::vector<std::string> path;
      std::string current = destNode;
        while (!current.empty()) {
     path.push_back(current);
    current = parent[current];
    }
        std::reverse(path.begin(), path.end());
        
        // Color the path
        for (const auto& node : path) {
finalColors[node] = "#4CAF50"; // Green for path
        }
        for (const auto& node : visited) {
         if (finalColors.find(node) == finalColors.end()) {
        finalColors[node] = "#9E9E9E"; // Gray for visited but not on path
 }
        }
     
      // Build result message
        std::ostringstream summary;
        summary << "Dijkstra's Algorithm Complete!\n\n";
        summary << "Shortest path from " << startNode << " to " << destNode << ":\n";
        summary << "Distance: " << dist[destNode] << "\n";
      summary << "Path: ";
        for (size_t i = 0; i < path.size(); i++) {
    if (i > 0) summary << " → ";
        summary << path[i];
        }
        
        createFrame("COMPLETE", path, finalColors, summary.str());
    } else {
  // Show paths to all nodes
        for (const auto& node : visited) {
     finalColors[node] = "#4CAF50"; // Green for all visited
}

        std::ostringstream summary;
     summary << "Dijkstra's Algorithm Complete!\n\nShortest distances from " << startNode << ":\n";
        for (const auto& nodeId : nodeIds) {
            if (nodeId != startNode) {
       summary << nodeId << ": ";
        if (dist[nodeId] == INF) {
  summary << "∞ (unreachable)";
   }
       else {
      summary << dist[nodeId];
          if (!parent[nodeId].empty()) {
     // Reconstruct path
     std::vector<std::string> path;
             std::string current = nodeId;
      while (!current.empty()) {
          path.push_back(current);
       current = parent[current];
     }
              std::reverse(path.begin(), path.end());

        summary << " (path: ";
        for (size_t i = 0; i < path.size(); i++) {
        if (i > 0) summary << "→";
        summary << path[i];
   }
       summary << ")";
           }
   }
      summary << "\n";
            }
        }
        summary << "\nVisited " << visited.size() << " out of " << nodeIds.size() << " nodes";

     std::vector<std::string> allVisited(visited.begin(), visited.end());
        createFrame("COMPLETE", allVisited, finalColors, summary.str());
    }

    qDebug() << "Dijkstra generated" << frames.size() << "animation frames";
    return frames;
}