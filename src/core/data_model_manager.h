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
 * @brief Manages creation and lifecycle of multiple data structures
 */
class DataModelManager {
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
    DataModelManager() = default;
    ~DataModelManager() = default;

    /**
     * @brief Create a data structure from configuration (generated)
   * @param type Type of data structure
     * @param size Initial size
     * @param name Optional custom name
     * @return ID of created structure
     */
    std::string createDataStructure(const std::string& type, int size, 
     const std::string& name = "");

    /**
     * @brief Create an empty interactive structure
     * @param type Type of data structure
     * @param name Optional custom name
     * @return ID of created structure
  */
    std::string createInteractiveStructure(const std::string& type,
       const std::string& name = "");

    /**
     * @brief Build a structure from user-created nodes and edges
     * @param type Detected or specified type
     * @param nodes Node IDs and their values
  * @param edges Edge connections
     * @param name Optional name
     * @return ID of created structure
     */
    std::string buildStructureFromNodes(
        const std::string& type,
        const std::map<std::string, int>& nodes,
        const std::vector<std::pair<std::string, std::string>>& edges,
  const std::string& name = "");

    /**
     * @brief Auto-detect structure type from topology
     * @param nodeCount Number of nodes
     * @param edges Edge list
   * @return Detected type: "Graph", "Tree", "List", or "Unknown"
     */
    std::string detectStructureType(
        int nodeCount,
        const std::vector<std::pair<std::string, std::string>>& edges);

 /**
     * @brief Select a structure for algorithm execution
     * @param structureId ID of structure to select
     * @return True if selection successful
     */
    bool selectStructure(const std::string& structureId);

    /**
     * @brief Get the currently selected structure
     * @return Pointer to selected structure, or nullptr
 */
    DataStructure* getSelectedStructure();
    const DataStructure* getSelectedStructure() const;

    /**
     * @brief Get a specific structure by ID
     */
    DataStructure* getStructure(const std::string& id);
    const DataStructure* getStructure(const std::string& id) const;

    /**
     * @brief Get all structure metadata
     * @return Vector of metadata for all structures
     */
    std::vector<StructureMetadata> getAllStructures() const;

    /**
     * @brief Get currently selected structure ID
     */
    std::string getSelectedStructureId() const { return selectedStructureId; }

 /**
     * @brief Remove a structure
     */
  bool removeStructure(const std::string& structureId);

    /**
   * @brief Clear all structures
     */
    void clearAll();

    /**
  * @brief Rename a structure
   */
bool renameStructure(const std::string& structureId, const std::string& newName);
    
    // LEGACY SUPPORT - for backward compatibility
    DataStructure* getCurrentStructure() { return getSelectedStructure(); }
    const DataStructure* getCurrentStructure() const { return getSelectedStructure(); }
};
