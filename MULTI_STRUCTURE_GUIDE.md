# Multi-Structure Architecture - Complete Guide

## ?? Overview

The DataViz UIT application now supports **multiple concurrent data structures** with the ability to:
- Create structures from configuration (generated)
- Create structures interactively by drawing nodes/edges (custom)
- Select which structure to run algorithms on
- Auto-detect structure types from topology
- Manage multiple structures simultaneously

---

## ?? Architecture Components

### 1. **DataModelManager** (Multi-Structure Core)

**Location:** `src/core/data_model_manager.h/cpp`

**Key Features:**
- Manages multiple structures with unique IDs
- Tracks metadata for each structure (name, type, creation method)
- Maintains a "selected" structure for algorithm execution
- Auto-detects structure types from node/edge topology

**Main Methods:**

```cpp
// Create from configuration
std::string createDataStructure(const std::string& type, int size, const std::string& name = "");

// Create empty interactive structure
std::string createInteractiveStructure(const std::string& type, const std::string& name = "");

// Build from user-drawn nodes/edges
std::string buildStructureFromNodes(
    const std::string& type,  // "Auto" for detection
    const std::map<std::string, int>& nodes,
    const std::vector<std::pair<std::string, std::string>>& edges,
    const std::string& name = "");

// Auto-detect structure type
std::string detectStructureType(int nodeCount, const std::vector<...>& edges);

// Selection
bool selectStructure(const std::string& structureId);
DataStructure* getSelectedStructure();

// Management
std::vector<StructureMetadata> getAllStructures() const;
bool removeStructure(const std::string& structureId);
bool renameStructure(const std::string& structureId, const std::string& newName);
```

### 2. **StructureSelector** (UI Widget)

**Location:** `src/ui/structure_selector.h/cpp`

**Features:**
- Lists all created structures
- Shows structure type and creation method indicators
- Allows selection, renaming, and removal
- Provides "Finalize Interactive" controls

**UI Elements:**
- **Structure List**: Displays all structures with indicators:
  - `?` = Generated from configuration
  - `?` = Created interactively
  - `[Type]` = Structure type (Graph, Tree, List, Array)
- **Finalize Interactive Group**:
  - Type selector (Auto, Graph, Tree, List, Array)
  - Name input field
  - "Create Structure" button - converts drawn nodes/edges to structure
  - "Clear Canvas" button - clears interactive drawing
- **Manage Structures Group**:
  - Rename button
  - Remove button (red)

### 3. **InteractionManager** (Interactive Drawing)

**Location:** `src/visualization/interaction_manager.h/cpp`

**New Methods:**

```cpp
// Finalize interactive nodes/edges into a structure
std::string finalizeStructure(const std::string& type = "Auto", 
         const std::string& name = "");

// Clear all interactive data
void clearInteractive();

// Get statistics
bool hasInteractiveData() const;
std::pair<int, int> getInteractiveStats() const; // returns {nodeCount, edgeCount}
```

### 4. **Structure Detection Algorithm**

**How it works:**

```cpp
std::string detectStructureType(int nodeCount, const std::vector<...>& edges) {
    if (edges.empty()) return "Array";  // No connections
    
    // Check for LinkedList: Linear chain
    if (each node has ?1 outgoing && ?1 incoming edge)
 return "List";
    
    // Check for Tree:
    // - Exactly 1 root (in-degree 0)
    // - All other nodes have in-degree 1
    // - edge_count == node_count - 1
    // - Graph is connected
    if (meets tree criteria)
        return "Tree";
    
    // Default
    return "Graph";
}
```

---

## ?? Complete User Workflows

### Workflow 1: Create Structure from Configuration

**User Steps:**
1. Select "Graph" from Control Panel ? Data Structure dropdown
2. Set size to 10
3. Click dropdown ? structure is created automatically
4. Structure appears in **Structure Selector** list as "Graph 1 [Graph] ?"

**What Happens:**
```
ControlPanel::dataStructureSelected("Graph")
 ?
MainWindow::onDataStructureSelected("Graph")
 ?
MainWindow::loadDataStructure("Graph", 10)
 ?
DataModelManager::createDataStructure("Graph", 10)
 ?
Returns structureId: "struct_1"
 ?
StructureSelector::refreshStructureList()
 ?
Structure auto-selected for algorithms
```

### Workflow 2: Draw Custom Structure Interactively

**User Steps:**
1. Use Toolbox ? Drag "Circle" nodes onto canvas
2. Double-click nodes to set values
3. Click "Link" tool ? click two nodes to connect them
4. Repeat to build desired topology
5. In Structure Selector:
   - Choose type: "Auto" (or specify "Graph", "Tree", etc.)
   - Enter name: "My Custom Graph"
   - Click "Create Structure"
6. Structure finalized and appears in list

**What Happens:**
```
User drags node ? VisualizationPane::dropEvent()
 ?
InteractionManager::addNode(x, y, "CIRCLE")
 ?
[Nodes stored as "mock" data locally]
 ?
User clicks "Link" tool ? connects nodes
 ?
InteractionManager::addEdge(source, target)
 ?
User clicks "Create Structure" in StructureSelector
 ?
StructureSelector::finalizeInteractiveRequested("Auto", "My Custom Graph")
 ?
MainWindow::onFinalizeInteractive()
 ?
InteractionManager::finalizeStructure()
 ?
Collects node values and edges
 ?
DataModelManager::buildStructureFromNodes("Auto", nodes, edges, name)
 ?
detectStructureType() ? determines "Graph" or "Tree" or "List"
 ?
Creates actual GraphStructure/TreeStructure/ListStructure
 ?
Returns structureId: "custom_2"
 ?
StructureSelector::refreshStructureList()
 ?
Shows "My Custom Graph [Graph] ?"
```

### Workflow 3: Select Structure and Run Algorithm

**User Steps:**
1. Click on a structure in Structure Selector list
2. Structure becomes **bold** and highlighted
3. Select algorithm from Control Panel ? Algorithm dropdown
4. Click "Play"
5. Algorithm runs on the selected structure

**What Happens:**
```
User clicks structure in list
 ?
StructureSelector::onStructureClicked()
 ?
emit structureSelected("struct_1")
 ?
MainWindow::onStructureSelected("struct_1")
 ?
DataModelManager::selectStructure("struct_1")
 ?
Updates metadata: structures["struct_1"].isSelected = true
 ?
Updates visualization (optional)
 ?
User selects algorithm
 ?
User clicks Play
 ?
MainWindow::executeAlgorithm()
 ?
Gets selected structure: dataModelManager->getSelectedStructure()
 ?
Creates algorithm: algoManager.createAlgorithm("Sorting", "BubbleSort")
 ?
Passes structure data: algo->setData(structure->getDataForRunner())
 ?
algo->execute() ? runs on selected structure only
```

---

## ?? UI Layout Changes

### Before (Single Structure):
```
???????????????????????????????????????
? Control Panel     ?
? - Algorithm dropdown     ?
? - Play/Pause/Reset      ?
?      ?
? Metrics Panel        ?
?         ?
???????????????????????????????????????
```

### After (Multi-Structure):
```
???????????????????????????????????????
? Structure Selector     [NEW]        ?
? ??????????????????????????????????? ?
? ? ? Array 1 [Array] ?      ? ?
? ?   Graph 2 [Graph] ?    ? ?
? ?   My Tree [Tree] ?  ? ?
? ??????????????????????????????????? ?
? Selected: Array 1      ?
?      ?
? Finalize Interactive             ?
? Type: [Auto ?]  Name: [________]    ?
? [Create Structure] [Clear Canvas]   ?
?         ?
? Manage Structures ?
? [Rename] [Remove]        ?
???????????????????????????????????????
? Control Panel       ?
? - Data Structure dropdown           ?
? - Algorithm dropdown              ?
? - Play/Pause/Reset               ?
???????????????????????????????????????
? Metrics Panel           ?
???????????????????????????????????????
```

---

## ?? Structure Detection Examples

### Example 1: Linked List Detection
```
Nodes: A, B, C, D
Edges: A?B, B?C, C?D

Analysis:
- Each node has ?1 outgoing edge: ?
- Each node has ?1 incoming edge: ?
- Forms a linear chain: ?

Result: "List"
```

### Example 2: Binary Tree Detection
```
Nodes: A, B, C, D, E
Edges: A?B, A?C, B?D, B?E

Analysis:
- Exactly 1 root (A has in-degree 0): ?
- All others have in-degree 1: ?
- edge_count (4) == node_count (5) - 1: ?
- Connected graph: ?

Result: "Tree"
```

### Example 3: Graph Detection
```
Nodes: A, B, C, D
Edges: A?B, B?C, C?A, A?D

Analysis:
- Has a cycle (A?B?C?A): cycle detected
- Node A has in-degree 2: ? (not a tree)

Result: "Graph"
```

---

## ?? Data Flow: Interactive ? Structure

### Step-by-Step Data Transformation:

```
1. User Drawing Phase (Interactive)
   ????????????????????????????????
   ? InteractionManager    ?
   ? - MockNode{id, x, y, type}   ?
   ? - MockEdge{source, target}   ?
   ? - nodeValues{id ? value}     ?
   ????????????????????????????????
    ? (visualization only)
   ????????????????????????????????
   ? VisualizationPane renders    ?
   ? nodes and edges on canvas    ?
   ????????????????????????????????

2. Finalization Request
   User clicks "Create Structure"
   ?
   InteractionManager::finalizeStructure()
    ?
   Converts to standard format:
   ????????????????????????????????
   ? std::map<std::string, int>   ?
   ? nodeData = {      ?
   ? {"n1", 10},    ?
   ?   {"n2", 20},    ?
   ?   {"n3", 30}               ?
   ? }     ?
   ?              ?
? std::vector<pair<...>>       ?
   ? edgeData = {      ?
   ?   {"n1", "n2"},     ?
   ?   {"n2", "n3"}           ?
   ? }          ?
 ????????????????????????????????
              ?
   DataModelManager::buildStructureFromNodes()
   ?
   detectStructureType() ? "List"
              ?
   Creates ListStructure instance
   ?
   Populates with nodes and edges
  ?
   Stores in structures map with ID "custom_1"
           ?
   Returns ID to UI
            ?
   Structure now available for algorithms! ?
```

---

## ?? Common Issues & Solutions

### Issue 1: "No Structure Selected" when running algorithm

**Cause:** No structure is selected in DataModelManager

**Solution:**
- Click on a structure in the Structure Selector list
- Or create a structure first (either method)
- Selected structure shows in **bold** in the list

### Issue 2: Interactive nodes don't become a structure

**Cause:** User forgot to finalize

**Solution:**
- After drawing nodes/edges, click "Create Structure"
- Interactive data is separate from actual structures
- Finalization converts drawing ? data structure

### Issue 3: Wrong structure type detected

**Cause:** Topology is ambiguous

**Solution:**
- Manually specify type in dropdown (don't use "Auto")
- Or adjust edges to match desired topology:
  - **List**: Linear chain only
  - **Tree**: No cycles, single root, n-1 edges
  - **Graph**: Anything else

### Issue 4: Can't remove/rename structure

**Cause:** No structure selected in list

**Solution:**
- First click on a structure in the list
- Then click Rename or Remove button

---

## ?? Future Enhancements

1. **Visual Selection Indicator**
   - Highlight selected structure in visualization pane
   - Different colors for different structures

2. **Structure Comparison**
   - Run same algorithm on multiple structures
   - Side-by-side visualization

3. **Structure Export/Import**
   - Save structures to JSON files
   - Load saved structures

4. **Advanced Detection**
   - Detect AVL/Red-Black trees
   - Detect directed vs undirected graphs
   - Detect weighted graphs

5. **Structure Transformation**
   - Convert List ? Array
   - Convert Tree ? Graph
   - Merge multiple structures

---

## ?? Summary

### Key Concepts:
1. **Multiple Structures**: App can hold many structures simultaneously
2. **Selection**: Only ONE structure is "selected" for algorithms at a time
3. **Interactive vs Generated**: Two ways to create structures
4. **Auto-Detection**: System can guess structure type from topology
5. **Finalization**: Interactive drawings must be "finalized" to become usable structures

### User Benefits:
- ? Create multiple structures without losing previous work
- ? Compare different structures visually
- ? Draw custom topologies freehand
- ? No need to manually specify structure type
- ? Easy structure management (rename, remove)
- ? Clear visual feedback on which structure is active

### Developer Benefits:
- ? Clean separation of concerns
- ? Extensible architecture
- ? Type-safe structure handling
- ? Easy to add new structure types
- ? Consistent API across all structure types

---

**End of Multi-Structure Architecture Guide**
