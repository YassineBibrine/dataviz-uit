#include "control_panel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

ControlPanel::ControlPanel(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    connectSignals();

    // Valeurs par défaut pour tester immédiatement
    populateAlgorithms({ "BFS", "DFS", "Dijkstra" });
    populateDataStructures({ "Graph", "Binary Tree", "Linked List" });
}

void ControlPanel::setupUI()
{
    // Layout principal vertical
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);

    // --- Section 1 : Configuration ---
    QGroupBox* configGroup = new QGroupBox("Configuration", this);
    QVBoxLayout* configLayout = new QVBoxLayout(configGroup);

    // Choix de la structure
    configLayout->addWidget(new QLabel("Structure de données:"));
    dataStructureCombo = new QComboBox();
    configLayout->addWidget(dataStructureCombo);

    // Choix de l'algorithme
    configLayout->addWidget(new QLabel("Algorithme:"));
    algorithmCombo = new QComboBox();
    configLayout->addWidget(algorithmCombo);

    // Taille des données
    configLayout->addWidget(new QLabel("Taille (Nœuds):"));
    dataSizeSpinBox = new QSpinBox();
    dataSizeSpinBox->setRange(1, 100);
    dataSizeSpinBox->setValue(20);
    configLayout->addWidget(dataSizeSpinBox);

    mainLayout->addWidget(configGroup);

    // --- Section 2 : Contrôles de lecture ---
    QGroupBox* controlGroup = new QGroupBox("Contrôles", this);
    QVBoxLayout* controlGroupLayout = new QVBoxLayout(controlGroup);

    // Ligne de boutons (Play, Pause, Reset, Steps)
    QHBoxLayout* playLayout = new QHBoxLayout();
    stepBackwardButton = new QPushButton("⏮");
    playButton = new QPushButton("▶ Play");
    pauseButton = new QPushButton("⏸ Pause");
    stepForwardButton = new QPushButton("⏭");

    // On ajoute les boutons au petit layout horizontal
    playLayout->addWidget(stepBackwardButton);
    playLayout->addWidget(playButton);
    playLayout->addWidget(pauseButton);
    playLayout->addWidget(stepForwardButton);

    controlGroupLayout->addLayout(playLayout);

    // Bouton Reset en dessous
    resetButton = new QPushButton("⟲ Reset");
    controlGroupLayout->addWidget(resetButton);

    // Vitesse
    controlGroupLayout->addWidget(new QLabel("Vitesse d'animation:"));
    speedSlider = new QSlider(Qt::Horizontal);
    speedSlider->setRange(1, 100);
    speedSlider->setValue(50);
    controlGroupLayout->addWidget(speedSlider);

    // Label de frame
    currentFrameLabel = new QLabel("Frame: 0 / 0");
    currentFrameLabel->setAlignment(Qt::AlignCenter);
    controlGroupLayout->addWidget(currentFrameLabel);

    mainLayout->addWidget(controlGroup);

    // Spacer pour pousser tout vers le haut
    mainLayout->addStretch();
}

void ControlPanel::connectSignals()
{
    // Connexions des boutons aux signaux de la classe
    connect(playButton, &QPushButton::clicked, this, &ControlPanel::playClicked);
    connect(pauseButton, &QPushButton::clicked, this, &ControlPanel::pauseClicked);
    connect(resetButton, &QPushButton::clicked, this, &ControlPanel::resetClicked);
    connect(stepForwardButton, &QPushButton::clicked, this, &ControlPanel::stepForwardClicked);
    connect(stepBackwardButton, &QPushButton::clicked, this, &ControlPanel::stepBackwardClicked);

    // Connexions des changements de valeur
    connect(dataStructureCombo, &QComboBox::currentTextChanged, this, &ControlPanel::dataStructureSelected);
    connect(algorithmCombo, &QComboBox::currentTextChanged, this, &ControlPanel::algorithmSelected);

    // Pour la vitesse et la taille
    connect(speedSlider, &QSlider::valueChanged, this, &ControlPanel::speedChanged);
    connect(dataSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ControlPanel::dataSizeChanged);
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

QString ControlPanel::getSelectedDataStructure() const {
    return dataStructureCombo->currentText();
}

void ControlPanel::setPlayingState(bool playing) {
    // Désactive Play si on joue, active Pause, et inversement
    playButton->setEnabled(!playing);
    pauseButton->setEnabled(playing);
}

void ControlPanel::enableControls(bool enabled) {
    // Active ou désactive tout le panneau (utile quand un algo tourne)
    this->setEnabled(enabled);
}