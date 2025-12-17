#pragma once

#include <QDialog>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QTabWidget>
#include <memory>

class DataModelManager;
class DataStructure;

/**
 * @class CodeGeneratorDialog
 * @brief Dialog for generating C++ code from structures and parsing code to create structures
 */
class CodeGeneratorDialog : public QDialog {
    Q_OBJECT

public:
    explicit CodeGeneratorDialog(DataModelManager* dataManager, QWidget* parent = nullptr);
    ~CodeGeneratorDialog() override = default;

signals:
    void structureCreatedFromCode(QString structureId);

private slots:
    void onGenerateCode();
    void onParseCode();
    void onCopyToClipboard();
    void onStructureSelectionChanged(int index);

private:
    void setupUI();
    void populateStructureList();
    void showError(const QString& message);
    void showSuccess(const QString& message);

    // Data
 DataModelManager* dataManager;
  
    // UI Components - Generate Tab
    QComboBox* structureCombo;
    QCheckBox* includeCommentsCheck;
    QTextEdit* generatedCodeText;
 QPushButton* generateBtn;
    QPushButton* copyBtn;
    
  // UI Components - Parse Tab
    QTextEdit* inputCodeText;
  QPushButton* parseBtn;
    QTextEdit* parseResultText;
    
    // Tabs
    QTabWidget* tabWidget;
};
