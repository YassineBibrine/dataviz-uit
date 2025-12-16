#pragma once

#include <QMainWindow>
#include <memory>
#include <string>

// On inclut les managers 
#include "../core/data_model_manager.h"
#include "../orchestration/algorithm_manager.h"
#include "../algorithms/frame_recorder.h"   //  le chemin si nécessaire
#include "../algorithms/algorithm_runner.h"

class VisualizationPane;
class ControlPanel;
class MetricsPanel;
class ToolboxPanel;
class StructureSelector;
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
    
    // --- Structure Selector slots ---
    void onStructureSelected(QString structureId);
    void onStructureRemoved(QString structureId);
    void onFinalizeInteractive(QString type, QString name);
    void onClearInteractive();

private:
    void setupUI();
    void connectSignals();
    void createMenuBar();

    // Fonctions métier
    void executeAlgorithm(const std::string& algorithm);
    void updateVisualizationForStructure(const std::string& structureId);

    // --- COMPOSANTS UI ---
    ToolboxPanel* toolboxPanel = nullptr;
    StructureSelector* structureSelector = nullptr;  // NEW
    std::unique_ptr<VisualizationPane> visualizationPane;
    std::unique_ptr<ControlPanel> controlPanel;
    std::unique_ptr<MetricsPanel> metricsPanel;

    // --- COEUR DU SYSTEME (BACKEND) ---
    // C'est la ligne la plus importante pour ton projet :
    std::unique_ptr<DataModelManager> dataModelManager;

    // --- ALGORITHMES ---
    AlgorithmManager& algoManager;
    FrameRecorder frameRecorder;   // 👉 nouveau membre
    AlgorithmRunner* currentAlgorithm = nullptr;
    std::string selectedAlgorithm;
};
