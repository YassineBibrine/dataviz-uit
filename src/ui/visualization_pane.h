#pragma once

#include <QWidget>
#include <memory>
#include <vector>
#include <string>
#include "../visualization/visualization_renderer.h"
#include "../visualization/interaction_manager.h"
#include "../visualization/GraphvizLayoutEngine.h"

class VisualizationPane : public QWidget {
    Q_OBJECT
        std::unique_ptr<VisualizationRenderer> renderer;
    std::unique_ptr<InteractionManager> interaction;
    std::unique_ptr<GraphvizLayoutEngine> layoutEngine;
    std::vector<std::string> currentHighlights;

public:
    explicit VisualizationPane(QWidget* parent = nullptr);
    ~VisualizationPane() override;

    void highlightNodes(const std::vector<std::string>& ids, const std::string& color);

    // --- AJOUT POUR LA TAILLE ---
    void setRenderSize(int size);
    // ----------------------------

    void setInteractionMode(const QString& mode);

protected:
    void paintEvent(QPaintEvent* e) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

private:
    void updateDisplay();
    bool isLinkingMode = false;
    bool isEraserMode = false;
    std::string tempSourceNodeId = "";
};