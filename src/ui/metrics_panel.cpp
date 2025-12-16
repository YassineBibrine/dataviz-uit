#include "metrics_panel.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QTableWidgetItem>
#include <QGroupBox>

MetricsPanel::MetricsPanel(QWidget* parent) : QWidget(parent) {
    setAttribute(Qt::WA_StyledBackground, true);
    setupUI();
}

MetricsPanel::~MetricsPanel() = default;

void MetricsPanel::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    // MARGE HAUT A 15px : C'est ça qui fait rentrer le titre "Algorithm Metrics" DANS la boîte
    mainLayout->setContentsMargins(5, 15, 5, 5);

    QGroupBox* metricsGroup = new QGroupBox("Algorithm Metrics", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(metricsGroup);
    groupLayout->setSpacing(10);
    // Marge interne du groupe pour le contenu
    groupLayout->setContentsMargins(10, 25, 10, 10);

    metricsDisplay = new QTableWidget();
    metricsDisplay->setColumnCount(2);
    metricsDisplay->setHorizontalHeaderLabels({ "Metric", "Value" });
    metricsDisplay->setMaximumHeight(200);

    metricsDisplay->setStyleSheet(R"(
        QTableWidget { border: 1px solid #E2E8F0; gridline-color: #E2E8F0; background-color: white; }
        QHeaderView::section {
            background-color: #EDF2F7; padding: 4px; border: none;
            border-bottom: 2px solid #CBD5E0; font-weight: bold; color: #4A5568;
        }
    )");

    metricsDisplay->verticalHeader()->setVisible(false);
    metricsDisplay->setAlternatingRowColors(true);
    metricsDisplay->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    metricsDisplay->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    groupLayout->addWidget(metricsDisplay);

    QLabel* titleComp = new QLabel("Complexity:");
    titleComp->setStyleSheet("font-weight: bold; color: #2D3748; margin-top: 10px;");
    titleComp->setAlignment(Qt::AlignLeft);
    groupLayout->addWidget(titleComp);

    complexityLabel = new QLabel("Time: O(?) | Space: O(?)");
    complexityLabel->setWordWrap(true);
    complexityLabel->setAlignment(Qt::AlignLeft);
    complexityLabel->setStyleSheet("color: #718096; margin-left: 5px;");

    groupLayout->addWidget(complexityLabel);
    groupLayout->addStretch();

    mainLayout->addWidget(metricsGroup);
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

void MetricsPanel::appendStep(const QString& stepInfo) {}
void MetricsPanel::clear() {
    metricsDisplay->setRowCount(0);
    complexityLabel->setText("Time: O(?) | Space: O(?)");
}