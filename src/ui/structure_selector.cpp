#include "structure_selector.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QGroupBox>

StructureSelector::StructureSelector(QWidget* parent)
    : QWidget(parent), dataManager(nullptr) {
    setupUI();
}

void StructureSelector::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(8);
  
    // Title
  QLabel* title = new QLabel("Data Structures", this);
    title->setStyleSheet("font-weight: bold; font-size: 12pt;");
    mainLayout->addWidget(title);
    
    // Status label
selectedLabel = new QLabel("No structure selected", this);
    selectedLabel->setStyleSheet("color: #666; font-size: 9pt;");
    mainLayout->addWidget(selectedLabel);
    
    // Structure list
    structureList = new QListWidget(this);
    structureList->setMinimumHeight(120);
 structureList->setMaximumHeight(180);
    connect(structureList, &QListWidget::itemClicked, 
       this, &StructureSelector::onStructureClicked);
    mainLayout->addWidget(structureList);
    
    // Statistics label
  statsLabel = new QLabel("", this);
    statsLabel->setStyleSheet("color: #888; font-size: 8pt; font-style: italic;");
    mainLayout->addWidget(statsLabel);
    
 // --- Interactive Finalization Group ---
    QGroupBox* finalizeGroup = new QGroupBox("Finalize Interactive", this);
    QVBoxLayout* finalizeLayout = new QVBoxLayout(finalizeGroup);
    finalizeLayout->setContentsMargins(8, 15, 8, 8);
    finalizeLayout->setSpacing(6);
 
    // Type selection
    QHBoxLayout* typeLayout = new QHBoxLayout();
    typeLayout->addWidget(new QLabel("Type:", this));
    typeCombo = new QComboBox(this);
    typeCombo->addItems({"Auto", "Graph", "Tree", "List", "Array"});
    typeLayout->addWidget(typeCombo, 1);
 finalizeLayout->addLayout(typeLayout);
 
// Name input
    QHBoxLayout* nameLayout = new QHBoxLayout();
    nameLayout->addWidget(new QLabel("Name:", this));
    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("Optional custom name");
 nameLayout->addWidget(nameEdit, 1);
    finalizeLayout->addLayout(nameLayout);
    
 // Finalize button
  finalizeBtn = new QPushButton("Create Structure", this);
    finalizeBtn->setToolTip("Convert drawn nodes/edges into a data structure");
    connect(finalizeBtn, &QPushButton::clicked, 
      this, &StructureSelector::onFinalizeClicked);
    finalizeLayout->addWidget(finalizeBtn);
    
    // Clear interactive button
    clearInteractiveBtn = new QPushButton("Clear Canvas", this);
    clearInteractiveBtn->setToolTip("Clear all drawn nodes/edges");
    connect(clearInteractiveBtn, &QPushButton::clicked,
   this, &StructureSelector::onClearInteractiveClicked);
    finalizeLayout->addWidget(clearInteractiveBtn);
    
    mainLayout->addWidget(finalizeGroup);
  
    // --- Management Buttons ---
    QGroupBox* manageGroup = new QGroupBox("Manage Structures", this);
    QVBoxLayout* manageLayout = new QVBoxLayout(manageGroup);
    manageLayout->setContentsMargins(8, 12, 8, 8);
    manageLayout->setSpacing(6);
    
    // Add "Create Samples" button at the top
    QPushButton* createSamplesBtn = new QPushButton("?? Create Sample Structures", this);
    createSamplesBtn->setToolTip("Create one sample structure for each type (Array, List, Tree, Graph)");
    createSamplesBtn->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
  connect(createSamplesBtn, &QPushButton::clicked, 
            this, &StructureSelector::onCreateSamplesClicked);
    manageLayout->addWidget(createSamplesBtn);
    
    manageLayout->addSpacing(4);
  
    renameBtn = new QPushButton("Rename", this);
    connect(renameBtn, &QPushButton::clicked, 
            this, &StructureSelector::onRenameClicked);
  manageLayout->addWidget(renameBtn);
 
    removeBtn = new QPushButton("Remove", this);
    removeBtn->setStyleSheet("background-color: #d9534f; color: white;");
    connect(removeBtn, &QPushButton::clicked, 
          this, &StructureSelector::onRemoveClicked);
    manageLayout->addWidget(removeBtn);

    mainLayout->addWidget(manageGroup);
    
    // Don't add stretch - let it take natural size
    
    updateButtonStates();
}

void StructureSelector::setDataModelManager(DataModelManager* manager) {
    dataManager = manager;
    refreshStructureList();
}

void StructureSelector::refreshStructureList() {
    structureList->clear();
    
    if (!dataManager) {
    return;
    }
    
  auto structures = dataManager->getAllStructures();
    
    for (const auto& meta : structures) {
   QString displayText = QString::fromStdString(meta.name);
   
        // Add type indicator
        displayText += QString(" [%1]").arg(QString::fromStdString(meta.type));
      
      // Add creation type indicator
  if (meta.creationType == StructureCreationType::INTERACTIVE) {
            displayText += " ?"; // Pencil icon for interactive
        } else {
       displayText += " ?"; // Gear icon for generated
        }
        
      QListWidgetItem* item = new QListWidgetItem(displayText);
     item->setData(Qt::UserRole, QString::fromStdString(meta.id));

        // Highlight selected structure
  if (meta.isSelected) {
         QFont font = item->font();
     font.setBold(true);
      item->setFont(font);
     item->setBackground(QColor("#e3f2fd"));
        }
        
      structureList->addItem(item);
    }
    
    // Update selected label
    std::string selectedId = dataManager->getSelectedStructureId();
    if (!selectedId.empty()) {
        auto structures = dataManager->getAllStructures();
        for (const auto& meta : structures) {
          if (meta.id == selectedId) {
        selectedLabel->setText(QString("Selected: %1")
           .arg(QString::fromStdString(meta.name)));
              break;
            }
        }
    } else {
 selectedLabel->setText("No structure selected");
    }
    
    // Update stats
    statsLabel->setText(QString("%1 structure(s)").arg(structures.size()));
    
  updateButtonStates();
}

void StructureSelector::onStructureClicked(QListWidgetItem* item) {
    if (!item) return;
    
    QString structureId = item->data(Qt::UserRole).toString();
  currentSelectedId = structureId.toStdString();
    
    if (dataManager) {
   dataManager->selectStructure(currentSelectedId);
}
 
    emit structureSelected(structureId);
    refreshStructureList();
}

void StructureSelector::onRemoveClicked() {
    if (currentSelectedId.empty()) {
        QMessageBox::warning(this, "No Selection", 
      "Please select a structure to remove.");
        return;
    }
    
    auto reply = QMessageBox::question(this, "Confirm Removal",
        "Are you sure you want to remove this structure?",
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes && dataManager) {
  dataManager->removeStructure(currentSelectedId);
 emit structureRemoved(QString::fromStdString(currentSelectedId));
        currentSelectedId = "";
    refreshStructureList();
    }
}

void StructureSelector::onFinalizeClicked() {
    QString type = typeCombo->currentText();
    QString name = nameEdit->text().trimmed();
    
    emit finalizeInteractiveRequested(type, name);
    
    // Clear name field after finalization
    nameEdit->clear();
}

void StructureSelector::onRenameClicked() {
  if (currentSelectedId.empty()) {
        QMessageBox::warning(this, "No Selection",
            "Please select a structure to rename.");
      return;
    }
    
 if (!dataManager) return;
    
    // Get current name
    auto structures = dataManager->getAllStructures();
    std::string currentName;
    for (const auto& meta : structures) {
     if (meta.id == currentSelectedId) {
     currentName = meta.name;
          break;
      }
    }
    
    bool ok;
 QString newName = QInputDialog::getText(this, "Rename Structure",
        "Enter new name:", QLineEdit::Normal,
        QString::fromStdString(currentName), &ok);
  
  if (ok && !newName.isEmpty()) {
        dataManager->renameStructure(currentSelectedId, newName.toStdString());
        refreshStructureList();
    }
}

void StructureSelector::onClearInteractiveClicked() {
 auto reply = QMessageBox::question(this, "Clear Canvas",
        "Clear all drawn nodes and edges?",
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
    emit clearInteractiveRequested();
    }
}

void StructureSelector::updateButtonStates() {
    bool hasSelection = !currentSelectedId.empty();
    
    removeBtn->setEnabled(hasSelection);
    renameBtn->setEnabled(hasSelection);
}

void StructureSelector::onCreateSamplesClicked() {
    if (!dataManager) {
      QMessageBox::warning(this, "No Data Manager",
            "Data manager not initialized.");
   return;
  }
    
    // Confirm with the user
    auto reply = QMessageBox::question(this, "Create Sample Structures",
        "This will create 4 sample structures:\n"
      "• Sample Array [5, 12, 8, 3, 15, 10]\n"
     "• Sample Linked List [1?3?5?7?9]\n"
        "• Sample Binary Tree [BST with 6 nodes]\n"
   "• Sample Graph [4 nodes, connected]\n\n"
        "Continue?",
 QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        // Create the sample structures
        std::vector<std::string> createdIds = dataManager->createSampleStructures();
        
  // Refresh the list to show new structures
   refreshStructureList();
        
        // Notify success
        QMessageBox::information(this, "Samples Created",
            QString("Successfully created %1 sample structure(s).\n"
     "You can now select and run algorithms on them!")
      .arg(createdIds.size()));
        
    // Emit signal that samples were created
 emit samplesCreated();
    }
}
