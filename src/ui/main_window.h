#pragma once

#include <QMainWindow>
#include <memory>
#include "../orchestration/algorithm_manager.h"

class VisualizationPane;
class ControlPanel;
class MetricsPanel;
class AlgorithmRunner;
class ToolboxPanel;
/**
 * @class MainWindow
 * @brief Main application window
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
<<<<<<< HEAD

=======
     
>>>>>>> ede87d8 (WIP: sauvegarde avant merge)
    void executeAlgorithm(const std::string& algorithm);
    void loadDataStructure(const std::string& type, int size);

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    void setupUI();
    void connectSignals();
    void createMenuBar();
    ToolboxPanel* toolboxPanel = nullptr;

    // UI Components
    std::unique_ptr<VisualizationPane> visualizationPane;
    std::unique_ptr<ControlPanel> controlPanel;
    std::unique_ptr<MetricsPanel> metricsPanel;

    // Core components
<<<<<<< HEAD
    AlgorithmRunner* currentAlgorithm{ nullptr };
    AlgorithmManager& algoManager;

=======
    AlgorithmRunner* currentAlgorithm{nullptr};
    AlgorithmManager& algoManager = AlgorithmManager::getInstance();
    
>>>>>>> ede87d8 (WIP: sauvegarde avant merge)
private slots:
    // --- AJOUTS POUR LE CONTROL PANEL ---
    void onPlayClicked();
<<<<<<< HEAD
    void onPauseClicked();
    void onResetClicked();
    void onStepForwardClicked();
    void onStepBackwardClicked();
    void onSpeedChanged(int speed);
    void onAlgorithmSelected(QString algorithm); // Note: QString ici c'est mieux
    void onDataStructureSelected(QString structure);
    void onDataSizeChanged(int size);
};
=======
    
private:
    std::string selectedAlgorithm;
};

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      algoManager(AlgorithmManager::getInstance()) // Ajoute cette initialisation
{
    setWindowTitle("DataViz-UIT: Algorithm Visualization");
    setGeometry(100, 100, 1200, 800);
    setMinimumSize(1000, 600);

    setupUI();
    connectSignals();
    createMenuBar();
}
>>>>>>> ede87d8 (WIP: sauvegarde avant merge)
