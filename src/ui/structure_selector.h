#pragma once

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include "../core/data_model_manager.h"

/**
 * @class StructureSelector
 * @brief Widget for displaying and selecting data structures
 */
class StructureSelector : public QWidget {
    Q_OBJECT

public:
    explicit StructureSelector(QWidget* parent = nullptr);
    
    void setDataModelManager(DataModelManager* manager);
    void refreshStructureList();

signals:
    void structureSelected(const QString& structureId);
    void structureRemoved(const QString& structureId);
    void finalizeInteractiveRequested(const QString& type, const QString& name);
    void clearInteractiveRequested();
    void samplesCreated(); // NEW: Signal when samples are created

private slots:
    void onStructureClicked(QListWidgetItem* item);
    void onRemoveClicked();
    void onFinalizeClicked();
    void onRenameClicked();
    void onClearInteractiveClicked();
    void onCreateSamplesClicked(); // NEW: Slot for creating samples

private:
    void setupUI();
    void updateButtonStates();
    
    QListWidget* structureList;
    QPushButton* removeBtn;
    QPushButton* finalizeBtn;
    QPushButton* clearInteractiveBtn;
    QPushButton* renameBtn;
    QLabel* selectedLabel;
    QLabel* statsLabel;
    QComboBox* typeCombo;
    QLineEdit* nameEdit;
    QSpinBox* sizeSpinBox;
    
    DataModelManager* dataManager = nullptr;
    std::string currentSelectedId;
};
