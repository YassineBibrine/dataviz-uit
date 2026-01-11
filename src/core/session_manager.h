#pragma once

#include <string>
#include <QJsonObject>
#include "data_structure.h"

struct SessionData {
    std::string structureType;
    int size;
};

class SessionManager {
public:
    // Sauvegarde la structure complète dans session.json
    static void save(const DataStructure* structure);

    // Charge la structure complète depuis session.json
    static QJsonObject loadJson();

    // Compatibilité simple type/size (optionnel)
    static void saveTypeSize(const std::string& type, int size);
    static bool loadTypeSize(std::string& type, int& size);

    // ✅ Nouvelles fonctions pour MainWindow
    static bool hasSession();
    static SessionData loadSession();
};
