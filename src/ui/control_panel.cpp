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
}

void ControlPanel::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(12);

    // --- ALGORITHM SELECTION ---
    QGroupBox* algoGroup = new QGroupBox("Algorithm", this);
    QVBoxLayout* algoLayout = new QVBoxLayout(algoGroup);
    algoLayout->setContentsMargins(10, 15, 10, 10);
    algoLayout->setSpacing(8);

    algoLayout->addWidget(new QLabel("Select algorithm:"));
    algorithmCombo = new QComboBox();
    algoLayout->addWidget(algorithmCombo);

    mainLayout->addWidget(algoGroup);

    // --- PLAYBACK CONTROLS ---
    QGroupBox* controlGroup = new QGroupBox("Playback Controls", this);
    QVBoxLayout* controlLayout = new QVBoxLayout(controlGroup);
    controlLayout->setContentsMargins(10, 12, 10, 10);
    controlLayout->setSpacing(8);

    // Play/Pause/Step buttons
    QHBoxLayout* playLayout = new QHBoxLayout();
    playLayout->setSpacing(8);

    int btnH = 26;       
    int btnW_Small = 45; 
    int btnW_Large = 70;  

    stepBackwardButton = new QPushButton("⏮");
    stepBackwardButton->setFixedSize(btnW_Small, btnH);

    playButton = new QPushButton("▶ Play");
    playButton->setFixedSize(btnW_Large, btnH); 

    pauseButton = new QPushButton("⏸ Pause");
    pauseButton->setFixedSize(btnW_Large, btnH);

    stepForwardButton = new QPushButton("⏭");
    stepForwardButton->setFixedSize(btnW_Small, btnH);

    playLayout->addWidget(stepBackwardButton);
    playLayout->addWidget(playButton);
    playLayout->addWidget(pauseButton);
    playLayout->addWidget(stepForwardButton);

    controlLayout->addLayout(playLayout);
    controlLayout->addSpacing(8); 

    // Reset button
    resetButton = new QPushButton("⟲ Reset");
    resetButton->setFixedHeight(btnH);
    controlLayout->addWidget(resetButton);

    controlLayout->addSpacing(8);

    // Speed slider
    controlLayout->addWidget(new QLabel("Animation Speed:"));

    speedSlider = new QSlider(Qt::Horizontal);
    speedSlider->setRange(1, 100);
    speedSlider->setValue(50);
    controlLayout->addWidget(speedSlider);

    currentFrameLabel = new QLabel("Frame: 0 / 0");
    currentFrameLabel->setAlignment(Qt::AlignCenter);
    currentFrameLabel->setStyleSheet("color: #718096; font-size: 8pt;");
    controlLayout->addWidget(currentFrameLabel);

    mainLayout->addWidget(controlGroup);
}

void ControlPanel::connectSignals()
{
    connect(playButton, &QPushButton::clicked, this, &ControlPanel::playClicked);
    connect(pauseButton, &QPushButton::clicked, this, &ControlPanel::pauseClicked);
    connect(resetButton, &QPushButton::clicked, this, &ControlPanel::resetClicked);
    connect(stepForwardButton, &QPushButton::clicked, this, &ControlPanel::stepForwardClicked);
    connect(stepBackwardButton, &QPushButton::clicked, this, &ControlPanel::stepBackwardClicked);
    connect(algorithmCombo, &QComboBox::currentTextChanged, this, &ControlPanel::algorithmSelected);
    connect(speedSlider, &QSlider::valueChanged, this, &ControlPanel::speedChanged);
}

// --- Public methods ---

void ControlPanel::populateAlgorithms(const std::vector<QString>& algorithms)
{
    algorithmCombo->clear();
    for (const auto& algo : algorithms) {
        algorithmCombo->addItem(algo);
    }
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
