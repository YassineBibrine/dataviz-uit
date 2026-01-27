#include <set> 
#include "interaction_manager.h"
#include <cmath>
#include <algorithm>
#include <string>
#include <queue>

#include "../core/data_model_manager.h"
#include "../core/graph_structure.h"
#include "../core/array_structure.h"
#include "../core/list_structure.h"
#include "../core/tree_structure.h"
#include "../core/tree_node.h"
#include "../core/graph.h"
#include <QDebug>

InteractionManager::InteractionManager() : backend(nullptr), syncWithBackend(false) {}
InteractionManager::~InteractionManager() {}

void InteractionManager::setBackend(DataModelManager* manager) {
    this->backend = manager;
}

void InteractionManager::setSyncWithBackend(bool sync) {
    syncWithBackend = sync;
}

void InteractionManager::setCurrentStructureId(const std::string& structureId) {
    currentStructureId = structureId;
}

std::string InteractionManager::getStructureType() const {
    if (!backend || currentStructureId.empty()) {
        return "";
    }

    auto allStructures = backend->getAllStructures();
    for (const auto& meta : allStructures) {
        if (meta.id == currentStructureId) {
            return meta.type;
        }
    }
    return "";
}

bool InteractionManager::saveToCurrentStructure() {
    if (!backend || currentStructureId.empty()) {
        qDebug() << "saveToCurrentStructure: No backend or no currentStructureId";
        return false;
    }

    std::string structureType = getStructureType();
    qDebug() << "saveToCurrentStructure: type=" << QString::fromStdString(structureType)
        << "nodes=" << nodes.size();

    DataStructure* structure = backend->getStructure(currentStructureId);
    if (!structure) {
        qDebug() << "saveToCurrentStructure: structure not found";
        return false;
    }

    if (structureType == "Array") {
        if (auto* arr = dynamic_cast<ArrayStructure*>(structure)) {
            std::vector<size_t> sortedIndices(nodes.size());
            for (size_t i = 0; i < nodes.size(); ++i) sortedIndices[i] = i;
            std::sort(sortedIndices.begin(), sortedIndices.end(), [this](size_t a, size_t b) {
                return nodes[a].x < nodes[b].x;
                });

            auto& data = arr->getData();
            data.resize(nodes.size(), 0);

            for (size_t i = 0; i < sortedIndices.size(); ++i) {
                size_t nodeIdx = sortedIndices[i];
                auto it = nodeValues.find(nodes[nodeIdx].id);
                if (it != nodeValues.end()) {
                    data[i] = it->second;
                }
                std::string arrNodeId = "arr_" + std::to_string(i);
                canvasToStructureNodeId[nodes[nodeIdx].id] = arrNodeId;
            }
            qDebug() << "saveToCurrentStructure: Array saved, size=" << data.size();
            return true;
        }
    }
    else if (structureType == "List") {
        if (auto* list = dynamic_cast<ListStructure*>(structure)) {
            std::vector<size_t> sortedIndices(nodes.size());
            for (size_t i = 0; i < nodes.size(); ++i) sortedIndices[i] = i;
            std::sort(sortedIndices.begin(), sortedIndices.end(), [this](size_t a, size_t b) {
                return nodes[a].x < nodes[b].x;
                });

            list->clearList();
            for (size_t i = 0; i < sortedIndices.size(); ++i) {
                size_t nodeIdx = sortedIndices[i];
                auto it = nodeValues.find(nodes[nodeIdx].id);
                int value = (it != nodeValues.end()) ? it->second : 0;
                list->append(value);

                std::string listNodeId = "list_" + std::to_string(i);
                canvasToStructureNodeId[nodes[nodeIdx].id] = listNodeId;
            }
            qDebug() << "saveToCurrentStructure: List saved, size=" << sortedIndices.size();
            return true;
        }
    }
    else if (structureType == "Binary Tree" || structureType == "Tree") {
        if (auto* tree = dynamic_cast<TreeStructure*>(structure)) {
            // For manual binary tree, PRESERVE existing tree and only sync changes
            // DON'T rebuild entire tree - just update what changed

            qDebug() << "Syncing tree structure (preserving existing tree)";

            // Get current canvas state
            auto canvasEdges = edges;

            // Build mapping: canvas node ID -> tree node (by matching values)
            std::map<std::string, TreeNode*> canvasIdToTreeNode;

            // Traverse existing tree and match to canvas nodes by value
            TreeNode* existingRoot = const_cast<TreeNode*>(tree->getRoot());

            if (existingRoot) {
                // Tree exists - match canvas nodes to existing tree nodes
                std::queue<TreeNode*> bfsQueue;
                bfsQueue.push(existingRoot);

                while (!bfsQueue.empty()) {
                    TreeNode* treeNode = bfsQueue.front();
                    bfsQueue.pop();

                    if (treeNode) {
                        // Find canvas node with matching value
                        for (const auto& canvasNode : nodes) {
                            auto valIt = nodeValues.find(canvasNode.id);
                            if (valIt != nodeValues.end() && valIt->second == treeNode->value) {
                                canvasIdToTreeNode[canvasNode.id] = treeNode;
                                break;
                            }
                        }

                        if (treeNode->left) bfsQueue.push(treeNode->left);
                        if (treeNode->right) bfsQueue.push(treeNode->right);
                    }
                }
            }

            // Check if we have NEW nodes (not yet in tree)
            std::vector<std::string> newNodeIds;
            for (const auto& canvasNode : nodes) {
                if (canvasIdToTreeNode.find(canvasNode.id) == canvasIdToTreeNode.end()) {
                    newNodeIds.push_back(canvasNode.id);
                }
            }

            // If there are new nodes OR no existing root, rebuild tree
            if (!newNodeIds.empty() || !existingRoot) {
                qDebug() << "New nodes detected or no root - rebuilding tree structure";

                // Clear and rebuild
                tree->clearTree();

                // Find root (node with no incoming edges)
                std::set<std::string> hasParent;
                for (const auto& edge : canvasEdges) {
                    hasParent.insert(edge.target);
                }

                std::string rootCanvasId;
                for (const auto& node : nodes) {
                    if (hasParent.find(node.id) == hasParent.end()) {
                        rootCanvasId = node.id;
                        break;
                    }
                }

                if (rootCanvasId.empty() && !nodes.empty()) rootCanvasId = nodes[0].id;

                if (!rootCanvasId.empty()) {
                    // Build tree structure from canvas
                    std::map<std::string, TreeNode*> newTreeNodes;

                    // Create all tree nodes first
                    for (const auto& canvasNode : nodes) {
                        auto valIt = nodeValues.find(canvasNode.id);
                        int value = (valIt != nodeValues.end()) ? valIt->second : 0;
                        newTreeNodes[canvasNode.id] = new TreeNode(value);
                    }

                    // Group edges by parent to enforce binary tree constraint
                    std::map<std::string, std::vector<std::string>> parentToChildren;
                    for (const auto& edge : canvasEdges) {
                        parentToChildren[edge.source].push_back(edge.target);
                    }

                    // Set up parent-child relationships based on canvas edges
                    // BINARY TREE CONSTRAINT: Only first 2 children per parent
                    // TREE CONSTRAINT: Each child can have only ONE parent
                    std::set<std::string> alreadyAssignedChildren;

                    for (const auto& [parentId, children] : parentToChildren) {
                        if (!newTreeNodes.count(parentId)) continue;

                        TreeNode* parent = newTreeNodes[parentId];

                        // Binary tree: max 2 children
                        for (size_t i = 0; i < std::min(children.size(), size_t(2)); ++i) {
                            const std::string& childId = children[i];
                            if (!newTreeNodes.count(childId)) continue;

                            // Check if this child already has a parent
                            if (alreadyAssignedChildren.count(childId)) {
                                qDebug() << "Warning: Node" << QString::fromStdString(childId)
                                    << "already has a parent. Skipping.";
                                continue;
                            }

                            TreeNode* child = newTreeNodes[childId];
                            child->parent = parent;
                            alreadyAssignedChildren.insert(childId);

                            if (parent->left == nullptr) {
                                parent->left = child;
                            }
                            else if (parent->right == nullptr) {
                                parent->right = child;
                            }
                        }

                        if (children.size() > 2) {
                            qDebug() << "Warning: Node" << QString::fromStdString(parentId)
                                << "has" << children.size() << "children. Max 2 used.";
                        }
                    }

                    tree->setRoot(newTreeNodes[rootCanvasId]);
                    canvasIdToTreeNode = newTreeNodes;
                }
            }
            else {
                qDebug() << "No new nodes - preserving existing tree structure";
                // Tree structure preserved - canvas already mapped to existing tree
            }

            // Rebuild mappings for serialization
            TreeNode* root = const_cast<TreeNode*>(tree->getRoot());
            if (root) {
                // Build BFS index -> TreeNode* mapping
                std::map<int, TreeNode*> indexToNode;
                std::map<TreeNode*, int> nodeToIndex;
                std::map<int, int> valueToIndex;

                std::queue<TreeNode*> bfsQueue;
                bfsQueue.push(root);
                int index = 0;

                while (!bfsQueue.empty()) {
                    TreeNode* node = bfsQueue.front();
                    bfsQueue.pop();
                    if (node) {
                        indexToNode[index] = node;
                        nodeToIndex[node] = index;
                        valueToIndex[node->value] = index;
                        index++;
                        if (node->left) bfsQueue.push(node->left);
                        if (node->right) bfsQueue.push(node->right);
                    }
                }

                // Clear and rebuild ALL canvas node mappings based on their values
                for (const auto& canvasNode : nodes) {
                    auto valIt = nodeValues.find(canvasNode.id);
                    if (valIt != nodeValues.end()) {
                        int value = valIt->second;
                        auto idxIt = valueToIndex.find(value);
                        if (idxIt != valueToIndex.end()) {
                            std::string treeNodeId = "tree_" + std::to_string(idxIt->second);
                            canvasToStructureNodeId[canvasNode.id] = treeNodeId;
                        }
                    }
                }
            }

            // DO NOT save custom edges for trees - edges are part of tree structure
            structure->clearCustomEdges();

            qDebug() << "saveToCurrentStructure: Tree synced (structure preserved).";
            return true;
        }
    }
    else if (structureType == "Graph") {
        if (auto* graphStruct = dynamic_cast<GraphStructure*>(structure)) {
            if (auto* g = graphStruct->getGraph()) {
                // Process all nodes
                for (const auto& node : nodes) {
                    auto mappingIt = canvasToStructureNodeId.find(node.id);
                    std::string origNodeId;

                    if (mappingIt != canvasToStructureNodeId.end()) {
                        origNodeId = mappingIt->second;
                    }
                    else {
                        int counter = g->getNodeCount();
                        do {
                            origNodeId = "n" + std::to_string(counter++);
                        } while (g->hasNode(origNodeId));
                        canvasToStructureNodeId[node.id] = origNodeId;
                    }

                    if (!g->hasNode(origNodeId)) {
                        std::map<std::string, std::string> props;
                        auto valIt = nodeValues.find(node.id);
                        int value = (valIt != nodeValues.end()) ? valIt->second : 0;
                        props["value"] = std::to_string(value);
                        props["label"] = std::to_string(value);
                        g->addNode(origNodeId, props);
                        qDebug() << "Added new node to graph:" << QString::fromStdString(origNodeId)
                            << "with value:" << value;
                    }
                    else {
                        // Node exists - update its value ONLY if we have a value for it
                        auto valIt = nodeValues.find(node.id);
                        if (valIt != nodeValues.end()) {
                            Graph::Node* graphNode = g->getNode(origNodeId);
                            if (graphNode) {
                                graphNode->properties["value"] = std::to_string(valIt->second);
                                graphNode->properties["label"] = std::to_string(valIt->second);
                                qDebug() << "Updated existing node" << QString::fromStdString(origNodeId)
                                    << "value to:" << valIt->second;
                            }
                        }
                        else {
                            // No value in nodeValues, keep existing graph node value
                            qDebug() << "Keeping existing value for node" << QString::fromStdString(origNodeId);
                        }
                    }
                }

                // Process all edges
                for (const auto& edge : edges) {
                    auto srcMapping = canvasToStructureNodeId.find(edge.source);
                    auto tgtMapping = canvasToStructureNodeId.find(edge.target);

                    std::string srcId = (srcMapping != canvasToStructureNodeId.end())
                        ? srcMapping->second : edge.source;
                    std::string tgtId = (tgtMapping != canvasToStructureNodeId.end())
                        ? tgtMapping->second : edge.target;

                    if (g->hasNode(srcId) && g->hasNode(tgtId) && !g->hasEdge(srcId, tgtId)) {
                        g->addEdge(srcId, tgtId);
                        qDebug() << "Added new edge to graph:" << QString::fromStdString(srcId)
                            << "->" << QString::fromStdString(tgtId);
                    }
                }

                qDebug() << "saveToCurrentStructure: Graph saved, nodes=" << g->getNodeCount()
                    << "edges=" << g->getEdgeCount();
                return true;
            }
        }
    }

    return false;
}

void InteractionManager::saveNodePositionsToStructure() {
    if (!backend || currentStructureId.empty()) {
        return;
    }

    DataStructure* structure = backend->getStructure(currentStructureId);
    if (!structure) return;

    for (const auto& node : nodes) {
        auto it = canvasToStructureNodeId.find(node.id);
        if (it != canvasToStructureNodeId.end()) {
            structure->setNodePosition(it->second, node.x, node.y);
        }
        else {
            structure->setNodePosition(node.id, node.x, node.y);
        }
    }
}

std::string InteractionManager::addNode(double x, double y, const std::string& type) {
    std::string canvasId = "n" + std::to_string(nextId++);
    nodes.push_back({ canvasId, x, y, type });

    qDebug() << "addNode: canvasId=" << QString::fromStdString(canvasId)
        << "syncWithBackend=" << syncWithBackend
        << "currentStructureId=" << QString::fromStdString(currentStructureId);

    if (syncWithBackend && !currentStructureId.empty()) {
        saveToCurrentStructure();
        saveNodePositionsToStructure();
    }

    return canvasId;
}

std::string InteractionManager::addNodeWithMapping(double x, double y, const std::string& type, const std::string& originalNodeId) {
    std::string canvasId = "n" + std::to_string(nextId++);
    nodes.push_back({ canvasId, x, y, type });

    canvasToStructureNodeId[canvasId] = originalNodeId;

    if (syncWithBackend) {
        saveToCurrentStructure();
        saveNodePositionsToStructure();
    }
    return canvasId;
}

void InteractionManager::removeNode(const std::string& nodeId) {
    nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
        [&](const MockNode& n) { return n.id == nodeId; }), nodes.end());

    edges.erase(std::remove_if(edges.begin(), edges.end(),
        [&](const MockEdge& e) { return e.source == nodeId || e.target == nodeId; }), edges.end());

    nodeValues.erase(nodeId);
    canvasToStructureNodeId.erase(nodeId);

    if (syncWithBackend) {
        saveToCurrentStructure();
        saveNodePositionsToStructure();
    }
}

void InteractionManager::updateNodeValue(const std::string& nodeId, int value) {
    nodeValues[nodeId] = value;

    if (syncWithBackend) {
        saveToCurrentStructure();
        saveNodePositionsToStructure();
    }
}

void InteractionManager::updateNodePosition(const std::string& nodeId, double x, double y) {
    for (auto& node : nodes) {
        if (node.id == nodeId) {
            node.x = x;
            node.y = y;
            break;
        }
    }
}

void InteractionManager::addEdge(const std::string& sourceId, const std::string& targetId) {
    edges.push_back({ sourceId, targetId });

    qDebug() << "addEdge:" << QString::fromStdString(sourceId) << "->" << QString::fromStdString(targetId)
        << "syncWithBackend=" << syncWithBackend;

    if (syncWithBackend) {
        saveToCurrentStructure();
        saveNodePositionsToStructure();
    }
}

void InteractionManager::removeEdge(const std::string& sourceId, const std::string& targetId) {
    edges.erase(std::remove_if(edges.begin(), edges.end(),
        [&](const MockEdge& e) {
            return (e.source == sourceId && e.target == targetId) ||
                (e.source == targetId && e.target == sourceId);
        }), edges.end());

    if (syncWithBackend) {
        saveToCurrentStructure();
    }
}

bool InteractionManager::startDragging(double x, double y) {
    std::string id = getNodeAtPosition(x, y);
    if (!id.empty()) {
        draggedNodeId = id;
        return true;
    }
    return false;
}

// --- MODIF 2: Nouvelle fonction AIMANT (Magnet) ---
void InteractionManager::updateDragging(double x, double y) {
    if (draggedNodeId.empty()) return;

    // Paramètres d'aimant
    const double SNAP_DISTANCE = 20.0; // Distance d'attraction en pixels
    const double CELL_SIZE = 50.0;     // Taille des cellules (doit matcher le Renderer)

    double finalX = x;
    double finalY = y;

    // 1. Quel est le type du nœud que je bouge ?
    std::string myType = "CIRCLE";
    for (const auto& n : nodes) {
        if (n.id == draggedNodeId) {
            myType = n.type;
            break;
        }
    }

    // 2. Si c'est un ARRAY, on active l'aimant
    if (myType == "ARRAY") {
        for (const auto& other : nodes) {
            if (other.id == draggedNodeId) continue; // Ne pas s'aimanter à soi-même
            if (other.type != "ARRAY") continue;     // Seulement entre tableaux

            // Vérifier alignement vertical (Y)
            double dy = std::abs(y - other.y);
            if (dy < SNAP_DISTANCE) {
                // Hop ! On s'aligne en hauteur
                finalY = other.y;

                // Vérifier alignement horizontal (X)
                // Cible Gauche : other.x - 50
                // Cible Droite : other.x + 50
                double targetLeft = other.x - CELL_SIZE;
                double targetRight = other.x + CELL_SIZE;

                if (std::abs(x - targetLeft) < SNAP_DISTANCE) {
                    finalX = targetLeft; // CLAC ! Collé à gauche
                }
                else if (std::abs(x - targetRight) < SNAP_DISTANCE) {
                    finalX = targetRight; // CLAC ! Collé à droite
                }
            }
        }
    }

    // 3. Appliquer la position finale
    for (auto& n : nodes) {
        if (n.id == draggedNodeId) {
            n.x = finalX;
            n.y = finalY;
            break;
        }
    }
}

void InteractionManager::endDrag() {
    if (!draggedNodeId.empty()) {
        saveNodePositionsToStructure();
    }
    draggedNodeId = "";
}

std::vector<NodePosition> InteractionManager::getAllNodePositions() {
    std::vector<NodePosition> result;
    for (const auto& n : nodes) {
        result.push_back({ n.id, n.x, n.y, n.type });
    }
    return result;
}

std::vector<EdgeDisplay> InteractionManager::getAllEdges() {
    std::vector<EdgeDisplay> result;
    for (const auto& e : edges) {
        result.push_back({ e.source, e.target });
    }
    return result;
}

std::string InteractionManager::getNodeAtPosition(double x, double y) {
    for (auto it = nodes.rbegin(); it != nodes.rend(); ++it) {
        double dx = x - it->x;
        double dy = y - it->y;

        // --- MODIF: Hitbox plus précise pour les tableaux (Carré vs Cercle) ---
        if (it->type == "ARRAY" || it->type == "RECT") {
            if (std::abs(dx) < 25 && std::abs(dy) < 25) return it->id;
        }
        else {
            if (std::sqrt(dx * dx + dy * dy) <= 20.0) return it->id;
        }
    }
    return "";
}

std::pair<std::string, std::string> InteractionManager::getEdgeAtPosition(double mouseX, double mouseY) {
    const double threshold = 10.0;

    for (const auto& edge : edges) {
        double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
        bool foundSource = false, foundTarget = false;

        for (const auto& node : nodes) {
            if (node.id == edge.source) {
                x1 = node.x; y1 = node.y; foundSource = true;
            }
            else if (node.id == edge.target) {
                x2 = node.x; y2 = node.y; foundTarget = true;
            }
            if (foundSource && foundTarget) break;
        }

        if (!foundSource || !foundTarget) continue;

        double A = mouseX - x1;
        double B = mouseY - y1;
        double C = x2 - x1;
        double D = y2 - y1;

        double dot = A * C + B * D;
        double len_sq = C * C + D * D;
        double param = -1.0;

        if (len_sq != 0)
            param = dot / len_sq;

        double xx, yy;

        if (param < 0) {
            xx = x1;
            yy = y1;
        }
        else if (param > 1) {
            xx = x2;
            yy = y2;
        }
        else {
            xx = x1 + param * C;
            yy = y1 + param * D;
        }

        double dx = mouseX - xx;
        double dy = mouseY - yy;
        double distance = std::sqrt(dx * dx + dy * dy);

        if (distance < threshold) {
            return { edge.source, edge.target };
        }
    }

    return { "", "" };
}

std::string InteractionManager::finalizeStructure(const std::string& type, const std::string& name) {
    if (!backend) {
        return "";
    }

    if (nodes.empty()) {
        return "";
    }

    std::map<std::string, int> nodeData;
    for (const auto& node : nodes) {
        int value = (nodeValues.find(node.id) != nodeValues.end())
            ? nodeValues[node.id] : 0;
        nodeData[node.id] = value;
    }

    std::vector<std::pair<std::string, std::string>> edgeData;
    for (const auto& edge : edges) {
        edgeData.emplace_back(edge.source, edge.target);
    }

    std::string structId = backend->buildStructureFromNodes(type, nodeData, edgeData, name);

    return structId;
}

void InteractionManager::clearInteractive() {
    nodes.clear();
    edges.clear();
    nodeValues.clear();
    canvasToStructureNodeId.clear();
    nextId = 1;
    currentStructureId = "";
}

std::map<std::string, int> InteractionManager::getNodeValues() const {
    return nodeValues;
}