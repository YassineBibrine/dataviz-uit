#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>
#include "data_structure.h"
#include "array_structure.h"
#include "graph_structure.h"
#include "list_structure.h"
#include "tree_structure.h"

/**
 * @enum StructureCreationType
 * @brief How the structure was created
 */
enum class StructureCreationType {
    GENERATED,  // Created from config panel
    INTERACTIVE // Created by user via drag-and-drop
};

/**
 * @struct StructureMetadata
 * @brief Metadata about a data structure instance
 */
struct StructureMetadata {
    std::string id;
    std::string name;
  std::string type; // "Array", "Graph", "Tree", "List"
  StructureCreationType creationType;
    bool isSelected;
    
    StructureMetadata() : isSelected(false) {}
    
    StructureMetadata(const std::string& structId, 
             const std::string& structName,
    const std::string& structType,
      StructureCreationType creation)
        : id(structId), name(structName), type(structType), 
   creationType(creation), isSelected(false) {}
};

/**
 * @class DataModelManager
 * @brief Manages creation, lifecycle, and restoration of data structures
 */
class DataModelManager
{
private:
    // Map of structure ID -> structure instance
    std::map<std::string, std::unique_ptr<DataStructure>> structures;
    
    // Map of structure ID -> metadata
    std::map<std::string, StructureMetadata> metadata;
    
    // Currently selected structure for algorithm execution
    std::string selectedStructureId;
    
    // ID counter for auto-generated IDs
    int nextStructureId = 1;
    
    // Legacy support - points to currently selected structure
    std::unique_ptr<DataStructure> currentStructure;

public:
    // ✅ Constructor: restore last structure if exists
    DataModelManager();

    ~DataModelManager() = default;

    DataModelManager(const DataModelManager&) = delete;
    DataModelManager& operator=(const DataModelManager&) = delete;
    DataModelManager(DataModelManager&&) = default;
    DataModelManager& operator=(DataModelManager&&) = default;

    DataStructure* createDataStructure(const std::string& type, int size);

    DataStructure* getCurrentStructure() noexcept { return currentStructure.get(); }
    const DataStructure* getCurrentStructure() const noexcept { return currentStructure.get(); }

    void clear() noexcept { currentStructure.reset(); }
    
    // Get structure by ID
    DataStructure* getStructure(const std::string& structureId);
 
    // Get all structures metadata
 std::vector<StructureMetadata> getAllStructures() const;
    
    // Build structure from interactive nodes
    std::string buildStructureFromNodes(
        const std::string& type,
        const std::map<std::string, int>& nodeData,
        const std::vector<std::pair<std::string, std::string>>& edgeData,
        const std::string& name
    );
    
    // Create structure with name
    std::string createDataStructure(const std::string& type, int size, const std::string& name);
    
    // Structure selection and management
    void selectStructure(const std::string& structureId);
    std::string getSelectedStructureId() const;
    void removeStructure(const std::string& structureId);
    void renameStructure(const std::string& structureId, const std::string& newName);
    
    // Create sample structures
    std::vector<std::string> createSampleStructures();
    
    /**
     * @brief Get selected structure (legacy support)
     */
    DataStructure* getSelectedStructure();
    
    /**
     * @brief Save current session to disk
     */
    void saveSession();
    
    /**
     * @brief Load session from disk and restore all structures
     * @return True if session was loaded successfully
     */
    bool loadSession();
    
    /**
     * @brief Get all structures as JSON objects for session saving
     */
    std::vector<QJsonObject> getAllStructuresAsJson() const;
 
    /**
  * @brief Restore structures from JSON objects
     * @param structuresJson Vector of JSON objects representing structures
     * @return Vector of created structure IDs
     */
    std::vector<std::string> restoreStructuresFromJson(const std::vector<QJsonObject>& structuresJson);
};
