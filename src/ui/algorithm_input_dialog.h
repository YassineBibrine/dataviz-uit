#ifndef ALGORITHM_INPUT_DIALOG_H
#define ALGORITHM_INPUT_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QString>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <vector>
#include <string>

/**
 * @class AlgorithmInputDialog
 * @brief Dialog for collecting algorithm parameters from user
 */
class AlgorithmInputDialog : public QDialog {
  Q_OBJECT

public:
    explicit AlgorithmInputDialog(const QString& algorithmName, QWidget* parent = nullptr);
    
  // Add different input types
    void addIntInput(const QString& label, int defaultValue = 0, int min = -999999, int max = 999999);
    void addStringInput(const QString& label, const QString& defaultValue = "");
    void addNodeSelection(const QString& label, const std::vector<std::string>& nodeIds);
    void addDoubleInput(const QString& label, double defaultValue = 0.0, double min = -999999.0, double max = 999999.0);
    
    // Get results
    int getIntValue(int index) const;
    QString getStringValue(int index) const;
    double getDoubleValue(int index) const;

private:
QFormLayout* formLayout;
 QDialogButtonBox* buttonBox;
    std::vector<QWidget*> inputWidgets;
    std::vector<QString> inputTypes;
};

#endif // ALGORITHM_INPUT_DIALOG_H
