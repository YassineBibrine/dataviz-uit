#pragma once

#include <string>
#include <vector>
#include <QJsonObject>
#include <QJsonArray>
#include "data_structure.h"

/**
 * @struct SessionData
 * @brief Contains complete session data for all structures
 */
struct SessionData {
    std::string structureType;  // Legacy: for backward compatibility
    int size;        // Legacy: for backward compatibility
    
    // New: Complete session data
    std::vector<QJsonObject> structures;  // All structures in session
    std::string selectedStructureId;       // Currently selected structure
};

/**
 * @class SessionManager
 * @brief Manages saving and loading complete application sessions
 */
class SessionManager {
public:
    /**
     * @brief Save complete session with all structures
     * @param structures Vector of all structure data
     * @param selectedId ID of currently selected structure
     */
    static void saveCompleteSession(const std::vector<QJsonObject>& structures, const std::string& selectedId);
    
    /**
     * @brief Load complete session with all structures
     * @return SessionData containing all structures and selected ID
     */
    static SessionData loadCompleteSession();
    
    /**
     * @brief Save a single structure (legacy compatibility)
     */
    static void save(const DataStructure* structure);

    /**
     * @brief Load JSON from session file (legacy compatibility)
     */
    static QJsonObject loadJson();

    /**
     * @brief Check if session file exists
     */
    static bool hasSession();
  
    /**
     * @brief Load legacy session data
     */
    static SessionData loadSession();
    
    /**
     * @brief Clear session file
     */
    static void clearSession();

private:
    static const char* SESSION_FILE;
};
