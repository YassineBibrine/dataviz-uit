#pragma once

#include <QMainWindow>
#include <memory>
#include <string>
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
    AlgorithmRunner* currentAlgorithm{nullptr};
    AlgorithmManager& algoManager = AlgorithmManager::getInstance();

    // Selected algorithm name
    std::string selectedAlgorithm;

private slots:
    // --- AJOUTS POUR LE CONTROL PANEL ---
    void onPlayClicked();
    void onPauseClicked();
    void onResetClicked();
    void onStepForwardClicked();
    void onStepBackwardClicked();
    void onSpeedChanged(int speed);
    void onAlgorithmSelected(QString algorithm); // Note: QString ici c'est mieux
    void onDataStructureSelected(QString structure);
    void onDataSizeChanged(int size);
};
