#pragma once

#include <QMainWindow>
#include <QSettings>
#include <memory>
#include <string>

#include "../core/data_model_manager.h"
#include "../orchestration/algorithm_manager.h"
#include "../algorithms/frame_recorder.h"
#include "../session/session_manager.h"

class VisualizationPane;
class ControlPanel;
class MetricsPanel;
class ToolboxPanel;
class Algorithm;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
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
    void onClearInteractive();
    void onSamplesCreated();  // NEW: Handle samples created
    
    // NEW: Code Generator slots
    void onShowCodeGenerator();
    void onStructureCreatedFromCode(QString structureId);
  
    // NEW: Metrics Panel toggle slot
    void onToggleMetricsPanel(bool show);

    // Tutorial slots
    void onTutorialCompleted();
    void onTutorialSkipped();
    void onShowTutorial();

private:
    void setupUI();
    void connectSignals();
    void createMenuBar();
    void restorePreviousSession();

    void executeAlgorithm(const std::string& algorithm);
    void updateVisualizationForStructure(const std::string& structureId);
    void loadStructureIntoCanvas(const std::string& structureId);  // NEW: Load structure into interactive canvas

    ToolboxPanel* toolboxPanel = nullptr;
    StructureSelector* structureSelector = nullptr;
    std::unique_ptr<VisualizationPane> visualizationPane;
    std::unique_ptr<ControlPanel> controlPanel;
    std::unique_ptr<MetricsPanel> metricsPanel;

    FrameRecorder frameRecorder;
    std::unique_ptr<DataModelManager> dataModelManager;

    AlgorithmManager& algoManager;
    FrameRecorder frameRecorder;
    AlgorithmRunner* currentAlgorithm = nullptr;
    std::string selectedAlgorithm;
};
