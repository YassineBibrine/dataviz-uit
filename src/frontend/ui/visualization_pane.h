#ifndef VISUALIZATION_PANE_H
#define VISUALIZATION_PANE_H

#include <QWidget>
#include <memory>

/**
 * @class VisualizationPane
 * @brief Main visualization area for rendering data structures and algorithms
 * 
 * Displays the graphical representation of:
 * - Graphs and trees
 * - Linear structures (arrays, linked lists, stacks, queues)
 * - Hash tables
 * - Algorithmic state changes
 */
class VisualizationPane : public QWidget {
 Q_OBJECT

public:
    VisualizationPane(QWidget *parent = nullptr);
    ~VisualizationPane();
    
    void renderVisualization(const QString &dotCode);
    void clearVisualization();
    void zoomIn();
    void zoomOut();
    void resetZoom();
    void toggleMemoryView(bool enable);

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void updateLayout();
    
    float zoomLevel = 1.0f;
    bool memoryViewEnabled = false;
};

#endif // VISUALIZATION_PANE_H
