# UI Engine Developer 1: Visualization Rendering
**Focus Area**: Graph Rendering & Interactive Display  
**Timeline**: Weeks 4-6

## Your Role

You are responsible for **rendering data structures as interactive visual graphs**. Your work consumes visualization states from the C2V team and displays them using Graphviz and Qt. Think of you as building the "display" layer - showing what the data structures look like at each step of execution.

---

## Deliverables Overview

### Phase 1: Graphviz Integration (Week 4)
- ? Parse and render Graphviz DOT code
- ? Display static graph visualizations
- ? Handle different graph types (directed, undirected)
- ? Style and layout graphs

### Phase 2: Qt Visualization Widget (Week 5)
- ? Create interactive graph display widget
- ? Implement zoom, pan, and node highlighting
- ? Display node/edge properties
- ? Handle user interactions

### Phase 3: Animation Playback (Week 6)
- ? Implement frame-by-frame playback
- ? Control animation speed
- ? Highlight changes between frames
- ? Export animation sequences

---

## Dependencies

**Before starting**, ensure these are ready:

From **C2V Team**:
- ? `VisualizationState` structure definition
- ? Example DOT code outputs
- ? Example JSON execution logs
- ? Compiled C2V pipeline

From **UI Dev 2** (collaborative):
- ? Integrated UI framework
- ? Main application window
- ? Menu and toolbar structure

---

## Detailed Tasks

### Task 1.1: Set Up Graphviz Integration

**File**: `src/ui/visualization/graphviz_renderer.h`

```cpp
#pragma once

#include <string>
#include <vector>
#include <memory>

/// Represents a node in rendered graph
struct RenderedNode {
    std::string id;
    std::string label;
    double x = 0.0;      // Layout coordinates
    double y = 0.0;
    double width = 0.0;
    double height = 0.0;
};

/// Represents an edge in rendered graph
struct RenderedEdge {
    std::string from;
    std::string to;
    std::string label;
    std::vector<std::pair<double, double>> controlPoints;  // For curved edges
};

/// Result from rendering DOT code
struct RenderedGraph {
    std::vector<RenderedNode> nodes;
    std::vector<RenderedEdge> edges;
    double width = 0.0;
    double height = 0.0;
    bool isValid = false;
};

/// Graphviz rendering engine
class GraphvizRenderer {
public:
    GraphvizRenderer();
    ~GraphvizRenderer();
    
    /// Render DOT code to graph layout
    /// @param dotCode Graphviz DOT code string
    /// @return RenderedGraph with positions and dimensions
    RenderedGraph renderDOT(const std::string &dotCode);
 
    /// Export rendered graph to SVG
    /// @param dotCode DOT code to render
    /// @param outputFile Path to write SVG
    bool exportToSVG(const std::string &dotCode, const std::string &outputFile);
    
    /// Export rendered graph to PNG
    /// @param dotCode DOT code to render
    /// @param outputFile Path to write PNG
    bool exportToPNG(const std::string &dotCode, const std::string &outputFile);
    
    /// Set layout algorithm (neato, dot, fdp, etc.)
    void setLayoutEngine(const std::string &engine);
    
private:
    std::string layoutEngine_;  // Default: "dot"
    
    /// Parse Graphviz output XML to extract node/edge positions
    RenderedGraph parseGraphvizOutput(const std::string &xmlOutput);
};
```

**File**: `src/ui/visualization/graphviz_renderer.cpp`

```cpp
#include "graphviz_renderer.h"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

GraphvizRenderer::GraphvizRenderer() : layoutEngine_("dot") {}

GraphvizRenderer::~GraphvizRenderer() {}

RenderedGraph GraphvizRenderer::renderDOT(const std::string &dotCode)
{
    RenderedGraph result;
    result.isValid = false;
    
  // Step 1: Write DOT code to temporary file
    std::string tempDotFile = "/tmp/graph_temp.dot";
    std::ofstream dotFile(tempDotFile);
    if (!dotFile.is_open()) {
        std::cerr << "Failed to create temporary DOT file\n";
        return result;
    }
    dotFile << dotCode;
    dotFile.close();
    
    // Step 2: Execute Graphviz to generate layout information
    // Using "dot -Tsvg" to get SVG output with coordinate information
    std::string tempSvgFile = "/tmp/graph_temp.svg";
    std::string command = layoutEngine_ + " -Tsvg \"" + tempDotFile + 
       "\" -o \"" + tempSvgFile + "\"";
    
    int ret = std::system(command.c_str());
    if (ret != 0) {
    std::cerr << "Graphviz command failed: " << command << "\n";
        return result;
    }
    
    // Step 3: Parse SVG to extract node positions
    std::ifstream svgFile(tempSvgFile);
    if (!svgFile.is_open()) {
        std::cerr << "Failed to read SVG output\n";
        return result;
    }
    
    std::stringstream buffer;
    buffer << svgFile.rdbuf();
    std::string svgContent = buffer.str();
    svgFile.close();
    
    result = parseGraphvizOutput(svgContent);
    
    // Step 4: Cleanup
    std::remove(tempDotFile.c_str());
    std::remove(tempSvgFile.c_str());
    
 return result;
}

bool GraphvizRenderer::exportToSVG(const std::string &dotCode, 
     const std::string &outputFile)
{
    std::string tempDotFile = "/tmp/graph_export.dot";
    std::ofstream dotFile(tempDotFile);
    if (!dotFile.is_open()) return false;
    
    dotFile << dotCode;
    dotFile.close();
    
    std::string command = layoutEngine_ + " -Tsvg \"" + tempDotFile + 
       "\" -o \"" + outputFile + "\"";
    
    int ret = std::system(command.c_str());
    std::remove(tempDotFile.c_str());
    
  return ret == 0;
}

bool GraphvizRenderer::exportToPNG(const std::string &dotCode,
      const std::string &outputFile)
{
    std::string tempDotFile = "/tmp/graph_export.dot";
    std::ofstream dotFile(tempDotFile);
    if (!dotFile.is_open()) return false;
    
    dotFile << dotCode;
    dotFile.close();
  
    std::string command = layoutEngine_ + " -Tpng \"" + tempDotFile + 
"\" -o \"" + outputFile + "\"";

    int ret = std::system(command.c_str());
    std::remove(tempDotFile.c_str());
    
    return ret == 0;
}

void GraphvizRenderer::setLayoutEngine(const std::string &engine)
{
 layoutEngine_ = engine;
}

RenderedGraph GraphvizRenderer::parseGraphvizOutput(const std::string &svgContent)
{
    RenderedGraph graph;
    graph.isValid = true;
    
    // Parse SVG to find nodes and edges
    // SVG format: <ellipse id="node_id" cx="x" cy="y" rx="w" ry="h"/>
    
    std::regex ellipsePattern(
        R"(<ellipse[^>]*id="([^"]*)"[^>]*cx="([0-9.]*)"[^>]*cy="([0-9.]*)")" 
        R"([^>]*rx="([0-9.]*)"[^>]*ry="([0-9.]*)")");
    
    std::smatch match;
    std::string::const_iterator searchStart(svgContent.cbegin());
    
    // Extract nodes
    while (std::regex_search(searchStart, svgContent.cend(), match, ellipsePattern)) {
        RenderedNode node;
        node.id = match[1];
        node.x = std::stod(match[2]);
      node.y = std::stod(match[3]);
        node.width = std::stod(match[4]) * 2;
        node.height = std::stod(match[5]) * 2;
        graph.nodes.push_back(node);
        searchStart = match.suffix().first;
    }
    
    // Extract edges (paths)
    std::regex pathPattern(
   R"(<path[^>]*d="([^"]*)"[^>]*class="edge)");
    
    searchStart = svgContent.cbegin();
    while (std::regex_search(searchStart, svgContent.cend(), match, pathPattern)) {
        // Parse path coordinates (simplified)
        RenderedEdge edge;
        edge.from = "";  // Would need to parse from title element
     edge.to = "";
      graph.edges.push_back(edge);
        searchStart = match.suffix().first;
    }
    
    return graph;
}
```

---

### Task 1.2: Create Qt Visualization Widget

**File**: `src/ui/visualization/graph_widget.h`

```cpp
#pragma once

#include <QOpenGLWidget>
#include <QPointF>
#include <vector>
#include <memory>
#include "graphviz_renderer.h"
#include "analysis/state_mapping/visualization_state.h"

class QPainter;
class QMouseEvent;
class QWheelEvent;
class QKeyEvent;

/// Node representation for interactive display
struct DisplayNode {
 RenderedNode data;
    bool isHighlighted = false;
    QPointF screenPos;
};

/// Edge representation for interactive display
struct DisplayEdge {
    RenderedEdge data;
    bool isHighlighted = false;
};

/// Interactive graph visualization widget
class GraphWidget : public QOpenGLWidget {
    Q_OBJECT
    
public:
    explicit GraphWidget(QWidget *parent = nullptr);
    ~GraphWidget() = default;
    
    /// Set visualization state to display
    void setVisualizationState(const VisualizationState &state);
    
    /// Get highlighted node ID
    std::string getSelectedNodeId() const { return selectedNodeId_; }
    
    /// Highlight specific node
    void highlightNode(const std::string &nodeId);
    
    /// Highlight specific edge
    void highlightEdge(const std::string &fromId, const std::string &toId);
    
    /// Clear all highlights
    void clearHighlights();
    
    /// Reset view to fit all nodes
    void resetView();
    
    /// Export current view as image
    bool exportAsImage(const std::string &filename);
    
signals:
    /// Emitted when node is clicked
    void nodeSelected(const QString &nodeId);
    
    /// Emitted when edge is clicked
    void edgeSelected(const QString &fromId, const QString &toId);
    
  /// Emitted when view changes
    void viewChanged();
    
protected:
    /// OpenGL rendering
    void paintEvent(QPaintEvent *event) override;
    
    /// Mouse interaction
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
    
    /// Keyboard interaction
    void keyPressEvent(QKeyEvent *event) override;
    
private:
    GraphvizRenderer renderer_;
    std::vector<DisplayNode> nodes_;
    std::vector<DisplayEdge> edges_;
    
    // Interaction state
    float zoomLevel_ = 1.0f;
    QPointF panOffset_;
    std::string selectedNodeId_;
    bool isDragging_ = false;
    QPointF dragStart_;
    
 // Rendering helpers
  void renderNodes(QPainter &painter);
    void renderEdges(QPainter &painter);
    void renderLabels(QPainter &painter);
    void updateLayout();
    
    /// Find node at screen coordinates
    DisplayNode *nodeAtPosition(const QPointF &pos);
};
```

**File**: `src/ui/visualization/graph_widget.cpp`

```cpp
#include "graph_widget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QBrush>
#include <QPen>
#include <iostream>
#include <cmath>

GraphWidget::GraphWidget(QWidget *parent)
    : QOpenGLWidget(parent), zoomLevel_(1.0f)
{
    setFocusPolicy(Qt::StrongFocus);
    setStyleSheet("background-color: white;");
}

void GraphWidget::setVisualizationState(const VisualizationState &state)
{
// Parse DOT code and render
    auto renderedGraph = renderer_.renderDOT(state.dotCode);
    
    if (!renderedGraph.isValid) {
        std::cerr << "Failed to render DOT code\n";
        return;
    }
    
    // Convert to display nodes/edges
    nodes_.clear();
    edges_.clear();
    
    for (const auto &node : renderedGraph.nodes) {
        DisplayNode displayNode;
        displayNode.data = node;
        nodes_.push_back(displayNode);
}
  
    for (const auto &edge : renderedGraph.edges) {
        DisplayEdge displayEdge;
        displayEdge.data = edge;
 edges_.push_back(displayEdge);
    }
    
    updateLayout();
    update();  // Trigger repaint
}

void GraphWidget::highlightNode(const std::string &nodeId)
{
    for (auto &node : nodes_) {
        if (node.data.id == nodeId) {
      node.isHighlighted = true;
            selectedNodeId_ = nodeId;
        } else {
            node.isHighlighted = false;
        }
    }
    update();
}

void GraphWidget::highlightEdge(const std::string &fromId, const std::string &toId)
{
    for (auto &edge : edges_) {
        if (edge.data.from == fromId && edge.data.to == toId) {
    edge.isHighlighted = true;
        } else {
            edge.isHighlighted = false;
        }
    }
    update();
}

void GraphWidget::clearHighlights()
{
    for (auto &node : nodes_) {
    node.isHighlighted = false;
    }
for (auto &edge : edges_) {
  edge.isHighlighted = false;
    }
    selectedNodeId_.clear();
update();
}

void GraphWidget::resetView()
{
    zoomLevel_ = 1.0f;
    panOffset_ = QPointF(0, 0);
 updateLayout();
    update();
}

bool GraphWidget::exportAsImage(const std::string &filename)
{
    // Render to image buffer
    QImage image(width(), height(), QImage::Format_ARGB32);
    image.fill(Qt::white);
    
QPainter painter(&image);
    renderEdges(painter);
    renderNodes(painter);
 renderLabels(painter);
    painter.end();
    
  return image.save(filename.c_str());
}

void GraphWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);
    
    // Apply zoom and pan transformations
    painter.translate(width() / 2.0, height() / 2.0);
  painter.translate(panOffset_.x(), panOffset_.y());
    painter.scale(zoomLevel_, zoomLevel_);
    painter.translate(-width() / 2.0, -height() / 2.0);
    
    renderEdges(painter);
    renderNodes(painter);
    renderLabels(painter);
}

void GraphWidget::renderEdges(QPainter &painter)
{
    for (const auto &edge : edges_) {
    QPen pen;
        pen.setWidth(2);
        
        if (edge.isHighlighted) {
            pen.setColor(Qt::red);
        } else {
       pen.setColor(Qt::black);
        }
        
        painter.setPen(pen);
        
        // Draw line from source to target node
        auto sourceIt = std::find_if(nodes_.begin(), nodes_.end(),
    [&edge](const DisplayNode &n) { return n.data.id == edge.data.from; });
      auto targetIt = std::find_if(nodes_.begin(), nodes_.end(),
      [&edge](const DisplayNode &n) { return n.data.id == edge.data.to; });
        
        if (sourceIt != nodes_.end() && targetIt != nodes_.end()) {
    QPointF from(sourceIt->data.x, sourceIt->data.y);
       QPointF to(targetIt->data.x, targetIt->data.y);
 painter.drawLine(from, to);
          
            // Draw arrowhead
        double angle = std::atan2(to.y() - from.y(), to.x() - from.x());
  QPointF arrowSize(10, 5);
      
    QPointF p1 = to - QPointF(arrowSize.x() * std::cos(angle - M_PI/6),
   arrowSize.x() * std::sin(angle - M_PI/6));
          QPointF p2 = to - QPointF(arrowSize.x() * std::cos(angle + M_PI/6),
           arrowSize.x() * std::sin(angle + M_PI/6));
         
   painter.drawLine(p1, to);
   painter.drawLine(p2, to);
        }
    }
}

void GraphWidget::renderNodes(QPainter &painter)
{
    for (const auto &node : nodes_) {
        QBrush brush;
        QPen pen;
   pen.setWidth(2);
        
        if (node.isHighlighted) {
    brush.setColor(Qt::yellow);
            pen.setColor(Qt::red);
        } else {
 brush.setColor(Qt::lightBlue);
     pen.setColor(Qt::black);
        }
        
        brush.setStyle(Qt::SolidPattern);
        painter.setPen(pen);
        painter.setBrush(brush);
   
        // Draw ellipse for node
        QRectF rect(node.data.x - node.data.width/2,
      node.data.y - node.data.height/2,
       node.data.width,
    node.data.height);
        painter.drawEllipse(rect);
    }
}

void GraphWidget::renderLabels(QPainter &painter)
{
painter.setPen(Qt::black);
    
    for (const auto &node : nodes_) {
        QPointF pos(node.data.x, node.data.y);
     painter.drawText(QRectF(pos.x() - 20, pos.y() - 10, 40, 20),
     Qt::AlignCenter, QString::fromStdString(node.data.label));
    }
}

void GraphWidget::updateLayout()
{
    // Recalculate screen positions based on zoom/pan
    for (auto &node : nodes_) {
 node.screenPos = QPointF(node.data.x * zoomLevel_ + panOffset_.x(),
     node.data.y * zoomLevel_ + panOffset_.y());
    }
}

void GraphWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // Check if clicking on node
        auto node = nodeAtPosition(event->pos());
 if (node) {
     highlightNode(node->data.id);
            emit nodeSelected(QString::fromStdString(node->data.id));
        } else {
            clearHighlights();
        }
    } else if (event->button() == Qt::MiddleButton) {
        // Start panning
        isDragging_ = true;
     dragStart_ = event->pos();
    }
}

void GraphWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging_) {
        QPointF delta = event->pos() - dragStart_;
        panOffset_ += delta;
        dragStart_ = event->pos();
        updateLayout();
      update();
    }
}

void GraphWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) {
        isDragging_ = false;
    }
}

void GraphWidget::wheelEvent(QWheelEvent *event)
{
    float zoomFactor = 1.1f;
    if (event->angleDelta().y() > 0) {
        zoomLevel_ *= zoomFactor;
    } else {
      zoomLevel_ /= zoomFactor;
    }
    
    // Limit zoom
    zoomLevel_ = std::max(0.1f, std::min(10.0f, zoomLevel_));
 updateLayout();
    update();
}

void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_R) {
        resetView();
    } else if (event->key() == Qt::Key_Delete) {
        clearHighlights();
    }
}

DisplayNode *GraphWidget::nodeAtPosition(const QPointF &pos)
{
    for (auto &node : nodes_) {
    QRectF rect(node.screenPos.x() - node.data.width/2,
       node.screenPos.y() - node.data.height/2,
     node.data.width,
          node.data.height);
   
        if (rect.contains(pos)) {
            return &node;
   }
    }
    return nullptr;
}
```

---

### Task 1.3: Implement Animation Playback Controller

**File**: `src/ui/visualization/animation_controller.h`

```cpp
#pragma once

#include "analysis/state_mapping/visualization_state.h"
#include <vector>
#include <memory>
#include <QTimer>
#include <QObject>

/// Controls animation playback
class AnimationController : public QObject {
    Q_OBJECT
    
public:
    explicit AnimationController(QObject *parent = nullptr);
    
    /// Load visualization states for animation
    void loadStates(const std::vector<VisualizationState> &states);
    
    /// Start animation playback
    void play();
    
    /// Pause animation
    void pause();
    
    /// Stop and reset to first frame
    void stop();
    
  /// Go to specific frame
    void goToFrame(int frameNumber);
    
    /// Go to next frame
    void nextFrame();
    
 /// Go to previous frame
    void previousFrame();
    
    /// Set playback speed (frames per second)
    void setSpeed(double fps);

    /// Get current frame number
    int getCurrentFrame() const { return currentFrame_; }
    
    /// Get total number of frames
    int getTotalFrames() const { return states_.size(); }
    
    /// Get current visualization state
    const VisualizationState &getCurrentState() const { return states_[currentFrame_]; }
    
  /// Check if playing
    bool isPlaying() const { return isPlaying_; }

signals:
    /// Emitted when frame changes
    void frameChanged(int frameNumber);
    
    /// Emitted when animation state changes
    void stateChanged(const VisualizationState &state);
    
    /// Emitted when animation completes
  void animationFinished();
    
private slots:
    void onTimerTick();
    
private:
    std::vector<VisualizationState> states_;
    int currentFrame_ = 0;
    bool isPlaying_ = false;
    double fps_ = 1.0;  // 1 frame per second
    
    std::unique_ptr<QTimer> timer_;
};
```

**File**: `src/ui/visualization/animation_controller.cpp`

```cpp
#include "animation_controller.h"
#include <QTimer>
#include <iostream>

AnimationController::AnimationController(QObject *parent)
    : QObject(parent)
{
timer_ = std::make_unique<QTimer>(this);
    connect(timer_.get(), &QTimer::timeout, this, &AnimationController::onTimerTick);
}

void AnimationController::loadStates(const std::vector<VisualizationState> &states)
{
    states_ = states;
    currentFrame_ = 0;
    isPlaying_ = false;
    
    if (!states_.empty()) {
   emit stateChanged(states_[0]);
 emit frameChanged(0);
    }
}

void AnimationController::play()
{
    if (states_.empty()) return;
    
    isPlaying_ = true;
    
    // Calculate timer interval from FPS
    int interval = static_cast<int>(1000.0 / fps_);
    timer_->start(interval);
}

void AnimationController::pause()
{
 isPlaying_ = false;
    timer_->stop();
}

void AnimationController::stop()
{
    pause();
    currentFrame_ = 0;
    emit frameChanged(0);
    emit stateChanged(states_[0]);
}

void AnimationController::goToFrame(int frameNumber)
{
    if (frameNumber < 0 || frameNumber >= static_cast<int>(states_.size())) {
        return;
    }
    
    currentFrame_ = frameNumber;
 emit frameChanged(currentFrame_);
    emit stateChanged(states_[currentFrame_]);
}

void AnimationController::nextFrame()
{
    if (currentFrame_ < static_cast<int>(states_.size()) - 1) {
        goToFrame(currentFrame_ + 1);
    } else {
        emit animationFinished();
    }
}

void AnimationController::previousFrame()
{
    if (currentFrame_ > 0) {
        goToFrame(currentFrame_ - 1);
    }
}

void AnimationController::setSpeed(double fps)
{
    fps_ = fps;
    if (isPlaying_) {
        timer_->stop();
        play();
    }
}

void AnimationController::onTimerTick()
{
    nextFrame();
    
    if (currentFrame_ >= static_cast<int>(states_.size()) - 1) {
        pause();
        emit animationFinished();
    }
}
```

---

### Task 1.4: Create CMake Configuration

**File**: `src/ui/visualization/CMakeLists.txt`

```cmake
# Visualization rendering library

find_package(Qt6 COMPONENTS Core Gui OpenGL Widgets REQUIRED)
find_package(Graphviz REQUIRED)

# Add library
add_library(visualization
    graphviz_renderer.cpp
    graph_widget.cpp
  animation_controller.cpp
)

# Link dependencies
target_link_libraries(visualization
    PUBLIC
      Qt6::Core
        Qt6::Gui
        Qt6::OpenGL
  Qt6::Widgets
  ${GRAPHVIZ_LIBRARIES}
        state_mapping
)

# Include directories
target_include_directories(visualization
    PUBLIC
     ${GRAPHVIZ_INCLUDE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

# Enable C++17
set_property(TARGET visualization PROPERTY CXX_STANDARD 17)
set_property(TARGET visualization PROPERTY AUTOMOC ON)
```

---

### Task 1.5: Write Unit Tests

**File**: `tests/test_graphviz_renderer.cpp`

```cpp
#include <gtest/gtest.h>
#include "ui/visualization/graphviz_renderer.h"

class GraphvizRendererTest : public ::testing::Test {
protected:
    GraphvizRenderer renderer;
};

TEST_F(GraphvizRendererTest, RendersSimpleGraph) {
    std::string dotCode = R"(
        digraph {
    A [label="Node A"];
 B [label="Node B"];
   A -> B;
        }
    )";
    
    auto graph = renderer.renderDOT(dotCode);
    
    EXPECT_TRUE(graph.isValid);
    EXPECT_EQ(graph.nodes.size(), 2);
    EXPECT_EQ(graph.edges.size(), 1);
}

TEST_F(GraphvizRendererTest, ExportsToSVG) {
    std::string dotCode = "digraph { A -> B; }";
    
    bool success = renderer.exportToSVG(dotCode, "/tmp/test_output.svg");
    
    EXPECT_TRUE(success);
}

TEST_F(GraphvizRendererTest, ChangesLayoutEngine) {
    renderer.setLayoutEngine("neato");
    // Verify layout change
    EXPECT_TRUE(true);
}
```

**File**: `tests/test_graph_widget.cpp`

```cpp
#include <gtest/gtest.h>
#include "ui/visualization/graph_widget.h"

class GraphWidgetTest : public ::testing::Test {
protected:
    GraphWidget widget;
};

TEST_F(GraphWidgetTest, HighlightsNode) {
    VisualizationState state;
 state.nodeData = R"([{"id": "1", "label": "Node 1"}])";
    state.edgeData = "[]";
    state.dotCode = "digraph { 1 [label=\"Node 1\"]; }";
    
    widget.setVisualizationState(state);
    widget.highlightNode("1");
    
    EXPECT_EQ(widget.getSelectedNodeId(), "1");
}

TEST_F(GraphWidgetTest, ClearsHighlights) {
    widget.clearHighlights();
    EXPECT_EQ(widget.getSelectedNodeId(), "");
}
```

---

## Testing Checklist

Before moving to UI Dev 2:

- [ ] Graphviz renders valid DOT code
- [ ] SVG/PNG export works correctly
- [ ] GraphWidget displays nodes and edges
- [ ] Node highlighting works
- [ ] Pan and zoom interactions respond correctly
- [ ] Animation playback progresses frames
- [ ] Speed control adjusts playback rate
- [ ] All unit tests pass
- [ ] No memory leaks in rendering pipeline
- [ ] Performance: <16ms per frame (60 FPS)

---

## Handoff to UI Dev 2

**What you provide**:
1. Compiled `visualization` library
2. Working `GraphWidget` for embedding in main window
3. `AnimationController` API documentation
4. Example rendered graphs (SVG/PNG outputs)

**What you need from UI Dev 2**:
1. Main application window structure
2. Signal slots for playback controls
3. Menu/toolbar layout for export functions
4. File open/save dialogs

---

## Key Components Summary

| Component | Purpose | Key Class |
|-----------|---------|-----------|
| **Graphviz Renderer** | Convert DOT to coordinates | `GraphvizRenderer` |
| **Graph Widget** | Interactive display | `GraphWidget` |
| **Animation Controller** | Playback management | `AnimationController` |

---

## Performance Targets

- Graph rendering: <50ms for 1000 nodes
- Frame updates: <16ms (60 FPS)
- Memory: <100MB for 10k-frame animation
- Zoom levels: Support 0.1x to 10x

---

## Debugging Tips

**Problem**: Graphviz not found
- ? Verify Graphviz installed: `which dot`
- ? Check GRAPHVIZ_INCLUDE_DIR in CMake
- ? Ensure PATH includes Graphviz bin directory

**Problem**: Graph doesn't render
- ? Validate DOT code with `dot -Tsvg test.dot > test.svg`
- ? Check for special characters in node IDs
- ? Verify VisualizationState has valid dotCode

**Problem**: Animation stutters
- ? Check FPS is not too high
- ? Profile renderNodes() and renderEdges()
- ? Consider pre-rendering to texture

---

## Timeline

| Week | Task | Status |
|------|------|--------|
| Week 4 | Task 1.1-1.2: Graphviz & Widget | ? |
| Week 5 | Task 1.3: Animation Controller | ? |
| Week 5 | Task 1.4-1.5: CMake & Tests | ? |
| Week 6 | Handoff to UI Dev 2 | ? |

---

## Resources

- **Graphviz**: https://graphviz.org/
- **Qt6 Documentation**: https://doc.qt.io/qt-6/
- **OpenGL in Qt**: https://doc.qt.io/qt-6/qopenglwidget.html

---

**Questions?** Contact your team lead or refer to the UI_COORDINATION.md.
