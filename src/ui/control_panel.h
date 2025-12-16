#pragma once

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>
#include <QLabel>
#include <QGroupBox>
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
    // On garde ce signal au cas où, mais il ne sera plus déclenché par l'UI
    void dataSizeChanged(int size);

private:
    // Boutons
    QPushButton* playButton;
    QPushButton* pauseButton;
    QPushButton* stopButton;
    QPushButton* stepForwardButton;
    QPushButton* stepBackwardButton;
    QPushButton* resetButton;

    // Sliders et Menus
    QSlider* speedSlider;
    QComboBox* algorithmCombo;
    QComboBox* dataStructureCombo;

    // (J'ai supprimé le ZoomSlider ici)

    QLabel* currentFrameLabel;
};