#include "control_panel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QSpacerItem>

ControlPanel::ControlPanel(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setupUI();
    connectSignals();
    populateAlgorithms({ "BFS", "DFS", "Dijkstra" });
    populateDataStructures({ "Graph", "Binary Tree", "Linked List" });
}

void ControlPanel::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(15);

    // --- CONFIGURATION ---
    QGroupBox* configGroup = new QGroupBox("Configuration", this);
    QVBoxLayout* configLayout = new QVBoxLayout(configGroup);

    configLayout->setContentsMargins(10, 20, 10, 10);
    configLayout->setSpacing(8);

    configLayout->addWidget(new QLabel("Structure de données:"));
    dataStructureCombo = new QComboBox();
    configLayout->addWidget(dataStructureCombo);

    configLayout->addWidget(new QLabel("Algorithme:"));
    algorithmCombo = new QComboBox();
    configLayout->addWidget(algorithmCombo);

    // Taille des données
    configLayout->addWidget(new QLabel("Taille (Nœuds):"));
    QHBoxLayout* nodeLayout = new QHBoxLayout();

    dataSizeSpinBox = new QSpinBox();
    dataSizeSpinBox->setRange(1, 100);
    dataSizeSpinBox->setValue(20);

    generateNodesButton = new QPushButton("Generate Nodes");

    nodeLayout->addWidget(dataSizeSpinBox);
    nodeLayout->addWidget(generateNodesButton);

    configLayout->addLayout(nodeLayout);

    mainLayout->addWidget(configGroup);

    // --- CONTRÔLES ---
    QGroupBox* controlGroup = new QGroupBox("Contrôles", this);
    QVBoxLayout* controlLayout = new QVBoxLayout(controlGroup);

    controlLayout->setContentsMargins(10, 10, 10, 10);
    controlLayout->setSpacing(0);

    // 1. PLAY / PAUSE
    QHBoxLayout* playLayout = new QHBoxLayout();
    playLayout->setSpacing(14);

    int btnH = 23;       
    int btnW_Small = 57; 
    int btnW_Large = 85;  

    stepBackwardButton = new QPushButton("⏮");
    stepBackwardButton->setFixedSize(btnW_Small, btnH);

    playButton = new QPushButton("▶ Play");
    playButton->setFixedSize(btnW_Large, btnH); 

    pauseButton = new QPushButton("⏸ Pause");
    pauseButton->setFixedSize(btnW_Large, btnH);

    stepForwardButton = new QPushButton("⏭");
    stepForwardButton->setFixedSize(btnW_Small, btnH);

    playLayout->addStretch();
    playLayout->addWidget(stepBackwardButton);
    playLayout->addWidget(playButton);
    playLayout->addWidget(pauseButton);
    playLayout->addWidget(stepForwardButton);
    playLayout->addStretch();

    controlLayout->addLayout(playLayout);
    controlLayout->addSpacing(20); 

    // 2. RESET
    resetButton = new QPushButton("⟲ Reset");
    resetButton->setFixedSize(320, btnH);

    QHBoxLayout* resetLayout = new QHBoxLayout();
    resetLayout->addStretch();
    resetLayout->addWidget(resetButton);
    resetLayout->addStretch();
    controlLayout->addLayout(resetLayout);

    // Espace réduit avant le slider
    controlLayout->addSpacing(10);

    // 3. SLIDER
    controlLayout->addWidget(new QLabel("Vitesse d'animation:"));

    speedSlider = new QSlider(Qt::Horizontal);
    speedSlider->setRange(1, 100);
    speedSlider->setValue(50);
    controlLayout->addWidget(speedSlider);

    currentFrameLabel = new QLabel("Frame: 0 / 0");
    currentFrameLabel->setAlignment(Qt::AlignCenter);
    currentFrameLabel->setStyleSheet("color: #718096; margin-top: 2px; font-size: 8pt;");
    controlLayout->addWidget(currentFrameLabel);

    mainLayout->addWidget(controlGroup);
    mainLayout->addStretch();
}

void ControlPanel::connectSignals()
{
    connect(playButton, &QPushButton::clicked, this, &ControlPanel::playClicked);
    connect(pauseButton, &QPushButton::clicked, this, &ControlPanel::pauseClicked);
    connect(resetButton, &QPushButton::clicked, this, &ControlPanel::resetClicked);
    connect(stepForwardButton, &QPushButton::clicked, this, &ControlPanel::stepForwardClicked);
    connect(stepBackwardButton, &QPushButton::clicked, this, &ControlPanel::stepBackwardClicked);
    connect(dataStructureCombo, &QComboBox::currentTextChanged, this, &ControlPanel::dataStructureSelected);
    connect(algorithmCombo, &QComboBox::currentTextChanged, this, &ControlPanel::algorithmSelected);
    connect(speedSlider, &QSlider::valueChanged, this, &ControlPanel::speedChanged);

    connect(dataSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ControlPanel::dataSizeChanged);

    connect(generateNodesButton, &QPushButton::clicked, this, [this]() {
        emit generateNodesRequested(dataSizeSpinBox->value());
    });
}

// --- Implémentation des méthodes publiques ---

void ControlPanel::populateAlgorithms(const std::vector<QString>& algorithms)
{
    algorithmCombo->clear();
    for (const auto& algo : algorithms) {
        algorithmCombo->addItem(algo);
    }
}

void ControlPanel::populateDataStructures(const std::vector<QString>& structures)
{
    dataStructureCombo->clear();
    for (const auto& s : structures) {
        dataStructureCombo->addItem(s);
    }
}

QString ControlPanel::getSelectedDataStructure() const 
{ 
    return dataStructureCombo->currentText(); 
}

void ControlPanel::setPlayingState(bool playing) 
{ 
    playButton->setEnabled(!playing); 
    pauseButton->setEnabled(playing); 
}

void ControlPanel::enableControls(bool enabled) 
{ 
    this->setEnabled(enabled); 
}
