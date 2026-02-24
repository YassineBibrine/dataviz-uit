#pragma once
#include <string>
#include <map>
#include <vector>
#include <utility>

/**
 * @struct AnimationFrame
 * @brief Represents a single visual state in the animation timeline.
 *
 * Algorithms generate a sequence of AnimationFrame objects that describe
 * node highlights, colors, positions and annotations. The renderer consumes
 * these frames to display animated transitions.
 */
struct AnimationFrame {
 int frameNumber;
 std::string operationType;

 // List of node IDs that should be highlighted for this frame.
 std::vector<std::string> highlightedNodes;
 // Edges highlighted as pairs (from, to).
 std::vector<std::pair<std::string, std::string>> highlightedEdges;

 // Explicit per-node positions for precise rendering: nodeId -> (x, y)
 std::map<std::string, std::pair<double, double>> nodePositions;
 // Per-node color mapping used by the renderer: nodeId -> color string
 std::map<std::string, std::string> nodeColors;
 // Per-edge color mapping: key is typically "from-to" or similar
 std::map<std::string, std::string> edgeColors;

 // Additional visualization hints
 std::map<std::string, std::string> nodeShapes; // nodeId -> shape identifier
 std::vector<std::pair<std::string, std::string>> edges; // explicit edge list

 // Optional labels for nodes/edges to show values or annotations
 std::map<std::string, std::string> nodeLabels;
 std::map<std::string, std::string> edgeLabels;

 // Free-form annotations (text messages shown to the user)
 std::vector<std::string> annotations;

 // Optional DOT code to supply a layout or fallback visualization
 std::string dotCode;

 // Frame playback controls
 int duration; // milliseconds this frame should be shown
 long long timestamp; // creation timestamp (ms since epoch)

 // Default constructor (brief signature in header)
 AnimationFrame();

 // Helper functions used by algorithms to populate frames.
 void addHighlightedNode(const std::string& nodeId, const std::string& color);
 void addHighlightedEdge(const std::string& from, const std::string& to, const std::string& color);
 void setNodePosition(const std::string& nodeId, double x, double y);
 void addAnnotation(const std::string& text);
 void setDuration(int ms);
 void generateNodes(int count);

};
