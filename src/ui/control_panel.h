#pragma once

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>
#include <QLabel>
#include <QGroupBox>
#include <QSpinBox>
#include <vector>
#include <QString>

class ControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ControlPanel(QWidget* parent = nullptr);

    void setPlayingState(bool playing);
    void enableControls(bool enabled);

    // ⚠️ Remplace populateAlgorithms (obsolète)
    void updateAlgorithmList(const QString& structureType);
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

private:
    // Boutons
    QPushButton* playButton{nullptr};
    QPushButton* pauseButton{nullptr};
    QPushButton* stepForwardButton{nullptr};
    QPushButton* stepBackwardButton{nullptr};
    QPushButton* resetButton{nullptr};

    // Controls
    QSlider* speedSlider{nullptr};
    QComboBox* algorithmCombo{nullptr};

    QLabel* currentFrameLabel{nullptr};
};