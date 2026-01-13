#include "algorithm_input_dialog.h"
#include <QVBoxLayout>
#include <QPushButton>

AlgorithmInputDialog::AlgorithmInputDialog(const QString& algorithmName, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Algorithm Parameters: " + algorithmName);
    setModal(true);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QLabel* titleLabel = new QLabel("Enter parameters for " + algorithmName + ":", this);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 12pt; margin-bottom: 10px;");
    mainLayout->addWidget(titleLabel);
    
    formLayout = new QFormLayout();
    mainLayout->addLayout(formLayout);
    
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);
    
    setMinimumWidth(350);
}

void AlgorithmInputDialog::addIntInput(const QString& label, int defaultValue, int min, int max) {
 QSpinBox* spinBox = new QSpinBox(this);
    spinBox->setRange(min, max);
    spinBox->setValue(defaultValue);
 spinBox->setMinimumWidth(200);
    formLayout->addRow(label + ":", spinBox);
    inputWidgets.push_back(spinBox);
    inputTypes.push_back("int");
}

void AlgorithmInputDialog::addStringInput(const QString& label, const QString& defaultValue) {
 QLineEdit* lineEdit = new QLineEdit(defaultValue, this);
    lineEdit->setMinimumWidth(200);
    formLayout->addRow(label + ":", lineEdit);
    inputWidgets.push_back(lineEdit);
    inputTypes.push_back("string");
}

void AlgorithmInputDialog::addNodeSelection(const QString& label, const std::vector<std::string>& nodeIds) {
    QComboBox* comboBox = new QComboBox(this);
    for (const auto& nodeId : nodeIds) {
  comboBox->addItem(QString::fromStdString(nodeId));
    }
    comboBox->setMinimumWidth(200);
    formLayout->addRow(label + ":", comboBox);
    inputWidgets.push_back(comboBox);
    inputTypes.push_back("combo");
}

void AlgorithmInputDialog::addDoubleInput(const QString& label, double defaultValue, double min, double max) {
    QDoubleSpinBox* spinBox = new QDoubleSpinBox(this);
    spinBox->setRange(min, max);
    spinBox->setValue(defaultValue);
    spinBox->setDecimals(2);
    spinBox->setMinimumWidth(200);
    formLayout->addRow(label + ":", spinBox);
    inputWidgets.push_back(spinBox);
    inputTypes.push_back("double");
}

int AlgorithmInputDialog::getIntValue(int index) const {
    if (index >= 0 && index < static_cast<int>(inputWidgets.size()) && inputTypes[index] == "int") {
        return static_cast<QSpinBox*>(inputWidgets[index])->value();
    }
    return 0;
}

QString AlgorithmInputDialog::getStringValue(int index) const {
    if (index >= 0 && index < static_cast<int>(inputWidgets.size())) {
        if (inputTypes[index] == "string") {
     return static_cast<QLineEdit*>(inputWidgets[index])->text();
      } else if (inputTypes[index] == "combo") {
            return static_cast<QComboBox*>(inputWidgets[index])->currentText();
 }
    }
    return "";
}

double AlgorithmInputDialog::getDoubleValue(int index) const {
    if (index >= 0 && index < static_cast<int>(inputWidgets.size()) && inputTypes[index] == "double") {
     return static_cast<QDoubleSpinBox*>(inputWidgets[index])->value();
    }
    return 0.0;
}
