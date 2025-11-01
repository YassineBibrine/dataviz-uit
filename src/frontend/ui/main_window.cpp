#include "main_window.h"
#include "code_editor.h"
#include "visualization_pane.h"
#include "control_panel.h"
#include "diagnostic_output.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QMenu>
#include <QMenuBar>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("C++ Data Structure & Algorithm Visualizer");
    setGeometry(100, 100, 1600, 1000);
  
    setupUI();
    connectSignals();
    createMenuBar();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI()
{
    // Create central widget and main layout
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    
    // Create left panel (code editor)
  codeEditor = std::make_unique<CodeEditor>();
    
    // Create right panel with splitter
    QSplitter *rightSplitter = new QSplitter(Qt::Vertical);
    
    visualizationPane = std::make_unique<VisualizationPane>();
    controlPanel = std::make_unique<ControlPanel>();
    diagnosticOutput = std::make_unique<DiagnosticOutput>();
    
    rightSplitter->addWidget(visualizationPane.get());
    rightSplitter->addWidget(controlPanel.get());
    rightSplitter->addWidget(diagnosticOutput.get());
    rightSplitter->setStretchFactor(0, 3);
    rightSplitter->setStretchFactor(1, 1);
    rightSplitter->setStretchFactor(2, 1);
    
    // Create horizontal splitter for left and right panels
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal);
    mainSplitter->addWidget(codeEditor.get());
    mainSplitter->addWidget(rightSplitter);
    mainSplitter->setStretchFactor(0, 1);
    mainSplitter->setStretchFactor(1, 2);
    
    mainLayout->addWidget(mainSplitter);
    setCentralWidget(centralWidget);
}

void MainWindow::connectSignals()
{
    // Connect signals between components
    // This will be populated as components are fully implemented
}

void MainWindow::createMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu("&File");
    QMenu *editMenu = menuBar()->addMenu("&Edit");
    QMenu *viewMenu = menuBar()->addMenu("&View");
    QMenu *helpMenu = menuBar()->addMenu("&Help");
    
    // Add menu actions as needed
}
