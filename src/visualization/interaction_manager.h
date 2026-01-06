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
    DataModelManager* getBackend() const { return backend; }
   

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
    
    /**
     * @brief Finalize interactive structure creation
     * @param type Structure type or "Auto" for detection
  * @param name Optional name
     * @return ID of created structure in DataModelManager
     */
  std::string finalizeStructure(const std::string& type = "Auto", 
     const std::string& name = "");
    
    /**
     * @brief Clear all interactive nodes/edges (after finalization)
     */
    void clearInteractive();
    
    /**
     * @brief Get all node values for structure building
     */
    std::map<std::string, int> getNodeValues() const;
    
    /**
     * @brief Check if there are any interactive nodes/edges
     */
    bool hasInteractiveData() const { return !nodes.empty(); }
 
    /**
  * @brief Get count of interactive nodes and edges
     */
    std::pair<int, int> getInteractiveStats() const { 
    return {static_cast<int>(nodes.size()), static_cast<int>(edges.size())}; 
}

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
  
  // Store node values for finalization
    std::map<std::string, int> nodeValues;

    std::string draggedNodeId = "";
    int nextId = 1;

    // --- MODIFICATION  : Le pointeur qui stocke le Backend ---
    DataModelManager* backend = nullptr;
 // ---------------------------------------------------------
};