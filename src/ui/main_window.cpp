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
#include <QDebug> // Indispensable pour voir si ça marche
#include "../algorithms/algorithm_runner.h"
#include "toolbox_panel.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), algoManager(AlgorithmManager::getInstance()) {
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
    visualizationPane = std::make_unique<VisualizationPane>(this);
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
    (void)algorithm; // Sera relié à AlgorithmManager plus tard
}

void MainWindow::loadDataStructure(const std::string& type, int size) {
    (void)type; (void)size; // Sera relié au DataModelManager
}

void MainWindow::closeEvent(QCloseEvent* e) {
    e->accept();
}

// --- NOS SLOTS D'ACTIONS (Pour tester) ---

void MainWindow::onPlayClicked() {
    qDebug() << ">>> PLAY cliqué !";
    controlPanel->setPlayingState(true); // Grise le bouton play

    // Code existant de ton collègue (je le laisse, mais sécurisé)
    if (currentAlgorithm) {
        // currentAlgorithm->execute(); 
        qDebug() << "Lancement de l'algo via le backend...";
    }
    else {
        qDebug() << "Aucun algorithme chargé pour l'instant.";
    }
}

void MainWindow::onPauseClicked() {
    qDebug() << ">>> PAUSE cliqué !";
    controlPanel->setPlayingState(false); // Réactive le bouton play
}

void MainWindow::onResetClicked() {
    qDebug() << ">>> RESET cliqué !";
    controlPanel->setPlayingState(false);
    // Ici on ajoutera plus tard : visualizationPane->clear();
}

void MainWindow::onStepForwardClicked() {
    qDebug() << ">>> STEP FORWARD";
}

void MainWindow::onStepBackwardClicked() {
    qDebug() << ">>> STEP BACKWARD";
}

void MainWindow::onSpeedChanged(int speed) {
    qDebug() << "Vitesse :" << speed;
}

void MainWindow::onAlgorithmSelected(QString algorithm) {
    qDebug() << "Algo choisi :" << algorithm;
}


void MainWindow::onDataStructureSelected(QString structure) {
    qDebug() << "Structure choisie :" << structure;


    if (toolboxPanel) {
        toolboxPanel->updateTools(structure);
    }
}

void MainWindow::onDataSizeChanged(int size) {
    qDebug() << "Taille :" << size;

    // On envoie l'ordre au VisualizationPane
    if (visualizationPane) {
        visualizationPane->setRenderSize(size);
    }
}