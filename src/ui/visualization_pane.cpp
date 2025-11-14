#include "visualization_pane.h"
#include <QVBoxLayout>

VisualizationPane::VisualizationPane(QWidget* parent)
    : QWidget(parent) {
    auto layout = new QVBoxLayout(this);
    renderer = std::make_unique<VisualizationRenderer>(this);
    layout->addWidget(renderer.get());
    interaction = std::make_unique<InteractionManager>();
    
    // Seed some positions for visualization
    for (int i = 0; i < 8; ++i) {
        double x = 80 + (i % 4) * 100;
        double y = 80 + (i / 4) * 100;
        interaction->updateNodePosition("n" + std::to_string(i + 1), x, y);
    }
    
    AnimationFrame f;
    f.nodePositions = {}; // copy from interaction
    for (auto np : interaction->getAllNodePositions()) {
        f.nodePositions[np.id] = {np.x, np.y};
    }
    renderer->renderFrame(f);
}

VisualizationPane::~VisualizationPane() = default;

void VisualizationPane::highlightNodes(const std::vector<std::string>& ids, const std::string& color) {
    (void)color;
    currentHighlights = ids;
    AnimationFrame f;
    for (auto np : interaction->getAllNodePositions()) {
        f.nodePositions[np.id] = {np.x, np.y};
    }
    f.highlightedNodes = currentHighlights;
    renderer->renderFrame(f);
}

void VisualizationPane::paintEvent(QPaintEvent* e) {
    QWidget::paintEvent(e);
}
