#include "main_window.h"
#include "visualization_pane.h"
#include "control_panel.h"
#include "metrics_panel.h"
#include "toolbox_panel.h"
#include "color_legend_panel.h"
#include "structure_selector.h"
#include "code_generator_dialog.h"
#include "tutorial_overlay.h"
#include "../orchestration/algorithm_manager.h"
#include "../visualization/animation_frame.h"
#include "../visualization/visualization_renderer.h"
#include "../core/array_structure.h"
#include "../core/list_structure.h"
#include "../core/tree_structure.h"
#include "../core/graph_structure.h"
#include "../core/list_node.h"
#include "../core/tree_node.h"
#include "../core/graph.h"

// Include all algorithm implementations
#include "../algorithms/tree_inorder.h"
#include "../algorithms/tree_preorder.h"
#include "../algorithms/tree_postorder.h"
#include "../algorithms/tree_levelorder.h"
#include "../algorithms/tree_find_height.h"
#include "../algorithms/tree_find_min.h"
#include "../algorithms/tree_find_max.h"
#include "../algorithms/insertion_sort.h"
#include "../algorithms/selection_sort.h"
#include "../algorithms/remove_duplicates.h"
#include "../algorithms/reverse.h"
#include "../algorithms/map_transform.h"
#include "../algorithms/graph_algorithms.h"
#include "../algorithms/graph_detect_cycle.h"
#include "../algorithms/graph_topological_sort.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QShowEvent>
#include <QScrollArea>
#include <QTimer>
#include <cmath>
#include <functional>
#include <queue>
#include <random>
#include <set>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    dataModelManager(std::make_unique<DataModelManager>()),
    visualizationPane(std::make_unique<VisualizationPane>(this)),
    controlPanel(std::make_unique<ControlPanel>(this)),
    metricsPanel(std::make_unique<MetricsPanel>(this)),
    playbackController(std::make_unique<PlaybackController>(this)),  // NEW: Initialize playback controller
    toolboxPanel(nullptr),
    structureSelector(nullptr),
    tutorialOverlay(nullptr),
    algoManager(AlgorithmManager::getInstance()),
    frameRecorder(),
    currentAlgorithm(nullptr),
    toggleMetricsAction(nullptr),
    showTutorialAction(nullptr),
    autoSaveAction(nullptr),
    autoSaveEnabled(true),
    isAnimationPlaying(false),  // NEW: Initialize animation state
    settings("DataVizUIT", "DataVizUIT")
{
    setWindowTitle("DataViz UIT - Data Structure Visualization");
    resize(1350, 900);
    setMinimumSize(1200, 750);

    // Load auto-save preference from settings
    autoSaveEnabled = settings.value("autoSaveSession", true).toBool();

    if (visualizationPane && visualizationPane->getInteractionManager()) {
        visualizationPane->getInteractionManager()->setBackend(dataModelManager.get());
    }

    // NEW: Connect playback controller to visualization pane
    connect(playbackController.get(), &PlaybackController::frameReady,
        this, &MainWindow::onFrameReady);

    setupUI();
    connectSignals();
    createMenuBar();
    setupTutorial();  // Initialize tutorial

    // ✅ RESTAURATION SESSION
    if (autoSaveEnabled) {
        restorePreviousSession();
    }
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(10, 5, 10, 10);
    mainLayout->setSpacing(10);

    // Header
    QHBoxLayout* headerLayout = new QHBoxLayout();
    headerLayout->addStretch();
    QLabel* titleLabel = new QLabel("DataViz UIT", this);
    titleLabel->setObjectName("appTitle");
    titleLabel->setAlignment(Qt::AlignCenter);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    mainLayout->addLayout(headerLayout, 0);

    // Main Content
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(15);

    // Left: Toolbox (hidden by default until a structure is selected)
    toolboxPanel = new ToolboxPanel(this);
    toolboxPanel->setObjectName("borderedPanel");
    toolboxPanel->setFixedWidth(110);
    toolboxPanel->setVisible(false);
    contentLayout->addWidget(toolboxPanel);

    // Center: Visualization
    visualizationPane->setObjectName("borderedPanel");
    contentLayout->addWidget(visualizationPane.get(), 1);

    // Right: Scrollable Panel
    QScrollArea* rightScrollArea = new QScrollArea(this);
    rightScrollArea->setWidgetResizable(true);
    rightScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    rightScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    rightScrollArea->setFrameShape(QFrame::NoFrame);
    rightScrollArea->setStyleSheet("QScrollArea { background: transparent; border: none; }");

    QWidget* rightContainer = new QWidget();
    QVBoxLayout* rightLayout = new QVBoxLayout(rightContainer);
    rightLayout->setContentsMargins(5, 5, 10, 5);
    rightLayout->setSpacing(15);

    // Structure Selector
    structureSelector = new StructureSelector(this);
    structureSelector->setDataModelManager(dataModelManager.get());
    structureSelector->setObjectName("borderedPanel");
    rightLayout->addWidget(structureSelector);

    // Control Panel
    controlPanel->setObjectName("borderedPanel");
    rightLayout->addWidget(controlPanel.get());

    // Color Legend Panel - Shows during animation instead of toolbox
    colorLegendPanel = new ColorLegendPanel(this);
    colorLegendPanel->setObjectName("borderedPanel");
    colorLegendPanel->setVisible(false);  // Hidden initially
    rightLayout->addWidget(colorLegendPanel);

    // Metrics Panel - HIDDEN BY DEFAULT
    metricsPanel->setObjectName("borderedPanel");
    metricsPanel->setVisible(false);  // Hidden by default
    rightLayout->addWidget(metricsPanel.get());
    rightLayout->addStretch();

    rightScrollArea->setWidget(rightContainer);
    rightScrollArea->setFixedWidth(400);
    contentLayout->addWidget(rightScrollArea);

    mainLayout->addLayout(contentLayout, 1);

    connect(toolboxPanel, &ToolboxPanel::toolSelected,
        visualizationPane.get(), &VisualizationPane::setInteractionMode);
}

void MainWindow::setupTutorial() {
    tutorialOverlay = new TutorialOverlay(this);
    tutorialOverlay->hide();  // Hide initially until tutorial is started

    // Definition of tutorial steps
    tutorialOverlay->addStep(
        structureSelector,
        "Welcome to DataViz UIT!",
        "This is the Structure Manager where you can create, select, and manage your data structures.\n\n"
        "Click 'Create Sample Structures' to get started quickly with pre-built examples!",
        "left"
    );

    tutorialOverlay->addStep(
        visualizationPane.get(),
        "Visualization Canvas",
        "This is where your data structures come to life!\n\n"
        "You can drag nodes to rearrange them, double-click to edit values, and watch algorithms animate step-by-step.",
        "right"
    );

    tutorialOverlay->addStep(
        controlPanel.get(),
        "Algorithm Controls",
        "Select an algorithm from the dropdown and control playback here.\n\n"
        "Use Play to run, Pause to stop, and the step buttons for frame-by-frame analysis.",
        "left"
    );

    tutorialOverlay->addStep(
        nullptr,  // Will show centered
        "Toolbox (Appears on Selection)",
        "When you select a structure, a toolbox will appear on the left.\n\n"
        "Use it to:\n- Add new nodes (Circle/Rectangle)\n- Link nodes together\n- Erase nodes or edges",
        "center"
    );

    tutorialOverlay->addStep(
        nullptr,
        "Pro Tips",
        "- Double-click any node to change its value\n"
        "- Use the mouse wheel to zoom in/out on the canvas\n"
        "- Check 'Tools > Code Generator' to convert structures to C++ code\n"
        "- Access this tutorial anytime from Help > Show Tutorial",
        "center"
    );

    connect(tutorialOverlay, &TutorialOverlay::tutorialCompleted, this, &MainWindow::onTutorialCompleted);
    connect(tutorialOverlay, &TutorialOverlay::tutorialSkipped, this, &MainWindow::onTutorialSkipped);
}

void MainWindow::checkFirstLaunch() {
    bool hasLaunchedBefore = settings.value("hasLaunchedBefore", false).toBool();

    if (!hasLaunchedBefore) {
        // First launch - show tutorial after a short delay to let UI settle
        QTimer::singleShot(500, this, [this]() {
            if (tutorialOverlay) {
                tutorialOverlay->setGeometry(rect());
                tutorialOverlay->start();
            }
            });

        // Mark as launched
        settings.setValue("hasLaunchedBefore", true);
    }
}

void MainWindow::showEvent(QShowEvent* event) {
    QMainWindow::showEvent(event);

    // Check first launch only once
    if (!firstLaunchChecked) {
        firstLaunchChecked = true;
        // Delay slightly to ensure window is fully visible
        QTimer::singleShot(300, this, &MainWindow::checkFirstLaunch);
    }
}

void MainWindow::connectSignals() {
    connect(controlPanel.get(), &ControlPanel::playClicked, this, &MainWindow::onPlayClicked);
    connect(controlPanel.get(), &ControlPanel::pauseClicked, this, &MainWindow::onPauseClicked);
    connect(controlPanel.get(), &ControlPanel::resetClicked, this, &MainWindow::onResetClicked);
    connect(controlPanel.get(), &ControlPanel::stepForwardClicked, this, &MainWindow::onStepForwardClicked);
    connect(controlPanel.get(), &ControlPanel::stepBackwardClicked, this, &MainWindow::onStepBackwardClicked);

    connect(controlPanel.get(), &ControlPanel::algorithmSelected, this, &MainWindow::onAlgorithmSelected);
    connect(controlPanel.get(), &ControlPanel::speedChanged, this, &MainWindow::onSpeedChanged);

    connect(structureSelector, &StructureSelector::structureSelected,
        this, &MainWindow::onStructureSelected);
    connect(structureSelector, &StructureSelector::structureRemoved,
        this, &MainWindow::onStructureRemoved);
    connect(structureSelector, &StructureSelector::clearInteractiveRequested,
        this, &MainWindow::onClearInteractive);
    connect(structureSelector, &StructureSelector::samplesCreated,
        this, &MainWindow::onSamplesCreated);
}

void MainWindow::onStructureSelected(QString structureId) {
    std::string id = structureId.toStdString();

  if (dataModelManager) {
   // **IMPROVED**: Save current structure state before switching
        std::string previousId = dataModelManager->getSelectedStructureId();
if (!previousId.empty() && previousId != id) {
    // Save the current canvas state to the previous structure
   auto* interactionMgr = visualizationPane->getInteractionManager();
        if (interactionMgr && interactionMgr->isSyncEnabled()) {
   qDebug() << "Saving state of previous structure:" << QString::fromStdString(previousId);
            interactionMgr->saveToCurrentStructure();
       interactionMgr->saveNodePositionsToStructure();
       }
    
         // **NEW**: Immediately save to session after modification
      if (autoSaveEnabled) {
   dataModelManager->saveSession();
          qDebug() << "Session auto-saved after structure switch";
    }
     }

 dataModelManager->selectStructure(id);

        auto structures = dataModelManager->getAllStructures();
        for (const auto& meta : structures) {
   if (meta.id == id) {
   QString structureType = QString::fromStdString(meta.type);

        if (toolboxPanel) {
        toolboxPanel->setVisible(true);
        toolboxPanel->updateTools(structureType);
     qDebug() << "Toolbox shown and updated for structure type:" << structureType;
     }

  if (controlPanel) {
      controlPanel->updateAlgorithmList(structureType);
        qDebug() << "Algorithm list updated for structure type:" << structureType;
  }
         break;
         }
        }

        loadStructureIntoCanvas(id);
qDebug() << "Structure selected:" << structureId;
}
}

void MainWindow::onAlgorithmSelected(QString algorithm) {
    selectedAlgorithm = algorithm.toStdString();

    // Just store the selection without showing warnings here
    // Warnings will only show when actually trying to execute (Play click)
    qDebug() << "Algorithm selected:" << algorithm
        << "for structure:" << QString::fromStdString(dataModelManager->getSelectedStructureId());
}

void MainWindow::onPlayClicked() {
    // Check algorithm first
    if (selectedAlgorithm.empty()) {
        QMessageBox::warning(this, "No Algorithm Selected",
            "Please select an algorithm from the Algorithm dropdown before clicking Play.");
        return;
    }

    // Check structure second
    if (!dataModelManager->getSelectedStructure()) {
        QMessageBox::warning(this, "No Structure Selected",
            "Please select or create a data structure before running an algorithm.\n\n"
            "You can:\n"
            "- Create a structure from the Structure Manager\n"
            "- Click 'Create Sample Structures' for quick examples\n"
            "- Select an existing structure from the list");
        return;
    }

    // NEW: Hide toolbox and show color legend during animation
    if (toolboxPanel) {
        toolboxPanel->setVisible(false);
    }
    if (colorLegendPanel) {
        colorLegendPanel->setAlgorithmLegend(selectedAlgorithm);
        colorLegendPanel->setVisible(true);
    }

    controlPanel->setPlayingState(true);
    isAnimationPlaying = true;
    executeAlgorithm(selectedAlgorithm);
}

void MainWindow::onPauseClicked() {
    controlPanel->setPlayingState(false);
    isAnimationPlaying = false;  // NEW: Clear animation state
    if (playbackController) {
        playbackController->pause();
    }

    // NEW: Hide color legend and show toolbox when paused
    if (colorLegendPanel) {
        colorLegendPanel->setVisible(false);
    }
    if (toolboxPanel) {
        toolboxPanel->setVisible(true);
    }
}

void MainWindow::onResetClicked() {
    controlPanel->setPlayingState(false);
    isAnimationPlaying = false;  // NEW: Clear animation state
    if (playbackController) {
        playbackController->pause();
        // Reload structure to reset to initial state
        std::string structId = dataModelManager->getSelectedStructureId();
        if (!structId.empty()) {
            loadStructureIntoCanvas(structId);
        }
    }

    // NEW: Hide color legend and show toolbox when reset
    if (colorLegendPanel) {
        colorLegendPanel->setVisible(false);
    }
    if (toolboxPanel) {
        toolboxPanel->setVisible(true);
    }
}

void MainWindow::onStepForwardClicked() {
    if (playbackController && !currentAnimationFrames.empty()) {
        playbackController->stepForward();
    }
}

void MainWindow::onStepBackwardClicked() {
    if (playbackController && !currentAnimationFrames.empty()) {
        playbackController->stepBackward();
    }
}

void MainWindow::onSpeedChanged(int speed) {
    qDebug() << "MainWindow::onSpeedChanged - Slider value:" << speed;
    if (playbackController) {
        // Slider range is 1-100
        // Convert to playback speed: 1->0.1x, 50->1.0x, 100->2.0x
        // Using formula: speed = (sliderValue / 50.0f)
        float playbackSpeed = speed / 50.0f;
        qDebug() << "MainWindow::onSpeedChanged - Converted speed:" << playbackSpeed << "x";
        playbackController->setSpeed(playbackSpeed);
    }
}

// NEW: Handle animation frame rendering
void MainWindow::onFrameReady(const AnimationFrame& frame) {
    if (visualizationPane) {
        visualizationPane->renderAnimationFrame(frame);
    }
}

void MainWindow::executeAlgorithm(const std::string& algorithm) {
    DataStructure* targetStructure = dataModelManager->getSelectedStructure();
    if (!targetStructure) {
        QMessageBox::warning(this, "No Structure",
            "Please select a data structure from the Structure Selector.");
        controlPanel->setPlayingState(false);
        return;
    }

    std::string category = algoManager.getCategoryForAlgorithm(algorithm);
    qDebug() << "Executing algorithm:" << QString::fromStdString(algorithm)
        << "Category:" << QString::fromStdString(category);

    try {
        // Create algorithm with structure context
        std::unique_ptr<Algorithm> algo;

        // For tree algorithms, cast to TreeStructure
        if (category == "Tree") {
            if (TreeStructure* treeStruct = dynamic_cast<TreeStructure*>(targetStructure)) {
                // Inject tree structure into algorithm
                if (algorithm == "InOrder") {
                    algo = std::make_unique<TreeInOrder>(treeStruct);
                }
                else if (algorithm == "PreOrder") {
                    algo = std::make_unique<TreePreOrder>(treeStruct);
                }
                else if (algorithm == "PostOrder") {
                    algo = std::make_unique<TreePostOrder>(treeStruct);
                }
                else if (algorithm == "LevelOrder") {
                    algo = std::make_unique<TreeLevelOrder>(treeStruct);
                }
                else if (algorithm == "FindHeight") {
                    algo = std::make_unique<TreeFindHeight>(treeStruct);
                }
                else if (algorithm == "FindMin") {
                    algo = std::make_unique<TreeFindMin>(treeStruct);
                }
                else if (algorithm == "FindMax") {
                    algo = std::make_unique<TreeFindMax>(treeStruct);
                }
            }
        }
        // For graph algorithms, cast to GraphStructure
        else if (category == "Graph") {
            if (GraphStructure* graphStruct = dynamic_cast<GraphStructure*>(targetStructure)) {
                if (algorithm == "BFS") {
                    algo = std::make_unique<BFSAlgorithm>(graphStruct);
                }
                else if (algorithm == "DFS") {
                    algo = std::make_unique<DFSAlgorithm>(graphStruct);
                }
                else if (algorithm == "Dijkstra") {
                    algo = std::make_unique<DijkstraAlgorithm>(graphStruct);
                }
                else if (algorithm == "DetectCycle") {
                    algo = std::make_unique<GraphDetectCycle>(graphStruct);
                }
                else if (algorithm == "TopologicalSort") {
                    algo = std::make_unique<GraphTopologicalSort>(graphStruct);
                }
            }
        }
        // For sorting/filtering/transform algorithms
        else {
            if (algorithm == "InsertionSort") {
                algo = std::make_unique<InsertionSort>(targetStructure);
            }
            else if (algorithm == "SelectionSort") {
                algo = std::make_unique<SelectionSort>(targetStructure);
            }
            else if (algorithm == "RemoveDuplicates") {
                algo = std::make_unique<RemoveDuplicates>(targetStructure);
            }
            else if (algorithm == "Reverse") {
                algo = std::make_unique<Reverse>(targetStructure);
            }
            else if (algorithm == "Map") {
                algo = std::make_unique<MapTransform>(targetStructure);
            }
            else {
                // Fall back to factory creation for other algorithms
                algo = algoManager.createAlgorithm(category, algorithm);
            }
        }

        if (algo) {
            qDebug() << "Executing:" << QString::fromStdString(algorithm);

            // NEW: Execute with animation frames
            currentAnimationFrames = algo->executeWithFrames();

            if (currentAnimationFrames.empty()) {
                // Fallback: algorithm doesn't support animation yet
                qDebug() << "Algorithm doesn't generate frames yet, executing directly";
                algo->execute();

                std::string structId = dataModelManager->getSelectedStructureId();
                if (!structId.empty()) {
                    loadStructureIntoCanvas(structId);
                }
                controlPanel->setPlayingState(false);
                isAnimationPlaying = false;
            }
            else {
                // NEW: Load frames into playback controller and start animation
                qDebug() << "Loaded" << currentAnimationFrames.size() << "animation frames";
                playbackController->loadFrames(currentAnimationFrames);
                playbackController->play();
            }
        }
        else {
            qDebug() << "Algorithm not found:" << QString::fromStdString(algorithm);
            controlPanel->setPlayingState(false);
            isAnimationPlaying = false;
            QMessageBox::warning(this, "Algorithm Not Implemented",
                QString("The algorithm '%1' could not be created.")
                .arg(QString::fromStdString(algorithm)));
        }
    }
    catch (const std::exception& e) {
        qDebug() << "Algorithm execution error:" << e.what();
        controlPanel->setPlayingState(false);
        isAnimationPlaying = false;
        QMessageBox::critical(this, "Execution Error",
            QString("Error executing algorithm '%1':\n%2")
            .arg(QString::fromStdString(algorithm))
            .arg(e.what()));
    }
}

void MainWindow::onStructureRemoved(QString structureId) {
    qDebug() << "Structure removed:" << structureId;
    if (visualizationPane) {
        visualizationPane->reset();
    }

    if (dataModelManager && dataModelManager->getAllStructures().empty()) {
        if (toolboxPanel) {
            toolboxPanel->setVisible(false);
        }
    }
}

void MainWindow::onClearInteractive() {
    if (visualizationPane && visualizationPane->getInteractionManager()) {
        visualizationPane->getInteractionManager()->clearInteractive();
        visualizationPane->clearNodeValues();
        visualizationPane->refreshDisplay();
        qDebug() << "Interactive canvas cleared";
    }
}

void MainWindow::onSamplesCreated() {
    if (dataModelManager) {
        auto structures = dataModelManager->getAllStructures();
        if (!structures.empty()) {
          std::string firstId = structures[0].id;
            dataModelManager->selectStructure(firstId);

    QString structureType = QString::fromStdString(structures[0].type);
            if (toolboxPanel) {
       toolboxPanel->setVisible(true);
        toolboxPanel->updateTools(structureType);
   }
        if (controlPanel) {
         controlPanel->updateAlgorithmList(structureType);
        }

  loadStructureIntoCanvas(firstId);

            // **NEW**: Save session after creating samples
       if (autoSaveEnabled) {
          dataModelManager->saveSession();
       qDebug() << "Session auto-saved after creating samples";
            }

            qDebug() << "Sample structures created, auto-selected:" << QString::fromStdString(firstId);
        }
    }
}

void MainWindow::onShowCodeGenerator() {
    CodeGeneratorDialog dialog(dataModelManager.get(), this);
    connect(&dialog, &CodeGeneratorDialog::structureCreatedFromCode,
        this, &MainWindow::onStructureCreatedFromCode);
    dialog.exec();
}

void MainWindow::onStructureCreatedFromCode(QString structureId) {
    qDebug() << "Structure created from code:" << structureId;

    if (structureSelector) {
      structureSelector->refreshStructureList();
  }

  std::string id = structureId.toStdString();
    if (dataModelManager) {
  dataModelManager->selectStructure(id);

        auto structures = dataModelManager->getAllStructures();
        for (const auto& meta : structures) {
   if (meta.id == id) {
           QString structureType = QString::fromStdString(meta.type);

      if (toolboxPanel) {
        toolboxPanel->setVisible(true);
      toolboxPanel->updateTools(structureType);
        }

     if (controlPanel) {
     controlPanel->updateAlgorithmList(structureType);
 }
          break;
  }
        }

  loadStructureIntoCanvas(id);
        
      // **NEW**: Save session after creating structure from code
   if (autoSaveEnabled) {
        dataModelManager->saveSession();
 qDebug() << "Session auto-saved after code structure creation";
      }
    }
}

void MainWindow::onToggleMetricsPanel(bool show) {
    if (metricsPanel) {
        metricsPanel->setVisible(show);
        qDebug() << "Metrics panel" << (show ? "shown" : "hidden");
    }
}

void MainWindow::onTutorialCompleted() {
    qDebug() << "Tutorial completed";
}

void MainWindow::onTutorialSkipped() {
    qDebug() << "Tutorial skipped";
}

void MainWindow::onShowTutorial() {
    if (tutorialOverlay) {
        tutorialOverlay->setGeometry(rect());
        tutorialOverlay->start();
    }
}

void MainWindow::createMenuBar() {
    QMenu* fileMenu = menuBar()->addMenu("File");

    autoSaveAction = fileMenu->addAction("Auto-save Session");
    autoSaveAction->setCheckable(true);
    autoSaveAction->setChecked(autoSaveEnabled);
    autoSaveAction->setToolTip("Automatically save and restore session on startup");
    connect(autoSaveAction, &QAction::toggled, this, [this](bool enabled) {
        autoSaveEnabled = enabled;
        settings.setValue("autoSaveSession", enabled);

        if (enabled) {
            QMessageBox::information(this, "Auto-save Enabled",
                "Session will be automatically saved on exit and restored on startup.");
        }
        else {
            QMessageBox::information(this, "Auto-save Disabled",
                "Session will NOT be saved automatically. You can still manually save structures.");
        }
        });

    fileMenu->addSeparator();
    QAction* exitAction = fileMenu->addAction("Exit");
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    QMenu* toolsMenu = menuBar()->addMenu("Tools");
    QAction* codeGenAction = toolsMenu->addAction("Code Generator && Parser...");
    codeGenAction->setToolTip("Generate C++ code from structures or parse code to create structures");
    connect(codeGenAction, &QAction::triggered, this, &MainWindow::onShowCodeGenerator);

    toolsMenu->addSeparator();
    toggleMetricsAction = toolsMenu->addAction("Show Algorithm Metrics");
    toggleMetricsAction->setCheckable(true);
    toggleMetricsAction->setChecked(false);
    toggleMetricsAction->setToolTip("Toggle algorithm metrics panel visibility");
    connect(toggleMetricsAction, &QAction::toggled, this, &MainWindow::onToggleMetricsPanel);

    QMenu* helpMenu = menuBar()->addMenu("Help");
    showTutorialAction = helpMenu->addAction("Show Tutorial");
    showTutorialAction->setToolTip("Show the welcome tutorial again");
    connect(showTutorialAction, &QAction::triggered, this, &MainWindow::onShowTutorial);

    helpMenu->addSeparator();
    QAction* aboutAction = helpMenu->addAction("About");
    connect(aboutAction, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, "About DataViz UIT",
            "<h2>DataViz UIT</h2>"
            "<p>Version 1.0</p>"
            "<p>A modern data structure visualization tool.</p>"
            "<p>Create, visualize, and run algorithms on various data structures including:</p>"
            "<ul>"
            "<li>Arrays</li>"
            "<li>Linked Lists</li>"
            "<li>Binary Trees</li>"
            "<li>Graphs</li>"
            "</ul>");
        });
}

void MainWindow::loadStructureIntoCanvas(const std::string& structureId) {
    if (!dataModelManager || !visualizationPane) return;

    DataStructure* structure = dataModelManager->getStructure(structureId);
    if (!structure) return;

    std::string structureType;
    auto structures = dataModelManager->getAllStructures();
    for (const auto& meta : structures) {
        if (meta.id == structureId) {
            structureType = meta.type;
            break;
        }
    }

    auto nodes = structure->getNodes();
    auto edges = structure->getEdges();

    qDebug() << "Loading structure into canvas:" << QString::fromStdString(structureId)
        << "Type:" << QString::fromStdString(structureType)
        << "Nodes:" << nodes.size() << "Edges:" << edges.size()
        << "Has saved positions:" << structure->hasAnyPositions();

    auto* interactionMgr = visualizationPane->getInteractionManager();
    if (!interactionMgr) return;

    // **FIX**: Turn off sync FIRST, before any changes
    interactionMgr->setSyncWithBackend(false);
  interactionMgr->setCurrentStructureId(structureId);
    
    // **FIX**: Clear nodes/edges/values BUT preserve canvasToStructureNodeId mapping
    // We'll rebuild the mapping correctly as we load nodes
    interactionMgr->clearInteractive();
    interactionMgr->setCurrentStructureId(structureId);  // Set again after clear

    visualizationPane->clearNodeValues();

    int nodeCount = static_cast<int>(nodes.size());
    if (nodeCount == 0) {
  visualizationPane->refreshDisplay();
 interactionMgr->setSyncWithBackend(true);
        return;
    }

    std::string shape = "CIRCLE";
    if (structureType == "Array") shape = "ARRAY";
    else if (structureType == "List" || structureType == "LinkedList") shape = "RECT";  // **FIX**: Check both variants

 std::map<std::string, std::string> oldToNewId;

    bool useSavedPositions = false;
    if (structureType != "Tree" && structureType != "Binary Tree" && structureType != "BinaryTree") {
        useSavedPositions = structure->hasAnyPositions();
    }
    else {
    useSavedPositions = structure->hasAnyPositions();
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    if (structureType == "Array") {
        if (auto* arrayStruct = dynamic_cast<ArrayStructure*>(structure)) {
            const auto& arrayData = arrayStruct->getData();
    double startX = 200.0;
        double y = 300.0;
       double spacing = 50.0;

     for (size_t i = 0; i < nodes.size(); ++i) {
            double x = startX + i * spacing;

          double savedX, savedY;
          if (structure->getNodePosition(nodes[i].id, savedX, savedY)) {
         x = savedX;
   y = savedY;
           }

                std::string newId = interactionMgr->addNodeWithMapping(x, y, shape, nodes[i].id);
    oldToNewId[nodes[i].id] = newId;

        if (i < arrayData.size()) {
               interactionMgr->updateNodeValue(newId, arrayData[i]);
     }
     }
        }
    }
    else if (structureType == "List" || structureType == "LinkedList") {
        if (auto* listStruct = dynamic_cast<ListStructure*>(structure)) {
            std::vector<int> listValues;
            const ListNode* current = listStruct->getHead();
            while (current != nullptr) {
      listValues.push_back(current->value);
     current = current->next;
    }

   double startX = 150.0;
       double y = 300.0;
            double spacing = 100.0;

   std::vector<std::string> newNodeIds;
          for (size_t i = 0; i < nodes.size(); ++i) {
        double x = startX + i * spacing;

                double savedX, savedY;
     if (structure->getNodePosition(nodes[i].id, savedX, savedY)) {
         x = savedX;
       y = savedY;
     }

  std::string newId = interactionMgr->addNodeWithMapping(x, y, shape, nodes[i].id);
           oldToNewId[nodes[i].id] = newId;
 newNodeIds.push_back(newId);

     if (i < listValues.size()) {
         interactionMgr->updateNodeValue(newId, listValues[i]);
     }
            }

            for (size_t i = 0; i + 1 < newNodeIds.size(); ++i) {
    interactionMgr->addEdge(newNodeIds[i], newNodeIds[i + 1]);
            }
        }
    }
    else {
        if (structureType == "Tree" || structureType == "Binary Tree" || structureType == "BinaryTree") {
       if (auto* treeStruct = dynamic_cast<TreeStructure*>(structure)) {
       layoutTreeHierarchically(treeStruct, nodes, edges, oldToNewId, interactionMgr, structure);
  }
        }
     else {
    std::uniform_real_distribution<> xDis(150.0, 650.0);
      std::uniform_real_distribution<> yDis(100.0, 500.0);

  const double MIN_DISTANCE = 60.0;
        const int MAX_ATTEMPTS = 50;

            for (size_t i = 0; i < nodes.size(); ++i) {
           double x, y;
  bool positionFound = false;
      int attempts = 0;

    do {
         x = xDis(gen);
        y = yDis(gen);

      double savedX, savedY;
  if (structure->getNodePosition(nodes[i].id, savedX, savedY)) {
     x = savedX;
y = savedY;
     positionFound = true;
           break;
               }

      positionFound = true;
      for (size_t j = 0; j < i; ++j) {
          if (oldToNewId.count(nodes[j].id)) {
      std::string prevId = oldToNewId[nodes[j].id];
auto nodePositions = interactionMgr->getAllNodePositions();
             for (const auto& np : nodePositions) {
     if (np.id == prevId) {
   double dx = x - np.x;
         double dy = y - np.y;
    double dist = std::sqrt(dx * dx + dy * dy);
           if (dist < MIN_DISTANCE) {
               positionFound = false;
   break;
     }
       }
    }
      if (!positionFound) break;
     }
  }

      attempts++;
   } while (!positionFound && attempts < MAX_ATTEMPTS);

  std::string newId = interactionMgr->addNodeWithMapping(x, y, shape, nodes[i].id);
                oldToNewId[nodes[i].id] = newId;

              try {
    int nodeValue = std::stoi(nodes[i].value.empty() ? "0" : nodes[i].value);
  interactionMgr->updateNodeValue(newId, nodeValue);
      }
        catch (...) {
          interactionMgr->updateNodeValue(newId, 0);
                }
            }

  for (const auto& edge : edges) {
  if (oldToNewId.count(edge.from) && oldToNewId.count(edge.to)) {
 interactionMgr->addEdge(oldToNewId[edge.from], oldToNewId[edge.to]);
     }
            }
        }
    }

    if (!useSavedPositions) {
        // **FIX**: Save positions BEFORE enabling sync
        // This prevents saveToCurrentStructure from being called during position save
        interactionMgr->saveNodePositionsToStructure();
    }

    // **FIX**: Enable sync LAST, after everything is loaded correctly
    // At this point, the mapping is complete and consistent
    interactionMgr->setSyncWithBackend(true);
    
    qDebug() << "Structure loaded with" << nodes.size() << "nodes and mapping complete";
    
    visualizationPane->refreshDisplay();
}

void MainWindow::restorePreviousSession() {
    if (!dataModelManager) {
        qDebug() << "No data model manager";
        return;
    }

    // Try to load complete session
    bool sessionLoaded = dataModelManager->loadSession();

    if (sessionLoaded) {
        qDebug() << "Session restored successfully";

        // Refresh UI
        if (structureSelector) {
            structureSelector->refreshStructureList();
        }

        // Load the selected structure into canvas
        std::string selectedId = dataModelManager->getSelectedStructureId();
        if (!selectedId.empty()) {
            auto structures = dataModelManager->getAllStructures();
            for (const auto& meta : structures) {
                if (meta.id == selectedId) {
                    QString structureType = QString::fromStdString(meta.type);

                    if (toolboxPanel) {
                        toolboxPanel->setVisible(true);
                        toolboxPanel->updateTools(structureType);
                    }

                    if (controlPanel) {
                        controlPanel->updateAlgorithmList(structureType);
                    }

                    loadStructureIntoCanvas(selectedId);
                    break;
                }
            }
        }
    }
    else {
        qDebug() << "No previous session found";
    }
}

void MainWindow::layoutTreeHierarchically(
    TreeStructure* treeStruct,
    const std::vector<DSNode>& /*nodes*/,
    const std::vector<DSEdge>& /*edges*/,
    std::map<std::string, std::string>& oldToNewId,
    InteractionManager* interactionMgr,
    DataStructure* structure) {

    if (!treeStruct) return;
    TreeNode* root = treeStruct->getRoot();
    if (!root) {
        qDebug() << "layoutTreeHierarchically: NO ROOT!";
        return;
    }

    // **FIX 1**: Check if structure already has saved positions
    // If it does, use them instead of recalculating layout
    bool hasSavedPositions = structure && structure->hasAnyPositions();
  
    if (hasSavedPositions) {
        qDebug() << "Tree has saved positions - using them instead of recalculating layout";
  
        // **FIXED**: Use BFS to preserve tree structure, NOT inorder
        std::map<const TreeNode*, std::string> nodePtrToId;
        std::map<std::string, std::pair<std::string, std::string>> edgesMap;
        
        // BFS traversal to assign IDs (preserves tree level order, not value order)
        std::queue<TreeNode*> bfsQueue;
        bfsQueue.push(root);
        int counter = 0;
        
      while (!bfsQueue.empty()) {
            TreeNode* node = bfsQueue.front();
        bfsQueue.pop();
            if (!node) continue;
         
            std::string id = "tree_" + std::to_string(counter++);
            nodePtrToId[node] = id;
    
      if (node->left) {
                edgesMap[id].first = "";
   bfsQueue.push(node->left);
          }
            if (node->right) {
          edgesMap[id].second = "";
     bfsQueue.push(node->right);
   }
        }
        
        // Fill left/right ids in edgesMap
      for (const auto& p : nodePtrToId) {
            const TreeNode* node = p.first;
            std::string id = p.second;
 if (node->left) {
      auto it = nodePtrToId.find(node->left);
        if (it != nodePtrToId.end()) edgesMap[id].first = it->second;
    }
        if (node->right) {
  auto it = nodePtrToId.find(node->right);
             if (it != nodePtrToId.end()) edgesMap[id].second = it->second;
      }
     }
        
        // Create nodes using SAVED positions and VALUES from tree nodes
    for (const auto& p : nodePtrToId) {
            const TreeNode* node = p.first;
       const std::string id = p.second;
        
            double savedX, savedY;
            if (structure->getNodePosition(id, savedX, savedY)) {
          // Use saved position
            std::string newId = interactionMgr->addNodeWithMapping(savedX, savedY, "CIRCLE", id);
oldToNewId[id] = newId;
         
       // Use the ACTUAL tree node value (not reordered)
          try {
          interactionMgr->updateNodeValue(newId, node->value);
          }
             catch (...) {
        interactionMgr->updateNodeValue(newId, 0);
  }
            }
        }
      
 // **FIX**: Restore custom edges if they exist, otherwise use tree structure edges
      if (structure->hasCustomEdges()) {
            qDebug() << "Restoring" << structure->getCustomEdges().size() << "custom edges";
    const auto& customEdges = structure->getCustomEdges();
         for (const auto& edge : customEdges) {
         if (oldToNewId.count(edge.from) && oldToNewId.count(edge.to)) {
    interactionMgr->addEdge(oldToNewId[edge.from], oldToNewId[edge.to]);
             qDebug() << "Restored edge:" << QString::fromStdString(edge.from)
            << "->" << QString::fromStdString(edge.to);
  }
        }
        }
        else {
      // No custom edges - use tree structure edges
       for (const auto& kv : edgesMap) {
             const std::string parentId = kv.first;
     const std::string leftId = kv.second.first;
     const std::string rightId = kv.second.second;
  if (!leftId.empty() && oldToNewId.count(parentId) && oldToNewId.count(leftId)) {
        interactionMgr->addEdge(oldToNewId[parentId], oldToNewId[leftId]);
    }
       if (!rightId.empty() && oldToNewId.count(parentId) && oldToNewId.count(rightId)) {
                interactionMgr->addEdge(oldToNewId[parentId], oldToNewId[rightId]);
   }
            }
        }
   
        return; // Done - positions already saved
    }

    // **FIX 2**: Calculate new layout with proper root centering
    // Layout parameters
    const double HORIZONTAL_SPACING = 90.0;
    const double VERTICAL_SPACING = 120.0;
    const double START_Y = 80.0;

    // **FIXED**: Use BFS (level-order) traversal instead of inorder
    // This preserves tree structure without sorting by value
    std::map<const TreeNode*, std::string> nodePtrToId;
    std::map<std::string, std::pair<double, double>> positions;
    std::map<std::string, std::pair<std::string, std::string>> edgesMap;

    // BFS to assign indices (preserves tree structure)
    std::queue<std::pair<TreeNode*, int>> bfsQueue;
  bfsQueue.push({root, 0});
    int counter = 0;
    std::map<TreeNode*, int> nodeDepths;
    
    while (!bfsQueue.empty()) {
        auto [node, depth] = bfsQueue.front();
      bfsQueue.pop();
     if (!node) continue;
    
    std::string id = "tree_" + std::to_string(counter++);
        nodePtrToId[node] = id;
        nodeDepths[node] = depth;
        
        // Track edges
  if (node->left) {
            edgesMap[id].first = "";
     bfsQueue.push({node->left, depth + 1});
        }
        if (node->right) {
       edgesMap[id].second = "";
      bfsQueue.push({node->right, depth + 1});
   }
    }

    // Now calculate positions using inorder for X-coordinates (visual layout)
    // but keep the BFS IDs we already assigned
    counter = 0;
    std::function<void(TreeNode*, int)> assignPositions = [&](TreeNode* node, int depth) {
        if (!node) return;
     assignPositions(node->left, depth + 1);
        
        std::string id = nodePtrToId[node];
 double x = static_cast<double>(counter++) * HORIZONTAL_SPACING;
        double y = START_Y + depth * VERTICAL_SPACING;
        positions[id] = { x, y };
        
        assignPositions(node->right, depth + 1);
    };
    
    assignPositions(root, 0);

    // Fill left/right ids in edgesMap
    for (const auto& p : nodePtrToId) {
        const TreeNode* node = p.first;
        std::string id = p.second;
   if (node->left) {
     auto it = nodePtrToId.find(node->left);
     if (it != nodePtrToId.end()) edgesMap[id].first = it->second;
      }
            if (node->right) {
  auto it = nodePtrToId.find(node->right);
      if (it != nodePtrToId.end()) edgesMap[id].second = it->second;
        }
    }

    // **FIX 3**: Center the tree based on ROOT position, not bounding box
    // Find the root node ID and its X position
    std::string rootId = nodePtrToId[root];
    double rootX = positions[rootId].first;
    
    // We want the root to be at a fixed center position (e.g., 400.0)
    const double TARGET_CENTER_X = 400.0;
    double shiftX = TARGET_CENTER_X - rootX;
    
    // Shift all nodes so root is centered
    for (auto& kv : positions) {
        kv.second.first += shiftX;
    }

    // Create nodes on canvas using positions and set values from tree nodes
    for (const auto& p : nodePtrToId) {
        const TreeNode* node = p.first;
        const std::string id = p.second;
        auto posIt = positions.find(id);
        double x = TARGET_CENTER_X;
        double y = START_Y;
        if (posIt != positions.end()) { 
     x = posIt->second.first; 
            y = posIt->second.second; 
}

        std::string newId = interactionMgr->addNodeWithMapping(x, y, "CIRCLE", id);
     oldToNewId[id] = newId;
        try {
   interactionMgr->updateNodeValue(newId, node->value);
        }
        catch (...) {
 interactionMgr->updateNodeValue(newId, 0);
 }
    }

  // Add edges according to left/right mapping
    for (const auto& kv : edgesMap) {
        const std::string parentId = kv.first;
  const std::string leftId = kv.second.first;
        const std::string rightId = kv.second.second;
        if (!leftId.empty() && oldToNewId.count(parentId) && oldToNewId.count(leftId)) {
 interactionMgr->addEdge(oldToNewId[parentId], oldToNewId[leftId]);
        }
      if (!rightId.empty() && oldToNewId.count(parentId) && oldToNewId.count(rightId)) {
    interactionMgr->addEdge(oldToNewId[parentId], oldToNewId[rightId]);
        }
    }

    // Save positions into the structure for persistence
    structure->clearNodePositions();
  for (const auto& kv : positions) {
        const std::string& treeId = kv.first;
        double x = kv.second.first;
        double y = kv.second.second;
        structure->setNodePosition(treeId, x, y);
    }

}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (autoSaveEnabled) {
     // **IMPROVED**: Save current structure before saving session
    std::string currentId = dataModelManager->getSelectedStructureId();
  if (!currentId.empty()) {
     auto* interactionMgr = visualizationPane->getInteractionManager();
  if (interactionMgr && interactionMgr->isSyncEnabled()) {
        qDebug() << "Saving current structure before app close:" << QString::fromStdString(currentId);
  interactionMgr->saveToCurrentStructure();
   interactionMgr->saveNodePositionsToStructure();
      }
   }
     
   // Save complete session with all structures
      if (dataModelManager) {
    dataModelManager->saveSession();
 qDebug() << "Session saved on exit (auto-save enabled)";
        }
    }
    else {
   // **NEW**: Clear session file when auto-save is disabled
      if (dataModelManager) {
   SessionManager::clearSession();
      qDebug() << "Session cleared on exit (auto-save disabled)";
    }
    }

    event->accept();
}