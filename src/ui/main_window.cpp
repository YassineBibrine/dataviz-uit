#include "main_window.h"
#include "visualization_pane.h"
#include "control_panel.h"
#include "metrics_panel.h"
#include "toolbox_panel.h"
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
    
    // Define tutorial steps
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

    if (!dataModelManager->getSelectedStructure()) {
        QMessageBox::warning(this, "No Structure Selected",
        "Please select or create a data structure before choosing an algorithm.\n\n"
"You can:\n"
            "- Create a structure from the Structure Manager\n"
            "- Click 'Create Sample Structures' for quick examples\n"
            "- Select an existing structure from the list");
        selectedAlgorithm = "";
      return;
    }

    qDebug() << "Algorithm selected:" << algorithm
      << "for structure:" << QString::fromStdString(dataModelManager->getSelectedStructureId());
}

void MainWindow::onPlayClicked() {
    if (selectedAlgorithm.empty()) {
        QMessageBox::warning(this, "No Algorithm", "Please select an algorithm first.");
    return;
    }

    if (!dataModelManager->getSelectedStructure()) {
        QMessageBox::warning(this, "No Structure", "Please select a data structure first.");
  return;
  }

    controlPanel->setPlayingState(true);
    isAnimationPlaying = true;  // NEW: Set animation state
    executeAlgorithm(selectedAlgorithm);
}

void MainWindow::onPauseClicked() {
    controlPanel->setPlayingState(false);
    isAnimationPlaying = false;  // NEW: Clear animation state
    if (playbackController) {
        playbackController->pause();
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
    qDebug() << "Speed:" << speed;
    if (playbackController) {
   // Convert slider value (1-100) to playback speed (0.1x - 5.0x)
 float playbackSpeed = speed / 20.0f;  // 1->0.05, 50->2.5, 100->5.0
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
    } else if (algorithm == "PreOrder") {
 algo = std::make_unique<TreePreOrder>(treeStruct);
   } else if (algorithm == "PostOrder") {
   algo = std::make_unique<TreePostOrder>(treeStruct);
   } else if (algorithm == "LevelOrder") {
 algo = std::make_unique<TreeLevelOrder>(treeStruct);
  } else if (algorithm == "FindHeight") {
    algo = std::make_unique<TreeFindHeight>(treeStruct);
   } else if (algorithm == "FindMin") {
   algo = std::make_unique<TreeFindMin>(treeStruct);
     } else if (algorithm == "FindMax") {
  algo = std::make_unique<TreeFindMax>(treeStruct);
   }
   }
}
  // For graph algorithms, cast to GraphStructure
  else if (category == "Graph") {
    if (GraphStructure* graphStruct = dynamic_cast<GraphStructure*>(targetStructure)) {
   if (algorithm == "BFS") {
algo = std::make_unique<BFSAlgorithm>(graphStruct);
   } else if (algorithm == "DFS") {
  algo = std::make_unique<DFSAlgorithm>(graphStruct);
      } else if (algorithm == "Dijkstra") {
      algo = std::make_unique<DijkstraAlgorithm>(graphStruct);
 } else if (algorithm == "DetectCycle") {
 algo = std::make_unique<GraphDetectCycle>(graphStruct);
 } else if (algorithm == "TopologicalSort") {
  algo = std::make_unique<GraphTopologicalSort>(graphStruct);
      }
  }
    }
   // For sorting/filtering/transform algorithms
  else {
  if (algorithm == "InsertionSort") {
     algo = std::make_unique<InsertionSort>(targetStructure);
 } else if (algorithm == "SelectionSort") {
      algo = std::make_unique<SelectionSort>(targetStructure);
 } else if (algorithm == "RemoveDuplicates") {
  algo = std::make_unique<RemoveDuplicates>(targetStructure);
   } else if (algorithm == "Reverse") {
algo = std::make_unique<Reverse>(targetStructure);
  } else if (algorithm == "Map") {
 algo = std::make_unique<MapTransform>(targetStructure);
      } else {
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
         } else {
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
    } catch (const std::exception& e) {
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
        } else {
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

    interactionMgr->setSyncWithBackend(false);
  interactionMgr->setCurrentStructureId(structureId);
    interactionMgr->clearInteractive();
  interactionMgr->setCurrentStructureId(structureId);
    
    visualizationPane->clearNodeValues();
    
    int nodeCount = static_cast<int>(nodes.size());
    if (nodeCount == 0) {
        visualizationPane->refreshDisplay();
  interactionMgr->setSyncWithBackend(true);
        return;
    }

    std::string shape = (structureType == "Array" || structureType == "List") ? "RECT" : "CIRCLE";
    std::map<std::string, std::string> oldToNewId;
    
    bool useSavedPositions = structure->hasAnyPositions();
    
    std::random_device rd;
    std::mt19937 gen(rd());
    
    if (structureType == "Array") {
 if (auto* arrayStruct = dynamic_cast<ArrayStructure*>(structure)) {
     const auto& arrayData = arrayStruct->getData();
            double startX = 200.0;
            double y = 300.0;
   double spacing = 80.0;
 
        for (size_t i = 0; i < nodes.size(); ++i) {
   double x = startX + i * spacing;
      
   // Check for saved position using original node ID
 double savedX, savedY;
                if (structure->getNodePosition(nodes[i].id, savedX, savedY)) {
    x = savedX;
   y = savedY;
                }
          
      // Use addNodeWithMapping to track original node ID
      std::string newId = interactionMgr->addNodeWithMapping(x, y, shape, nodes[i].id);
           oldToNewId[nodes[i].id] = newId;
     
        if (i < arrayData.size()) {
      interactionMgr->updateNodeValue(newId, arrayData[i]);
    }
    }
  }
    }
    else if (structureType == "List") {
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
        // Check if this is a tree structure for hierarchical layout
        if (structureType == "Tree" || structureType == "Binary Tree" || structureType == "BinaryTree") {
      // Use hierarchical tree layout for visual tree structure
            if (auto* treeStruct = dynamic_cast<TreeStructure*>(structure)) {
            layoutTreeHierarchically(treeStruct, nodes, edges, oldToNewId, interactionMgr, structure);
   }
     } else {
            // Random layout for graphs with collision detection
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
     } catch (...) {
        interactionMgr->updateNodeValue(newId, 0);
   }
            }
 
         // Add edges using the mapped IDs
        for (const auto& edge : edges) {
                if (oldToNewId.count(edge.from) && oldToNewId.count(edge.to)) {
     interactionMgr->addEdge(oldToNewId[edge.from], oldToNewId[edge.to]);
   }
}
   }
    }

    // Save initial positions if this is a new structure without positions
    if (!useSavedPositions) {
interactionMgr->saveNodePositionsToStructure();
    }
    
    interactionMgr->setSyncWithBackend(true);
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
    } else {
qDebug() << "No previous session found";
    }
}

void MainWindow::layoutTreeHierarchically(
    TreeStructure* treeStruct,
    const std::vector<DSNode>& nodes,
    const std::vector<DSEdge>& edges,
    std::map<std::string, std::string>& oldToNewId,
    InteractionManager* interactionMgr,
    DataStructure* structure) {
    
    if (!treeStruct || nodes.empty()) return;
    
    // Build adjacency map for tree navigation
    std::map<std::string, std::vector<std::string>> children;
    std::map<std::string, std::string> parent;
    
    for (const auto& edge : edges) {
        children[edge.from].push_back(edge.to);
      parent[edge.to] = edge.from;
    }
    
    // Find root (node with no parent)
    std::string rootId;
    for (const auto& node : nodes) {
      if (parent.find(node.id) == parent.end()) {
  rootId = node.id;
            break;
   }
    }
    
    if (rootId.empty() && !nodes.empty()) {
        rootId = nodes[0].id; // Fallback to first node
    }
    
    // Calculate tree dimensions for layout
 const double HORIZONTAL_SPACING = 80.0;
    const double VERTICAL_SPACING = 100.0;
    const double START_X = 400.0;
    const double START_Y = 100.0;
    
    // Build subtree widths map
 std::map<std::string, int> subtreeWidths;
    std::function<int(const std::string&)> calculateSubtreeWidth = [&](const std::string& nodeId) -> int {
        if (subtreeWidths.count(nodeId)) return subtreeWidths[nodeId];
     
        if (children.find(nodeId) == children.end() || children[nodeId].empty()) {
   subtreeWidths[nodeId] = 1;
        return 1;
        }
      
        int totalWidth = 0;
        for (const auto& childId : children[nodeId]) {
   totalWidth += calculateSubtreeWidth(childId);
        }
      
        subtreeWidths[nodeId] = std::max(1, totalWidth);
      return subtreeWidths[nodeId];
    };
 
    calculateSubtreeWidth(rootId);
    
    // Position nodes using BFS with horizontal spacing based on subtree widths
    std::map<std::string, std::pair<double, double>> positions;
    
    std::function<void(const std::string&, double, double, double)> layoutNode = 
 [&](const std::string& nodeId, double x, double y, double width) {
  
        // Check for saved position first
double savedX, savedY;
        if (structure->getNodePosition(nodeId, savedX, savedY)) {
 positions[nodeId] = {savedX, savedY};
  } else {
  positions[nodeId] = {x, y};
   }
    
        if (children.find(nodeId) == children.end() || children[nodeId].empty()) {
            return;
        }
  
        const auto& childList = children[nodeId];
        double totalChildWidth = 0;
        for (const auto& childId : childList) {
   totalChildWidth += subtreeWidths[childId];
      }
    
        double childX = x - (totalChildWidth * HORIZONTAL_SPACING) / 2.0;
        double childY = y + VERTICAL_SPACING;
        
 for (const auto& childId : childList) {
 double childWidth = subtreeWidths[childId] * HORIZONTAL_SPACING;
   double childCenterX = childX + childWidth / 2.0;

     layoutNode(childId, childCenterX, childY, childWidth);
          childX += childWidth;
        }
    };
    
    layoutNode(rootId, START_X, START_Y, subtreeWidths[rootId] * HORIZONTAL_SPACING);
    
    // Create nodes with calculated positions
    for (const auto& node : nodes) {
        double x = START_X;
        double y = START_Y;
    
        if (positions.count(node.id)) {
            x = positions[node.id].first;
y = positions[node.id].second;
  }
  
        std::string newId = interactionMgr->addNodeWithMapping(x, y, "CIRCLE", node.id);
    oldToNewId[node.id] = newId;
        
    try {
  int nodeValue = std::stoi(node.value.empty() ? "0" : node.value);
     interactionMgr->updateNodeValue(newId, nodeValue);
        } catch (...) {
        interactionMgr->updateNodeValue(newId, 0);
    }
    }
    
    // Add edges
    for (const auto& edge : edges) {
        if (oldToNewId.count(edge.from) && oldToNewId.count(edge.to)) {
 interactionMgr->addEdge(oldToNewId[edge.from], oldToNewId[edge.to]);
 }
    }
}

void MainWindow::repositionTreeNodesAfterEdit(const std::string& structureId) {
    if (!dataModelManager || !visualizationPane) return;
    
    DataStructure* structure = dataModelManager->getStructure(structureId);
    if (!structure) return;
    
    // Get structure type
    std::string structureType;
    auto structures = dataModelManager->getAllStructures();
    for (const auto& meta : structures) {
      if (meta.id == structureId) {
          structureType = meta.type;
            break;
        }
    }
  
    // Only proceed for tree structures
    if (structureType != "Tree" && structureType != "Binary Tree" && structureType != "BinaryTree") {
 return;
    }
    
    auto* treeStruct = dynamic_cast<TreeStructure*>(structure);
    if (!treeStruct) return;
    
    auto* interactionMgr = visualizationPane->getInteractionManager();
 if (!interactionMgr) return;
 
    // Get current nodes and edges from tree structure
    auto nodes = structure->getNodes();
    auto edges = structure->getEdges();
    
    if (nodes.empty()) return;
    
    // Build adjacency map for tree navigation
std::map<std::string, std::vector<std::string>> children;
    std::map<std::string, std::string> parent;
  
    for (const auto& edge : edges) {
        children[edge.from].push_back(edge.to);
        parent[edge.to] = edge.from;
    }
    
 // Find root (node with no parent)
    std::string rootId;
    for (const auto& node : nodes) {
        if (parent.find(node.id) == parent.end()) {
  rootId = node.id;
         break;
        }
    }
    
    if (rootId.empty() && !nodes.empty()) {
      rootId = nodes[0].id;
    }
    
    // Calculate tree dimensions for layout
    const double HORIZONTAL_SPACING = 80.0;
    const double VERTICAL_SPACING = 100.0;
    const double START_X = 400.0;
    const double START_Y = 100.0;
    
    // Build subtree widths map
    std::map<std::string, int> subtreeWidths;
    std::function<int(const std::string&)> calculateSubtreeWidth = [&](const std::string& nodeId) -> int {
      if (subtreeWidths.count(nodeId)) return subtreeWidths[nodeId];
   
        if (children.find(nodeId) == children.end() || children[nodeId].empty()) {
     subtreeWidths[nodeId] = 1;
     return 1;
        }
        
 int totalWidth = 0;
      for (const auto& childId : children[nodeId]) {
    totalWidth += calculateSubtreeWidth(childId);
        }
   
        subtreeWidths[nodeId] = std::max(1, totalWidth);
        return subtreeWidths[nodeId];
    };

    calculateSubtreeWidth(rootId);
    
    // Position nodes using BFS with horizontal spacing based on subtree widths
    std::map<std::string, std::pair<double, double>> positions;
    
    std::function<void(const std::string&, double, double, double)> layoutNode = 
        [&](const std::string& nodeId, double x, double y, double width) {
 
        // Check for saved position first
        double savedX, savedY;
    if (structure->getNodePosition(nodeId, savedX, savedY)) {
            positions[nodeId] = {savedX, savedY};
        } else {
            positions[nodeId] = {x, y};
        }
  
        if (children.find(nodeId) == children.end() || children[nodeId].empty()) {
            return;
        }
  
      const auto& childList = children[nodeId];
        double totalChildWidth = 0;
        for (const auto& childId : childList) {
          totalChildWidth += subtreeWidths[childId];
        }
        
        double childX = x - (totalChildWidth * HORIZONTAL_SPACING) / 2.0;
        double childY = y + VERTICAL_SPACING;
  
        for (const auto& childId : childList) {
            double childWidth = subtreeWidths[childId] * HORIZONTAL_SPACING;
     double childCenterX = childX + childWidth / 2.0;

      layoutNode(childId, childCenterX, childY, childWidth);
          childX += childWidth;
  }
    };
    
    layoutNode(rootId, START_X, START_Y, subtreeWidths[rootId] * HORIZONTAL_SPACING);
    
    // Update positions for EXISTING canvas nodes
    // Build mapping from tree node IDs to canvas node IDs
    std::map<std::string, std::string> treeToCanvasId;
    auto canvasPositions = interactionMgr->getAllNodePositions();
    auto nodeValuesMap = interactionMgr->getNodeValues();
  
    // Match canvas nodes to tree nodes by value
    for (const auto& canvasNode : canvasPositions) {
        auto valIt = nodeValuesMap.find(canvasNode.id);
        if (valIt != nodeValuesMap.end()) {
 int canvasValue = valIt->second;
       
     // Find matching tree node by value
          for (const auto& treeNode : nodes) {
     try {
  int treeValue = std::stoi(treeNode.value);
         if (treeValue == canvasValue) {
     treeToCanvasId[treeNode.id] = canvasNode.id;
        break;
       }
     } catch (...) {}
     }
        }
    }
    
    // Update canvas node positions to match calculated tree layout
    interactionMgr->setSyncWithBackend(false);  // Temporarily disable sync
    
    for (const auto& [treeId, canvasId] : treeToCanvasId) {
     if (positions.count(treeId)) {
         double newX = positions[treeId].first;
            double newY = positions[treeId].second;
    
            // Update node position in interaction manager
    interactionMgr->updateNodePosition(canvasId, newX, newY);
        }
  }
    
    // Add missing edges from tree structure to canvas
    // First, get existing canvas edges
    auto existingEdges = interactionMgr->getAllEdges();
    std::set<std::pair<std::string, std::string>> existingEdgeSet;
    for (const auto& edge : existingEdges) {
        existingEdgeSet.insert({edge.source, edge.target});
    }
    
    // Add edges that exist in tree but not in canvas
    for (const auto& treeEdge : edges) {
        // Map tree edge to canvas edge
     auto fromIt = treeToCanvasId.find(treeEdge.from);
        auto toIt = treeToCanvasId.find(treeEdge.to);
        
      if (fromIt != treeToCanvasId.end() && toIt != treeToCanvasId.end()) {
  std::string canvasFrom = fromIt->second;
            std::string canvasTo = toIt->second;
  
            // Check if this edge already exists
  if (existingEdgeSet.find({canvasFrom, canvasTo}) == existingEdgeSet.end()) {
        // Edge doesn't exist, add it
     interactionMgr->addEdge(canvasFrom, canvasTo);
 qDebug() << "Added missing edge:" << QString::fromStdString(canvasFrom) 
        << "->" << QString::fromStdString(canvasTo);
   }
        }
    }
    
    interactionMgr->setSyncWithBackend(true);
    interactionMgr->saveNodePositionsToStructure();
    
    // Refresh visualization
    visualizationPane->refreshDisplay();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    // Save complete session with all structures (only if auto-save is enabled)
    if (dataModelManager && autoSaveEnabled) {
        dataModelManager->saveSession();
        qDebug() << "Session saved on exit (auto-save enabled)";
    } else if (!autoSaveEnabled) {
        qDebug() << "Session NOT saved (auto-save disabled)";
    }

    event->accept();
}