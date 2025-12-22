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

    // Left: Toolbox
    toolboxPanel = new ToolboxPanel(this);
    toolboxPanel->setObjectName("borderedPanel");
    toolboxPanel->setFixedWidth(110);
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
    connect(structureSelector, &StructureSelector::finalizeInteractiveRequested,
        this, &MainWindow::onFinalizeInteractive);
    connect(structureSelector, &StructureSelector::clearInteractiveRequested,
        this, &MainWindow::onClearInteractive);
    connect(structureSelector, &StructureSelector::structureSelected,
        controlPanel.get(), &ControlPanel::updateAlgorithmList);
}

void MainWindow::onStructureSelected(QString structureId) {
    std::string id = structureId.toStdString();

    if (dataModelManager) {
        dataModelManager->selectStructure(id);

        auto structures = dataModelManager->getAllStructures();
        for (const auto& meta : structures) {
            if (meta.id == id) {
                if (toolboxPanel) {
                    QString structureType = QString::fromStdString(meta.type);
                    toolboxPanel->updateTools(structureType);
                    qDebug() << "Toolbox updated for structure type:" << structureType;
                }
                break;
            }
        }
        updateVisualizationForStructure(id);
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
        return;
    }

    // ✅ Utilise getCategoryForAlgorithm ici (à implémenter dans AlgorithmManager)
    std::string category = "Sorting"; // Fallback temporaire
    // Si tu as ajouté getCategoryForAlgorithm, remplace par :
    // auto& mgr = AlgorithmManager::getInstance();
    // std::string category = mgr.getCategoryForAlgorithm(algorithm);

    auto algo = algoManager.createAlgorithm(category, algorithm);
    if (algo) {
        qDebug() << "Executing:" << QString::fromStdString(algorithm);
        algo->execute();
    }
    else {
        qDebug() << "Algorithm not found:" << QString::fromStdString(algorithm);
        QMessageBox::warning(this, "Algorithm Not Implemented",
            QString("The algorithm '%1' is not yet implemented.").arg(QString::fromStdString(algorithm)));
    }
}

void MainWindow::onStructureRemoved(QString structureId) {
    qDebug() << "Structure removed:" << structureId;
    if (visualizationPane) {
        visualizationPane->reset();
    }
}

void MainWindow::onFinalizeInteractive(QString type, QString name) {
    if (!visualizationPane || !visualizationPane->getInteractionManager()) {
        return;
    }

    auto* interactionMgr = visualizationPane->getInteractionManager();
    if (!interactionMgr->hasInteractiveData()) {
        QMessageBox::information(this, "No Data",
            "No nodes or edges to finalize.\n\n"
            "Draw some nodes using the Node tool, then connect them with the Link tool.");
        return;
    }

    auto [nodeCount, edgeCount] = interactionMgr->getInteractiveStats();
    qDebug() << "Finalizing structure:" << type << name << "Nodes:" << nodeCount << "Edges:" << edgeCount;

    std::string structId = interactionMgr->finalizeStructure(type.toStdString(), name.toStdString());

    if (!structId.empty()) {
        if (structureSelector) {
            structureSelector->refreshStructureList();
        }
        if (toolboxPanel) {
            toolboxPanel->updateTools(type);
            qDebug() << "Toolbox updated for finalized structure type:" << type;
        }
        QMessageBox::information(this, "Structure Created",
            QString("Custom structure created successfully!\n\n"
                "Type: %1\nNodes: %2\nEdges: %3\n\nThe structure is now available for algorithms.")
            .arg(type).arg(nodeCount).arg(edgeCount));
    }
    else {
        QMessageBox::warning(this, "Creation Failed", "Failed to create structure. Please try again.");
    }
}

void MainWindow::onClearInteractive() {
    if (visualizationPane && visualizationPane->getInteractionManager()) {
        visualizationPane->getInteractionManager()->clearInteractive();
        visualizationPane->update();
        qDebug() << "Interactive canvas cleared";
    }
}

void MainWindow::updateVisualizationForStructure(const std::string& structureId) {
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
    
    qDebug() << "Updating visualization for structure:" << QString::fromStdString(structureId)
        << "Type:" << QString::fromStdString(structureType)
  << "Nodes:" << nodes.size() << "Edges:" << edges.size();
    
    // DON'T clear interactive - just update the frame
    // This prevents user-drawn structures from being deleted
    
    // Create an animation frame with the structure data
    AnimationFrame frame;
    
    int nodeCount = nodes.size();
    if (nodeCount == 0) {
        // Empty structure - just clear visualization
        if (auto* renderer = visualizationPane->findChild<VisualizationRenderer*>()) {
         renderer->renderFrame(frame);
      }
        visualizationPane->update();
        return;
 }
    
    // Layout and render based on structure type
 if (structureType == "Array") {
        // Get actual array values
        if (auto* arrayStruct = dynamic_cast<ArrayStructure*>(structure)) {
       const auto& arrayData = arrayStruct->getData();
      
            // Layout: Horizontal row
            double startX = 200.0;
          double y = 300.0;
        double spacing = 80.0;
     
     for (size_t i = 0; i < nodes.size(); ++i) {
              const auto& node = nodes[i];
                double x = startX + i * spacing;
           
     frame.nodePositions[node.id] = {x, y};
        frame.nodeShapes[node.id] = "RECT";  // Arrays use rectangles
      
   // Use actual array value
 if (i < arrayData.size()) {
          frame.nodeLabels[node.id] = std::to_string(arrayData[i]);
   } else {
           frame.nodeLabels[node.id] = "?";
      }
            }
      }
    }
    else if (structureType == "List") {
        // Get actual list values
        if (auto* listStruct = dynamic_cast<ListStructure*>(structure)) {
            std::vector<int> listValues;
    const ListNode* current = listStruct->getHead();
          while (current != nullptr) {
    listValues.push_back(current->value);
         current = current->next;
            }
 
            // Layout: Horizontal chain
            double startX = 150.0;
double y = 300.0;
 double spacing = 100.0;
  
            for (size_t i = 0; i < nodes.size(); ++i) {
          const auto& node = nodes[i];
 double x = startX + i * spacing;
    
      frame.nodePositions[node.id] = {x, y};
                frame.nodeShapes[node.id] = "RECT";  // Lists use rectangles with pointer
    
        // Use actual list value
                if (i < listValues.size()) {
      frame.nodeLabels[node.id] = std::to_string(listValues[i]);
     } else {
  frame.nodeLabels[node.id] = "?";
   }
            }
  
            // Add edges (arrows)
      for (const auto& edge : edges) {
                frame.edges.push_back({edge.from, edge.to});
            }
     }
    }
 else if (structureType == "Tree") {
        // Get actual tree values
        if (auto* treeStruct = dynamic_cast<TreeStructure*>(structure)) {
    std::map<std::string, int> nodeValueMap;
         
       // Collect node values
   std::function<void(const TreeNode*, std::vector<int>&)> collectValues = 
            [&](const TreeNode* node, std::vector<int>& values) {
      if (node == nullptr) return;
          collectValues(node->left, values);
          values.push_back(node->value);
nodeValueMap["tree_" + std::to_string(node->value)] = node->value;
     collectValues(node->right, values);
            };
   
            std::vector<int> treeValues;
      collectValues(treeStruct->getRoot(), treeValues);
            
         // Layout: Circular (will use Graphviz if available for tree layout)
            double centerX = 400.0;
 double centerY = 300.0;
double radius = std::min(200.0, 100.0 + nodeCount * 10.0);
            
            for (size_t i = 0; i < nodes.size(); ++i) {
    const auto& node = nodes[i];
   double angle = (2.0 * 3.14159 * i) / nodeCount;
 double x = centerX + radius * std::cos(angle);
 double y = centerY + radius * std::sin(angle);
                
  frame.nodePositions[node.id] = {x, y};
       frame.nodeShapes[node.id] = "CIRCLE";  // Trees use circles
     
                // Use actual tree value
            if (nodeValueMap.count(node.id)) {
           frame.nodeLabels[node.id] = std::to_string(nodeValueMap[node.id]);
    } else {
     frame.nodeLabels[node.id] = node.id;
    }
  }
      
            // Add edges
    for (const auto& edge : edges) {
            frame.edges.push_back({edge.from, edge.to});
            }
      }
    }
    else if (structureType == "Graph") {
        // Get actual graph values
   if (auto* graphStruct = dynamic_cast<GraphStructure*>(structure)) {
            const Graph* g = graphStruct->getGraph();
          
         // Layout: Circular
      double centerX = 400.0;
   double centerY = 300.0;
      double radius = std::min(200.0, 100.0 + nodeCount * 10.0);
  
            for (size_t i = 0; i < nodes.size(); ++i) {
       const auto& node = nodes[i];
     double angle = (2.0 * 3.14159 * i) / nodeCount;
     double x = centerX + radius * std::cos(angle);
        double y = centerY + radius * std::sin(angle);
    
    frame.nodePositions[node.id] = {x, y};
            frame.nodeShapes[node.id] = "CIRCLE";  // Graphs use circles
      
     // Try to get label from graph properties
                if (g) {
       if (const Graph::Node* graphNode = g->getNode(node.id)) {
        auto it = graphNode->properties.find("label");
               if (it != graphNode->properties.end()) {
   frame.nodeLabels[node.id] = it->second;
       } else {
          frame.nodeLabels[node.id] = node.id;
              }
         } else {
           frame.nodeLabels[node.id] = node.id;
             }
 } else {
      frame.nodeLabels[node.id] = node.id;
             }
     }
       
            // Add edges
          for (const auto& edge : edges) {
       frame.edges.push_back({edge.from, edge.to});
         }
      }
 }
    else {
        // Unknown type - use generic circular layout
        double centerX = 400.0;
    double centerY = 300.0;
        double radius = std::min(200.0, 100.0 + nodeCount * 10.0);
        
        for (size_t i = 0; i < nodes.size(); ++i) {
        const auto& node = nodes[i];
   double angle = (2.0 * 3.14159 * i) / nodeCount;
  double x = centerX + radius * std::cos(angle);
            double y = centerY + radius * std::sin(angle);
      
            frame.nodePositions[node.id] = {x, y};
     frame.nodeShapes[node.id] = "CIRCLE";
        frame.nodeLabels[node.id] = node.id;
        }
        
  for (const auto& edge : edges) {
frame.edges.push_back({edge.from, edge.to});
        }
    }
    
 // Render the frame
 if (auto* renderer = visualizationPane->findChild<VisualizationRenderer*>()) {
        renderer->renderFrame(frame);
    }
    
    visualizationPane->update();
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