# UI Team Coordination Guide
**Application Framework & Visualization Integration**

## Overview

The UI Team consists of **2 developers** working in parallel to build the interactive visualization application. This document coordinates their efforts to ensure seamless integration of rendering and framework components.

---

## Team Structure

| Role | Developer | Primary Focus | Timeline |
|------|-----------|---------------|----------|
| **UI Dev 1** | Rendering Dev | Graph Rendering & Animation | Weeks 4-6 |
| **UI Dev 2** | Framework Dev | Main Window & Integration | Weeks 4-6 |

---

## Detailed Responsibilities

### Developer 1: Visualization Rendering

**Weeks 4-6**

#### Core Deliverables
1. **GraphvizRenderer** (`src/ui/visualization/graphviz_renderer.h/.cpp`)
   - Parse DOT code from C2V team
   - Compute node/edge positions
   - Export to SVG/PNG

2. **GraphWidget** (`src/ui/visualization/graph_widget.h/.cpp`)
   - Interactive Qt widget for graph display
   - Zoom, pan, selection interactions
   - Node/edge highlighting

3. **AnimationController** (`src/ui/visualization/animation_controller.h/.cpp`)
   - Frame-by-frame playback
   - Speed control
   - Emit frame change signals

4. **Visualization Library** (`src/ui/visualization/CMakeLists.txt`)
   - Link Graphviz and Qt6
   - Enable meta-object compilation

#### Key Outputs (for Dev 2)
```cpp
class GraphWidget : public QOpenGLWidget { ... };
class AnimationController : public QObject { ... };
```

#### Milestones
- **Week 4**: GraphvizRenderer renders DOT code to positions
- **Week 5**: GraphWidget displays interactive graphs
- **Week 6**: AnimationController plays frame sequences

---

### Developer 2: Application Framework

**Weeks 4-6**

#### Core Deliverables
1. **MainWindow** (`src/ui/main_window.h/.cpp`)
   - Qt main window with menus/toolbars
   - File open/save dialogs
   - Component orchestration

2. **ControlPanel** (`src/ui/control_panel.h/.cpp`)
   - Playback buttons (play, pause, stop)
   - Frame slider
   - Speed control spinbox

3. **PropertiesPanel** (`src/ui/properties_panel.h/.cpp`)
   - Display selected node/edge properties
   - Show execution event details

4. **OutputConsole** (`src/ui/output_console.h/.cpp`)
   - Log messages from pipeline
   - Display errors and warnings

5. **Main Application** (`src/ui/main.cpp`)
   - QApplication entry point
   - Window creation and event loop

#### Key Dependencies (from Dev 1)
- `GraphWidget` for central display area
- `AnimationController` for playback control
- Signals/slots API for communication

#### Milestones
- **Week 4**: Main window with layout structure
- **Week 5**: Control panels fully functional
- **Week 6**: Full integration with C2V/V2C pipelines

---

## Integration Architecture

### Component Layout

```
???????????????????????????????????????????????????????????
?            MainWindow            ?
?  ????????????????????????????????????????????????????????
?  ?               ?   Control Panel      ??
?  ?              ?  ??????????????????????? ??
?  ?   ?  ? Play  Pause  Stop   ? ??
?  ?   GraphWidget ?  ??????????????????????? ??
?  ?  (from Dev 1)            ?  ? Frame Slider (0/100)? ??
?  ?    ?  ??????????????????????? ??
?  ?  (displays DOT code      ?  ? Speed: 1.0 FPS     ? ??
?  ?   from C2V)   ?  ??????????????????????? ??
?  ? ?  ??
?  ?  ?   Properties Panel       ??
?  ?              ?  ??????????????????????? ??
?  ?    ?  ? Node: id="5"        ? ??
?  ?   ?  ? Label: "Node 5"     ? ??
?  ?     ?  ? ...properties...    ? ??
?  ?      ?  ??????????????????????? ??
?  ????????????????????????????????????????????????????????
?  ?????????????????????????????????????????????????????????
?  ?  Output Console (from Dev 2)      ??
?  ?  [13:45:20] Opened: linked_list.cpp       ??
?  ?  [13:45:21] Loaded 42 frames      ??
?  ?  [13:45:22] Animation playing...                ??
?  ?????????????????????????????????????????????????????????
???????????????????????????????????????????????????????????
```

### Signal/Slot Flow

```
     AnimationController
  ?
         ?
   frameChanged(n)
stateChanged(state)
    ?
           ?????????????????
       ?               ?
    ControlPanel    GraphWidget
     ?   ?
          (play/pause/etc)   (displays graph)
 ?       ?
      ?????????????????
               ?
               nodeSelected(id)
        edgeSelected(f,t)
  ?
     MainWindow
         ?
              (updates PropertiesPanel)
```

---

## Parallel Development Timeline

```
Week 4: Setup & Core Components
?? Dev 1: GraphvizRenderer basics
?         GraphWidget skeleton
?? Dev 2: MainWindow structure
?         Menu/toolbar setup
?? Both: Verify compilation, dependency chain

Week 5: Functionality Implementation  
?? Dev 1: GraphWidget interaction
?         AnimationController logic
?? Dev 2: ControlPanel signals
?         PropertiesPanel display
?? Both: Integration testing begins

Week 6: Refinement & Polish
?? Dev 1: Performance optimization
?     Export functionality
?? Dev 2: C2V/V2C integration
?         Error handling
?? Both: Full end-to-end testing
```

---

## Data Interfaces

### From GraphvizRenderer ? GraphWidget

```cpp
struct RenderedGraph {
    std::vector<RenderedNode> nodes;
    std::vector<RenderedEdge> edges;
    double width;
  double height;
    bool isValid;
};
```

### From AnimationController ? GraphWidget

```cpp
void setVisualizationState(const VisualizationState &state) {
    // Receives state with:
    // - nodeData: JSON array of nodes
    // - edgeData: JSON array of edges
    // - dotCode: Graphviz DOT code
    // - operationDescription: What changed
}
```

### From ControlPanel ? AnimationController

```cpp
void play();
void pause();
void stop();
void goToFrame(int frameNumber);
void setSpeed(double fps);
```

### From GraphWidget ? MainWindow

```cpp
signals:
 void nodeSelected(const QString &nodeId);
void edgeSelected(const QString &fromId, const QString &toId);
    void viewChanged();
```

---

## CMake Dependency Chain

```
src/ui/visualization/CMakeLists.txt (Dev 1)
?? Graphviz
?? Qt6 (Core, Gui, OpenGL, Widgets)
?? state_mapping (from C2V)

src/ui/CMakeLists.txt (Dev 2)
?? Qt6 (Core, Gui, Widgets, OpenGL)
?? visualization (from Dev 1) ?? Dependency!

Root CMakeLists.txt
?? add_subdirectory(src/analysis)  # C2V
?? add_subdirectory(src/ui/visualization) # Dev 1 first
?? add_subdirectory(src/ui)        # Dev 2 after visualization
```

---

## Daily Coordination

### Interface Review (Daily)
Both developers should verify:
1. **Signal/Slot Compatibility**
   - Dev 1 emits: `frameChanged(int), stateChanged(state)`
   - Dev 2 connects: `connect(..., &AnimationController::frameChanged, ...)`

2. **Data Structure Compatibility**
   - `VisualizationState` fields match both uses
   - `RenderedGraph` coordinates are in correct space

3. **Compilation Status**
   - No linking errors
   - All dependencies resolved

### Weekly Sync (Video Call)
- Review progress against timeline
- Discuss any design issues
- Plan next week's integration points
- Demo working features

---

## Testing Strategy

### Unit Testing (Individual)

**Dev 1 Tests**:
```cpp
TEST(GraphvizRenderer, RendersDOTCode) { ... }
TEST(GraphWidget, HighlightsNode) { ... }
TEST(AnimationController, PlaysFrames) { ... }
```

**Dev 2 Tests**:
```cpp
TEST(MainWindow, OpensSourceFile) { ... }
TEST(ControlPanel, EmitsSignals) { ... }
TEST(OutputConsole, LogsMessages) { ... }
```

### Integration Testing (Joint)

**Full UI Pipeline**:
```cpp
TEST(UIIntegration, LoadsAndPlaysAnimation) {
    // 1. Load C2V generated states
    std::vector<VisualizationState> states = loadTestStates();
    
    // 2. Pass to AnimationController
    animationController->loadStates(states);
    
    // 3. Press play
    animationController->play();
    
    // 4. Verify GraphWidget renders each frame
    for (int i = 0; i < states.size(); ++i) {
  EXPECT_TRUE(graphWidget->isDisplaying(states[i]));
    }
}
```

---

## Error Handling & Logging

### Console Output Strategy

Dev 2's OutputConsole should display:
- File operations: "[13:45:20] Opening: linked_list.cpp"
- C2V progress: "[13:45:21] Parsed 15 data structures"
- Animation state: "[13:45:22] Playing frame 5/42"
- Errors: "[ERROR] Failed to render: invalid DOT code"

### Exception Handling

Both developers should:
1. Catch exceptions at component boundaries
2. Log descriptive error messages
3. Display QMessageBox for user-visible errors
4. Continue gracefully on non-critical errors

---

## Common Issues & Solutions

| Issue | Cause | Solution |
|-------|-------|----------|
| Signal not received | Connection wrong | Verify `SIGNAL()` and `SLOT()` macros match |
| Blank graph display | DOT code invalid | Dev 1: Check Graphviz output, Dev 2: Log DOT code |
| Animation stutters | UI thread blocked | Move heavy operations to worker thread |
| Memory leak | Circular ownership | Use `std::make_unique` with proper cleanup |
| Qt MOC errors | Missing AUTOMOC | Add `set_property(...AUTOMOC ON)` in CMake |

---

## Deliverables Checklist

### Dev 1 - Final Deliverables
- [ ] GraphvizRenderer fully renders DOT code
- [ ] GraphWidget interactive with zoom/pan/select
- [ ] AnimationController plays all frames smoothly
- [ ] All signals properly connected
- [ ] Unit tests passing (>90% coverage)
- [ ] No memory leaks (valgrind clean)
- [ ] Performance: 60 FPS capable
- [ ] Documentation: Code comments + README

### Dev 2 - Final Deliverables
- [ ] MainWindow with complete UI layout
- [ ] ControlPanel responsive to all interactions
- [ ] PropertiesPanel displays node/edge info
- [ ] OutputConsole logs all operations
- [ ] File dialogs open/save correctly
- [ ] All signal connections working
- [ ] Unit tests passing (>90% coverage)
- [ ] Full C2V pipeline integration

---

## Next Phase: Handoff to V2C Team

**What UI Team Provides**:
1. Complete UI application (executable)
2. Compiled libraries: `visualization`, `ui_engine`
3. Example: Load C++ file, visualize, export animation
4. API documentation for V2C integration:
   - How to trigger code generation on graph changes
   - How to receive updated source files
   - How to refresh visualization with new state

**Interface to V2C Team**:
- V2C listens to: `GraphWidget::nodeSelected`, `edgeSelected`
- UI listens to: V2C's code generation complete signal
- Both use: `VisualizationState` and `ExecutionEvent` structures

---

## Reference Documents

- [UI Developer 1 Guide](./UI_DEVELOPER_1_RENDERING.md)
- [UI Developer 2 Guide](./UI_DEVELOPER_2_FRAMEWORK.md)
- [C2V Team Guide](./C2V_GUIDE.md)
- [V2C Team Guide](./V2C_GUIDE.md)
- [Global Coordination](./GLOBAL_COORDINATION.md)

---

## Quick Reference: Key Files

| File | Developer | Purpose |
|------|-----------|---------|
| `src/ui/visualization/graphviz_renderer.h/.cpp` | Dev 1 | DOT?positions conversion |
| `src/ui/visualization/graph_widget.h/.cpp` | Dev 1 | Qt display widget |
| `src/ui/visualization/animation_controller.h/.cpp` | Dev 1 | Playback control |
| `src/ui/main_window.h/.cpp` | Dev 2 | Application frame |
| `src/ui/control_panel.h/.cpp` | Dev 2 | Playback controls |
| `src/ui/properties_panel.h/.cpp` | Dev 2 | Info display |
| `src/ui/output_console.h/.cpp` | Dev 2 | Logging |
| `src/ui/main.cpp` | Dev 2 | Entry point |

---

**Last Updated**: [Current Date]  
**Maintained By**: Team Lead  
**Next Review**: End of Week 5
