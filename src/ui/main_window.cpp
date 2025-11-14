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
    
    // Create visualization pane
    visualizationPane = std::make_unique<VisualizationPane>(this);
    layout->addWidget(visualizationPane.get(), 3);
    
    // Create right sidebar layout
    auto right = new QVBoxLayout();
    
    // Create control panel
    controlPanel = std::make_unique<ControlPanel>(this);
    right->addWidget(controlPanel.get());
    
    // Create metrics panel
    metricsPanel = std::make_unique<MetricsPanel>(this);
    right->addWidget(metricsPanel.get());
    
    auto rightWidget = new QWidget(); 
    rightWidget->setLayout(right);
    layout->addWidget(rightWidget, 1);
}

void MainWindow::connectSignals() {
    // Connect control panel signals
    connect(controlPanel.get(), &ControlPanel::playClicked, this, &MainWindow::onPlayClicked);
}

void MainWindow::createMenuBar() {
    QMenu* fileMenu = menuBar()->addMenu("File");
    QAction* exitAction = fileMenu->addAction("Exit");
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);
    
    QMenu* helpMenu = menuBar()->addMenu("Help");
    helpMenu->addAction("About");
}

void MainWindow::executeAlgorithm(const std::string& algorithm) {
    (void)algorithm; // To be wired with AlgorithmManager factories
}

void MainWindow::loadDataStructure(const std::string& type, int size) {
    (void)type; (void)size; // TODO: use DataModelManager
}

void MainWindow::onAlgorithmSelected(const QString& algorithm) {
    (void)algorithm; // TODO: select
}

void MainWindow::onPlayClicked() {
    if (currentAlgorithm) {
        currentAlgorithm->execute();
    }
}

void MainWindow::closeEvent(QCloseEvent* e) {
    e->accept();
}
