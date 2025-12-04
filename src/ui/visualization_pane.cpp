#include "visualization_pane.h"
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QListWidget>

VisualizationPane::VisualizationPane(QWidget* parent)
    : QWidget(parent) {

    setAcceptDrops(true);
    setMouseTracking(true);

    auto layout = new QVBoxLayout(this);
    renderer = std::make_unique<VisualizationRenderer>(this);
    layout->addWidget(renderer.get());

    // Set minimum size for the rendering area
    setMinimumSize(600, 400);

    interaction = std::make_unique<InteractionManager>();

    updateDisplay();
}

VisualizationPane::~VisualizationPane() = default;

void VisualizationPane::setRenderSize(int size) {
    if (renderer) {
        renderer->setNodeRadius(size);
    }
}

void VisualizationPane::updateDisplay() {
    AnimationFrame f;
    f.nodePositions = {};
    f.nodeShapes = {};
    f.edges = {};

    auto positions = interaction->getAllNodePositions();
    for (const auto& np : positions) {
        f.nodePositions[np.id] = { np.x, np.y };
        f.nodeShapes[np.id] = np.type;
    }

    auto edges = interaction->getAllEdges();
    for (const auto& e : edges) {
        f.edges.push_back({ e.source, e.target });
    }

    f.highlightedNodes = currentHighlights;
    renderer->renderFrame(f);
}

void VisualizationPane::highlightNodes(const std::vector<std::string>& ids, const std::string& color) {
    (void)color;
    currentHighlights = ids;
    updateDisplay();
}

// --- GESTION DES MODES (Normal, Lien, Gomme) ---
void VisualizationPane::setInteractionMode(const QString& mode) {
    // Reset
    isLinkingMode = false;
    isEraserMode = false; // Nouveau flag
    tempSourceNodeId = "";
    setCursor(Qt::ArrowCursor);

    if (mode == "ACTIVATE_LINK_MODE") {
        isLinkingMode = true;
        setCursor(Qt::CrossCursor);
    }
    else if (mode == "ACTIVATE_ERASER_MODE") {
        isEraserMode = true;
        setCursor(Qt::ForbiddenCursor); // Curseur "Interdit" pour la gomme
    }
}

void VisualizationPane::paintEvent(QPaintEvent* e) {
    QWidget::paintEvent(e);
}

// --- ÉVÉNEMENTS SOURIS ---

void VisualizationPane::mousePressEvent(QMouseEvent* event) {
    double x = event->pos().x();
    double y = event->pos().y();

    // 1. MODE GOMME
    if (isEraserMode) {
        // Essai suppression Noeud
        std::string nodeId = interaction->getNodeAtPosition(x, y);
        if (!nodeId.empty()) {
            interaction->removeNode(nodeId);
            updateDisplay();
            return;
        }
        // Essai suppression Lien
        auto edge = interaction->getEdgeAtPosition(x, y);
        if (!edge.first.empty()) {
            interaction->removeEdge(edge.first, edge.second);
            updateDisplay();
            return;
        }
        return;
    }

    // 2. MODE LIEN
    if (isLinkingMode) {
        std::string clickedId = interaction->getNodeAtPosition(x, y);
        if (clickedId.empty()) return;

        if (tempSourceNodeId.empty()) {
            tempSourceNodeId = clickedId;
            highlightNodes({ tempSourceNodeId }, "red");
        }
        else {
            if (clickedId != tempSourceNodeId) {
                interaction->addEdge(tempSourceNodeId, clickedId);
                tempSourceNodeId = "";
                highlightNodes({}, "");
                updateDisplay();
            }
        }
        return;
    }

    // 3. MODE NORMAL (DRAG)
    if (interaction->startDragging(x, y)) {
        setCursor(Qt::ClosedHandCursor);
    }
}

void VisualizationPane::mouseMoveEvent(QMouseEvent* event) {
    double x = event->pos().x();
    double y = event->pos().y();
    interaction->updateDragging(x, y);
    updateDisplay();
}

void VisualizationPane::mouseReleaseEvent(QMouseEvent* event) {
    (void)event;
    interaction->endDrag();
    // On remet le bon curseur selon le mode
    if (isEraserMode) setCursor(Qt::ForbiddenCursor);
    else if (isLinkingMode) setCursor(Qt::CrossCursor);
    else setCursor(Qt::ArrowCursor);
}

void VisualizationPane::dragEnterEvent(QDragEnterEvent* event) {
    event->acceptProposedAction();
}

void VisualizationPane::dropEvent(QDropEvent* event) {
    QListWidget* source = qobject_cast<QListWidget*>(event->source());
    if (source) {
        QListWidgetItem* item = source->currentItem();
        QString type = item->data(Qt::UserRole).toString();

        if (type == "CREATE_NODE_CIRCLE") {
            interaction->addNode(event->position().x(), event->position().y(), "CIRCLE");
            updateDisplay();
        }
        else if (type == "CREATE_NODE_RECT") {
            interaction->addNode(event->position().x(), event->position().y(), "RECT");
            updateDisplay();
        }
    }
    event->acceptProposedAction();
}