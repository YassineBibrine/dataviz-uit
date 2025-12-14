#include "visualization_pane.h"
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QListWidget>
#include <QInputDialog>

VisualizationPane::VisualizationPane(QWidget* parent)
    : QWidget(parent) {

    setAcceptDrops(true);
    setMouseTracking(true);

    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    renderer = std::make_unique<VisualizationRenderer>(this);
    layout->addWidget(renderer.get());

    interaction = std::make_unique<InteractionManager>();

    updateDisplay();
}

VisualizationPane::~VisualizationPane() = default;

// --- MÉTHODE ANTI-CRASH ---
void VisualizationPane::reset() {
    // 1. On vide les valeurs stockées localement
    nodeValues.clear();
    currentHighlights.clear();
    tempSourceNodeId = "";
    isLinkingMode = false;
    isEraserMode = false;
    setCursor(Qt::ArrowCursor);

    // 2. On dit au manager de tout oublier (Nœuds, Arêtes)
    if (interaction) {
        
    }

    // 3. On force un rendu vide pour effacer l'écran
    AnimationFrame emptyFrame;
    renderer->renderFrame(emptyFrame);
}
// ---------------------------

void VisualizationPane::setRenderSize(int size) {
    if (renderer) {
        float scale = (float)size / 20.0f;
        renderer->setZoomFactor(scale);
    }
}

void VisualizationPane::wheelEvent(QWheelEvent* event) {
    if (renderer) {
        float currentZoom = renderer->getZoomFactor();
        if (event->angleDelta().y() > 0) currentZoom *= 1.1f;
        else currentZoom *= 0.9f;

        if (currentZoom < 0.1f) currentZoom = 0.1f;
        if (currentZoom > 5.0f) currentZoom = 5.0f;

        renderer->setZoomFactor(currentZoom);
        event->accept();
    }
}

QPointF VisualizationPane::getLogicalPosition(QPoint mousePos) {
    if (!renderer) return mousePos;
    float zoom = renderer->getZoomFactor();
    double centerX = width() / 2.0;
    double centerY = height() / 2.0;
    double dx = mousePos.x() - centerX;
    double dy = mousePos.y() - centerY;
    dx /= zoom;
    dy /= zoom;
    return QPointF(dx + centerX, dy + centerY);
}

void VisualizationPane::updateDisplay() {
    AnimationFrame f;
    // ... (Initialisation vide) ...

    // PROTECTION CRASH : Si interaction est null ou backend pas prêt
    if (!interaction) return;

    auto positions = interaction->getAllNodePositions();
    for (const auto& np : positions) {
        f.nodePositions[np.id] = { np.x, np.y };
        f.nodeShapes[np.id] = np.type;
        if (nodeValues.count(np.id)) f.nodeLabels[np.id] = nodeValues[np.id];
        else f.nodeLabels[np.id] = np.id;
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

void VisualizationPane::setInteractionMode(const QString& mode) {
    isLinkingMode = false;
    isEraserMode = false;
    tempSourceNodeId = "";
    setCursor(Qt::ArrowCursor);

    if (mode == "ACTIVATE_LINK_MODE") {
        isLinkingMode = true;
        setCursor(Qt::CrossCursor);
    }
    else if (mode == "ACTIVATE_ERASER_MODE") {
        isEraserMode = true;
        setCursor(Qt::ForbiddenCursor);
    }
}

void VisualizationPane::paintEvent(QPaintEvent* e) {
    QWidget::paintEvent(e);
}

// --- EVENTS SOURIS ---
void VisualizationPane::mouseDoubleClickEvent(QMouseEvent* event) {
    QPointF logicalPos = getLogicalPosition(event->pos());
    std::string nodeId = interaction->getNodeAtPosition(logicalPos.x(), logicalPos.y());
    if (!nodeId.empty()) {
        bool ok;
        QString currentVal = nodeValues.count(nodeId) ? QString::fromStdString(nodeValues[nodeId]) : "";
        QString text = QInputDialog::getText(this, "Valeur", "Entier :", QLineEdit::Normal, currentVal, &ok);
        if (ok && !text.isEmpty()) {
            nodeValues[nodeId] = text.toStdString();
            interaction->updateNodeValue(nodeId, text.toInt());
            updateDisplay();
        }
    }
}

void VisualizationPane::mousePressEvent(QMouseEvent* event) {
    QPointF logicalPos = getLogicalPosition(event->pos());
    double x = logicalPos.x();
    double y = logicalPos.y();

    if (isEraserMode) {
        std::string nodeId = interaction->getNodeAtPosition(x, y);
        if (!nodeId.empty()) {
            interaction->removeNode(nodeId);
            nodeValues.erase(nodeId);
            updateDisplay();
            return;
        }
        auto edge = interaction->getEdgeAtPosition(x, y);
        if (!edge.first.empty()) {
            interaction->removeEdge(edge.first, edge.second);
            updateDisplay();
            return;
        }
        return;
    }

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

    if (interaction->startDragging(x, y)) setCursor(Qt::ClosedHandCursor);
}

void VisualizationPane::mouseMoveEvent(QMouseEvent* event) {
    QPointF logicalPos = getLogicalPosition(event->pos());
    interaction->updateDragging(logicalPos.x(), logicalPos.y());
    updateDisplay();
}

void VisualizationPane::mouseReleaseEvent(QMouseEvent* event) {
    (void)event;
    interaction->endDrag();
    if (isEraserMode) setCursor(Qt::ForbiddenCursor);
    else if (isLinkingMode) setCursor(Qt::CrossCursor);
    else setCursor(Qt::ArrowCursor);
}

void VisualizationPane::dragEnterEvent(QDragEnterEvent* event) { event->acceptProposedAction(); }

void VisualizationPane::dropEvent(QDropEvent* event) {
    QListWidget* source = qobject_cast<QListWidget*>(event->source());
    if (source) {
        QListWidgetItem* item = source->currentItem();
        QString type = item->data(Qt::UserRole).toString();
        QPoint dropPoint(event->position().x(), event->position().y());
        QPointF logicalPos = getLogicalPosition(dropPoint);
        double dropX = logicalPos.x();
        double dropY = logicalPos.y();

        if (type == "CREATE_NODE_CIRCLE") interaction->addNode(dropX, dropY, "CIRCLE");
        else if (type == "CREATE_NODE_RECT") interaction->addNode(dropX, dropY, "RECT");

        updateDisplay();
        std::string newNodeId = interaction->getNodeAtPosition(dropX, dropY);
        if (!newNodeId.empty()) {
            bool ok;
            QString text = QInputDialog::getText(this, "Nouvelle cellule", "Valeur :", QLineEdit::Normal, "0", &ok);
            if (ok && !text.isEmpty()) {
                nodeValues[newNodeId] = text.toStdString();
                interaction->updateNodeValue(newNodeId, text.toInt());
            }
        }
        updateDisplay();
    }
    event->acceptProposedAction();
}