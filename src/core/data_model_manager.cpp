#include "data_model_manager.h"
#include <stdexcept>
#include "../session/session_manager.h"
#include <QJsonObject>
#include <QString>

// ----------------------------------------------------
// Constructor: restore last session if exists
// ----------------------------------------------------
DataModelManager::DataModelManager()
{
    QJsonObject obj = SessionManager::loadJson();
    if (!obj.isEmpty())
    {
        std::string type = obj["type"].toString().toStdString();

        if (type == "Array") {
            auto array = std::make_unique<ArrayStructure>();
            array->deserialize(obj);
            currentStructure = std::move(array);
        }
        else if (type == "Graph") {
            auto graph = std::make_unique<GraphStructure>();
            graph->deserialize(obj);
            currentStructure = std::move(graph);
        }
        else if (type == "LinkedList") {
            auto list = std::make_unique<ListStructure>();
            list->deserialize(obj);
            currentStructure = std::move(list);
        }
        else if (type == "Tree" || type == "BinaryTree") {
            auto tree = std::make_unique<TreeStructure>();
            tree->deserialize(obj);
            currentStructure = std::move(tree);
        }
        // else: unknown type → ignore
    }
}

// ----------------------------------------------------
// Create new structure and save session
// ----------------------------------------------------
DataStructure* DataModelManager::createDataStructure(const std::string& type, int size)
{
    if (size <= 0) {
        throw std::invalid_argument("Size must be greater than 0");
    }

    if (type == "Array") {
        currentStructure = std::make_unique<ArrayStructure>(size);
    }
    else if (type == "Graph") {
        auto graph = std::make_unique<GraphStructure>(false);
        graph->generateRandom(size, static_cast<int>(size * 1.5));
        currentStructure = std::move(graph);
    }
    else if (type == "LinkedList") {
        auto list = std::make_unique<ListStructure>();
        list->generateRandom(size);
        currentStructure = std::move(list);
    }
    else if (type == "Tree" || type == "BinaryTree") {
        auto tree = std::make_unique<TreeStructure>();
        tree->generateRandom(size);
        currentStructure = std::move(tree);
    }
    else {
        throw std::invalid_argument("Unknown data structure type: " + type);
    }

    // ✅ Save full structure
    if (currentStructure) {
        SessionManager::save(currentStructure.get());
    }

    return currentStructure.get();
}
