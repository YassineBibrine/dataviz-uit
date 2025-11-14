#pragma once

#include <QWidget>
#include <memory>
#include <vector>
#include <string>
#include "../visualization/visualization_renderer.h"
#include "../visualization/interaction_manager.h"

class VisualizationPane : public QWidget {
    Q_OBJECT
    std::unique_ptr<VisualizationRenderer> renderer;
    std::unique_ptr<InteractionManager> interaction;
    std::vector<std::string> currentHighlights;
    
public:
    explicit VisualizationPane(QWidget* parent = nullptr);
    ~VisualizationPane() override;
    
    void highlightNodes(const std::vector<std::string>& ids, const std::string& color);
    
signals:
    void nodeDragged(const std::string& nodeId, double newX, double newY);
    
protected:
    void paintEvent(QPaintEvent* e) override;
};
