# UI Engine Developer 2: Application Framework & Integration
**Focus Area**: Main Window, Controls & System Integration  
**Timeline**: Weeks 4-6

## Your Role

You are responsible for **building the main application framework and integrating all visualization components**. Your work ties together the rendering engine from UI Dev 1 with the C2V/V2C pipelines. Think of you as building the "controller" layer - managing interactions, file I/O, and component orchestration.

---

## Deliverables Overview

### Phase 1: Application Framework (Week 4)
- ? Create Qt main window
- ? Build menu and toolbar structure
- ? Implement file dialogs and I/O
- ? Set up application state management

### Phase 2: Control Panel & Timeline (Week 5)
- ? Build animation playback controls
- ? Create frame timeline slider
- ? Display execution event information
- ? Implement settings/preferences

### Phase 3: Integration & Polish (Week 6)
- ? Integrate C2V pipeline
- ? Integrate V2C pipeline for code editing
- ? Add error handling and logging
- ? Performance optimization

---

## Dependencies

**Before starting**, ensure these are ready:

From **UI Dev 1** (collaborative):
- ? Working `GraphWidget`
- ? Working `AnimationController`
- ? Compiled `visualization` library

From **C2V Team**:
- ? C2V pipeline (parser + instrumentation)
- ? Execution logs (JSON files)
- ? Visualization states

From **V2C Team**:
- ? Code rewriting capability
- ? API mapping service

---

## Detailed Tasks

### Task 2.1: Create Main Application Window

**File**: `src/ui/main_window.h`

```cpp
#pragma once

#include <QMainWindow>
#include <memory>

class GraphWidget;
class AnimationController;
class ControlPanel;
class PropertiesPanel;
class OutputConsole;

/// Main application window
class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;
    
    /// Load and process C++ source file
    void openSourceFile(const QString &filePath);
    
    /// Export current animation to file
    void exportAnimation(const QString &format);
    
    /// Save project configuration
    void saveProject(const QString &filePath);
    
    /// Load project configuration
    void loadProject(const QString &filePath);
    
protected:
    void closeEvent(QCloseEvent *event) override;
    
private:
/// UI Components
    std::unique_ptr<GraphWidget> graphWidget_;
    std::unique_ptr<ControlPanel> controlPanel_;
    std::unique_ptr<PropertiesPanel> propertiesPanel_;
    std::unique_ptr<OutputConsole> outputConsole_;
    std::unique_ptr<AnimationController> animationController_;
    
/// Setup UI
    void setupMenuBar();
    void setupToolBar();
    void setupDockWidgets();
    void setupConnections();
    
    /// File menu slots
private slots:
    void onFileOpen();
    void onFileSave();
    void onFileExport();
    void onFileExit();
    
    /// View menu slots
    void onViewResetZoom();
    void onViewFitAll();
    void onViewShowGrid();
    
    /// Help menu slots
    void onHelpAbout();
    void onHelpDocumentation();
    
    /// Animation control slots
    void onAnimationPlay();
    void onAnimationPause();
    void onAnimationStop();
void onAnimationSpeedChanged(double fps);
  void onFrameChanged(int frameNumber);
 
    /// Graph interaction slots
    void onNodeSelected(const QString &nodeId);
    void onEdgeSelected(const QString &fromId, const QString &toId);
};
```

**File**: `src/ui/main_window.cpp`

```cpp
#include "main_window.h"
#include "control_panel.h"
#include "properties_panel.h"
#include "output_console.h"
#include "visualization/graph_widget.h"
#include "visualization/animation_controller.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QToolBar>
#include <QDockWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("DataViz - Code Visualization Engine");
    setWindowIcon(QIcon(":/icons/app_icon.png"));
    setGeometry(100, 100, 1400, 900);
    
    // Create central widget
    auto centralWidget = new QWidget(this);
    auto layout = new QHBoxLayout(centralWidget);
    
    // Create graph widget
    graphWidget_ = std::make_unique<GraphWidget>();
    layout->addWidget(graphWidget_.get(), 1);
    
    // Create right panel with control and properties
    auto rightPanel = new QWidget();
    auto rightLayout = new QVBoxLayout(rightPanel);
    
    controlPanel_ = std::make_unique<ControlPanel>();
    propertiesPanel_ = std::make_unique<PropertiesPanel>();
    
    rightLayout->addWidget(controlPanel_.get(), 0);
    rightLayout->addWidget(propertiesPanel_.get(), 1);
    
    layout->addWidget(rightPanel, 0);
    
    setCentralWidget(centralWidget);
    
    // Create animation controller
animationController_ = std::make_unique<AnimationController>();
    
    // Create console in dock widget
    outputConsole_ = std::make_unique<OutputConsole>();
    auto consoleDock = new QDockWidget("Output Console", this);
    consoleDock->setWidget(outputConsole_.get());
    addDockWidget(Qt::BottomDockWidgetArea, consoleDock);
    
    setupMenuBar();
    setupToolBar();
    setupConnections();
}

void MainWindow::setupMenuBar()
{
    auto fileMenu = menuBar()->addMenu("&File");
    
    auto openAction = fileMenu->addAction("&Open C++ File...");
    connect(openAction, &QAction::triggered, this, &MainWindow::onFileOpen);
    openAction->setShortcut(QKeySequence::Open);
    
    auto saveAction = fileMenu->addAction("&Save Project...");
    connect(saveAction, &QAction::triggered, this, &MainWindow::onFileSave);
    saveAction->setShortcut(QKeySequence::Save);
    
    fileMenu->addSeparator();
    
    auto exportAction = fileMenu->addAction("&Export Animation...");
    connect(exportAction, &QAction::triggered, this, &MainWindow::onFileExport);
    
    fileMenu->addSeparator();
    
    auto exitAction = fileMenu->addAction("E&xit");
    connect(exitAction, &QAction::triggered, this, &MainWindow::onFileExit);
    exitAction->setShortcut(QKeySequence::Quit);
    
    // View menu
    auto viewMenu = menuBar()->addMenu("&View");
    
    auto resetZoomAction = viewMenu->addAction("&Reset Zoom");
    connect(resetZoomAction, &QAction::triggered, this, &MainWindow::onViewResetZoom);
    
    auto fitAllAction = viewMenu->addAction("&Fit All");
connect(fitAllAction, &QAction::triggered, this, &MainWindow::onViewFitAll);
    
    // Help menu
    auto helpMenu = menuBar()->addMenu("&Help");
    
    auto aboutAction = helpMenu->addAction("&About");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onHelpAbout);

    auto docAction = helpMenu->addAction("&Documentation");
 connect(docAction, &QAction::triggered, this, &MainWindow::onHelpDocumentation);
}

void MainWindow::setupToolBar()
{
    auto toolbar = addToolBar("Main Toolbar");
    toolbar->setIconSize(QSize(24, 24));
    
    // Animation controls
    auto playAction = toolbar->addAction("? Play");
    connect(playAction, &QAction::triggered, this, &MainWindow::onAnimationPlay);
    
    auto pauseAction = toolbar->addAction("? Pause");
    connect(pauseAction, &QAction::triggered, this, &MainWindow::onAnimationPause);
    
    auto stopAction = toolbar->addAction("? Stop");
    connect(stopAction, &QAction::triggered, this, &MainWindow::onAnimationStop);
    
  toolbar->addSeparator();
    
    // Export
    auto exportAction = toolbar->addAction("?? Export");
 connect(exportAction, &QAction::triggered, this, &MainWindow::onFileExport);
}

void MainWindow::setupConnections()
{
    // Animation controller signals
 connect(animationController_.get(), QOverload<int>::of(&AnimationController::frameChanged),
         this, &MainWindow::onFrameChanged);
    
    connect(animationController_.get(), &AnimationController::stateChanged,
            graphWidget_.get(), &GraphWidget::setVisualizationState);
    
    // Control panel signals
    connect(controlPanel_.get(), &ControlPanel::playClicked,
            this, &MainWindow::onAnimationPlay);
    
    connect(controlPanel_.get(), &ControlPanel::pauseClicked,
          this, &MainWindow::onAnimationPause);
    
    connect(controlPanel_.get(), &ControlPanel::stopClicked,
            this, &MainWindow::onAnimationStop);
    
    connect(controlPanel_.get(), &ControlPanel::speedChanged,
            animationController_.get(), &AnimationController::setSpeed);
    
    connect(controlPanel_.get(), &ControlPanel::frameSelected,
       animationController_.get(), &AnimationController::goToFrame);
    
    // Graph widget signals
    connect(graphWidget_.get(), QOverload<const QString&>::of(&GraphWidget::nodeSelected),
            this, &MainWindow::onNodeSelected);
}

void MainWindow::openSourceFile(const QString &filePath)
{
    outputConsole_->log("Opening: " + filePath.toStdString());
  
    try {
        // Run C2V pipeline
        // 1. Parse and extract structures
 // 2. Instrument code
        // 3. Execute instrumented code
     // 4. Generate visualization states
        
      // Load states into animation controller
  std::vector<VisualizationState> states;
        // ... load from C2V pipeline ...
        
  animationController_->loadStates(states);
        
        outputConsole_->log("Loaded " + std::to_string(states.size()) + " frames");
        
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", 
           QString("Failed to open file: %1").arg(e.what()));
        outputConsole_->logError(std::string(e.what()));
    }
}

void MainWindow::exportAnimation(const QString &format)
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Export Animation", "",
   "MP4 Video (*.mp4);;PNG Sequence (*.png);;SVG Sequence (*.svg)");
    
    if (fileName.isEmpty()) return;
    
    outputConsole_->log("Exporting animation to: " + fileName.toStdString());
    // Implementation would follow...
}

void MainWindow::saveProject(const QString &filePath)
{
    outputConsole_->log("Saving project to: " + filePath.toStdString());
 // Save project state...
}

void MainWindow::loadProject(const QString &filePath)
{
    outputConsole_->log("Loading project from: " + filePath.toStdString());
    // Load project state...
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    outputConsole_->log("Application closing");
    QMainWindow::closeEvent(event);
}

void MainWindow::onFileOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Open C++ Source File", "",
        "C++ Files (*.cpp *.h *.hpp);;All Files (*)");
    
    if (!fileName.isEmpty()) {
        openSourceFile(fileName);
    }
}

void MainWindow::onFileSave()
{
    QString fileName = QFileDialog::getSaveFileName(this,
  "Save Project", "",
        "Project Files (*.dvz)");
    
    if (!fileName.isEmpty()) {
        saveProject(fileName);
    }
}

void MainWindow::onFileExport()
{
    exportAnimation("mp4");
}

void MainWindow::onFileExit()
{
    close();
}

void MainWindow::onViewResetZoom()
{
  graphWidget_->resetView();
    outputConsole_->log("View reset");
}

void MainWindow::onViewFitAll()
{
 graphWidget_->resetView();  // Simplified
    outputConsole_->log("Fit all nodes to view");
}

void MainWindow::onViewShowGrid()
{
    // Toggle grid display
}

void MainWindow::onHelpAbout()
{
    QMessageBox::about(this, "About DataViz",
        "DataViz - Code Visualization Engine\n\n"
   "Version 1.0\n"
        "Visualize C++ data structures in action");
}

void MainWindow::onHelpDocumentation()
{
    outputConsole_->log("Opening documentation...");
  // Open documentation
}

void MainWindow::onAnimationPlay()
{
    animationController_->play();
    outputConsole_->log("Animation playing");
}

void MainWindow::onAnimationPause()
{
    animationController_->pause();
    outputConsole_->log("Animation paused");
}

void MainWindow::onAnimationStop()
{
    animationController_->stop();
    outputConsole_->log("Animation stopped");
}

void MainWindow::onAnimationSpeedChanged(double fps)
{
    animationController_->setSpeed(fps);
    outputConsole_->log("Speed: " + std::to_string(fps) + " FPS");
}

void MainWindow::onFrameChanged(int frameNumber)
{
    outputConsole_->log("Frame: " + std::to_string(frameNumber));
    controlPanel_->updateFrameDisplay(frameNumber);
}

void MainWindow::onNodeSelected(const QString &nodeId)
{
  outputConsole_->log("Selected node: " + nodeId.toStdString());
    propertiesPanel_->displayNodeInfo(nodeId);
}

void MainWindow::onEdgeSelected(const QString &fromId, const QString &toId)
{
    outputConsole_->log("Selected edge: " + fromId.toStdString() + 
     " -> " + toId.toStdString());
}
```

---

### Task 2.2: Create Control Panel Widget

**File**: `src/ui/control_panel.h`

```cpp
#pragma once

#include <QWidget>
#include <memory>

class QSlider;
class QSpinBox;
class QDoubleSpinBox;
class QPushButton;
class QLabel;

/// Control panel for animation playback
class ControlPanel : public QWidget {
    Q_OBJECT
    
public:
    explicit ControlPanel(QWidget *parent = nullptr);
    
 /// Update displayed frame number
    void updateFrameDisplay(int frameNumber);
    
signals:
    void playClicked();
    void pauseClicked();
    void stopClicked();
    void frameSelected(int frameNumber);
    void speedChanged(double fps);
    
private slots:
    void onPlayClicked();
    void onPauseClicked();
  void onStopClicked();
  void onFrameSliderMoved(int value);
    void onSpeedSpinBoxChanged(double value);
    void onPreviousFrameClicked();
    void onNextFrameClicked();
    
private:
    void setupUI();
    void setupConnections();
    
    // UI elements
  std::unique_ptr<QPushButton> playButton_;
    std::unique_ptr<QPushButton> pauseButton_;
    std::unique_ptr<QPushButton> stopButton_;
  std::unique_ptr<QPushButton> prevButton_;
    std::unique_ptr<QPushButton> nextButton_;
    
    std::unique_ptr<QSlider> frameSlider_;
    std::unique_ptr<QSpinBox> currentFrameSpinBox_;
    std::unique_ptr<QLabel> totalFramesLabel_;
    
    std::unique_ptr<QDoubleSpinBox> speedSpinBox_;
  std::unique_ptr<QLabel> speedLabel_;
};
```

**File**: `src/ui/control_panel.cpp`

```cpp
#include "control_panel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGroupBox>

ControlPanel::ControlPanel(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
setupConnections();
}

void ControlPanel::setupUI()
{
    auto mainLayout = new QVBoxLayout(this);
    
    // Playback controls group
    auto playbackGroup = new QGroupBox("Playback Controls");
    auto playbackLayout = new QHBoxLayout();
    
  playButton_ = std::make_unique<QPushButton>("? Play");
    pauseButton_ = std::make_unique<QPushButton>("? Pause");
    stopButton_ = std::make_unique<QPushButton>("? Stop");
    prevButton_ = std::make_unique<QPushButton>("? Prev");
    nextButton_ = std::make_unique<QPushButton>("Next ?");
    
    playbackLayout->addWidget(prevButton_.get());
    playbackLayout->addWidget(playButton_.get());
    playbackLayout->addWidget(pauseButton_.get());
    playbackLayout->addWidget(stopButton_.get());
    playbackLayout->addWidget(nextButton_.get());
    
    playbackGroup->setLayout(playbackLayout);
    mainLayout->addWidget(playbackGroup);
    
    // Frame control group
    auto frameGroup = new QGroupBox("Frame Control");
    auto frameLayout = new QVBoxLayout();
    
    frameSlider_ = std::make_unique<QSlider>(Qt::Horizontal);
    frameSlider_->setMinimum(0);
    frameSlider_->setMaximum(100);
  
    auto frameHLayout = new QHBoxLayout();
    auto frameLabel = new QLabel("Frame:");
    currentFrameSpinBox_ = std::make_unique<QSpinBox>();
    totalFramesLabel_ = std::make_unique<QLabel>("/ 100");
    
    frameHLayout->addWidget(frameLabel);
    frameHLayout->addWidget(currentFrameSpinBox_.get());
    frameHLayout->addWidget(totalFramesLabel_.get());
    
    frameLayout->addWidget(frameSlider_.get());
    frameLayout->addLayout(frameHLayout);
    
    frameGroup->setLayout(frameLayout);
    mainLayout->addWidget(frameGroup);
    
    // Speed control group
    auto speedGroup = new QGroupBox("Speed Control");
    auto speedLayout = new QHBoxLayout();
    
    speedLabel_ = std::make_unique<QLabel>("Speed (FPS):");
    speedSpinBox_ = std::make_unique<QDoubleSpinBox>();
    speedSpinBox_->setMinimum(0.1);
    speedSpinBox_->setMaximum(60.0);
    speedSpinBox_->setValue(1.0);
    speedSpinBox_->setSingleStep(0.5);
    
    speedLayout->addWidget(speedLabel_.get());
    speedLayout->addWidget(speedSpinBox_.get());
    
    speedGroup->setLayout(speedLayout);
    mainLayout->addWidget(speedGroup);
    
    mainLayout->addStretch();
    setLayout(mainLayout);
}

void ControlPanel::setupConnections()
{
    connect(playButton_.get(), &QPushButton::clicked,
     this, &ControlPanel::onPlayClicked);
    
    connect(pauseButton_.get(), &QPushButton::clicked,
   this, &ControlPanel::onPauseClicked);
    
    connect(stopButton_.get(), &QPushButton::clicked,
      this, &ControlPanel::onStopClicked);
    
    connect(prevButton_.get(), &QPushButton::clicked,
            this, &ControlPanel::onPreviousFrameClicked);
    
    connect(nextButton_.get(), &QPushButton::clicked,
      this, &ControlPanel::onNextFrameClicked);
    
    connect(frameSlider_.get(), &QSlider::sliderMoved,
   this, &ControlPanel::onFrameSliderMoved);
    
    connect(speedSpinBox_.get(), QOverload<double>::of(&QDoubleSpinBox::valueChanged),
  this, &ControlPanel::onSpeedSpinBoxChanged);
}

void ControlPanel::updateFrameDisplay(int frameNumber)
{
    currentFrameSpinBox_->blockSignals(true);
    currentFrameSpinBox_->setValue(frameNumber);
    currentFrameSpinBox_->blockSignals(false);
    
    frameSlider_->blockSignals(true);
    frameSlider_->setValue(frameNumber);
    frameSlider_->blockSignals(false);
}

void ControlPanel::onPlayClicked()
{
    emit playClicked();
}

void ControlPanel::onPauseClicked()
{
    emit pauseClicked();
}

void ControlPanel::onStopClicked()
{
    emit stopClicked();
}

void ControlPanel::onFrameSliderMoved(int value)
{
emit frameSelected(value);
}

void ControlPanel::onSpeedSpinBoxChanged(double value)
{
    emit speedChanged(value);
}

void ControlPanel::onPreviousFrameClicked()
{
    int current = frameSlider_->value();
    emit frameSelected(current - 1);
}

void ControlPanel::onNextFrameClicked()
{
    int current = frameSlider_->value();
    emit frameSelected(current + 1);
}
```

---

### Task 2.3: Create Properties Panel Widget

**File**: `src/ui/properties_panel.h`

```cpp
#pragma once

#include <QWidget>
#include <QString>
#include <memory>

class QTableWidget;
class QTextEdit;
class QLabel;

/// Display properties of selected nodes/edges
class PropertiesPanel : public QWidget {
    Q_OBJECT
    
public:
    explicit PropertiesPanel(QWidget *parent = nullptr);
    
    /// Display information for a node
    void displayNodeInfo(const QString &nodeId);
    
    /// Display information for an edge
    void displayEdgeInfo(const QString &fromId, const QString &toId);
    
  /// Clear displayed information
    void clear();
    
private:
    void setupUI();
    
    std::unique_ptr<QLabel> titleLabel_;
    std::unique_ptr<QTableWidget> propertiesTable_;
 std::unique_ptr<QTextEdit> descriptionText_;
};
```

---

### Task 2.4: Create Output Console Widget

**File**: `src/ui/output_console.h`

```cpp
#pragma once

#include <QWidget>
#include <memory>
#include <string>

class QTextEdit;

/// Output console for logging messages
class OutputConsole : public QWidget {
    Q_OBJECT
    
public:
 explicit OutputConsole(QWidget *parent = nullptr);
    
    /// Log informational message
    void log(const std::string &message);
    
    /// Log warning message
    void logWarning(const std::string &message);
    
    /// Log error message
    void logError(const std::string &message);
    
    /// Clear all messages
    void clear();
    
private:
    void setupUI();
    
    std::unique_ptr<QTextEdit> outputText_;
};
```

**File**: `src/ui/output_console.cpp`

```cpp
#include "output_console.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QDateTime>

OutputConsole::OutputConsole(QWidget *parent)
 : QWidget(parent)
{
    setupUI();
}

void OutputConsole::setupUI()
{
    auto layout = new QVBoxLayout(this);
    
 outputText_ = std::make_unique<QTextEdit>();
    outputText_->setReadOnly(true);
    outputText_->setMaximumHeight(150);
    
    layout->addWidget(outputText_.get());
    setLayout(layout);
}

void OutputConsole::log(const std::string &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("HH:mm:ss");
    QString formatted = QString("[%1] %2")
        .arg(timestamp)
        .arg(QString::fromStdString(message));
    
    outputText_->append(formatted);
}

void OutputConsole::logWarning(const std::string &message)
{
    log("[WARNING] " + message);
}

void OutputConsole::logError(const std::string &message)
{
    log("[ERROR] " + message);
}

void OutputConsole::clear()
{
    outputText_->clear();
}
```

---

### Task 2.5: Create CMake Configuration

**File**: `src/ui/CMakeLists.txt`

```cmake
# UI Engine library

find_package(Qt6 COMPONENTS 
    Core 
    Gui 
    Widgets 
    OpenGL 
    REQUIRED
)

# Add library
add_library(ui_engine
    main_window.cpp
    control_panel.cpp
    properties_panel.cpp
    output_console.cpp
)

# Link dependencies
target_link_libraries(ui_engine
    PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::OpenGL
        visualization
        state_mapping
)

# Include directories
target_include_directories(ui_engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Enable C++17 and Qt Meta-Object Compiler
set_property(TARGET ui_engine PROPERTY CXX_STANDARD 17)
set_property(TARGET ui_engine PROPERTY AUTOMOC ON)
```

**File**: `src/ui/main.cpp` (Application Entry Point)

```cpp
#include <QApplication>
#include "main_window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
```

---

### Task 2.6: Write Integration Tests

**File**: `tests/test_main_window.cpp`

```cpp
#include <gtest/gtest.h>
#include "ui/main_window.h"
#include <QApplication>

class MainWindowTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        if (!qApp) {
            int argc = 0;
            new QApplication(argc, nullptr);
   }
    }
    
    MainWindow window;
};

TEST_F(MainWindowTest, OpensSourceFile) {
    // Create test file
    std::string testCode = R"(
        #include <vector>
        std::vector<int> v;
        v.push_back(1);
    )";
    
    // Would test file loading here
    EXPECT_TRUE(true);
}

TEST_F(MainWindowTest, AnimationControlsWork) {
    // Test that animation controls are connected
    EXPECT_NE(nullptr, window.graphWidget_);
}
```

---

## Testing Checklist

Before final integration:

- [ ] Main window displays correctly
- [ ] All menus and toolbars functional
- [ ] File open/save dialogs work
- [ ] Animation playback controls responsive
- [ ] Frame slider updates correctly
- [ ] Speed control adjusts animation
- [ ] Property panel displays selected nodes
- [ ] Console logs messages properly
- [ ] Window resizing handles layout correctly
- [ ] No crashes on edge cases
- [ ] Performance: UI responsive at 60 FPS

---

## Handoff from UI Dev 1

**What you receive**:
1. Compiled `visualization` library
2. Working `GraphWidget` 
3. Working `AnimationController`

**Integration Points**:
- Add `GraphWidget` to central area of main window
- Connect `AnimationController` signals to `GraphWidget`
- Connect control panel buttons to `AnimationController`

---

## Integration with C2V Pipeline

**File**: `src/ui/pipeline_bridge.h`

```cpp
#pragma once

#include "analysis/clang_integration/ast_parser.h"
#include "analysis/instrumentation/llvm_instrumentation.h"
#include "analysis/state_mapping/state_mapper.h"

/// Bridges UI with C2V pipeline
class PipelineBridge {
public:
    /// Run full C2V pipeline on source file
    std::vector<VisualizationState> processSourceFile(const std::string &filePath);
    
private:
    std::unique_ptr<ASTParser> parser_;
    std::unique_ptr<LLVMInstrumentation> instrumentation_;
    std::unique_ptr<StateMapper> stateMapper_;
};
```

---

## Key Components Summary

| Component | Purpose | Key Class |
|-----------|---------|-----------|
| **Main Window** | Application frame | `MainWindow` |
| **Control Panel** | Animation controls | `ControlPanel` |
| **Properties Panel** | Information display | `PropertiesPanel` |
| **Output Console** | Logging | `OutputConsole` |

---

## Performance Targets

- UI responsiveness: <100ms for any interaction
- File open: <2s for typical source file
- Frame update: <16ms (60 FPS)
- Memory: <200MB total (including visualization)

---

## Debugging Tips

**Problem**: Qt signal/slot not connecting
- ? Check object ownership (unique_ptr might destroy before connection)
- ? Verify signal/slot signatures match exactly
- ? Enable Qt debug output: `QT_DEBUG_PLUGINS=1`

**Problem**: File dialog not appearing
- ? Verify QFileDialog parent is valid main window
- ? Check file system permissions
- ? Ensure native dialogs enabled: `setOption(QFileDialog::DontUseNativeDialog, false)`

**Problem**: Animation stuttering
- ? Profile with Qt Creator profiler
- ? Check for expensive operations in paint event
- ? Verify timer interval is correct

---

## Timeline

| Week | Task | Status |
|------|------|--------|
| Week 4 | Task 2.1-2.2: Window & Controls | ? |
| Week 5 | Task 2.3-2.4: Panels | ? |
| Week 5 | Task 2.5: CMake & Main | ? |
| Week 6 | Task 2.6: Integration & Testing | ? |

---

## Resources

- **Qt6 Widgets**: https://doc.qt.io/qt-6/qtwidgets-index.html
- **Qt Signals/Slots**: https://doc.qt.io/qt-6/signalsandslots.html
- **File Dialogs**: https://doc.qt.io/qt-6/qfiledialog.html

---

**Questions?** Contact your team lead or refer to the UI_COORDINATION.md.
