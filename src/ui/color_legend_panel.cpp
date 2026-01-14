#include "color_legend_panel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QScrollBar>
#include <QFont>

ColorLegendPanel::ColorLegendPanel(QWidget* parent)
    : QWidget(parent)
{
    // Main layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // Title
  titleLabel = new QLabel("Algorithm Color Legend");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);

    // Description
    descriptionLabel = new QLabel();
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setStyleSheet("color: #666; font-size: 11px;");
    mainLayout->addWidget(descriptionLabel);

    // Scrollable area for legend
    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; }");

    contentWidget = new QWidget();
legendLayout = new QVBoxLayout(contentWidget);
    legendLayout->setContentsMargins(0, 0, 0, 0);
    legendLayout->setSpacing(8);

    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea, 1);

    // Set background
    setStyleSheet(
        "ColorLegendPanel {"
   "  background-color: #f5f5f5;"
  "  border: 1px solid #ddd;"
        "  border-radius: 4px;"
        "}"
    );
}

void ColorLegendPanel::setAlgorithmLegend(const std::string& algorithmName)
{
    clearLegendContent();

    if (algorithmName == "InsertionSort") {
createInsertionSortLegend();
    }
    else if (algorithmName == "SelectionSort") {
        createSelectionSortLegend();
    }
    else if (algorithmName == "Reverse") {
        createReverseLegend();
    }
  else if (algorithmName == "RemoveDuplicates") {
        createRemoveDuplicatesLegend();
    }
    else if (algorithmName == "InOrder") {
  createInOrderLegend();
    }
    else if (algorithmName == "PreOrder") {
        createPreOrderLegend();
    }
 else if (algorithmName == "PostOrder") {
        createPostOrderLegend();
    }

    legendLayout->addStretch();
}

void ColorLegendPanel::clearLegend()
{
    clearLegendContent();
    titleLabel->setText("No Algorithm Running");
    descriptionLabel->setText("Select an algorithm and click Play to see the color legend here.");
}

void ColorLegendPanel::clearLegendContent()
{
    while (QLayoutItem* item = legendLayout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
         widget->deleteLater();
        }
        delete item;
    }
}

void ColorLegendPanel::addColorRow(const QString& colorCode, const QString& colorName, const QString& meaning)
{
 QHBoxLayout* row = new QHBoxLayout();
    row->setSpacing(10);
    row->setContentsMargins(0, 0, 0, 0);

    // Color box
    QLabel* colorBox = new QLabel();
    colorBox->setFixedSize(24, 24);
    colorBox->setStyleSheet(QString(
      "QLabel {"
      "  background-color: %1;"
        "  border: 1px solid #999;"
        "  border-radius: 2px;"
     "}"
    ).arg(colorCode));
    row->addWidget(colorBox);

    // Color name and meaning
  QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->setSpacing(2);
    textLayout->setContentsMargins(0, 0, 0, 0);

  QLabel* nameLabel = new QLabel(colorName);
    QFont nameFont = nameLabel->font();
    nameFont.setBold(true);
    nameFont.setPointSize(10);
 nameLabel->setFont(nameFont);

    QLabel* meaningLabel = new QLabel(meaning);
    meaningLabel->setWordWrap(true);
    meaningLabel->setStyleSheet("color: #666; font-size: 9px;");

    textLayout->addWidget(nameLabel);
    textLayout->addWidget(meaningLabel);

    row->addLayout(textLayout, 1);

    QWidget* rowWidget = new QWidget();
    rowWidget->setLayout(row);
    legendLayout->addWidget(rowWidget);
}

void ColorLegendPanel::addDescription(const QString& text)
{
    QLabel* descLabel = new QLabel(text);
    descLabel->setWordWrap(true);
    descLabel->setStyleSheet("color: #333; font-size: 10px; margin: 8px 0px;");
    legendLayout->addWidget(descLabel);
}

void ColorLegendPanel::addOperationExample(const QString& example)
{
    QLabel* exampleLabel = new QLabel(example);
    exampleLabel->setWordWrap(true);
    exampleLabel->setStyleSheet(
        "background-color: #e8f4f8; "
 "padding: 6px; "
        "border-left: 3px solid #3498db; "
        "font-size: 9px; "
        "color: #333; "
        "font-family: 'Courier New', monospace;"
    );
  legendLayout->addWidget(exampleLabel);
}

void ColorLegendPanel::createInsertionSortLegend()
{
    titleLabel->setText("?? InsertionSort - Color Legend");
descriptionLabel->setText("Insertion Sort builds the sorted array one element at a time by inserting each element into its correct position.");

    addColorRow("#3498db", "?? Blue", "Initial state - array at the start");
    addColorRow("#f39c12", "?? Yellow", "Key element being inserted");
    addColorRow("#e74c3c", "?? Red", "Element being compared with key");
    addColorRow("#e67e22", "?? Orange", "Element being shifted to make space");
    addColorRow("#1abc9c", "?? Cyan", "Insertion point - final position for key");
    addColorRow("#2ecc71", "?? Green", "Sorted portion - elements in final positions");
    addColorRow("#bdc3c7", "? Gray", "Unsorted portion - not yet processed");

    addDescription("Process:");
    addOperationExample("1. Select next key (Yellow)\n2. Compare with sorted part (Red)\n3. Shift larger elements (Orange)\n4. Insert at correct position (Cyan)\n5. Repeat until sorted (Green)");
}

void ColorLegendPanel::createSelectionSortLegend()
{
  titleLabel->setText("?? SelectionSort - Color Legend");
    descriptionLabel->setText("Selection Sort repeatedly finds the minimum element and places it at the beginning of unsorted portion.");

    addColorRow("#3498db", "?? Blue", "Initial state - array at the start");
    addColorRow("#f39c12", "?? Yellow", "Unsorted region being searched");
    addColorRow("#e74c3c", "?? Red", "Current minimum element found");
    addColorRow("#e67e22", "?? Orange", "Candidate being compared to current minimum");
    addColorRow("#9b59b6", "?? Magenta", "New minimum discovered");
    addColorRow("#1abc9c", "?? Cyan", "Elements being swapped");
    addColorRow("#2ecc71", "?? Green", "Correctly placed in final position");

    addDescription("Process:");
    addOperationExample("1. Search for minimum (Yellow region)\n2. Compare elements (Red vs Orange)\n3. Update if smaller (Magenta)\n4. Swap with current position (Cyan)\n5. Mark as placed (Green)\n6. Repeat for rest");
}

void ColorLegendPanel::createReverseLegend()
{
    titleLabel->setText("?? Reverse - Color Legend");
  descriptionLabel->setText("Reverse transforms array by swapping elements from both ends moving toward the center.");

    addColorRow("#e74c3c", "?? Red", "Left pointer - starting position");
    addColorRow("#e67e22", "?? Orange", "Right pointer - ending position");
    addColorRow("#1abc9c", "?? Cyan", "Elements just swapped");
    addColorRow("#2ecc71", "?? Green", "Completed reversed portion");

    addDescription("Process:");
 addOperationExample("1. Position left and right pointers\n2. Swap elements (shown in Cyan)\n3. Move pointers toward center\n4. Repeat until middle\n5. Result is reversed array (Green)");
}

void ColorLegendPanel::createRemoveDuplicatesLegend()
{
    titleLabel->setText("?? RemoveDuplicates - Color Legend");
    descriptionLabel->setText("RemoveDuplicates removes duplicate elements while keeping the first occurrence of each value.");

    addColorRow("#f39c12", "?? Yellow", "Currently checking element");
    addColorRow("#e74c3c", "?? Red", "Duplicate found - will be removed");
    addColorRow("#1abc9c", "?? Cyan", "Unique element - will be kept");
    addColorRow("#2ecc71", "?? Green", "Already processed unique elements");

    addDescription("Process:");
    addOperationExample("1. Check if element seen (Yellow)\n2. If duplicate found (Red) ? Remove it\n3. If unique (Cyan) ? Keep it\n4. Mark as processed (Green)\n5. Continue to next element");
}

void ColorLegendPanel::createInOrderLegend()
{
    titleLabel->setText("?? InOrder Traversal - Color Legend");
    descriptionLabel->setText("InOrder traversal visits nodes in Left-Root-Right order, resulting in sorted output for binary search trees.");

    addColorRow("#1abc9c", "?? Cyan", "Currently visiting node");
    addColorRow("#2ecc71", "?? Green", "Node visited and processed");
    addColorRow("#bdc3c7", "? Gray", "Not yet visited");

    addDescription("Traversal Order: Left Subtree ? Root ? Right Subtree");
    addOperationExample("Example tree:\n       5\n      / \\\n     3   7\n\nInOrder: 3 ? 5 ? 7");
}

void ColorLegendPanel::createPreOrderLegend()
{
  titleLabel->setText("?? PreOrder Traversal - Color Legend");
    descriptionLabel->setText("PreOrder traversal visits the root first, then left and right subtrees. Useful for copying the tree structure.");

  addColorRow("#f39c12", "?? Yellow", "Currently visiting node (Root first!)");
    addColorRow("#2ecc71", "?? Green", "Node visited and processed");
    addColorRow("#bdc3c7", "? Gray", "Not yet visited");

  addDescription("Traversal Order: Root ? Left Subtree ? Right Subtree");
    addOperationExample("Example tree:\n       5\n  / \\\n     3   7\n\nPreOrder: 5 ? 3 ? 7");
}

void ColorLegendPanel::createPostOrderLegend()
{
    titleLabel->setText("?? PostOrder Traversal - Color Legend");
    descriptionLabel->setText("PostOrder traversal visits children first, then the root. Useful for deleting trees or postfix expressions.");

    addColorRow("#e67e22", "?? Orange", "Currently visiting node (Root last!)");
    addColorRow("#2ecc71", "?? Green", "Node visited and processed");
    addColorRow("#bdc3c7", "? Gray", "Not yet visited");

    addDescription("Traversal Order: Left Subtree ? Right Subtree ? Root");
    addOperationExample("Example tree:\n       5\n / \\\n  3   7\n\nPostOrder: 3 ? 7 ? 5");
}
