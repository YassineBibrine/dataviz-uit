#pragma once

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>
#include <QLabel>
#include <QGroupBox>
#include <QSpinBox>
#include <vector>

class ControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ControlPanel(QWidget* parent = nullptr);

    void setPlayingState(bool playing);
    void enableControls(bool enabled);
    void populateAlgorithms(const std::vector<QString>& algorithms);
    void populateDataStructures(const std::vector<QString>& structures);
    QString getSelectedDataStructure() const;

    void setupUI();
    void connectSignals();

signals:
    void playClicked();
    void pauseClicked();
    void stepForwardClicked();
    void stepBackwardClicked();
    void resetClicked();
    void speedChanged(int speed);
    void algorithmSelected(QString algorithm);
    void dataStructureSelected(QString structure);
    void dataSizeChanged(int size);
    void generateNodesRequested(int count);

private:
    // Boutons
    QPushButton* playButton{nullptr};
    QPushButton* pauseButton{nullptr};
    QPushButton* stopButton{nullptr};            // gardé depuis main
    QPushButton* stepForwardButton{nullptr};
    QPushButton* stepBackwardButton{nullptr};
    QPushButton* resetButton{nullptr};
    QPushButton* generateNodesButton{nullptr};   // ajouté depuis membre4

    // Sliders et Menus
    QSlider* speedSlider{nullptr};
    QComboBox* algorithmCombo{nullptr};
    QComboBox* dataStructureCombo{nullptr};
    QSpinBox* dataSizeSpinBox{nullptr};

    QLabel* currentFrameLabel{nullptr};
};
