#include "graph_algorithms.h"
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

    // Start from first node
    auto nodeIds = graph->getAllNodeIds();
    if (nodeIds.empty()) return frames;

    std::string startNode = nodeIds[0];
    std::queue<std::string> q;
    std::set<std::string> visited;
    std::vector<std::string> traversalOrder;

    // Initial frame
    createFrame("START", { startNode }, { {startNode, "#4CAF50"} },
        "BFS Traversal starting from node " + startNode);

    q.push(startNode);
    visited.insert(startNode);
    traversalOrder.push_back(startNode);

    // Process queue
    while (!q.empty()) {
        std::string current = q.front();
        q.pop();

        // Frame: Processing current node
        std::map<std::string, std::string> colors;
        colors[current] = "#FF9800"; // Orange for current
        for (const auto& v : visited) {
            if (v != current) {
                colors[v] = "#9E9E9E"; // Gray for visited
            }
        }

        createFrame("PROCESS", { current }, colors,
            "Processing node: " + current);

        // Get neighbors
        auto neighbors = graph->getNeighbors(current);

        for (const auto& neighbor : neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
                traversalOrder.push_back(neighbor);

                // Frame: Discovered new node
                colors[neighbor] = "#2196F3"; // Blue for discovered
                std::vector<std::string> highlighted = { current, neighbor };

                AnimationFrame frame;
                frame.frameNumber = frames.size();
                frame.operationType = "DISCOVER";
                frame.highlightedNodes = highlighted;
                frame.highlightedEdges.push_back({ current, neighbor });
                frame.nodeColors = colors;
                frame.annotations.push_back("Discovered node: " + neighbor + " from " + current);
                frame.duration = 800;
                frame.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
                ).count();
                frames.push_back(frame);
            }
        }

        // Mark current as completely visited
        colors[current] = "#9E9E9E"; // Gray
        createFrame("VISITED", { current }, colors,
            "Finished processing node: " + current);
    }

    // Final frame with complete traversal order
    std::map<std::string, std::string> finalColors;
    for (const auto& node : visited) {
        finalColors[node] = "#4CAF50"; // Green for all visited
    }

    std::ostringstream oss;
    oss << "BFS Traversal Complete!\n\nOrder: ";
    for (size_t i = 0; i < traversalOrder.size(); ++i) {
        if (i > 0) oss << " → ";
        oss << traversalOrder[i];
    }

    createFrame("COMPLETE", traversalOrder, finalColors, oss.str());

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
    std::ostringstream& oss) {
    visited.insert(node);
    oss << node << " ";

    // Frame: Visiting node
    std::map<std::string, std::string> colors;
    colors[node] = "#FF9800"; // Orange for current
    for (const auto& v : visited) {
        if (v != node) {
            colors[v] = "#9E9E9E"; // Gray for visited
        }
    }

    createFrame("VISIT", { node }, colors,
        "Visiting node: " + node);

    auto neighbors = graph->getNeighbors(node);
    for (const auto& neighbor : neighbors) {
        if (visited.find(neighbor) == visited.end()) {
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
            dfsVisit(neighbor, visited, graph, oss);
        }
    }

    // Frame: Backtracking
    colors[node] = "#9E9E9E"; // Gray
    createFrame("BACKTRACK", { node }, colors,
        "Backtracking from node: " + node);
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

    std::string startNode = nodeIds[0];
    std::set<std::string> visited;
    std::ostringstream oss;

    // Initial frame
    createFrame("START", { startNode }, { {startNode, "#4CAF50"} },
        "DFS Traversal starting from node " + startNode);

    oss << "DFS Order: ";
    dfsVisit(startNode, visited, graph, oss);

    // Final frame
    std::map<std::string, std::string> finalColors;
    for (const auto& node : visited) {
        finalColors[node] = "#4CAF50"; // Green for all visited
    }

    std::vector<std::string> allVisited(visited.begin(), visited.end());
    createFrame("COMPLETE", allVisited, finalColors,
        "DFS Traversal Complete!\n\n" + oss.str());

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

    std::string startNode = nodeIds[0];
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
    createFrame("START", { startNode }, { {startNode, "#4CAF50"} },
        "Dijkstra's Algorithm starting from node " + startNode + "\nInitial distance: 0");

    // Priority queue: (distance, node)
    using PII = std::pair<double, std::string>;
    std::priority_queue<PII, std::vector<PII>, std::greater<PII>> pq;
    pq.push({ 0, startNode });

    while (!pq.empty()) {
        double d = pq.top().first;
        std::string u = pq.top().second;
        pq.pop();

        if (visited.find(u) != visited.end()) continue;
        visited.insert(u);

        // Frame: Processing node
        std::map<std::string, std::string> colors;
        colors[u] = "#FF9800"; // Orange for current
        for (const auto& v : visited) {
            if (v != u) colors[v] = "#9E9E9E"; // Gray for visited
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

    // Final frame with all distances
    std::map<std::string, std::string> finalColors;
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
                    summary << " (via " << parent[nodeId] << ")";
                }
            }
            summary << "\n";
        }
    }

    std::vector<std::string> allVisited(visited.begin(), visited.end());
    createFrame("COMPLETE", allVisited, finalColors, summary.str());

    qDebug() << "Dijkstra generated" << frames.size() << "animation frames";
    return frames;
}