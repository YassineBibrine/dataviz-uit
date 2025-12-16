#include "main_window.h"
#include "visualization_pane.h"
#include "control_panel.h"
#include "metrics_panel.h"
#include "toolbox_panel.h"
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
    // On garde ton titre et ta taille (plus confortables)
    setWindowTitle("DataViz UIT - Visualisation de Structures de Données");
    resize(1300, 850);
    setMinimumSize(1150, 750);

    // Connexion vitale : Lier l'interface graphique au cerveau (Backend)
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

    // --- EN-TÊTE (Ton Design) ---
    QHBoxLayout* headerLayout = new QHBoxLayout();
    headerLayout->addStretch();
    QLabel* titleLabel = new QLabel("DataViz UIT", this);
    titleLabel->setObjectName("appTitle"); // Pour le CSS
    titleLabel->setAlignment(Qt::AlignCenter);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    mainLayout->addLayout(headerLayout, 0);

    // --- CONTENU PRINCIPAL ---
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(15);

    // 1. Outils (Gauche)
    toolboxPanel = new ToolboxPanel(this);
    toolboxPanel->setObjectName("borderedPanel");
    toolboxPanel->setFixedWidth(110);
    contentLayout->addWidget(toolboxPanel);

    // 2. Visualisation (Centre)
    visualizationPane->setObjectName("borderedPanel");
    contentLayout->addWidget(visualizationPane.get(), 1);

    // 3. Droite (Contrôles + Métriques)
    QWidget* rightContainer = new QWidget();
    QVBoxLayout* rightLayout = new QVBoxLayout(rightContainer);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(15);

    controlPanel->setObjectName("borderedPanel");
    metricsPanel->setObjectName("borderedPanel");

    rightLayout->addWidget(controlPanel.get());
    rightLayout->addWidget(metricsPanel.get());

    rightContainer->setFixedWidth(360);
    contentLayout->addWidget(rightContainer);

    mainLayout->addLayout(contentLayout, 1);

    // Connexion Outils -> Visualisation
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
    connect(controlPanel.get(), &ControlPanel::dataStructureSelected, this, &MainWindow::onDataStructureSelected);
    connect(controlPanel.get(), &ControlPanel::speedChanged, this, &MainWindow::onSpeedChanged);
    connect(controlPanel.get(), &ControlPanel::dataSizeChanged, this, &MainWindow::onDataSizeChanged);
    // --- NOUVEAU : génération de nœuds ---
    connect(controlPanel.get(), &ControlPanel::generateNodesRequested,
        this, [this](int count) {
            frameRecorder.generateNodesFrame(count);
        });
}

void MainWindow::createMenuBar() {
    QMenu* fileMenu = menuBar()->addMenu("File");
    QAction* exitAction = fileMenu->addAction("Exit");
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);
    QMenu* helpMenu = menuBar()->addMenu("Help");
    helpMenu->addAction("About");
}

void MainWindow::executeAlgorithm(const std::string& algorithm) {
    auto algo = algoManager.createAlgorithm("Sorting", algorithm);
    if (algo) {
        qDebug() << "Algo created:" << QString::fromStdString(algorithm);
        // Ici on pourra ajouter algo->execute() plus tard si besoin
    }
    else {
        qDebug() << "Algorithm not found:" << QString::fromStdString(algorithm);
    }
}

void MainWindow::loadDataStructure(const std::string& type, int size) {
    // C'est ici que la magie opère : On demande au Backend de créer la structure
    if (dataModelManager) {
        dataModelManager->createDataStructure(type, size);
    }
}

void MainWindow::onAlgorithmSelected(QString algorithm) {
    selectedAlgorithm = algorithm.toStdString();
}

void MainWindow::onPlayClicked() {
    controlPanel->setPlayingState(true);
    if (!selectedAlgorithm.empty()) executeAlgorithm(selectedAlgorithm);
}

void MainWindow::onPauseClicked() { controlPanel->setPlayingState(false); }
void MainWindow::onResetClicked() { controlPanel->setPlayingState(false); }
void MainWindow::onStepForwardClicked() {}
void MainWindow::onStepBackwardClicked() {}
void MainWindow::onSpeedChanged(int speed) { qDebug() << "Speed:" << speed; }

void MainWindow::onDataStructureSelected(QString structure) {
    qDebug() << "Structure selected (UI):" << structure;

    // 1. Mise à jour de la Toolbox
    if (toolboxPanel) {
        toolboxPanel->updateTools(structure);
    }

    // 2. Nettoyage de l'affichage (Anti-Crash Visuel)
    if (visualizationPane) {
        visualizationPane->reset();
    }

    // 3. MAPPING DES NOMS (Correction "Binary Tree" -> "BinaryTree")
    std::string backendKey = structure.toStdString();
    if (structure == "Binary Tree") {
        backendKey = "BinaryTree";
    }
    else if (structure == "Linked List") {
        backendKey = "LinkedList";
    }

    // 4. PROTECTION ANTI-CRASH (Try-Catch)
    try {
        loadDataStructure(backendKey, 10);
    }
    catch (const std::exception& e) {
        QString errorMsg = QString("Le backend ne connait pas encore cette structure : %1\n\nErreur technique : %2")
            .arg(structure)
            .arg(e.what());
        QMessageBox::critical(this, "Structure non implémentée", errorMsg);
    }
}

void MainWindow::onDataSizeChanged(int size) {
    if (visualizationPane) visualizationPane->setRenderSize(size);
}


void MainWindow::closeEvent(QCloseEvent* e) {
    e->accept();
}