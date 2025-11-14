#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <map>
#include <string>

class MetricsPanel : public QWidget {
 Q_OBJECT
public:
 explicit MetricsPanel(QWidget* parent = nullptr);
 ~MetricsPanel() override;
 void updateMetrics(const std::map<std::string, int>& metrics);
 void displayComplexity(const std::string& timeComplexity, const std::string& spaceComplexity);
 void appendStep(const QString& stepInfo);
 void clear();
private:
 void setupUI();
 QTableWidget* metricsDisplay;
 QLabel* complexityLabel;
 std::map<std::string, std::string> currentMetrics;
};
