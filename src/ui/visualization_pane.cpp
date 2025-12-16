#include "visualization_pane.h"
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QListWidget>
#include <sstream>
#include <set>

VisualizationPane::VisualizationPane(QWidget* parent)
    : QWidget(parent) {

    setAcceptDrops(true);
    setMouseTracking(true);

    auto layout = new QVBoxLayout(this);
    renderer = std::make_unique<VisualizationRenderer>(this);
    layout->addWidget(renderer.get());

    interaction = std::make_unique<InteractionManager>();
    layoutEngine = std::make_unique<GraphvizLayoutEngine>();

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
    std::vector<std::string> nodeIds;
    nodeIds.reserve(positions.size());
    std::map<std::string, std::string> nodeTypeMap;
    for (const auto& np : positions) {
        nodeIds.push_back(np.id);
        nodeTypeMap[np.id] = np.type;
    }

    auto edges = interaction->getAllEdges();
    for (const auto& e : edges) {
        f.edges.push_back({ e.source, e.target });
    }

    // Heuristic: detect tree when edge count == node count - 1, no node has indegree > 1,
    // exactly one root (indegree == 0), and graph is connected.
    bool isTree = false;
    size_t n = nodeIds.size();
    if (n > 0 && edges.size() == (n - 1)) {
        std::map<std::string,int> indeg;
        std::map<std::string,std::vector<std::string>> adjUndir;
        for (const auto& id : nodeIds) { indeg[id] = 0; }
        for (const auto& e : edges) {
            if (indeg.find(e.target) == indeg.end() || indeg.find(e.source) == indeg.end()) {
                // unknown node -> not a tree
                isTree = false;
                break;
            }
            indeg[e.target]++;
            // build undirected adjacency for connectivity test
            adjUndir[e.source].push_back(e.target);
            adjUndir[e.target].push_back(e.source);
        }

        if (!isTree) {
            // continue only if we didn't break
        }

        bool indegOk = true;
        int roots = 0;
        std::string rootId;
        for (const auto& kv : indeg) {
            if (kv.second > 1) { indegOk = false; break; }
            if (kv.second == 0) { roots++; rootId = kv.first; }
        }

        if (indegOk && roots == 1) {
            // connectivity via BFS
            std::set<std::string> seen;
            std::vector<std::string> q;
            q.push_back(rootId);
            seen.insert(rootId);
            for (size_t i = 0; i < q.size(); ++i) {
                auto u = q[i];
                for (const auto& v : adjUndir[u]) {
                    if (!seen.count(v)) {
                        seen.insert(v);
                        q.push_back(v);
                    }
                }
            }
            if (seen.size() == n) isTree = true;
        }
    }

    if (isTree && layoutEngine && layoutEngine->isAvailable()) {
        // Build DOT string for the tree and ask Graphviz for layout
        std::ostringstream oss;
        oss << "digraph BinaryTree {\n";
        oss << "  node [shape=circle];\n";
        for (const auto& id : nodeIds) {
            oss << "  " << id << " [label=\"" << id << "\"];\n";
        }
        for (const auto& e : edges) {
            oss << "  " << e.source << " -> " << e.target << ";\n";
        }
        oss << "}\n";

        auto posMap = layoutEngine->computeLayout(oss.str());
        if (!posMap.empty()) {
            for (const auto& kv : posMap) {
                f.nodePositions[kv.first] = { kv.second.first, kv.second.second };
            }
            for (const auto& nt : nodeTypeMap) {
                f.nodeShapes[nt.first] = nt.second;
            }
        }
        else {
            // fallback to manual positions
            for (const auto& np : positions) {
                f.nodePositions[np.id] = { np.x, np.y };
                f.nodeShapes[np.id] = np.type;
            }
        }
    }
    else {
        // Not a tree or graphviz not available: use positions from interaction manager
        for (const auto& np : positions) {
            f.nodePositions[np.id] = { np.x, np.y };
            f.nodeShapes[np.id] = np.type;
        }
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