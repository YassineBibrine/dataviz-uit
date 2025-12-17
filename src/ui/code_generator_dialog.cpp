#include "code_generator_dialog.h"
#include "../codegen/structure_code_generator.h"
#include "../codegen/code_structure_parser.h"
#include "../core/data_model_manager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QApplication>
#include <QClipboard>
#include <QGroupBox>
#include <QSplitter>

CodeGeneratorDialog::CodeGeneratorDialog(DataModelManager* dataManager, QWidget* parent)
    : QDialog(parent), dataManager(dataManager) {
    setupUI();
    populateStructureList();
    
    setWindowTitle("Code Generator & Parser");
    resize(900, 700);
}

void CodeGeneratorDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Create tab widget
    tabWidget = new QTabWidget(this);
    
  // ============ TAB 1: GENERATE CODE ============
    QWidget* generateTab = new QWidget();
    QVBoxLayout* generateLayout = new QVBoxLayout(generateTab);
    
    // Structure selection
    QGroupBox* selectionGroup = new QGroupBox("Select Structure", generateTab);
    QVBoxLayout* selectionLayout = new QVBoxLayout(selectionGroup);
  
    QHBoxLayout* structureSelectLayout = new QHBoxLayout();
    structureSelectLayout->addWidget(new QLabel("Structure:", this));
    structureCombo = new QComboBox(this);
    structureSelectLayout->addWidget(structureCombo, 1);
    selectionLayout->addLayout(structureSelectLayout);
    
 includeCommentsCheck = new QCheckBox("Include comments", this);
    includeCommentsCheck->setChecked(true);
selectionLayout->addWidget(includeCommentsCheck);
    
    generateLayout->addWidget(selectionGroup);
    
    // Generate button
    QHBoxLayout* generateBtnLayout = new QHBoxLayout();
    generateBtn = new QPushButton("Generate C++ Code", this);
    generateBtn->setStyleSheet("background-color: #3182CE; color: white; font-weight: bold; padding: 8px;");
    connect(generateBtn, &QPushButton::clicked, this, &CodeGeneratorDialog::onGenerateCode);
    generateBtnLayout->addStretch();
    generateBtnLayout->addWidget(generateBtn);
    generateBtnLayout->addStretch();
    generateLayout->addLayout(generateBtnLayout);
    
    // Generated code display
    QGroupBox* codeGroup = new QGroupBox("Generated Code", generateTab);
    QVBoxLayout* codeLayout = new QVBoxLayout(codeGroup);
    
    generatedCodeText = new QTextEdit(this);
    generatedCodeText->setReadOnly(true);
    generatedCodeText->setFont(QFont("Courier New", 10));
    generatedCodeText->setPlaceholderText("Generated C++ code will appear here...");
    codeLayout->addWidget(generatedCodeText);
    
    // Copy button
    copyBtn = new QPushButton("Copy to Clipboard", this);
    copyBtn->setEnabled(false);
    connect(copyBtn, &QPushButton::clicked, this, &CodeGeneratorDialog::onCopyToClipboard);
    codeLayout->addWidget(copyBtn);
    
    generateLayout->addWidget(codeGroup);
    
    // ============ TAB 2: PARSE CODE ============
    QWidget* parseTab = new QWidget();
    QVBoxLayout* parseLayout = new QVBoxLayout(parseTab);
    
    // Input code
    QGroupBox* inputGroup = new QGroupBox("Input C++ Code", parseTab);
    QVBoxLayout* inputLayout = new QVBoxLayout(inputGroup);
    
    inputCodeText = new QTextEdit(this);
    inputCodeText->setFont(QFont("Courier New", 10));
    inputCodeText->setPlaceholderText(
        "Paste your C++ code here...\n\n"
     "Supported structures:\n"
        "- Array: vector<int> arr = {1, 2, 3};\n"
  "- LinkedList: new Node(value);\n"
        "- BinaryTree: insert(root, value);\n"
    "- Graph: addNode(\"id\"); addEdge(\"from\", \"to\");"
    );
    inputLayout->addWidget(inputCodeText);
    
 parseLayout->addWidget(inputGroup);
    
    // Parse button
    QHBoxLayout* parseBtnLayout = new QHBoxLayout();
    parseBtn = new QPushButton("Parse & Create Structure", this);
    parseBtn->setStyleSheet("background-color: #38A169; color: white; font-weight: bold; padding: 8px;");
    connect(parseBtn, &QPushButton::clicked, this, &CodeGeneratorDialog::onParseCode);
    parseBtnLayout->addStretch();
    parseBtnLayout->addWidget(parseBtn);
    parseBtnLayout->addStretch();
    parseLayout->addLayout(parseBtnLayout);
    
    // Parse result
    QGroupBox* resultGroup = new QGroupBox("Parse Result", parseTab);
    QVBoxLayout* resultLayout = new QVBoxLayout(resultGroup);
    
    parseResultText = new QTextEdit(this);
    parseResultText->setReadOnly(true);
    parseResultText->setFont(QFont("Segoe UI", 10));
    parseResultText->setPlaceholderText("Parse results will appear here...");
    resultLayout->addWidget(parseResultText);
    
    parseLayout->addWidget(resultGroup);
    
    // Add tabs
tabWidget->addTab(generateTab, "Generate Code from Structure");
    tabWidget->addTab(parseTab, "Parse Code to Create Structure");
    
    mainLayout->addWidget(tabWidget);
    
    // Close button
    QHBoxLayout* closeLayout = new QHBoxLayout();
    QPushButton* closeBtn = new QPushButton("Close", this);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    closeLayout->addStretch();
    closeLayout->addWidget(closeBtn);
    mainLayout->addLayout(closeLayout);
    
    connect(structureCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CodeGeneratorDialog::onStructureSelectionChanged);
}

void CodeGeneratorDialog::populateStructureList() {
    structureCombo->clear();
    
    if (!dataManager) {
  return;
  }
    
    auto structures = dataManager->getAllStructures();
    
    if (structures.empty()) {
structureCombo->addItem("No structures available", "");
    generateBtn->setEnabled(false);
     return;
    }
    
    for (const auto& meta : structures) {
        QString displayText = QString::fromStdString(meta.name) + 
          " [" + QString::fromStdString(meta.type) + "]";
        structureCombo->addItem(displayText, QString::fromStdString(meta.id));
    }
    
    generateBtn->setEnabled(true);
}

void CodeGeneratorDialog::onStructureSelectionChanged(int index) {
    // Clear previous generated code when selection changes
    generatedCodeText->clear();
    copyBtn->setEnabled(false);
}

void CodeGeneratorDialog::onGenerateCode() {
    if (!dataManager) {
 showError("Data manager not available");
        return;
    }
    
    QString structureId = structureCombo->currentData().toString();
    if (structureId.isEmpty()) {
    showError("No structure selected");
        return;
    }
 
    const DataStructure* structure = dataManager->getStructure(structureId.toStdString());
    if (!structure) {
        showError("Structure not found");
        return;
    }
    
    bool includeComments = includeCommentsCheck->isChecked();
    std::string generatedCode = StructureCodeGenerator::generateCode(structure, includeComments);
    
    if (generatedCode.find("Error:") == 0) {
        showError(QString::fromStdString(generatedCode));
     return;
    }
    
 generatedCodeText->setPlainText(QString::fromStdString(generatedCode));
    copyBtn->setEnabled(true);
    
 showSuccess("Code generated successfully!");
}

void CodeGeneratorDialog::onParseCode() {
  QString code = inputCodeText->toPlainText();
    
    if (code.trimmed().isEmpty()) {
      showError("Please enter C++ code to parse");
 return;
    }
    
    // Parse the code
    ParsedStructure parsed = CodeStructureParser::parse(code.toStdString());
    
    if (!parsed.success) {
        parseResultText->setPlainText(
    "? Parse Failed\n\n" +
    QString::fromStdString(parsed.errorMessage)
     );
        return;
    }
    
    // Display parse results
    QString resultText = "? Parse Successful!\n\n";
    
    std::string typeStr;
    switch (parsed.type) {
        case ParsedStructure::Type::ARRAY:
            typeStr = "Array";
break;
        case ParsedStructure::Type::LINKED_LIST:
     typeStr = "Linked List";
            break;
        case ParsedStructure::Type::BINARY_TREE:
            typeStr = "Binary Tree";
       break;
        case ParsedStructure::Type::GRAPH:
            typeStr = "Graph";
    break;
        default:
            typeStr = "Unknown";
    }
    
    resultText += "Detected Type: " + QString::fromStdString(typeStr) + "\n\n";
  
    if (!parsed.values.empty()) {
        resultText += "Values: ";
        for (size_t i = 0; i < parsed.values.size(); ++i) {
   if (i > 0) resultText += ", ";
            resultText += QString::number(parsed.values[i]);
        }
        resultText += "\n\n";
    }
    
    if (!parsed.nodeValues.empty()) {
   resultText += "Nodes:\n";
        for (const auto& [nodeId, value] : parsed.nodeValues) {
   resultText += "  " + QString::fromStdString(nodeId) + 
         " (value: " + QString::number(value) + ")\n";
    }
     resultText += "\n";
    }
    
    if (!parsed.edges.empty()) {
        resultText += "Edges:\n";
        for (const auto& [from, to] : parsed.edges) {
         resultText += "  " + QString::fromStdString(from) + 
        " -> " + QString::fromStdString(to) + "\n";
        }
        resultText += "\n";
    }
    
    parseResultText->setPlainText(resultText);
    
    // Create structure in data manager
    if (!dataManager) {
      showError("Data manager not available");
        return;
    }
  
    std::string structureId;
    
    try {
        if (parsed.type == ParsedStructure::Type::ARRAY) {
       // Create array structure
    structureId = dataManager->createDataStructure("Array", parsed.values.size(), "Parsed Array");
            if (auto* structure = dataManager->getStructure(structureId)) {
     if (auto* arr = dynamic_cast<ArrayStructure*>(structure)) {
  arr->getData() = parsed.values;
     }
      }
}
        else if (parsed.type == ParsedStructure::Type::LINKED_LIST) {
        // Create list structure
     structureId = dataManager->createDataStructure("List", 0, "Parsed List");
          if (auto* structure = dataManager->getStructure(structureId)) {
          if (auto* list = dynamic_cast<ListStructure*>(structure)) {
       for (int value : parsed.values) {
         list->append(value);
            }
                }
 }
        }
        else if (parsed.type == ParsedStructure::Type::BINARY_TREE) {
       // Create tree structure
       structureId = dataManager->createDataStructure("Tree", 0, "Parsed Tree");
     if (auto* structure = dataManager->getStructure(structureId)) {
  if (auto* tree = dynamic_cast<TreeStructure*>(structure)) {
                  for (int value : parsed.values) {
         tree->insert(value);
         }
    }
            }
        }
        else if (parsed.type == ParsedStructure::Type::GRAPH) {
   // Create graph from nodes and edges
            structureId = dataManager->buildStructureFromNodes(
       "Graph",
       parsed.nodeValues,
        parsed.edges,
                "Parsed Graph"
            );
  }
        
  if (!structureId.empty()) {
            dataManager->selectStructure(structureId);
      emit structureCreatedFromCode(QString::fromStdString(structureId));
      
            resultText += "\n? Structure created successfully!\n";
            resultText += "ID: " + QString::fromStdString(structureId);
    parseResultText->setPlainText(resultText);
            
            showSuccess("Structure created and selected!");
        } else {
   showError("Failed to create structure");
      }
    }
    catch (const std::exception& e) {
   showError(QString("Error creating structure: ") + e.what());
 }
}

void CodeGeneratorDialog::onCopyToClipboard() {
    QString code = generatedCodeText->toPlainText();
    if (code.isEmpty()) {
 return;
    }
    
  QApplication::clipboard()->setText(code);
    showSuccess("Code copied to clipboard!");
}

void CodeGeneratorDialog::showError(const QString& message) {
    QMessageBox::warning(this, "Error", message);
}

void CodeGeneratorDialog::showSuccess(const QString& message) {
    QMessageBox::information(this, "Success", message);
}
