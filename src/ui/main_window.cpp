#include "main_window.h"
#include "visualization_pane.h"
#include "control_panel.h"
#include "metrics_panel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QCloseEvent>
#include <QDebug>
#include "../algorithms/algorithm_runner.h"
#include "toolbox_panel.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      visualizationPane(std::make_unique<VisualizationPane>(this)),
      controlPanel(std::make_unique<ControlPanel>(this)),
      metricsPanel(std::make_unique<MetricsPanel>(this)),
      currentAlgorithm(nullptr),
      algoManager(AlgorithmManager::getInstance())
{
    setWindowTitle("DataViz-UIT: Algorithm Visualization");
    setGeometry(100, 100, 1200, 800);
    setMinimumSize(1000, 600);

    setupUI();
    connectSignals();
    createMenuBar();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    auto layout = new QHBoxLayout(central);

    // --- 1. GAUCHE : La Toolbox (NOUVEAU) ---
    // On l'ajoute en premier pour qu'elle soit à gauche
    toolboxPanel = new ToolboxPanel(this);
    layout->addWidget(toolboxPanel, 0); // Ratio 0 = taille fixe (ne s'étire pas)

    // --- 2. CENTRE : Visualization Pane (Zone de dessin) ---
    layout->addWidget(visualizationPane.get(), 4); // Ratio 4 (prend le plus de place)

    // --- 3. DROITE : Sidebar (Contrôles + Métriques) ---
    auto right = new QVBoxLayout();

    // Create control panel
    controlPanel = std::make_unique<ControlPanel>(this);
    right->addWidget(controlPanel.get());

    // Create metrics panel
    metricsPanel = std::make_unique<MetricsPanel>(this);
    right->addWidget(metricsPanel.get());

    // Container pour la droite
    auto rightWidget = new QWidget();
    rightWidget->setLayout(right);
    layout->addWidget(rightWidget, 1); // Ratio 1

    connect(toolboxPanel, &ToolboxPanel::toolSelected,
        visualizationPane.get(), &VisualizationPane::setInteractionMode);
}

void MainWindow::connectSignals() {
    // --- C'EST ICI QU'ON BRANCHE TOUT ---

    // Boutons de lecture
    connect(controlPanel.get(), &ControlPanel::playClicked, this, &MainWindow::onPlayClicked);
    connect(controlPanel.get(), &ControlPanel::pauseClicked, this, &MainWindow::onPauseClicked);
    connect(controlPanel.get(), &ControlPanel::resetClicked, this, &MainWindow::onResetClicked);
    connect(controlPanel.get(), &ControlPanel::stepForwardClicked, this, &MainWindow::onStepForwardClicked);
    connect(controlPanel.get(), &ControlPanel::stepBackwardClicked, this, &MainWindow::onStepBackwardClicked);

    // Changements de valeurs
    connect(controlPanel.get(), &ControlPanel::algorithmSelected, this, &MainWindow::onAlgorithmSelected);
    connect(controlPanel.get(), &ControlPanel::dataStructureSelected, this, &MainWindow::onDataStructureSelected);
    connect(controlPanel.get(), &ControlPanel::speedChanged, this, &MainWindow::onSpeedChanged);
    connect(controlPanel.get(), &ControlPanel::dataSizeChanged, this, &MainWindow::onDataSizeChanged);
}

void MainWindow::createMenuBar() {
    QMenu* fileMenu = menuBar()->addMenu("File");
    QAction* exitAction = fileMenu->addAction("Exit");
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    QMenu* helpMenu = menuBar()->addMenu("Help");
    helpMenu->addAction("About");
}

// --- LOGIQUE METIER (Backend hooks) ---

void MainWindow::executeAlgorithm(const std::string& algorithm) {
<<<<<<< Updated upstream
    auto algo = algoManager.createAlgorithm("Sorting", algorithm);
    if (algo) {
        // If Algorithm has name() method; otherwise skip
        // qDebug() << "Executing algorithm:" << QString::fromStdString(algo->name());
        algo->execute();
=======
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
             << "Category:" << QString::fromStdString(category)
             << "Structure ID:" << QString::fromStdString(dataModelManager->getSelectedStructureId());

    try {
        // Create algorithm WITH the actual data structure
        auto algo = algoManager.createAlgorithmWithData(category, algorithm, targetStructure);
        
        if (algo) {
            qDebug() << "✅ Algorithm created successfully and linked to data structure";
            
            // Execute the algorithm on the data
            algo->execute();
            qDebug() << "✅ Algorithm executed successfully";
            
            // Update visualization after algorithm execution
            std::string structId = dataModelManager->getSelectedStructureId();
            if (!structId.empty()) {
                loadStructureIntoCanvas(structId);
                qDebug() << "✅ Visualization updated";
            }
            
            controlPanel->setPlayingState(false);
            QMessageBox::information(this, "Algorithm Complete",
                QString("Algorithm '%1' completed successfully!")
                .arg(QString::fromStdString(algorithm)));
        }
        else {
            qDebug() << "❌ Algorithm not found:" << QString::fromStdString(algorithm);
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
        qDebug() << "❌ Algorithm execution error:" << e.what();
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
>>>>>>> Stashed changes
    }
    else {
        qDebug() << "Algorithm not found:" << QString::fromStdString(algorithm);
    }
}

void MainWindow::loadDataStructure(const std::string& type, int size) {
    (void)type; (void)size;
}

void MainWindow::onAlgorithmSelected(QString algorithm) {
    selectedAlgorithm = algorithm.toStdString();
}

void MainWindow::onPlayClicked() {
    qDebug() << ">>> PLAY clicked";
    controlPanel->setPlayingState(true);

    if (!selectedAlgorithm.empty()) {
        executeAlgorithm(selectedAlgorithm);
    }
    else if (currentAlgorithm) {
        qDebug() << "Launching algorithm via backend...";
    }
    else {
        qDebug() << "No algorithm loaded yet.";
    }
}

void MainWindow::onPauseClicked() {
    qDebug() << ">>> PAUSE clicked";
    controlPanel->setPlayingState(false);
}

void MainWindow::onResetClicked() {
    qDebug() << ">>> RESET clicked";
    controlPanel->setPlayingState(false);
}

void MainWindow::onStepForwardClicked() {
    qDebug() << ">>> STEP FORWARD";
}

void MainWindow::onStepBackwardClicked() {
    qDebug() << ">>> STEP BACKWARD";
}

void MainWindow::onSpeedChanged(int speed) {
    qDebug() << "Speed:" << speed;
}

void MainWindow::onDataStructureSelected(QString structure) {
    qDebug() << "Structure selected:" << structure;
    if (toolboxPanel) {
        toolboxPanel->updateTools(structure);
    }
}

void MainWindow::onDataSizeChanged(int size) {
    qDebug() << "Size:" << size;
    if (visualizationPane) {
        visualizationPane->setRenderSize(size);
    }
}

void MainWindow::closeEvent(QCloseEvent* e) {
    e->accept();
}