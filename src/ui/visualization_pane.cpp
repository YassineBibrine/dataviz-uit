#include "visualization_pane.h"
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QListWidget>
#include <sstream>
#include <set>
#include <cmath>
#include <random>
#include <QInputDialog>
#include "../core/data_model_manager.h"
#include "../core/data_structure.h"
#include "main_window.h"

VisualizationPane::VisualizationPane(QWidget* parent)
    : QWidget(parent) {

    setAcceptDrops(true);
    setMouseTracking(true);

    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    renderer = std::make_unique<VisualizationRenderer>(this);
    layout->addWidget(renderer.get());

    // Set minimum size for the rendering area
    setMinimumSize(600, 400);

    interaction = std::make_unique<InteractionManager>();
    layoutEngine = std::make_unique<GraphvizLayoutEngine>();

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
        interaction->clearInteractive();
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

    // PROTECTION CRASH : Si interaction est null ou backend pas prêt
    if (!interaction) return;

    auto positions = interaction->getAllNodePositions();
    std::vector<std::string> nodeIds;
    nodeIds.reserve(positions.size());
    std::map<std::string, std::string> nodeTypeMap;
    
    // Get node values from interaction manager
    auto interactionNodeValues = interaction->getNodeValues();
    
    for (const auto& np : positions) {
        nodeIds.push_back(np.id);
   nodeTypeMap[np.id] = np.type;
        f.nodePositions[np.id] = { np.x, np.y };
        f.nodeShapes[np.id] = np.type;
     
        // Priority: local nodeValues > interaction manager values > node id
   if (nodeValues.count(np.id)) {
            f.nodeLabels[np.id] = nodeValues[np.id];
        } else if (interactionNodeValues.count(np.id)) {
            f.nodeLabels[np.id] = std::to_string(interactionNodeValues[np.id]);
     } else {
  f.nodeLabels[np.id] = np.id;
        }
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
     f.nodePositions = posMap;
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

void VisualizationPane::refreshDisplay() {
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

// Load a structure from the backend into the interactive canvas for editing
void VisualizationPane::loadStructureForEditing(const std::string& structureId) {
    if (!interaction) return;
  
    // Get the backend manager
 DataModelManager* backend = interaction->getBackend();
 if (!backend) return;
    
    DataStructure* structure = backend->getStructure(structureId);
    if (!structure) return;
    
    // Clear current interactive data
    interaction->clearInteractive();
    nodeValues.clear();
    
 // Get structure metadata to determine type
    std::string structureType;
    auto structures = backend->getAllStructures();
    for (const auto& meta : structures) {
        if (meta.id == structureId) {
            structureType = meta.type;
            break;
        }
    }
    
    // Get nodes and edges from the structure
    auto nodes = structure->getNodes();
    auto edges = structure->getEdges();
    
    // Layout nodes based on structure type and add them to interaction manager
    int nodeCount = static_cast<int>(nodes.size());
    if (nodeCount == 0) return;
    
    // Determine shape based on type
    std::string shape = (structureType == "Array" || structureType == "List") ? "RECT" : "CIRCLE";
    
    // Calculate layout positions
    if (structureType == "Array") {
    double startX = 200.0;
        double y = 300.0;
        double spacing = 80.0;
        
        for (size_t i = 0; i < nodes.size(); ++i) {
 double x = startX + i * spacing;
         std::string newId = interaction->addNode(x, y, shape);
            // Use value for display
            nodeValues[newId] = nodes[i].value;
        interaction->updateNodeValue(newId, std::stoi(nodes[i].value.empty() ? "0" : nodes[i].value));
   }
    }
  else if (structureType == "List") {
 double startX = 150.0;
        double y = 300.0;
     double spacing = 100.0;
        
      std::vector<std::string> newNodeIds;
     for (size_t i = 0; i < nodes.size(); ++i) {
   double x = startX + i * spacing;
     std::string newId = interaction->addNode(x, y, shape);
            newNodeIds.push_back(newId);
            // Use value for display
      nodeValues[newId] = nodes[i].value;
        interaction->updateNodeValue(newId, std::stoi(nodes[i].value.empty() ? "0" : nodes[i].value));
        }
        
     // Add edges for list (sequential)
        for (size_t i = 0; i + 1 < newNodeIds.size(); ++i) {
   interaction->addEdge(newNodeIds[i], newNodeIds[i + 1]);
        }
    }
    else {
        // Random layout for trees and graphs
        std::random_device rd;
        std::mt19937 gen(rd());
  std::uniform_real_distribution<> xDis(150.0, 650.0);
  std::uniform_real_distribution<> yDis(100.0, 500.0);
        
        std::map<std::string, std::string> oldToNewId;
        
        for (size_t i = 0; i < nodes.size(); ++i) {
       double x = xDis(gen);
          double y = yDis(gen);
      
 std::string newId = interaction->addNode(x, y, shape);
      oldToNewId[nodes[i].id] = newId;
   // Use value for display
      nodeValues[newId] = nodes[i].value;
        try {
          interaction->updateNodeValue(newId, std::stoi(nodes[i].value.empty() ? "0" : nodes[i].value));
} catch (...) {
          // If value is not a number, just use 0
  interaction->updateNodeValue(newId, 0);
 }
        }
    
        // Add edges using the mapped IDs
        for (const auto& edge : edges) {
         if (oldToNewId.count(edge.from) && oldToNewId.count(edge.to)) {
       interaction->addEdge(oldToNewId[edge.from], oldToNewId[edge.to]);
            }
   }
    }
 
updateDisplay();
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

        // Temporarily disable sync to prevent value overwrite
      bool wasSyncing = interaction->isSyncEnabled();
  interaction->setSyncWithBackend(false);
   
    std::string newNodeId;
        if (type == "CREATE_NODE_CIRCLE") newNodeId = interaction->addNode(dropX, dropY, "CIRCLE");
      else if (type == "CREATE_NODE_RECT") newNodeId = interaction->addNode(dropX, dropY, "RECT");

   if (!newNodeId.empty()) {
   bool ok;
   QString text = QInputDialog::getText(this, "Nouvelle cellule", "Valeur :", QLineEdit::Normal, "0", &ok);
  if (ok && !text.isEmpty()) {
    nodeValues[newNodeId] = text.toStdString();
   interaction->updateNodeValue(newNodeId, text.toInt());
     }
    }
     
     // Re-enable sync and save once with the correct value
   interaction->setSyncWithBackend(wasSyncing);
 if (wasSyncing) {
     interaction->saveToCurrentStructure();
 interaction->saveNodePositionsToStructure();
     
     // Removed automatic tree repositioning - users control node positions manually
    }
        
  updateDisplay();
    }
    event->acceptProposedAction();
}