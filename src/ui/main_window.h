#pragma once

#include <QMainWindow>
#include <memory>
#include "../orchestration/algorithm_manager.h"

class VisualizationPane;
class ControlPanel;
class MetricsPanel;
class AlgorithmRunner;

/**
 * @class MainWindow
 * @brief Main application window
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
     
    void executeAlgorithm(const std::string& algorithm);
    void loadDataStructure(const std::string& type, int size);
    
protected:
    void closeEvent(QCloseEvent* event) override;
    
private:
    void setupUI();
    void connectSignals();
    void createMenuBar();
    
    // UI Components
    std::unique_ptr<VisualizationPane> visualizationPane;
    std::unique_ptr<ControlPanel> controlPanel;
    std::unique_ptr<MetricsPanel> metricsPanel;
    
    // Core components
    AlgorithmRunner* currentAlgorithm{nullptr};
    AlgorithmManager& algoManager = AlgorithmManager::getInstance();
    
private slots:
    void onAlgorithmSelected(const QString& algorithm);
    void onPlayClicked();
    
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
