#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>
#include <string>

class ColorLegendPanel : public QWidget {
    Q_OBJECT

public:
    explicit ColorLegendPanel(QWidget* parent = nullptr);
    ~ColorLegendPanel() override = default;

    void setAlgorithmLegend(const std::string& algorithmName);
    void clearLegend();

private:
    QVBoxLayout* mainLayout;
    QScrollArea* scrollArea;
    QWidget* contentWidget;
    QLabel* titleLabel;
    QLabel* descriptionLabel;
    QVBoxLayout* legendLayout;

    void createInsertionSortLegend();
void createSelectionSortLegend();
    void createReverseLegend();
  void createRemoveDuplicatesLegend();
    void createInOrderLegend();
    void createPreOrderLegend();
    void createPostOrderLegend();

    void clearLegendContent();
    void addColorRow(const QString& colorCode, const QString& colorName, const QString& meaning);
    void addDescription(const QString& text);
void addOperationExample(const QString& example);
};
