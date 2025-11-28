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

void MainWindow::executeAlgorithm(const std::string& algorithm) {
    // ? Utilise le manager local pour créer un algorithme
    // Pour l'instant, on suppose que tous les algorithmes de tri sont sous "Sorting"
    auto algo = algoManager.createAlgorithm("Sorting", algorithm);
    if (algo) {
        qDebug() << "Executing algorithm:" << QString::fromStdString(algo->name());
        algo->execute(); // Affiche dans la console pour l'instant
    }
    else {
        qDebug() << "Algorithm not found:" << QString::fromStdString(algorithm);
    }
}

void MainWindow::loadDataStructure(const std::string& type, int size) {
    (void)type; (void)size; // TODO: use DataModelManager
}

void MainWindow::onAlgorithmSelected(const QString& algorithm) {
    // TODO: stocker le choix ou appeler executeAlgorithm
    // Exemple :
    // selectedAlgorithm = algorithm.toStdString();
}

void MainWindow::onPlayClicked() {
    // Si tu utilises AlgorithmRunner, il faudra le recréér ici
    // Mais pour l'instant, on peut exécuter directement via executeAlgorithm
    // Exemple temporaire :
    if (!selectedAlgorithm.empty()) {
        executeAlgorithm(selectedAlgorithm);
    }
}

void MainWindow::closeEvent(QCloseEvent* e) {
    e->accept();
}