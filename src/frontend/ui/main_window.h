#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <memory>

class CodeEditor;
class VisualizationPane;
class ControlPanel;
class DiagnosticOutput;

/**
 * @class MainWindow
 * @brief Main application window integrating all UI components
 * 
 * Serves as the container for the code editor, visualization pane,
 * control panel, and diagnostic output. Manages signal/slot connections
 * between components.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupUI();
    void connectSignals();
    void createMenuBar();

    std::unique_ptr<CodeEditor> codeEditor;
  std::unique_ptr<VisualizationPane> visualizationPane;
    std::unique_ptr<ControlPanel> controlPanel;
    std::unique_ptr<DiagnosticOutput> diagnosticOutput;
};

#endif // MAIN_WINDOW_H
