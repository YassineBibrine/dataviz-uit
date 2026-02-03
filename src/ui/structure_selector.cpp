#include "structure_selector.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QGroupBox>
#include <QWheelEvent>

// Custom ComboBox that ignores wheel events to prevent scroll hijacking
class NoScrollComboBox : public QComboBox {
public:
  explicit NoScrollComboBox(QWidget* parent = nullptr) : QComboBox(parent) {
      setFocusPolicy(Qt::StrongFocus);
    }
protected:
 void wheelEvent(QWheelEvent* event) override {
        if (!hasFocus()) { event->ignore(); } else { QComboBox::wheelEvent(event); }
    }
};

// Custom SpinBox that ignores wheel events to prevent scroll hijacking
class NoScrollSpinBox : public QSpinBox {
public:
    explicit NoScrollSpinBox(QWidget* parent = nullptr) : QSpinBox(parent) {
        setFocusPolicy(Qt::StrongFocus);
    }
protected:
    void wheelEvent(QWheelEvent* event) override {
      if (!hasFocus()) { event->ignore(); } else { QSpinBox::wheelEvent(event); }
    }
};

StructureSelector::StructureSelector(QWidget* parent)
    : QWidget(parent), dataManager(nullptr) {
    setupUI();
}

void StructureSelector::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
 mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(8);
  
    QLabel* title = new QLabel("Data Structures", this);
    title->setStyleSheet("font-weight: bold; font-size: 12pt;");
    mainLayout->addWidget(title);
    
    selectedLabel = new QLabel("No structure selected", this);
    selectedLabel->setStyleSheet("color: #666; font-size: 9pt;");
    mainLayout->addWidget(selectedLabel);
    
    structureList = new QListWidget(this);
    structureList->setMinimumHeight(120);
    structureList->setMaximumHeight(180);
    connect(structureList, &QListWidget::itemClicked, this, &StructureSelector::onStructureClicked);
    mainLayout->addWidget(structureList);
    
    statsLabel = new QLabel("", this);
    statsLabel->setStyleSheet("color: #888; font-size: 8pt; font-style: italic;");
    mainLayout->addWidget(statsLabel);
  
    QGroupBox* manageGroup = new QGroupBox("Structure Management", this);
    QVBoxLayout* manageLayout = new QVBoxLayout(manageGroup);
    manageLayout->setContentsMargins(8, 15, 8, 8);
manageLayout->setSpacing(6);
    
    QLabel* createLabel = new QLabel("Create New:", this);
    createLabel->setStyleSheet("font-weight: bold; color: #555;");
    manageLayout->addWidget(createLabel);
  
    QHBoxLayout* typeRowLayout = new QHBoxLayout();
    typeRowLayout->addWidget(new QLabel("Type:", this));
 typeCombo = new NoScrollComboBox(this);
    typeCombo->addItems({"Array", "List", "Binary Tree", "Graph"});
    typeRowLayout->addWidget(typeCombo, 1);
    typeRowLayout->addSpacing(10);
    typeRowLayout->addWidget(new QLabel("Size:", this));
    sizeSpinBox = new NoScrollSpinBox(this);
    sizeSpinBox->setRange(1, 100);
    sizeSpinBox->setValue(6);
    sizeSpinBox->setFixedWidth(60);
    typeRowLayout->addWidget(sizeSpinBox);
    manageLayout->addLayout(typeRowLayout);
    
    QHBoxLayout* nameLayout = new QHBoxLayout();
    nameLayout->addWidget(new QLabel("Name:", this));
    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("Optional");
    nameLayout->addWidget(nameEdit, 1);
    manageLayout->addLayout(nameLayout);
    
    finalizeBtn = new QPushButton("+ Create Structure", this);
    finalizeBtn->setToolTip("Create a new data structure with random data");
    finalizeBtn->setStyleSheet("background-color: #2196F3; color: white; font-weight: bold;");
    connect(finalizeBtn, &QPushButton::clicked, this, &StructureSelector::onFinalizeClicked);
    manageLayout->addWidget(finalizeBtn);
    
    manageLayout->addSpacing(8);
    
    QFrame* separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    manageLayout->addWidget(separator);
    
    manageLayout->addSpacing(4);
    
    QLabel* actionsLabel = new QLabel("Quick Actions:", this);
    actionsLabel->setStyleSheet("font-weight: bold; color: #555;");
    manageLayout->addWidget(actionsLabel);
    
    QHBoxLayout* editButtonsLayout = new QHBoxLayout();
    renameBtn = new QPushButton("Rename", this);
    renameBtn->setToolTip("Rename selected structure");
    connect(renameBtn, &QPushButton::clicked, this, &StructureSelector::onRenameClicked);
    editButtonsLayout->addWidget(renameBtn);
    
    removeBtn = new QPushButton("Remove", this);
removeBtn->setToolTip("Remove selected structure");
    removeBtn->setStyleSheet("background-color: #d9534f; color: white;");
    connect(removeBtn, &QPushButton::clicked, this, &StructureSelector::onRemoveClicked);
    editButtonsLayout->addWidget(removeBtn);
    manageLayout->addLayout(editButtonsLayout);

    mainLayout->addWidget(manageGroup);
    updateButtonStates();
}

void StructureSelector::setDataModelManager(DataModelManager* manager) {
    dataManager = manager;
 refreshStructureList();
}

void StructureSelector::refreshStructureList() {
    structureList->clear();
    if (!dataManager) return;
  
    auto structures = dataManager->getAllStructures();
    for (const auto& meta : structures) {
    QString displayText = QString::fromStdString(meta.name);
        displayText += QString(" [%1]").arg(QString::fromStdString(meta.type));
      displayText += (meta.creationType == StructureCreationType::INTERACTIVE) ? " (custom)" : " (generated)";
        
      QListWidgetItem* item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, QString::fromStdString(meta.id));
        if (meta.isSelected) {
            QFont font = item->font();
        font.setBold(true);
    item->setFont(font);
            item->setBackground(QColor("#e3f2fd"));
        }
    structureList->addItem(item);
    }
    
    std::string selectedId = dataManager->getSelectedStructureId();
    if (!selectedId.empty()) {
        for (const auto& meta : structures) {
     if (meta.id == selectedId) {
    selectedLabel->setText(QString("Selected: %1").arg(QString::fromStdString(meta.name)));
   break;
            }
        }
    } else {
      selectedLabel->setText("No structure selected");
    }
    statsLabel->setText(QString("%1 structure(s)").arg(structures.size()));
    updateButtonStates();
}

void StructureSelector::onStructureClicked(QListWidgetItem* item) {
    if (!item) return;
    QString structureId = item->data(Qt::UserRole).toString();
    currentSelectedId = structureId.toStdString();
    if (dataManager) dataManager->selectStructure(currentSelectedId);
    emit structureSelected(structureId);
    refreshStructureList();
}

void StructureSelector::onRemoveClicked() {
    if (currentSelectedId.empty()) {
     QMessageBox::warning(this, "No Selection", "Please select a structure to remove.");
        return;
    }
    auto reply = QMessageBox::question(this, "Confirm Removal", "Are you sure you want to remove this structure?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes && dataManager) {
        dataManager->removeStructure(currentSelectedId);
 emit structureRemoved(QString::fromStdString(currentSelectedId));
        currentSelectedId = "";
        refreshStructureList();
    }
}

void StructureSelector::onFinalizeClicked() {
    if (!dataManager) {
        QMessageBox::warning(this, "Error", "Data manager not initialized.");
    return;
    }
  QString type = typeCombo->currentText();
    QString name = nameEdit->text().trimmed();
    int size = sizeSpinBox->value();
    std::string structId = dataManager->createDataStructure(type.toStdString(), size, name.isEmpty() ? "" : name.toStdString());
    if (!structId.empty()) {
        refreshStructureList();
      dataManager->selectStructure(structId);
      currentSelectedId = structId;
        
  // **NEW**: Save session after creating structure
     dataManager->saveSession();
    qDebug() << "Structure created and session saved:" << QString::fromStdString(structId);
     
    emit structureSelected(QString::fromStdString(structId));
      nameEdit->clear();
    } else {
      QMessageBox::warning(this, "Creation Failed", "Failed to create structure. Please try again.");
    }
}

void StructureSelector::onRenameClicked() {
    if (currentSelectedId.empty()) {
        QMessageBox::warning(this, "No Selection", "Please select a structure to rename.");
        return;
    }
    if (!dataManager) return;
    auto structures = dataManager->getAllStructures();
  std::string currentName;
    for (const auto& meta : structures) {
        if (meta.id == currentSelectedId) { currentName = meta.name; break; }
    }
    bool ok;
    QString newName = QInputDialog::getText(this, "Rename Structure", "Enter new name:", QLineEdit::Normal, QString::fromStdString(currentName), &ok);
    if (ok && !newName.isEmpty()) {
   dataManager->renameStructure(currentSelectedId, newName.toStdString());
   refreshStructureList();
    }
}

void StructureSelector::updateButtonStates() {
    bool hasSelection = !currentSelectedId.empty();
    removeBtn->setEnabled(hasSelection);
  renameBtn->setEnabled(hasSelection);
}
