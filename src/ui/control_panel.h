#pragma once

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QGroupBox> // Important pour les groupes
#include <vector>

class ControlPanel : public QWidget
{
    Q_OBJECT // Macro obligatoire pour les signaux

public:
    explicit ControlPanel(QWidget* parent = nullptr);

    // Méthodes pour configurer le panneau depuis l'extérieur
    void setPlayingState(bool playing);
    void enableControls(bool enabled);
    void populateAlgorithms(const std::vector<QString>& algorithms);
    void populateDataStructures(const std::vector<QString>& structures);
    QString getSelectedDataStructure() const;

    // Initialisation
    void setupUI();
    void connectSignals();

signals:
    // Les signaux que ce panneau envoie au reste de l'app
    void playClicked();
    void pauseClicked();
    void stepForwardClicked();
    void stepBackwardClicked();
    void resetClicked();
    void speedChanged(int speed);
    void algorithmSelected(QString algorithm);
    void dataStructureSelected(QString structure);
    void dataSizeChanged(int size);

private:
    // --- LES VARIABLES (Ce qui manquait et causait les erreurs) ---

    // Boutons
    QPushButton* playButton{nullptr};
    QPushButton* pauseButton{nullptr};
    QPushButton* stepForwardButton{nullptr};
    QPushButton* stepBackwardButton{nullptr};
    QPushButton* resetButton{nullptr};

    // Sliders und Menus
    QSlider* speedSlider{nullptr};
    QComboBox* algorithmCombo{nullptr};
    QComboBox* dataStructureCombo{nullptr};
    QSpinBox* dataSizeSpinBox{nullptr};
    QLabel* currentFrameLabel{nullptr};
};