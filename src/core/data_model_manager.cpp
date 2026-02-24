#include "data_model_manager.h"
#include <stdexcept>
#include "../core/session_manager.h"
#include <QJsonObject>
#include <QString>

// ----------------------------------------------------
// Constructor: attempt to restore last saved (legacy) structure
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
 // Unknown types are ignored for legacy restore
 }
}

// ----------------------------------------------------
// Create a new data structure instance (does not persist by default)
// ----------------------------------------------------
DataStructure* DataModelManager::createDataStructure(const std::string& type, int size)
{
 if (size <=0) {
 throw std::invalid_argument("Size must be greater than0");
 }

 if (type == "Array") {
 currentStructure = std::make_unique<ArrayStructure>(size);
 }
 else if (type == "Graph") {
 auto graph = std::make_unique<GraphStructure>(false);
 // Create a more densely connected graph: aim for about (n * (n-1)) /4 edges for undirected
 // This ensures good connectivity without being fully connected
 int targetEdges = std::max(size *2, static_cast<int>(size * (size -1) /4));
 graph->generateRandom(size, targetEdges);
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

 // Persist the newly created structure using legacy session save
 if (currentStructure) {
 SessionManager::save(currentStructure.get());
 }

 return currentStructure.get();
}

// ----------------------------------------------------
// Get structure by identifier
// ----------------------------------------------------
DataStructure* DataModelManager::getStructure(const std::string& structureId)
{
 auto it = structures.find(structureId);
 if (it != structures.end()) {
 return it->second.get();
 }

 // Fallback to currentStructure when requested or when ID is empty
 if (currentStructure && (structureId.empty() || structureId == "current")) {
 return currentStructure.get();
 }

 return nullptr;
}

// ----------------------------------------------------
// List metadata for all managed structures
// ----------------------------------------------------
std::vector<StructureMetadata> DataModelManager::getAllStructures() const
{
 std::vector<StructureMetadata> result;
 for (const auto& pair : metadata) {
 result.push_back(pair.second);
 }
 return result;
}

// ----------------------------------------------------
// Build a new structure from interactive node/edge data provided by the UI
// The returned string is the new structure's internal ID.
// ----------------------------------------------------
std::string DataModelManager::buildStructureFromNodes(
 const std::string& type,
 const std::map<std::string, int>& nodeData,
 const std::vector<std::pair<std::string, std::string>>& edgeData,
 const std::string& name)
{
 std::string structId = "struct_" + std::to_string(nextStructureId++);
 std::unique_ptr<DataStructure> newStructure;

 if (type == "Array") {
 auto arr = std::make_unique<ArrayStructure>();
 std::vector<int> data;
 // Note: nodeData ordering is caller-defined; simply collect values.
 for (const auto& pair : nodeData) {
 data.push_back(pair.second);
 }
 arr->getData() = data;
 newStructure = std::move(arr);
 }
 else if (type == "Graph") {
 auto graph = std::make_unique<GraphStructure>(false);
 // Add nodes with properties containing the value as string
 for (const auto& pair : nodeData) {
 std::map<std::string, std::string> props;
 props["value"] = std::to_string(pair.second);
 graph->getGraph()->addNode(pair.first, props);
 }
 // Add edges
 for (const auto& edge : edgeData) {
 graph->getGraph()->addEdge(edge.first, edge.second);
 }
 newStructure = std::move(graph);
 }
 else if (type == "LinkedList" || type == "List") {
 auto list = std::make_unique<ListStructure>();
 for (const auto& pair : nodeData) {
 list->append(pair.second);
 }
 newStructure = std::move(list);
 }
 else if (type == "Tree" || type == "BinaryTree" || type == "Binary Tree") {
 auto tree = std::make_unique<TreeStructure>();
 for (const auto& pair : nodeData) {
 tree->insert(pair.second);
 }
 newStructure = std::move(tree);
 }

 if (newStructure) {
 structures[structId] = std::move(newStructure);
 metadata[structId] = StructureMetadata(
 structId,
 name.empty() ? type : name,
 type,
 StructureCreationType::INTERACTIVE
 );
 }

 return structId;
}

// ----------------------------------------------------
// Create a named structure and add to the manager without immediately saving
// ----------------------------------------------------
std::string DataModelManager::createDataStructure(const std::string& type, int size, const std::string& name)
{
 std::string structId = "struct_" + std::to_string(nextStructureId++);
 std::unique_ptr<DataStructure> newStructure;

 if (size <=0) {
 return "";
 }

 if (type == "Array") {
 newStructure = std::make_unique<ArrayStructure>(size);
 }
 else if (type == "Graph") {
 auto graph = std::make_unique<GraphStructure>(false);
 int targetEdges = std::max(size *2, static_cast<int>(size * (size -1) /4));
 graph->generateRandom(size, targetEdges);
 newStructure = std::move(graph);
 }
 else if (type == "LinkedList" || type == "List") {
 auto list = std::make_unique<ListStructure>();
 list->generateRandom(size);
 newStructure = std::move(list);
 }
 else if (type == "Tree" || type == "BinaryTree" || type == "Binary Tree") {
 auto tree = std::make_unique<TreeStructure>();
 tree->generateRandom(size);
 newStructure = std::move(tree);
 }
 else {
 return "";
 }

 if (newStructure) {
 structures[structId] = std::move(newStructure);
 metadata[structId] = StructureMetadata(
 structId,
 name.empty() ? type : name,
 type,
 StructureCreationType::GENERATED
 );

 // Do not automatically persist here; leave persistence to caller
 qDebug() << "Structure created:" << QString::fromStdString(structId);
 }

 return structId;
}

// ----------------------------------------------------
// Select a structure by id, update metadata and optionally persist
// ----------------------------------------------------
void DataModelManager::selectStructure(const std::string& structureId)
{
 // Deselect all
 for (auto& pair : metadata) {
 pair.second.isSelected = false;
 }

 // Select the requested one
 auto it = metadata.find(structureId);
 if (it != metadata.end()) {
 it->second.isSelected = true;
 selectedStructureId = structureId;

 // Update currentStructure for legacy support by saving the structure
 auto structIt = structures.find(structureId);
 if (structIt != structures.end()) {
 // Persist using SessionManager to enable legacy restore
 SessionManager::save(structIt->second.get());
 }
 }
}

// ----------------------------------------------------
// Get selected structure id
// ----------------------------------------------------
std::string DataModelManager::getSelectedStructureId() const
{
 return selectedStructureId;
}

// ----------------------------------------------------
// Remove a structure and its metadata
// ----------------------------------------------------
void DataModelManager::removeStructure(const std::string& structureId)
{
 structures.erase(structureId);
 metadata.erase(structureId);

 if (selectedStructureId == structureId) {
 selectedStructureId = "";
 }
}

// ----------------------------------------------------
// Rename a structure's display name
// ----------------------------------------------------
void DataModelManager::renameStructure(const std::string& structureId, const std::string& newName)
{
 auto it = metadata.find(structureId);
 if (it != metadata.end()) {
 it->second.name = newName;
 }
}

// ----------------------------------------------------
// Return pointer to selected structure (legacy support)
// ----------------------------------------------------
DataStructure* DataModelManager::getSelectedStructure()
{
 if (!selectedStructureId.empty()) {
 auto it = structures.find(selectedStructureId);
 if (it != structures.end()) {
 return it->second.get();
 }
 }
 // Fallback to current structure
 return currentStructure.get();
}

// ----------------------------------------------------
// Save the current session to disk using SessionManager
// ----------------------------------------------------
void DataModelManager::saveSession()
{
 std::vector<QJsonObject> structuresJson = getAllStructuresAsJson();
 SessionManager::saveCompleteSession(structuresJson, selectedStructureId);
}

// ----------------------------------------------------
// Load session and restore structures. Supports new (v2) and legacy formats
// ----------------------------------------------------
bool DataModelManager::loadSession()
{
 SessionData sessionData = SessionManager::loadCompleteSession();

 // New format handling: restore all structures
 if (!sessionData.structures.empty()) {
 // Clear existing structures
 structures.clear();
 metadata.clear();
 selectedStructureId = "";

 // Restore structures from JSON
 std::vector<std::string> restoredIds = restoreStructuresFromJson(sessionData.structures);

 // Restore selection
 if (!sessionData.selectedStructureId.empty()) {
 selectStructure(sessionData.selectedStructureId);
 } else if (!restoredIds.empty()) {
 selectStructure(restoredIds[0]);
 }

 return !restoredIds.empty();
 }
 // Legacy format fallback
 else if (!sessionData.structureType.empty() && sessionData.size >0) {
 std::string structId = createDataStructure(sessionData.structureType, sessionData.size, "Restored");
 if (!structId.empty()) {
 selectStructure(structId);
 return true;
 }
 }

 return false;
}

// ----------------------------------------------------
// Serialize all managed structures to JSON objects
// ----------------------------------------------------
std::vector<QJsonObject> DataModelManager::getAllStructuresAsJson() const
{
 std::vector<QJsonObject> result;

 for (const auto& pair : structures) {
 const std::string& structId = pair.first;
 const DataStructure* structure = pair.second.get();

 if (!structure) continue;

 // Serialize the structure
 QJsonObject structObj = structure->serialize();

 // Add metadata (id, name, creationType)
 auto metaIt = metadata.find(structId);
 if (metaIt != metadata.end()) {
 structObj["id"] = QString::fromStdString(metaIt->second.id);
 structObj["name"] = QString::fromStdString(metaIt->second.name);
 structObj["creationType"] = (metaIt->second.creationType == StructureCreationType::GENERATED) ? "generated" : "interactive";
 }

 result.push_back(structObj);
 }

 return result;
}

// ----------------------------------------------------
// Restore structures from a list of JSON objects and return created IDs
// ----------------------------------------------------
std::vector<std::string> DataModelManager::restoreStructuresFromJson(const std::vector<QJsonObject>& structuresJson)
{
 std::vector<std::string> restoredIds;

 for (const auto& structObj : structuresJson) {
 if (!structObj.contains("type")) continue;

 std::string type = structObj["type"].toString().toStdString();
 std::string id = structObj.contains("id") ?
 structObj["id"].toString().toStdString() :
 "struct_" + std::to_string(nextStructureId++);
 std::string name = structObj.contains("name") ?
 structObj["name"].toString().toStdString() : type;

 std::unique_ptr<DataStructure> newStructure;

 // Create structure based on type and deserialize
 if (type == "Array") {
 auto arr = std::make_unique<ArrayStructure>();
 arr->deserialize(structObj);
 newStructure = std::move(arr);
 }
 else if (type == "Graph") {
 auto graph = std::make_unique<GraphStructure>();
 graph->deserialize(structObj);
 newStructure = std::move(graph);
 }
 else if (type == "LinkedList" || type == "List") {
 auto list = std::make_unique<ListStructure>();
 list->deserialize(structObj);
 newStructure = std::move(list);
 }
 else if (type == "Tree" || type == "BinaryTree") {
 auto tree = std::make_unique<TreeStructure>();
 tree->deserialize(structObj);
 newStructure = std::move(tree);
 }

 if (newStructure) {
 structures[id] = std::move(newStructure);

 // Restore metadata
 StructureCreationType creationType = StructureCreationType::GENERATED;
 if (structObj.contains("creationType")) {
 creationType = (structObj["creationType"].toString() == "interactive") ?
 StructureCreationType::INTERACTIVE : StructureCreationType::GENERATED;
 }

 metadata[id] = StructureMetadata(id, name, type, creationType);
 restoredIds.push_back(id);

 // Update next ID counter to avoid conflicts when IDs follow "struct_N" pattern
 if (id.find("struct_") ==0) {
 try {
 int idNum = std::stoi(id.substr(7));
 if (idNum >= nextStructureId) {
 nextStructureId = idNum +1;
 }
 } catch (...) {
 // Invalid ID format; ignore
 }
 }
 }
 }

 return restoredIds;
}
