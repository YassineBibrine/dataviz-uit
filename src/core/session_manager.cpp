#include "session_manager.h"
#include <QFile>
#include <QJsonDocument>
#include <QString>
#include <QDebug>

const char* SessionManager::SESSION_FILE = "session.json";

// Save complete session with all structures
void SessionManager::saveCompleteSession(const std::vector<QJsonObject>& structures, const std::string& selectedId) {
    QJsonObject sessionObj;
    
    // Save all structures as an array
    QJsonArray structuresArray;
    for (const auto& structure : structures) {
        structuresArray.append(structure);
  }
    
    sessionObj["structures"] = structuresArray;
    sessionObj["selectedStructureId"] = QString::fromStdString(selectedId);
    sessionObj["version"] = "2.0";  // Session format version
    
    QFile file(SESSION_FILE);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to save session";
        return;
}
    
    QJsonDocument doc(sessionObj);
  file.write(doc.toJson());
    file.close();
    
    qDebug() << "Session saved:" << structures.size() << "structures, selected:" << QString::fromStdString(selectedId);
}

// Load complete session with all structures
SessionData SessionManager::loadCompleteSession() {
    SessionData data;
    data.structureType = "";
    data.size = 0;
    data.selectedStructureId = "";
    
    QFile file(SESSION_FILE);
  if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
     return data;
    }
  
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
  
    if (!doc.isObject()) return data;
    
    QJsonObject sessionObj = doc.object();
    
    // Check if this is new format (v2.0)
    if (sessionObj.contains("version") && sessionObj["version"].toString() == "2.0") {
    // Load all structures
        if (sessionObj.contains("structures")) {
    QJsonArray structuresArray = sessionObj["structures"].toArray();
            for (const auto& structureValue : structuresArray) {
    if (structureValue.isObject()) {
         data.structures.push_back(structureValue.toObject());
                }
      }
        }
        
        // Load selected structure ID
        if (sessionObj.contains("selectedStructureId")) {
            data.selectedStructureId = sessionObj["selectedStructureId"].toString().toStdString();
        }
        
        qDebug() << "Loaded session:" << data.structures.size() << "structures";
    }
    // Legacy format fallback
    else if (sessionObj.contains("type")) {
        data.structureType = sessionObj["type"].toString().toStdString();
        if (sessionObj.contains("size")) {
            data.size = sessionObj["size"].toInt();
        }
        qDebug() << "Loaded legacy session:" << QString::fromStdString(data.structureType);
    }
    
 return data;
}

// Save a single structure (legacy compatibility)
void SessionManager::save(const DataStructure* structure) {
    if (!structure) return;
    
    QJsonObject obj = structure->serialize();
    
    QFile file(SESSION_FILE);
    if (!file.open(QIODevice::WriteOnly)) return;
    
    QJsonDocument doc(obj);
    file.write(doc.toJson());
    file.close();
}

// Load JSON from session file (legacy compatibility)
QJsonObject SessionManager::loadJson() {
 QJsonObject obj;
    QFile file(SESSION_FILE);
    
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) return obj;
    
  QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    
  if (doc.isObject()) {
        obj = doc.object();
    }
    
  return obj;
}

// Check if session file exists
bool SessionManager::hasSession() {
    QFile file(SESSION_FILE);
    return file.exists();
}

// Load legacy session data
SessionData SessionManager::loadSession() {
    SessionData data;
    data.structureType = "";
    data.size = 0;
    
    QFile file(SESSION_FILE);
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) return data;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (!doc.isObject()) return data;

    QJsonObject obj = doc.object();
 if (obj.contains("type")) data.structureType = obj["type"].toString().toStdString();
if (obj.contains("size")) data.size = obj["size"].toInt();

    return data;
}

// Clear session file
void SessionManager::clearSession() {
    QFile file(SESSION_FILE);
    if (file.exists()) {
        file.remove();
        qDebug() << "Session cleared";
    }
}
