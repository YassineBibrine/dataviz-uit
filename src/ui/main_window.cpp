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

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
 dataModelManager(std::make_unique<DataModelManager>()),
    visualizationPane(std::make_unique<VisualizationPane>(this)),
    controlPanel(std::make_unique<ControlPanel>(this)),
    metricsPanel(std::make_unique<MetricsPanel>(this)),
    toolboxPanel(nullptr),
    structureSelector(nullptr),
    tutorialOverlay(nullptr),
    algoManager(AlgorithmManager::getInstance()),
    frameRecorder(),
    currentAlgorithm(nullptr),
 toggleMetricsAction(nullptr),
    showTutorialAction(nullptr),
    settings("DataVizUIT", "DataVizUIT")
{
    setWindowTitle("DataViz UIT - Data Structure Visualization");
    resize(1350, 900);
    setMinimumSize(1200, 750);

    if (visualizationPane && visualizationPane->getInteractionManager()) {
        visualizationPane->getInteractionManager()->setBackend(dataModelManager.get());
}

    setupUI();
    connectSignals();
    createMenuBar();
    // ✅ RESTAURATION SESSION
    restorePreviousSession();
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
    executeAlgorithm(selectedAlgorithm);
}

void MainWindow::onPauseClicked() {
    controlPanel->setPlayingState(false);
}

void MainWindow::onResetClicked() {
    controlPanel->setPlayingState(false);
}

void MainWindow::onStepForwardClicked() {}
void MainWindow::onStepBackwardClicked() {}

void MainWindow::onSpeedChanged(int speed) {
    qDebug() << "Speed:" << speed;
}

void MainWindow::createMenuBar() {
    QMenu* fileMenu = menuBar()->addMenu("File");
    QAction* exitAction = fileMenu->addAction("Exit");
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    QMenu* toolsMenu = menuBar()->addMenu("Tools");
    QAction* codeGenAction = toolsMenu->addAction("Code Generator && Parser...");
    codeGenAction->setToolTip("Generate C++ code from structures or parse code to create structures");
    connect(codeGenAction, &QAction::triggered, this, &MainWindow::onShowCodeGenerator);

    toolsMenu->addSeparator();
    toggleMetricsAction = toolsMenu->addAction("Show Algorithm Metrics");
    toggleMetricsAction->setCheckable(true);
    toggleMetricsAction->setChecked(false);  // Unchecked by default (metrics hidden)
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
        auto algo = algoManager.createAlgorithm(category, algorithm);
      if (algo) {
          qDebug() << "Executing:" << QString::fromStdString(algorithm);
       algo->execute();
  
            std::string structId = dataModelManager->getSelectedStructureId();
  if (!structId.empty()) {
    loadStructureIntoCanvas(structId);
            }
       
            controlPanel->setPlayingState(false);
  QMessageBox::information(this, "Algorithm Complete",
         QString("Algorithm '%1' completed successfully.")
         .arg(QString::fromStdString(algorithm)));
 }
        else {
            qDebug() << "Algorithm not found:" << QString::fromStdString(algorithm);
  controlPanel->setPlayingState(false);
            QMessageBox::warning(this, "Algorithm Not Implemented",
 QString("The algorithm '%1' is not yet implemented.\n\n"
  "Currently implemented algorithms:\n"
  "- Sorting: BubbleSort, QuickSort, MergeSort\n"
         "- Filtering: RangeFilter\n"
       "- Transform: Normalize")
       .arg(QString::fromStdString(algorithm)));
     }
    } catch (const std::exception& e) {
        qDebug() << "Algorithm execution error:" << e.what();
 controlPanel->setPlayingState(false);
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
    else if (structureType == "Binary Tree" || structureType == "Tree") {
        if (auto* treeStruct = dynamic_cast<TreeStructure*>(structure)) {
            const TreeNode* root = treeStruct->getRoot();
            if (!root) {
    visualizationPane->refreshDisplay();
    interactionMgr->setSyncWithBackend(true);
     return;
      }
       
      // Use BFS to match how getNodes() generates IDs (tree_0, tree_1, etc.)
            std::map<const TreeNode*, int> nodeToIndex;
  std::map<const TreeNode*, std::pair<double, double>> nodePositions;
          std::map<const TreeNode*, std::string> nodeToCanvasId;
    
         // First pass: assign indices using BFS (matches tree_structure.cpp)
   std::queue<const TreeNode*> indexQueue;
   indexQueue.push(root);
 int index = 0;
         while (!indexQueue.empty()) {
              const TreeNode* node = indexQueue.front();
 indexQueue.pop();
       if (node) {
            nodeToIndex[node] = index++;
       if (node->left) indexQueue.push(node->left);
         if (node->right) indexQueue.push(node->right);
       }
 }
        
            // Calculate tree depth for proper layout scaling
            std::function<int(const TreeNode*)> getDepth = [&](const TreeNode* n) -> int {
    if (!n) return 0;
    return 1 + std::max(getDepth(n->left), getDepth(n->right));
            };
       int treeDepth = getDepth(root);
    
            // Adjust layout parameters based on tree size
      double canvasWidth = 800.0;
            double canvasHeight = 600.0;
            double startX = canvasWidth / 2.0;
      double startY = 50.0;
   
            // Scale level height based on depth to fit canvas
            double levelHeight = std::min(80.0, (canvasHeight - 100.0) / std::max(treeDepth, 1));
          
            // Scale base width based on tree size
    double baseWidth = std::min(canvasWidth - 100.0, 100.0 * std::pow(2, treeDepth - 1));
   
          // Second pass: calculate positions using BFS with proper layout
   std::queue<std::tuple<const TreeNode*, int, double, double>> bfsQueue;
            bfsQueue.push({root, 0, startX, baseWidth});
            
 while (!bfsQueue.empty()) {
                auto [node, level, xPos, width] = bfsQueue.front();
  bfsQueue.pop();
      
            if (!node) continue;
     
  double yPos = startY + level * levelHeight;
          
     // Get the correct node ID (tree_INDEX, not tree_VALUE)
     std::string originalNodeId = "tree_" + std::to_string(nodeToIndex[node]);
                
      // Check for saved position
    double savedX, savedY;
if (structure->getNodePosition(originalNodeId, savedX, savedY)) {
            xPos = savedX;
   yPos = savedY;
        }
        
    nodePositions[node] = {xPos, yPos};
 
         std::string newId = interactionMgr->addNodeWithMapping(xPos, yPos, shape, originalNodeId);
      nodeToCanvasId[node] = newId;
       oldToNewId[originalNodeId] = newId;
            interactionMgr->updateNodeValue(newId, node->value);
        
                // Calculate child width - decrease as we go deeper
        double childWidth = width / 2.0;
        // Ensure minimum spacing between siblings
  double minSpacing = 40.0;
     childWidth = std::max(childWidth, minSpacing);
           
    if (node->left) {
  bfsQueue.push({node->left, level + 1, xPos - childWidth / 2.0, childWidth});
     }
   if (node->right) {
     bfsQueue.push({node->right, level + 1, xPos + childWidth / 2.0, childWidth});
 }
    }
     
            // Check if we have custom edges saved (user-drawn edges)
            if (structure->hasCustomEdges()) {
          // Use custom edges instead of BST-generated edges
         const auto& customEdges = structure->getCustomEdges();
      for (const auto& edge : customEdges) {
         if (oldToNewId.count(edge.from) && oldToNewId.count(edge.to)) {
           interactionMgr->addEdge(oldToNewId[edge.from], oldToNewId[edge.to]);
   }
   }
qDebug() << "Loaded" << customEdges.size() << "custom edges for tree";
       } else {
                // No custom edges - use BST structure edges
         std::function<void(const TreeNode*)> addTreeEdges = [&](const TreeNode* node) {
         if (!node) return;
         
auto parentIt = nodeToCanvasId.find(node);
      if (parentIt == nodeToCanvasId.end()) return;
         std::string parentId = parentIt->second;
    
                    if (node->left) {
   auto leftIt = nodeToCanvasId.find(node->left);
           if (leftIt != nodeToCanvasId.end()) {
   interactionMgr->addEdge(parentId, leftIt->second);
  addTreeEdges(node->left);
           }
        }
         if (node->right) {
     auto rightIt = nodeToCanvasId.find(node->right);
 if (rightIt != nodeToCanvasId.end()) {
            interactionMgr->addEdge(parentId, rightIt->second);
   addTreeEdges(node->right);
            }
   }
    };
 addTreeEdges(root);
            }
        }
    }
    else if (structureType == "Graph") {
        if (auto* graphStruct = dynamic_cast<GraphStructure*>(structure)) {
            const Graph* g = graphStruct->getGraph();
            
         double minX = 150.0, maxX = 650.0;
     double minY = 100.0, maxY = 500.0;
            double minDistance = 80.0;
 int maxAttempts = 50;
         
        std::uniform_real_distribution<> disX(minX, maxX);
      std::uniform_real_distribution<> disY(minY, maxY);
        
    std::vector<std::pair<double, double>> placedPositions;
         
            for (size_t i = 0; i < nodes.size(); ++i) {
     double x, y;
        
     // Check for saved position using original node ID
         double savedX, savedY;
                if (structure->getNodePosition(nodes[i].id, savedX, savedY)) {
      x = savedX;
             y = savedY;
      } else {
 bool validPosition = false;
            int attempts = 0;
      
       while (!validPosition && attempts < maxAttempts) {
        x = disX(gen);
    y = disY(gen);
   validPosition = true;
    
             for (const auto& pos : placedPositions) {
    double dx = x - pos.first;
double dy = y - pos.second;
             double distance = std::sqrt(dx * dx + dy * dy);
    
            if (distance < minDistance) {
      validPosition = false;
          break;
    }
             }
      attempts++;
  }

   if (!validPosition) {
      int cols = static_cast<int>(std::ceil(std::sqrt(nodeCount)));
        int row = static_cast<int>(i) / cols;
                int col = static_cast<int>(i) % cols;
       
          double gridSpacingX = (maxX - minX) / std::max(cols, 1);
            double gridSpacingY = (maxY - minY) / std::max((nodeCount + cols - 1) / cols, 1);
      
   x = minX + col * gridSpacingX + gridSpacingX / 2;
           y = minY + row * gridSpacingY + gridSpacingY / 2;
        
  std::uniform_real_distribution<> offset(-15.0, 15.0);
               x += offset(gen);
  y += offset(gen);
   }
  }
           
   placedPositions.push_back({x, y});
          
 // Use addNodeWithMapping to track original node ID
       std::string newId = interactionMgr->addNodeWithMapping(x, y, shape, nodes[i].id);
     oldToNewId[nodes[i].id] = newId;
         
       if (g) {
  if (const Graph::Node* graphNode = g->getNode(nodes[i].id)) {
  auto it = graphNode->properties.find("value");
      if (it != graphNode->properties.end()) {
         try {
    interactionMgr->updateNodeValue(newId, std::stoi(it->second));
      } catch (...) {
          interactionMgr->updateNodeValue(newId, static_cast<int>(i));
 }
           } else {
            interactionMgr->updateNodeValue(newId, static_cast<int>(i));
       }
    }
      }
            }
      
  for (const auto& edge : edges) {
  if (oldToNewId.count(edge.from) && oldToNewId.count(edge.to)) {
          interactionMgr->addEdge(oldToNewId[edge.from], oldToNewId[edge.to]);
 }
    }
        }
  }
    else {
    // Generic layout
        double minX = 150.0, maxX = 650.0;
    double minY = 100.0, maxY = 500.0;
        double minDistance = 80.0;
     int maxAttempts = 50;
      
        std::uniform_real_distribution<> disX(minX, maxX);
        std::uniform_real_distribution<> disY(minY, maxY);
        
        std::vector<std::pair<double, double>> placedPositions;
        
   for (size_t i = 0; i < nodes.size(); ++i) {
        double x, y;
   
            double savedX, savedY;
            if (structure->getNodePosition(nodes[i].id, savedX, savedY)) {
            x = savedX;
        y = savedY;
   } else {
   bool validPosition = false;
  int attempts = 0;
          
              while (!validPosition && attempts < maxAttempts) {
                x = disX(gen);
        y = disY(gen);
   validPosition = true;
         
        for (const auto& pos : placedPositions) {
          double dx = x - pos.first;
 double dy = y - pos.second;
     double distance = std::sqrt(dx * dx + dy * dy);
          
        if (distance < minDistance) {
       validPosition = false;
          break;
                     }
        }
           attempts++;
                }
            
    if (!validPosition) {
             int cols = static_cast<int>(std::ceil(std::sqrt(nodeCount)));
    int row = static_cast<int>(i) / cols;
                    int col = static_cast<int>(i) % cols;
          
       double gridSpacingX = (maxX - minX) / std::max(cols, 1);
       double gridSpacingY = (maxY - minY) / std::max((nodeCount + cols - 1) / cols, 1);
  
          x = minX + col * gridSpacingX + gridSpacingX / 2;
            y = minY + row * gridSpacingY + gridSpacingY / 2;

    std::uniform_real_distribution<> offset(-15.0, 15.0);
         x += offset(gen);
    y += offset(gen);
   }
       }
     
        placedPositions.push_back({x, y});
    
        std::string newId = interactionMgr->addNodeWithMapping(x, y, shape, nodes[i].id);
     oldToNewId[nodes[i].id] = newId;
    }
        
        for (const auto& edge : edges) {
          if (oldToNewId.count(edge.from) && oldToNewId.count(edge.to)) {
              interactionMgr->addEdge(oldToNewId[edge.from], oldToNewId[edge.to]);
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
    SessionManager session;

    if (!session.hasSession()) {
        qDebug() << "No previous session";
        return;
    }

    SessionData data = session.loadSession();

    frameRecorder.reset();
    if (visualizationPane) visualizationPane->reset();

    loadDataStructure(data.structureType, data.size);

    qDebug() << "Session restored";
}

void MainWindow::closeEvent(QCloseEvent* event) {
    SessionManager session;

    if (dataModelManager && dataModelManager->getCurrentStructure()) {
        session.saveSession(*dataModelManager->getCurrentStructure());
    }

    event->accept();
}