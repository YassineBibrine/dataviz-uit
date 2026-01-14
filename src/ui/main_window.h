#pragma once

#include <QMainWindow>
#include <QSettings>
#include <memory>
#include <string>
#include <vector>
#include <map>

#include "../core/data_model_manager.h"
#include "../orchestration/algorithm_manager.h"
#include "../algorithms/frame_recorder.h"
#include "../core/session_manager.h"

// Forward declarations
struct DSNode;
struct DSEdge;
class VisualizationPane;
class ControlPanel;
class MetricsPanel;
class ToolboxPanel;
class StructureSelector;
class Algorithm;
class AlgorithmRunner;
class TutorialOverlay;
class InteractionManager;
class TreeStructure;
class DataStructure;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
  
    // Public method to reposition tree nodes (called from VisualizationPane)
    void repositionTreeNodesAfterEdit(const std::string& structureId);

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
    
    // Tree layout helper
    void layoutTreeHierarchically(
        TreeStructure* treeStruct,
        const std::vector<DSNode>& nodes,
        const std::vector<DSEdge>& edges,
        std::map<std::string, std::string>& oldToNewId,
        InteractionManager* interactionMgr,
        DataStructure* structure
    );

    ToolboxPanel* toolboxPanel = nullptr;
    StructureSelector* structureSelector = nullptr;
    std::unique_ptr<VisualizationPane> visualizationPane;
    std::unique_ptr<ControlPanel> controlPanel;
    std::unique_ptr<MetricsPanel> metricsPanel;

    FrameRecorder frameRecorder;
    std::unique_ptr<DataModelManager> dataModelManager;

    AlgorithmManager& algoManager;
    AlgorithmRunner* currentAlgorithm = nullptr;
    std::string selectedAlgorithm;
    
    // Menu actions
    QAction* toggleMetricsAction = nullptr;
    QAction* showTutorialAction = nullptr;
    
    // Tutorial
    TutorialOverlay* tutorialOverlay = nullptr;
    bool firstLaunchChecked = false;
 
    // Session settings
    QSettings settings;
    
    // Tutorial methods
    void setupTutorial();
    void checkFirstLaunch();
    void showEvent(QShowEvent* event) override;
};
