#pragma once
#include <QWidget>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include "../visualization/visualization_renderer.h"
#include "../visualization/interaction_manager.h"
#include "../visualization/GraphvizLayoutEngine.h"

class VisualizationPane : public QWidget {
    Q_OBJECT

public:
    explicit VisualizationPane(QWidget* parent = nullptr);
    ~VisualizationPane();

    void setRenderSize(int size);
    void highlightNodes(const std::vector<std::string>& ids, const std::string& color);
    void setInteractionMode(const QString& mode);
    void reset();
    InteractionManager* getInteractionManager() const { return interaction.get(); }

protected:
    void paintEvent(QPaintEvent* event) override;

    // --- NOUVEAU : GESTION MOLETTE ---
    void wheelEvent(QWheelEvent* event) override;

    // Événements souris
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

    // Drag & Drop
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

private:
    void updateDisplay();

    // Fonction pour convertir (X,Y Écran) -> (X,Y Réel)
    QPointF getLogicalPosition(QPoint mousePos);

    std::unique_ptr<VisualizationRenderer> renderer;
    std::unique_ptr<InteractionManager> interaction;
    std::unique_ptr<GraphvizLayoutEngine> layoutEngine;

    std::vector<std::string> currentHighlights;

    // État des modes
    bool isLinkingMode = false;
    bool isEraserMode = false;
    std::string tempSourceNodeId = "";

    std::map<std::string, std::string> nodeValues;
};