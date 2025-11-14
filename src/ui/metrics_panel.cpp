#include "metrics_panel.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QTableWidgetItem>

MetricsPanel::MetricsPanel(QWidget* parent) : QWidget(parent) {
    setupUI();
}

MetricsPanel::~MetricsPanel() = default;

void MetricsPanel::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(8);
    
    layout->addWidget(new QLabel("Algorithm Metrics:"));
    
    metricsDisplay = new QTableWidget();
    metricsDisplay->setColumnCount(2);
    metricsDisplay->setHorizontalHeaderLabels({"Metric", "Value"});
    metricsDisplay->setMaximumHeight(200);
    metricsDisplay->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    metricsDisplay->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    layout->addWidget(metricsDisplay);
    
    layout->addWidget(new QLabel("Complexity:"));
    complexityLabel = new QLabel("Time: O(?) | Space: O(?)");
    complexityLabel->setWordWrap(true);
    layout->addWidget(complexityLabel);
    
    layout->addStretch();
    
    setMinimumWidth(250);
    setStyleSheet("QWidget { background-color: #f5f5f5; }");
}

void MetricsPanel::updateMetrics(const std::map<std::string, int>& metrics) {
    metricsDisplay->setRowCount(0);
    int row = 0;
    for (const auto& pair : metrics) {
        metricsDisplay->insertRow(row);
        metricsDisplay->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(pair.first)));
        metricsDisplay->setItem(row, 1, new QTableWidgetItem(QString::number(pair.second)));
        row++;
    }
}

void MetricsPanel::displayComplexity(const std::string& timeComplexity, const std::string& spaceComplexity) {
    complexityLabel->setText(QString("Time: %1 | Space: %2")
   .arg(QString::fromStdString(timeComplexity))
   .arg(QString::fromStdString(spaceComplexity)));
}

void MetricsPanel::appendStep(const QString& stepInfo) {
  // Append step information
}

void MetricsPanel::clear() {
    metricsDisplay->setRowCount(0);
    complexityLabel->setText("Time: O(?) | Space: O(?)");
}
