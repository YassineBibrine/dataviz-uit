#include "control_panel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>

ControlPanel::ControlPanel(QWidget *parent)
 : QWidget(parent)
{
    setupUI();
    connectSignals();
}

ControlPanel::~ControlPanel() = default;

void ControlPanel::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Playback controls
    QHBoxLayout *playbackLayout = new QHBoxLayout();
    
    playButton = new QPushButton("Play");
    pauseButton = new QPushButton("Pause");
    stepForwardButton = new QPushButton("Step Forward");
    stepBackwardButton = new QPushButton("Step Backward");
    
    playbackLayout->addWidget(playButton);
    playbackLayout->addWidget(pauseButton);
 playbackLayout->addWidget(stepForwardButton);
    playbackLayout->addWidget(stepBackwardButton);
    
    mainLayout->addLayout(playbackLayout);
    
    // Speed control
    QHBoxLayout *speedLayout = new QHBoxLayout();
    speedLayout->addWidget(new QLabel("Speed:"));
    speedSlider = new QSlider(Qt::Horizontal);
    speedSlider->setMinimum(1);
    speedSlider->setMaximum(10);
    speedSlider->setValue(5);
    speedLayout->addWidget(speedSlider);
    mainLayout->addLayout(speedLayout);
    
    // Algorithm selection
    QHBoxLayout *algorithmLayout = new QHBoxLayout();
  algorithmLayout->addWidget(new QLabel("Algorithm:"));
  algorithmCombo = new QComboBox();
    algorithmCombo->addItems({
     "Custom Code",
     "Bubble Sort",
        "Merge Sort",
        "Quick Sort",
        "Dijkstra",
        "BFS",
        "DFS"
    });
 algorithmLayout->addWidget(algorithmCombo);
    mainLayout->addLayout(algorithmLayout);
    
    // Data size
    QHBoxLayout *sizeLayout = new QHBoxLayout();
 sizeLayout->addWidget(new QLabel("Data Size:"));
    dataSizeSpinBox = new QSpinBox();
    dataSizeSpinBox->setMinimum(1);
    dataSizeSpinBox->setMaximum(1000);
    dataSizeSpinBox->setValue(10);
    sizeLayout->addWidget(dataSizeSpinBox);
    mainLayout->addLayout(sizeLayout);
    
    mainLayout->addStretch();
}

void ControlPanel::connectSignals()
{
    connect(playButton, &QPushButton::clicked, this, &ControlPanel::playClicked);
    connect(pauseButton, &QPushButton::clicked, this, &ControlPanel::pauseClicked);
    connect(stepForwardButton, &QPushButton::clicked, this, &ControlPanel::stepForwardClicked);
    connect(stepBackwardButton, &QPushButton::clicked, this, &ControlPanel::stepBackwardClicked);
    connect(speedSlider, QOverload<int>::of(&QSlider::valueChanged),
      this, &ControlPanel::speedChanged);
    connect(algorithmCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged),
      this, &ControlPanel::algorithmSelected);
    connect(dataSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
       this, &ControlPanel::dataSizeChanged);
}
