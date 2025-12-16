#pragma once

#include <QMainWindow>
#include <memory>
#include <string>

// On inclut les managers 
#include "../core/data_model_manager.h"
#include "../orchestration/algorithm_manager.h"
#include "../algorithms/frame_recorder.h"   //  le chemin si nécessaire


class VisualizationPane;
class ControlPanel;
class MetricsPanel;
class ToolboxPanel;
class Algorithm; // Forward declaration générique

/**
 * @class MainWindow
 * @brief Main application window
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

protected:
    //  On déclare la fonction de fermeture qu'on a ajoutée dans le CPP
    void closeEvent(QCloseEvent *event) override;

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

    // Core components
    AlgorithmRunner* currentAlgorithm{nullptr};
    AlgorithmManager& algoManager = AlgorithmManager::getInstance();
    FrameRecorder frameRecorder;   // 👉 nouveau membre
    // --- COEUR DU SYSTEME (BACKEND) ---
    // C'est la ligne la plus importante pour ton projet :
    std::unique_ptr<DataModelManager> dataModelManager;

    // --- ALGORITHMES ---
    AlgorithmManager& algoManager;
    Algorithm* currentAlgorithm = nullptr;
    std::string selectedAlgorithm;
};
