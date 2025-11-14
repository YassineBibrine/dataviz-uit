#pragma once

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <vector>
#include <string>

/**
 * @class ControlPanel
 * @brief Control panel with playback controls and draggable data structure selector
 */
class ControlPanel : public QWidget {
 Q_OBJECT
public:
  explicit ControlPanel(QWidget* parent = nullptr);
  ~ControlPanel() override;
    
    void populateAlgorithms(const std::vector<std::string>& algorithms);
    std::string getSelectedDataStructure() const;
    int getDataSize() const;
    
signals:
    void playClicked();
    void algorithmSelected(const QString& algorithm);

private:
    QPushButton* playButton{nullptr};
    QSlider* speedSlider{nullptr};
    QComboBox* algorithmCombo{nullptr};
    QComboBox* dataStructureCombo{nullptr};
    QSpinBox* dataSizeSpinBox{nullptr};
    QLabel* frameLabel{nullptr};
};
