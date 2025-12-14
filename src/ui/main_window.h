#pragma once

#include <QMainWindow>
#include <memory>
#include <string>

// On inclut les managers (Indispensable pour que ça marche)
#include "../core/data_model_manager.h"
#include "../orchestration/algorithm_manager.h" 

class VisualizationPane;
class ControlPanel;
class MetricsPanel;
class ToolboxPanel;
class Algorithm; // On utilise Algorithm générique pour l'instant

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    // Slots pour les signaux du ControlPanel
    void onPlayClicked();
    void onPauseClicked();
    void onResetClicked();
    void onStepForwardClicked();
    void onStepBackwardClicked();

    void onSpeedChanged(int speed);
    void onAlgorithmSelected(QString algorithm);
    void onDataStructureSelected(QString structure);
    void onDataSizeChanged(int size);

private:
    void setupUI();
    void connectSignals();
    void createMenuBar();

    // Fonctions métier
    void executeAlgorithm(const std::string& algorithm);
    void loadDataStructure(const std::string& type, int size);

    // --- COMPOSANTS UI ---
    ToolboxPanel* toolboxPanel = nullptr;
    std::unique_ptr<VisualizationPane> visualizationPane;
    std::unique_ptr<ControlPanel> controlPanel;
    std::unique_ptr<MetricsPanel> metricsPanel;

    // --- COEUR DU SYSTEME (BACKEND) ---
    std::unique_ptr<DataModelManager> dataModelManager;

    // --- ALGORITHMES ---
    AlgorithmManager& algoManager;
    Algorithm* currentAlgorithm = nullptr;
    std::string selectedAlgorithm;
};