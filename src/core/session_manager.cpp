#include "session_manager.h"
#include <QFile>
#include <QJsonDocument>
#include <QString>

static const char* SESSION_FILE = "session.json";

// ... tes fonctions save, loadJson, saveTypeSize, loadTypeSize ...

// ----------------------------------------------------
// Check if session file exists
// ----------------------------------------------------
bool SessionManager::hasSession() {
    QFile file(SESSION_FILE);
    return file.exists();
}

// ----------------------------------------------------
// Load type/size session
// ----------------------------------------------------
SessionData SessionManager::loadSession() {
    SessionData data{ "", 0 };
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
