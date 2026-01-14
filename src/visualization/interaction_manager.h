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
    
    /**
     * @brief Enable/disable syncing changes to backend structure
     * @param sync True to sync user edits to backend, false when just displaying
     */
    void setSyncWithBackend(bool sync);
    
    /**
     * @brief Check if syncing is currently enabled
     */
    bool isSyncEnabled() const { return syncWithBackend; }

    /**
     * @brief Set the ID of the structure currently being edited
     * @param structureId The ID of the structure loaded in the canvas
     */
    void setCurrentStructureId(const std::string& structureId);
    
    /**
     * @brief Get the ID of the structure currently being edited
     */
    std::string getCurrentStructureId() const { return currentStructureId; }
    
    /**
     * @brief Get the type of the current structure
     */
    std::string getStructureType() const;
 
    /**
     * @brief Save current canvas state back to the backend structure
     * @return True if save was successful
     */
    bool saveToCurrentStructure();
    
    /**
     * @brief Save node positions to the backend structure for persistence
     */
    void saveNodePositionsToStructure();

    /**
     * @brief Add a node and optionally track its original structure ID
*/
    std::string addNode(double x, double y, const std::string& type);
    
    /**
     * @brief Add a node with mapping to original structure node ID
     */
    std::string addNodeWithMapping(double x, double y, const std::string& type, const std::string& originalNodeId);
    
  void removeNode(const std::string& nodeId);
    void updateNodeValue(const std::string& nodeId, int value);
    
    /**
     * @brief Update the position of an existing node
     * @param nodeId Canvas node ID
     * @param x New X position
     * @param y New Y position
     */
    void updateNodePosition(const std::string& nodeId, double x, double y);

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

    // Mapping from canvas node ID to original structure node ID
    std::map<std::string, std::string> canvasToStructureNodeId;

    std::string draggedNodeId = "";
    int nextId = 1;

 // Backend connection
    DataModelManager* backend = nullptr;
    // Flag to control whether changes are synced to backend
    bool syncWithBackend = false;
    // ID of the structure currently being edited
    std::string currentStructureId;
};