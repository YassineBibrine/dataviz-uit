#include "control_panel.h"
#include "../orchestration/algorithm_manager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSpacerItem>
#include <QComboBox>
#include <QString>
#include <QWheelEvent>

// Custom ComboBox that ignores wheel events to prevent scroll hijacking
class NoScrollComboBox : public QComboBox {
public:
    explicit NoScrollComboBox(QWidget* parent = nullptr) : QComboBox(parent) {
   setFocusPolicy(Qt::StrongFocus);
    }
protected:
    void wheelEvent(QWheelEvent* event) override {
        // Only handle wheel events if the combo box has focus
        if (!hasFocus()) {
      event->ignore();
        } else {
 QComboBox::wheelEvent(event);
        }
    }
};

// Custom Slider that ignores wheel events to prevent scroll hijacking
class NoScrollSlider : public QSlider {
public:
    explicit NoScrollSlider(Qt::Orientation orientation, QWidget* parent = nullptr) 
        : QSlider(orientation, parent) {
     setFocusPolicy(Qt::StrongFocus);
    }
protected:
    void wheelEvent(QWheelEvent* event) override {
        // Only handle wheel events if the slider has focus
        if (!hasFocus()) {
        event->ignore();
        } else {
       QSlider::wheelEvent(event);
    }
    }
};

// Custom SpinBox that ignores wheel events to prevent scroll hijacking
class NoScrollSpinBox : public QSpinBox {
public:
    explicit NoScrollSpinBox(QWidget* parent = nullptr) : QSpinBox(parent) {
        setFocusPolicy(Qt::StrongFocus);
    }
protected:
void wheelEvent(QWheelEvent* event) override {
        // Only handle wheel events if the spin box has focus
    if (!hasFocus()) {
            event->ignore();
 } else {
      QSpinBox::wheelEvent(event);
        }
    }
};

ControlPanel::ControlPanel(QWidget* parent)
    : QWidget(parent),
    algorithmCombo(nullptr),
    playButton(nullptr),
    pauseButton(nullptr),
    resetButton(nullptr),
    stepForwardButton(nullptr),
    stepBackwardButton(nullptr),
    speedSlider(nullptr),
    currentFrameLabel(nullptr)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setupUI();
    connectSignals();

    // Initialize with default algorithms for Array type
    updateAlgorithmList("Array");
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
    algorithmCombo = new NoScrollComboBox();
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

    stepBackwardButton = new QPushButton("|<", this);
    stepBackwardButton->setFixedSize(btnW_Small, btnH);
    stepBackwardButton->setToolTip("Step Backward");

    playButton = new QPushButton("Play", this);
    playButton->setFixedSize(btnW_Large, btnH);
    playButton->setObjectName("playButton");

    pauseButton = new QPushButton("Pause", this);
    pauseButton->setFixedSize(btnW_Large, btnH);

    stepForwardButton = new QPushButton(">|", this);
    stepForwardButton->setFixedSize(btnW_Small, btnH);
  stepForwardButton->setToolTip("Step Forward");

    playLayout->addWidget(stepBackwardButton);
    playLayout->addWidget(playButton);
    playLayout->addWidget(pauseButton);
    playLayout->addWidget(stepForwardButton);

    controlLayout->addLayout(playLayout);
    controlLayout->addSpacing(8);

    // Reset button
    resetButton = new QPushButton("Reset", this);
    resetButton->setFixedHeight(btnH);
    controlLayout->addWidget(resetButton);

    controlLayout->addSpacing(8);

    // Speed slider - use custom NoScrollSlider
    controlLayout->addWidget(new QLabel("Animation Speed:"));

    speedSlider = new NoScrollSlider(Qt::Horizontal);
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

void ControlPanel::updateAlgorithmList(const QString& structureType) {
    algorithmCombo->clear();

    auto& manager = AlgorithmManager::getInstance();
    auto categories = manager.getCategories();

    QString type = structureType;

    if (type == "Graph") {
        for (const auto& algo : manager.getAlgorithmNames("Graph")) {
            algorithmCombo->addItem(QString::fromStdString(algo));
        }
    }
    else if (type == "Binary Tree" || type == "Tree") {
        for (const auto& algo : manager.getAlgorithmNames("Tree")) {
         algorithmCombo->addItem(QString::fromStdString(algo));
        }
        // Also add graph algorithms as fallback if tree category is empty
if (algorithmCombo->count() == 0) {
     for (const auto& algo : manager.getAlgorithmNames("Graph")) {
           algorithmCombo->addItem(QString::fromStdString(algo));
            }
        }
    }
  else if (type == "List" || type == "LinkedList") {
    // Show 3 sorting algorithms for lists
        algorithmCombo->addItem("InsertionSort");
        algorithmCombo->addItem("SelectionSort");
        algorithmCombo->addItem("BubbleSort");  // ? Added third algorithm
 }
    else if (type == "Array") {
        // Only show 3 sorting algorithms for arrays
        algorithmCombo->addItem("InsertionSort");
        algorithmCombo->addItem("SelectionSort");
        algorithmCombo->addItem("BubbleSort");
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