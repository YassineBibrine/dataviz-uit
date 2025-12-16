#include "main_window.h"
#include "visualization_pane.h"
#include "control_panel.h"
#include "metrics_panel.h"
#include "toolbox_panel.h"
#include "structure_selector.h"
#include "../orchestration/algorithm_manager.h" 

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

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      dataModelManager(std::make_unique<DataModelManager>()),
      visualizationPane(std::make_unique<VisualizationPane>(this)),
      controlPanel(std::make_unique<ControlPanel>(this)),
      metricsPanel(std::make_unique<MetricsPanel>(this)),
      algoManager(AlgorithmManager::getInstance()),
      frameRecorder(),
      currentAlgorithm(nullptr)
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
    QMenu* helpMenu = menuBar()->addMenu("Help");
    helpMenu->addAction("About");
}

void MainWindow::executeAlgorithm(const std::string& algorithm) {
    DataStructure* targetStructure = dataModelManager->getSelectedStructure();
    if (!targetStructure) {
    qDebug() << "No structure selected for algorithm execution";
        QMessageBox::warning(this, "No Structure", 
      "Please select a data structure from the Structure Selector.");
  return;
    }
    
    auto algo = algoManager.createAlgorithm("Sorting", algorithm);
    if (algo) {
      qDebug() << "Algo created:" << QString::fromStdString(algorithm)
    << "on structure:" << QString::fromStdString(dataModelManager->getSelectedStructureId());
    }
    else {
    qDebug() << "Algorithm not found:" << QString::fromStdString(algorithm);
    }
}

void MainWindow::onStructureSelected(QString structureId) {
    std::string id = structureId.toStdString();
    
    if (dataModelManager) {
        dataModelManager->selectStructure(id);
        
   // Get structure type and update toolbox
        auto structures = dataModelManager->getAllStructures();
        for (const auto& meta : structures) {
     if (meta.id == id) {
      // Update toolbox panel based on structure type
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
    
    qDebug() << "Finalizing structure:" << type << name 
      << "Nodes:" << nodeCount << "Edges:" << edgeCount;
  
    std::string structId = interactionMgr->finalizeStructure(
   type.toStdString(), 
        name.toStdString()
    );
    
    if (!structId.empty()) {
        if (structureSelector) {
      structureSelector->refreshStructureList();
        }
   
        // Update toolbox for the newly created structure type
if (toolboxPanel) {
        toolboxPanel->updateTools(type);
      qDebug() << "Toolbox updated for finalized structure type:" << type;
  }
        
        QMessageBox::information(this, "Structure Created",
            QString("Custom structure created successfully!\n\n"
          "Type: %1\n"
    "Nodes: %2\n"
       "Edges: %3\n\n"
        "The structure is now available for algorithms.")
          .arg(type).arg(nodeCount).arg(edgeCount));
    } else {
 QMessageBox::warning(this, "Creation Failed",
       "Failed to create structure. Please try again.");
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
    if (!dataModelManager || !visualizationPane) {
   return;
    }
    
    DataStructure* structure = dataModelManager->getStructure(structureId);
    if (!structure) {
     return;
    }
    
  auto nodes = structure->getNodes();
    auto edges = structure->getEdges();
    
    qDebug() << "Updating visualization for structure:" << QString::fromStdString(structureId)
       << "Nodes:" << nodes.size() << "Edges:" << edges.size();
    
    visualizationPane->update();
}

void MainWindow::closeEvent(QCloseEvent* e) {
    e->accept();
}
