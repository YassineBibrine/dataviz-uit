#pragma once
#include <vector>
#include <string>
#include <memory>
#include <map>

class DataModelManager;

struct NodePosition {
    std::string id;
    double x, y;
    std::string type;
};

struct EdgeDisplay {
    std::string source;
    std::string target;
};

class InteractionManager {
public:
    InteractionManager();
    ~InteractionManager();

    
    void setBackend(DataModelManager* manager);
   

    std::string addNode(double x, double y, const std::string& type);
    void removeNode(const std::string& nodeId);

    // Fonction qui enverra la valeur au backend
    void updateNodeValue(const std::string& nodeId, int value);

    void addEdge(const std::string& sourceId, const std::string& targetId);
    void removeEdge(const std::string& sourceId, const std::string& targetId);

    bool startDragging(double x, double y);
    void updateDragging(double x, double y);
    void endDrag();

    std::vector<NodePosition> getAllNodePositions();
    std::vector<EdgeDisplay> getAllEdges();
    std::string getNodeAtPosition(double x, double y);
    std::pair<std::string, std::string> getEdgeAtPosition(double x, double y);

private:
    // Simulation (Mock) - On garde ça pour l'affichage local rapide
    struct MockNode {
        std::string id;
        double x, y;
        std::string type;
    };
    struct MockEdge {
        std::string source;
        std::string target;
    };

    std::vector<MockNode> nodes;
    std::vector<MockEdge> edges;

    std::string draggedNodeId = "";
    int nextId = 1;

    // --- MODIFICATION  : Le pointeur qui stocke le Backend ---
    DataModelManager* backend = nullptr;
    // ---------------------------------------------------------
};