#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include <QWidget>

class QPushButton;
class QSlider;
class QComboBox;
class QSpinBox;

/**
 * @class ControlPanel
 * @brief Controls for algorithm execution and visualization playback
 * 
 * Provides user controls for:
 * - Play/Pause execution
 * - Step forward/backward
 * - Speed adjustment
 * - Algorithm selection
 * - Data size configuration
 */
class ControlPanel : public QWidget {
    Q_OBJECT

public:
    ControlPanel(QWidget *parent = nullptr);
    ~ControlPanel();

signals:
    void playClicked();
    void pauseClicked();
    void stepForwardClicked();
    void stepBackwardClicked();
    void speedChanged(int speed);
    void algorithmSelected(const QString &algorithm);
  void dataSizeChanged(int size);

private:
    void setupUI();
    void connectSignals();
    
    QPushButton *playButton;
    QPushButton *pauseButton;
    QPushButton *stepForwardButton;
    QPushButton *stepBackwardButton;
    QSlider *speedSlider;
    QComboBox *algorithmCombo;
    QSpinBox *dataSizeSpinBox;
};

#endif // CONTROL_PANEL_H
