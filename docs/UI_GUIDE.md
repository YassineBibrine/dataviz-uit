# UI Team Implementation Guide

## Overview

The UI Team is responsible for the **Frontend Engine**, which provides the graphical user interface and visualization rendering for the Data Structure Visualization Application. Your work is critical for user experience and interaction.

## Team Responsibilities

### Primary Responsibilities
1. **Code Editor** - Full-featured C++ editor with syntax highlighting
2. **Visualization Canvas** - Render graph/tree visualizations
3. **Playback Controls** - Play, pause, step, speed control
4. **Diagnostic Output** - Display logs and performance metrics
5. **Memory Viewer** - Display low-level memory state
6. **Main Window** - Integrate all components

### File Locations
All UI files are located in: `src/frontend/ui/`

```
src/frontend/
??? CMakeLists.txt
??? ui/
    ??? main_window.h/.cpp      ? Start here
    ??? code_editor.h/.cpp    ? Code editing interface
 ??? visualization_pane.h/.cpp    ? Your main visualization area
    ??? control_panel.h/.cpp    ? Playback controls
    ??? diagnostic_output.h/.cpp     ? Logs display
    ??? memory_viewer.h/.cpp         ? Memory visualization
```

## Component Details

### 1. MainWindow (Application Shell)
**File**: `src/frontend/ui/main_window.h/.cpp`

**Responsibilities**:
- Container for all UI components
- Layout management (horizontal/vertical splitters)
- Menu bar and toolbar
- Signal/slot connections between components
- Window state management

**Key Methods to Implement**:
```cpp
void setupUI();   // Create layout, instantiate components
void connectSignals();   // Connect signals between components
void createMenuBar();    // Add menu items and actions
```

**Integration Points**:
```cpp
MainWindow
??? CodeEditor (left panel)
??? VisualizationPane (center-top)
??? ControlPanel (center-bottom)
??? DiagnosticOutput (right panel)
```

### 2. CodeEditor
**File**: `src/frontend/ui/code_editor.h/.cpp`

**Responsibilities**:
- C++ syntax highlighting
- Line numbering
- Code folding (optional)
- File I/O (open, save)
- Current line highlighting

**Key Features to Implement**:
```cpp
void loadFile(const QString &filePath);        // Load .cpp file
void saveFile(const QString &filePath);      // Save .cpp file
QString getCode() const;      // Get current code text
void lineNumberAreaPaintEvent();          // Render line numbers
void setupSyntaxHighlighting();       // Apply C++ syntax coloring
```

**Qt Features to Use**:
- `QPlainTextEdit` - Base text editor
- `QSyntaxHighlighter` - For syntax highlighting
- `QPainter` - For custom rendering (line numbers)

**Signals to Emit**:
```cpp
void codeChanged(const QString &newCode);
void fileSaved(const QString &filePath);
```

### 3. VisualizationPane
**File**: `src/frontend/ui/visualization_pane.h/.cpp`

**Responsibilities**:
- Render graph/tree visualizations
- Handle zoom and pan interactions
- Display animation frames
- Support toggle between high-level and memory views

**Key Features to Implement**:
```cpp
void renderVisualization(const QString &dotCode);  // Render from DOT
void clearVisualization();   // Clear canvas
void zoomIn();      // Zoom x1.1
void zoomOut();      // Zoom x0.9
void resetZoom(); // Reset to 1.0
void toggleMemoryView(bool enable);       // Switch views
```

**Qt Features to Use**:
- `QWidget` - Base widget
- `QPainter` - Drawing graphics
- `QPaintEvent` - Handle redraws
- `QWheelEvent` - Zoom interaction
- `QMouseEvent` - Pan/interaction

**Signals to Emit**:
```cpp
void nodeClicked(const QString &nodeId);
void nodeDoubleClicked(const QString &nodeId);
void edgeClicked(const QString &fromNodeId, const QString &toNodeId);
void visualizationUpdated();
```

**Integration with Visualization Engine**:
- Receive DOT code from Analysis/Synthesis engines
- Convert to rendering instructions via Visualization Engine
- Display nodes and edges with proper styling

### 4. ControlPanel
**File**: `src/frontend/ui/control_panel.h/.cpp`

**Responsibilities**:
- Playback controls (play, pause, step)
- Speed adjustment
- Algorithm selection
- Data size configuration

**Key Features to Implement**:
```cpp
// Already partially implemented - complete the implementation
void setupUI();        // Create buttons and sliders
void connectSignals(); // Connect to slots
```

**Signals to Emit** (Already defined):
```cpp
void playClicked();
void pauseClicked();
void stepForwardClicked();
void stepBackwardClicked();
void speedChanged(int speed);     // 1-10 scale
void algorithmSelected(const QString &algorithm);
void dataSizeChanged(int size);
```

**UI Elements**:
- Play/Pause/Step buttons
- Speed slider (1-10)
- Algorithm dropdown
- Data size spinner

### 5. DiagnosticOutput
**File**: `src/frontend/ui/diagnostic_output.h/.cpp`

**Responsibilities**:
- Display compilation errors
- Show runtime logs
- Display performance metrics
- Show memory statistics

**Key Features to Implement**:
```cpp
void appendError(const QString &message);     // ERROR level
void appendWarning(const QString &message);    // WARNING level
void appendInfo(const QString &message);            // INFO level
void appendMetric(const QString &metricName, const QString &value);
void clear();
```

**Qt Features to Use**:
- `QPlainTextEdit` - Read-only text display
- `QTextEdit::ExtraSelection` - Color highlighting
- Timestamps for each message

### 6. MemoryViewer
**File**: `src/frontend/ui/memory_viewer.h/.cpp`

**Responsibilities**:
- Display heap memory layout
- Show pointer addresses
- Highlight memory regions
- Display uninitialized memory

**Key Features to Implement**:
```cpp
void updateMemoryState(const std::map<std::string, std::string> &memoryState);
void highlightAddress(uintptr_t address);
void clearHighlight();
void paintEvent(QPaintEvent *event);  // Custom rendering
```

## Signal/Slot Connection Pattern

### Example from MainWindow
```cpp
// In setupUI(), after creating components:
connect(codeEditor.get(), &CodeEditor::codeChanged,
 controlPanel.get(), [this](const QString &code) {
    // Handle code change
});

connect(controlPanel.get(), &ControlPanel::playClicked,
this, &MainWindow::onPlayClicked);
```

## Build Instructions

### Add New UI Files

If you create new UI files:

1. Add to `src/frontend/CMakeLists.txt`:
```cmake
set(FRONTEND_SOURCES
    ui/main_window.cpp
    ui/main_window.h
    ui/code_editor.cpp
    ui/code_editor.h
# ... add your new files here
)
```

2. Rebuild:
```bash
cd build
cmake ..
cmake --build . --config Release
```

## Best Practices

### 1. Separation of Concerns
- UI code should not contain business logic
- Delegate heavy computation to core/analysis/synthesis engines
- Use signals/slots for loose coupling

### 2. Qt Conventions
- Use Qt containers (QString, QVector, etc.)
- Follow Qt naming conventions (camelCase)
- Use RAII (Resource Acquisition Is Initialization)
- Use smart pointers (`std::unique_ptr`)

### 3. Event Handling
- Override `paintEvent()` for custom drawing
- Override `mousePressEvent()`, `mouseMoveEvent()`, etc. for interaction
- Use `QTimer` for periodic updates

### 4. Performance
- Batch drawing operations
- Use `update()` to schedule repaints, not `repaint()`
- Avoid expensive operations in event handlers
- Cache frequently used data

## Integration Checklist

- [ ] MainWindow integrates all 6 components
- [ ] Code editor loads/saves .cpp files
- [ ] Syntax highlighting working for C++
- [ ] Visualization pane accepts DOT code
- [ ] Zoom/pan works on visualization
- [ ] Control buttons emit correct signals
- [ ] Diagnostic output shows different message types
- [ ] Memory viewer displays memory layout
- [ ] All components use consistent styling
- [ ] Application builds without errors

## Debugging Tips

### Common Issues

**Issue**: Qt signals not working
- Check `Q_OBJECT` macro in header
- Verify signal/slot parameter types match exactly
- Use Qt Creator's debugger to inspect connections

**Issue**: Visualization not rendering
- Check if `paintEvent()` is being called
- Verify coordinate system (top-left origin)
- Use `update()` to schedule repaints

**Issue**: Code editor not showing line numbers
- Ensure line number area is created in constructor
- Check `lineNumberAreaWidth()` calculation
- Verify resize event is connected

## Useful Qt Resources

- Qt Documentation: https://doc.qt.io/
- Qt Design Patterns: https://doc.qt.io/qt-6/signalsandslots.html
- Qt Graphics View: https://doc.qt.io/qt-6/graphicsview.html (for advanced rendering)

## Next Steps

1. **Implement MainWindow**: Create the main application shell
2. **Implement CodeEditor**: Add syntax highlighting
3. **Implement VisualizationPane**: Set up basic rendering
4. **Connect Components**: Wire up signal/slot connections
5. **Test Integration**: Ensure all components work together

## Contact

Questions about UI implementation? Refer to `README.md` or check the team communication channels.
