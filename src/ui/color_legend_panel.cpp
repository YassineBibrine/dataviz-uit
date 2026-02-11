#include "color_legend_panel.h"
#include <QPainter>
#include <QFont>
#include <QPaintEvent>

ColorLegendPanel::ColorLegendPanel(QWidget* parent)
    : QWidget(parent)
{
    // Set fixed size for compact legend
    setFixedSize(220, 200);
    
    // Make it semi-transparent and position it
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    
    // Start hidden
hide();
}

void ColorLegendPanel::setAlgorithmLegend(const std::string& algorithmName)
{
clearColorMeanings();
    
    if (algorithmName == "InsertionSort" || 
        algorithmName == "SelectionSort" || 
     algorithmName == "BubbleSort") {
        setupSortingColors();
    }
    else if (algorithmName == "InOrder" || 
algorithmName == "PreOrder" || 
      algorithmName == "PostOrder") {
        setupTreeTraversalColors();
    }
    else if (algorithmName == "BFS" || 
             algorithmName == "DFS" || 
             algorithmName == "Dijkstra") {
        setupGraphColors();
    }
    else {
        // Default/generic colors
        addColorMeaning("#3498db", "Unprocessed");
        addColorMeaning("#f39c12", "Processing");
   addColorMeaning("#2ecc71", "Complete");
    }
    
    show();
    update();
}

void ColorLegendPanel::clearLegend()
{
    clearColorMeanings();
    hide();
}

void ColorLegendPanel::setupSortingColors()
{
    addColorMeaning("#f39c12", "Current");
    addColorMeaning("#e74c3c", "Comparing");
    addColorMeaning("#1abc9c", "Swapping");
    addColorMeaning("#2ecc71", "Sorted");
    addColorMeaning("#bdc3c7", "Unsorted");
}

void ColorLegendPanel::setupTreeTraversalColors()
{
    addColorMeaning("#f39c12", "Visiting");
    addColorMeaning("#2ecc71", "Visited");
    addColorMeaning("#bdc3c7", "Pending");
}

void ColorLegendPanel::setupGraphColors()
{
    addColorMeaning("#4CAF50", "Start");
    addColorMeaning("#FF9800", "Current");
 addColorMeaning("#2196F3", "Discovered");
    addColorMeaning("#9E9E9E", "Visited");
 addColorMeaning("#00FF00", "Found/Path");
}

void ColorLegendPanel::addColorMeaning(const QString& colorCode, const QString& meaning)
{
    colorMeanings.push_back({colorCode, meaning});
}

void ColorLegendPanel::clearColorMeanings()
{
    colorMeanings.clear();
}

void ColorLegendPanel::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw semi-transparent background
    painter.setBrush(QColor(40, 44, 52, 230));  // Dark background with transparency
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 8, 8);
    
    // Draw border
  painter.setPen(QPen(QColor(255, 255, 255, 100), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 8, 8);
    
    // Title
    QFont titleFont("Segoe UI", 11, QFont::Bold);
    painter.setFont(titleFont);
    painter.setPen(QColor(255, 255, 255, 250));
    painter.drawText(QRect(15, 10, width() - 30, 25), Qt::AlignLeft | Qt::AlignVCenter, "Colors");
    
    // Draw color meanings
    int y = 45;
    int rowHeight = 28;
    
    QFont meaningFont("Segoe UI", 9);
    painter.setFont(meaningFont);
 
    for (const auto& [colorCode, meaning] : colorMeanings) {
        if (y + rowHeight > height() - 10) break;  // Don't overflow
        
    // Draw color box
        QColor color(colorCode);
     painter.setBrush(color);
        painter.setPen(QPen(QColor(255, 255, 255, 150), 1));
        painter.drawRoundedRect(QRect(15, y, 20, 20), 3, 3);
        
        // Draw meaning text
 painter.setPen(QColor(255, 255, 255, 240));
        painter.drawText(QRect(45, y, width() - 60, 20), 
Qt::AlignLeft | Qt::AlignVCenter, 
        meaning);
 
      y += rowHeight;
    }
}
