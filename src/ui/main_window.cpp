#include "main_window.h"
#include "visualization_pane.h"
#include "control_panel.h"
#include "metrics_panel.h"
#include "toolbox_panel.h"
#include "structure_selector.h"
#include "code_generator_dialog.h"
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
#include <QScrollArea>
#include <cmath>
#include <functional>
#include <queue>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    dataModelManager(std::make_unique<DataModelManager>()),
    visualizationPane(std::make_unique<VisualizationPane>(this)),
    controlPanel(std::make_unique<ControlPanel>(this)),
    metricsPanel(std::make_unique<MetricsPanel>(this)),
    toolboxPanel(nullptr),
    structureSelector(nullptr),
    algoManager(AlgorithmManager::getInstance()),
    frameRecorder(),
    currentAlgorithm(nullptr),
    toggleMetricsAction(nullptr)
{
    setWindowTitle("DataViz UIT - Visualisation de Structures de Données");
    resize(1350, 900);
    setMinimumSize(1200, 750);

    if (visualizationPane && visualizationPane->getInteractionManager()) {
        visualizationPane->getInteractionManager()->setBackend(dataModelManager.get());
    }

    setupUI();
    connectSignals();
    createMenuBar();
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
    toolboxPanel->setVisible(false);  // Hidden initially
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

    // Metrics Panel
    metricsPanel->setObjectName("borderedPanel");
    rightLayout->addWidget(metricsPanel.get());
    rightLayout->addStretch();

    rightScrollArea->setWidget(rightContainer);
    rightScrollArea->setFixedWidth(400);
    contentLayout->addWidget(rightScrollArea);

    mainLayout->addLayout(contentLayout, 1);

    connect(toolboxPanel, &ToolboxPanel::toolSelected,
        visualizationPane.get(), &VisualizationPane::setInteractionMode);
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
       
    // Show and update toolbox for the selected structure type
     if (toolboxPanel) {
           toolboxPanel->setVisible(true);  // Show toolbox when structure is selected
    toolboxPanel->updateTools(structureType);
         qDebug() << "Toolbox shown and updated for structure type:" << structureType;
    }
         
     // Update algorithm list for the selected structure type
    if (controlPanel) {
         controlPanel->updateAlgorithmList(structureType);
qDebug() << "Algorithm list updated for structure type:" << structureType;
      }
 break;
        }
        }
        
    // Load structure into the interactive canvas so user can interact with it
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
            "• Create a structure from the Control Panel\n"
            "• Draw nodes/edges and finalize them\n"
            "• Select an existing structure from the Structure list");
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
    toggleMetricsAction->setChecked(true);
    toggleMetricsAction->setToolTip("Toggle algorithm metrics panel visibility");
    connect(toggleMetricsAction, &QAction::toggled, this, &MainWindow::onToggleMetricsPanel);

    QMenu* helpMenu = menuBar()->addMenu("Help");
    helpMenu->addAction("About");
}

void MainWindow::executeAlgorithm(const std::string& algorithm) {
    DataStructure* targetStructure = dataModelManager->getSelectedStructure();
    if (!targetStructure) {
     QMessageBox::warning(this, "No Structure",
  "Please select a data structure from the Structure Selector.");
        controlPanel->setPlayingState(false);
    return;
    }

 // Get the correct category for this algorithm
    std::string category = algoManager.getCategoryForAlgorithm(algorithm);
 qDebug() << "Executing algorithm:" << QString::fromStdString(algorithm) 
       << "Category:" << QString::fromStdString(category);

    try {
     auto algo = algoManager.createAlgorithm(category, algorithm);
        if (algo) {
            qDebug() << "Executing:" << QString::fromStdString(algorithm);
       algo->execute();
  
 // Update visualization after algorithm execution
            // Reload the structure to show any changes
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
             "• Sorting: BubbleSort, QuickSort, MergeSort\n"
       "• Filtering: RangeFilter\n"
           "• Transform: Normalize")
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
    
    // Hide toolbox if no structures remain
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
    // Auto-select the first sample structure and visualize it
    if (dataModelManager) {
        auto structures = dataModelManager->getAllStructures();
if (!structures.empty()) {
  // Select the first structure
         std::string firstId = structures[0].id;
            dataModelManager->selectStructure(firstId);
   
     // Update UI
   QString structureType = QString::fromStdString(structures[0].type);
      if (toolboxPanel) {
      toolboxPanel->setVisible(true);
        toolboxPanel->updateTools(structureType);
            }
 if (controlPanel) {
             controlPanel->updateAlgorithmList(structureType);
      }
         
       // Load into canvas
            loadStructureIntoCanvas(firstId);
   
            qDebug() << "Sample structures created, auto-selected:" << QString::fromStdString(firstId);
  }
    }
}

void MainWindow::loadStructureIntoCanvas(const std::string& structureId) {
    if (!dataModelManager || !visualizationPane) return;

    DataStructure* structure = dataModelManager->getStructure(structureId);
    if (!structure) return;

    // Get structure metadata to determine type
    std::string structureType;
    auto structures = dataModelManager->getAllStructures();
    for (const auto& meta : structures) {
        if (meta.id == structureId) {
   structureType = meta.type;
       break;
        }
    }

    // Get nodes and edges from the structure
    auto nodes = structure->getNodes();
    auto edges = structure->getEdges();
    
    qDebug() << "Loading structure into canvas:" << QString::fromStdString(structureId)
        << "Type:" << QString::fromStdString(structureType)
        << "Nodes:" << nodes.size() << "Edges:" << edges.size();

// Get interaction manager and clear existing data
    auto* interactionMgr = visualizationPane->getInteractionManager();
    if (!interactionMgr) return;
    
 interactionMgr->clearInteractive();
    visualizationPane->clearNodeValues();
    
    int nodeCount = nodes.size();
 if (nodeCount == 0) {
        visualizationPane->refreshDisplay();
  return;
    }

    // Determine shape based on type
    std::string shape = (structureType == "Array" || structureType == "List") ? "RECT" : "CIRCLE";
    
    // Map from old node IDs to new node IDs
    std::map<std::string, std::string> oldToNewId;
    
    // Calculate layout positions and add nodes to interaction manager
    if (structureType == "Array") {
        if (auto* arrayStruct = dynamic_cast<ArrayStructure*>(structure)) {
            const auto& arrayData = arrayStruct->getData();
   double startX = 200.0;
            double y = 300.0;
            double spacing = 80.0;
 
  for (size_t i = 0; i < nodes.size(); ++i) {
     double x = startX + i * spacing;
       std::string newId = interactionMgr->addNode(x, y, shape);
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
      std::string newId = interactionMgr->addNode(x, y, shape);
      oldToNewId[nodes[i].id] = newId;
     newNodeIds.push_back(newId);
  
   if (i < listValues.size()) {
    interactionMgr->updateNodeValue(newId, listValues[i]);
         }
      }
   
            // Add edges for list (sequential)
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
     return;
     }
     
            // Calculate tree layout using level-order traversal
          // Map node value to position info
         std::map<int, std::pair<double, double>> nodePositions;
            std::map<int, std::string> valueToNewId;
       
      double startX = 400.0;// Center X
    double startY = 100.0;  // Top Y
            double levelHeight = 80.0;
double baseWidth = 400.0;  // Width at the deepest level
            
 // BFS to assign positions level by level
            std::queue<std::tuple<const TreeNode*, int, double, double>> bfsQueue;
            // node, level, x position, width for children
   bfsQueue.push({root, 0, startX, baseWidth});
          
   while (!bfsQueue.empty()) {
          auto [node, level, xPos, width] = bfsQueue.front();
          bfsQueue.pop();
                
         if (!node) continue;
     
 double yPos = startY + level * levelHeight;
      nodePositions[node->value] = {xPos, yPos};
  
     // Create the node
    std::string newId = interactionMgr->addNode(xPos, yPos, shape);
                valueToNewId[node->value] = newId;
       oldToNewId["tree_" + std::to_string(node->value)] = newId;
       interactionMgr->updateNodeValue(newId, node->value);
     
          // Queue children with updated positions
     double childWidth = width / 2.0;
   if (node->left) {
               bfsQueue.push({node->left, level + 1, xPos - childWidth / 2.0, childWidth});
    }
        if (node->right) {
       bfsQueue.push({node->right, level + 1, xPos + childWidth / 2.0, childWidth});
                }
          }
 
    // Add edges from parent to children
            std::function<void(const TreeNode*)> addTreeEdges = [&](const TreeNode* node) {
   if (!node) return;
           
std::string parentId = valueToNewId[node->value];
       
     if (node->left && valueToNewId.count(node->left->value)) {
std::string leftId = valueToNewId[node->left->value];
      interactionMgr->addEdge(parentId, leftId);
        addTreeEdges(node->left);
      }
                if (node->right && valueToNewId.count(node->right->value)) {
       std::string rightId = valueToNewId[node->right->value];
           interactionMgr->addEdge(parentId, rightId);
         addTreeEdges(node->right);
        }
         };
          addTreeEdges(root);
}
    }
    else if (structureType == "Graph") {
  if (auto* graphStruct = dynamic_cast<GraphStructure*>(structure)) {
      const Graph* g = graphStruct->getGraph();
      
          // Layout: Circular
            double centerX = 400.0;
      double centerY = 300.0;
            double radius = std::min(200.0, 80.0 + nodeCount * 25.0);
      
for (size_t i = 0; i < nodes.size(); ++i) {
      double angle = (2.0 * 3.14159 * i) / nodeCount;
        double x = centerX + radius * std::cos(angle);
          double y = centerY + radius * std::sin(angle);
        
      std::string newId = interactionMgr->addNode(x, y, shape);
            oldToNewId[nodes[i].id] = newId;
       
          // Try to get value from graph properties
                if (g) {
      if (const Graph::Node* graphNode = g->getNode(nodes[i].id)) {
    auto it = graphNode->properties.find("label");
              if (it != graphNode->properties.end()) {
       try {
          interactionMgr->updateNodeValue(newId, std::stoi(it->second));
           } catch (...) {
            // Use index as value if label is not a number
    interactionMgr->updateNodeValue(newId, static_cast<int>(i));
       }
      } else {
     interactionMgr->updateNodeValue(newId, static_cast<int>(i));
        }
       }
       }
         }
  
   // Add edges
    for (const auto& edge : edges) {
           if (oldToNewId.count(edge.from) && oldToNewId.count(edge.to)) {
       interactionMgr->addEdge(oldToNewId[edge.from], oldToNewId[edge.to]);
                }
    }
        }
    }
    else {
        // Generic circular layout
        double centerX = 400.0;
        double centerY = 300.0;
        double radius = std::min(200.0, 100.0 + nodeCount * 10.0);
   
     for (size_t i = 0; i < nodes.size(); ++i) {
   double angle = (2.0 * 3.14159 * i) / nodeCount;
    double x = centerX + radius * std::cos(angle);
         double y = centerY + radius * std::sin(angle);
 
            std::string newId = interactionMgr->addNode(x, y, shape);
            oldToNewId[nodes[i].id] = newId;
        }

  // Add edges
    for (const auto& edge : edges) {
            if (oldToNewId.count(edge.from) && oldToNewId.count(edge.to)) {
           interactionMgr->addEdge(oldToNewId[edge.from], oldToNewId[edge.to]);
          }
        }
    }
    
    // Force refresh the display to show the loaded structure
    visualizationPane->refreshDisplay();
}

// Remove or simplify updateVisualizationForStructure since loadStructureIntoCanvas now handles this
void MainWindow::updateVisualizationForStructure(const std::string& structureId) {
    // Now just delegate to loadStructureIntoCanvas
    loadStructureIntoCanvas(structureId);
}

void MainWindow::closeEvent(QCloseEvent* e) {
    e->accept();
}

void MainWindow::onShowCodeGenerator() {
    CodeGeneratorDialog dialog(dataModelManager.get(), this);
    connect(&dialog, &CodeGeneratorDialog::structureCreatedFromCode,
        this, &MainWindow::onStructureCreatedFromCode);
    dialog.exec();
}

void MainWindow::onStructureCreatedFromCode(QString structureId) {
    if (structureSelector) {
        structureSelector->refreshStructureList();
    }
    updateVisualizationForStructure(structureId.toStdString());
    qDebug() << "Structure created from code:" << structureId;
}

void MainWindow::onToggleMetricsPanel(bool show) {
    if (metricsPanel) {
        metricsPanel->setVisible(show);
        if (toggleMetricsAction) {
            toggleMetricsAction->setText(show ? "Hide Algorithm Metrics" : "Show Algorithm Metrics");
        }
        qDebug() << "Metrics panel" << (show ? "shown" : "hidden");
    }
}