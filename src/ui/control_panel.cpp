#include "control_panel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

ControlPanel::ControlPanel(QWidget* parent): QWidget(parent) {
 auto main = new QVBoxLayout(this);
 auto row1 = new QHBoxLayout();
 playButton = new QPushButton("? Play");
 connect(playButton, &QPushButton::clicked, this, &ControlPanel::playClicked);
 row1->addWidget(playButton);
 main->addLayout(row1);
 auto speed = new QHBoxLayout();
 speed->addWidget(new QLabel("Speed:"));
 speedSlider = new QSlider(Qt::Horizontal); speedSlider->setRange(1,200); speedSlider->setValue(100);
 speed->addWidget(speedSlider);
 main->addLayout(speed);
 auto algo = new QHBoxLayout();
 algo->addWidget(new QLabel("Algorithm:"));
 algorithmCombo = new QComboBox();
 algo->addWidget(algorithmCombo);
 connect(algorithmCombo, &QComboBox::currentTextChanged, this, &ControlPanel::algorithmSelected);
 main->addLayout(algo);
 auto ds = new QHBoxLayout();
 ds->addWidget(new QLabel("Structure:"));
 dataStructureCombo = new QComboBox();
 ds->addWidget(dataStructureCombo);
 main->addLayout(ds);
 auto size = new QHBoxLayout();
 size->addWidget(new QLabel("Size:"));
 dataSizeSpinBox = new QSpinBox(); dataSizeSpinBox->setRange(1,100); dataSizeSpinBox->setValue(10);
 size->addWidget(dataSizeSpinBox);
 main->addLayout(size);
 frameLabel = new QLabel("Frame:0/0");
 main->addWidget(frameLabel);
}

ControlPanel::~ControlPanel() = default;

void ControlPanel::populateAlgorithms(const std::vector<std::string>& algos) {
 algorithmCombo->clear();
 for (auto& a : algos) algorithmCombo->addItem(QString::fromStdString(a));
}

std::string ControlPanel::getSelectedDataStructure() const { return dataStructureCombo->currentText().toStdString(); }
int ControlPanel::getDataSize() const { return dataSizeSpinBox->value(); }
