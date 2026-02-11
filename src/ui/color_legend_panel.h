#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <string>
#include <map>

/**
 * @brief Compact color legend overlay shown on bottom-left of visualization canvas
 *
 * Displays a simplified color legend during algorithm animation with just
 * the essential color meanings in a compact, semi-transparent overlay.
 */
class ColorLegendPanel : public QWidget {
    Q_OBJECT

public:
    explicit ColorLegendPanel(QWidget* parent = nullptr);
    ~ColorLegendPanel() override = default;

    /**
     * @brief Set the algorithm and display its color legend
     * @param algorithmName Name of the algorithm (e.g., "InsertionSort")
     */
    void setAlgorithmLegend(const std::string& algorithmName);

    /**
     * @brief Clear and hide the legend
     */
    void clearLegend();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QVBoxLayout* mainLayout;
    QLabel* titleLabel;

    // Store color meanings for current algorithm
    std::vector<std::pair<QString, QString>> colorMeanings;  // {color, meaning}

    void setupSortingColors();
    void setupTreeTraversalColors();
    void setupGraphColors();

    void addColorMeaning(const QString& colorCode, const QString& meaning);
    void clearColorMeanings();
};
