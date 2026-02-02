# Root Cause: Random Tree Rebuilds and Data Loss - PARTIALLY FIXED

## Problem Summary

Nodes disappear and values change randomly when:
1. You add nodes manually
2. You change values  
3. You switch structures and return
4. The tree **rebuilds itself**, losing your changes

## Root Cause Identified

### Primary Issue: Mapping Desynchronization  
**File**: `src/ui/main_window.cpp`  
**Function**: `loadStructureIntoCanvas()`

The problem occurs because:

1. **`clearInteractive()` wipes the mapping** - This is necessary to start fresh
2. **Nodes are loaded while sync is OFF** - Mapping rebuilt via `addNodeWithMapping()`
3. **Sync is turned ON** - Now the mapping should be complete
4. **User adds a node** - New canvas node "n4" created
5. **`saveToCurrentStructure()` called** - Checks mapping for "n4"
6. **"n4" not found in mapping** - Treated as NEW node ? triggers REBUILD
7. **Tree gets destroyed and rebuilt** - User changes lost

### The Core Problem

The `canvasToStructureNodeId` mapping **only tracks LOADED nodes**, not MANUALLY ADDED nodes.

When you:
- Add a node manually ? `addNode()` creates "n4"
- `addNode()` calls `saveToCurrentStructure()` with sync=true
- `saveToCurrentStructure()` looks for "n4" in mapping
- "n4" not in mapping ? treated as structural change
- Full rebuild triggered ? your edits lost

## Partial Fix Implemented

Changed the order in `loadStructureIntoCanvas()`:

```cpp
// OLD ORDER (BAD):
setSyncWithBackend(false);
clearInteractive();  // Wipes mapping
// ... load nodes ... (mapping rebuilt)
saveNodePositionsToStructure();  // ? Calls save with sync=false
setSyncWithBackend(true);

// NEW ORDER (BETTER):
setSyncWithBackend(false);
clearInteractive();  // Wipes mapping  
// ... load nodes ... (mapping rebuilt)
saveNodePositionsToStructure();  // ? Saves positions while sync=false
setSyncWithBackend(true);  // ? Sync enabled LAST
```

**What this fixes:**
- ? No save during load
- ? Mapping complete before sync enabled
- ? Positions saved before sync enabled

**What this doesn't fix:**
- ? Manually added nodes still not in mapping
- ? Rebuild still triggered when adding nodes
- ? The fundamental mapping issue remains

## Complete Solution Needed

To fully fix this, we need to:

### Option 1: Update Mapping on Add
```cpp
std::string InteractionManager::addNode(double x, double y, const std::string& type) {
    std::string canvasId = "n" + std::to_string(nextId++);
    nodes.push_back({ canvasId, x, y, type });

    if (syncWithBackend && !currentStructureId.empty()) {
        // **FIX**: Generate new structure ID for this node
        int newIndex = getNextTreeIndex();  // Get next available tree_X index
        std::string structureNodeId = "tree_" + std::to_string(newIndex);
        canvasToStructureNodeId[canvasId] = structureNodeId;
  
        // Now save with mapping complete
        saveToCurrentStructure();
        saveNodePositionsToStructure();
    }

    return canvasId;
}
```

### Option 2: Don't Rebuild on Missing Mapping
```cpp
// In saveToCurrentStructure():
for (const auto& canvasNode : nodes) {
    if (canvasIdToTreeNode.find(canvasNode.id) == canvasIdToTreeNode.end()) {
        // **FIX**: Create mapping for new node instead of rebuilding
        if (canvasToStructureNodeId.find(canvasNode.id) == canvasToStructureNodeId.end()) {
     // This is a NEW node - generate ID for it
            canvasToStructureNodeId[canvasNode.id] = generateNewTreeId();
  }
        newNodeIds.push_back(canvasNode.id);
    }
}

// Then handle new nodes WITHOUT full rebuild
if (!newNodeIds.empty()) {
    // Add new nodes to tree without destroying existing structure
    for (const auto& nodeId : newNodeIds) {
        // Add to tree...
    }
}
```

### Option 3: Track Manual Nodes Separately
```cpp
class InteractionManager {
    std::set<std::string> manuallyAddedNodes;  // Track which nodes user added
    
    void addNode() {
        // ... create node ...
        manuallyAddedNodes.insert(canvasId);
    // Don't trigger rebuild for manually added nodes
    }
}
```

## Why It Still Fails

Even with the partial fix, the workflow is:

```
1. Load tree: [n1?tree_0, n2?tree_1, n3?tree_2]
2. User clicks Add Node
3. addNode() creates n4
4. saveToCurrentStructure() called
5. Check mapping: n4 not found
6. Rebuild triggered ?
```

The mapping is complete **for loaded nodes** but **not for new nodes**.

## Test to Confirm

1. Create a binary tree with 3 nodes
2. Note the values (e.g., 10, 5, 15)
3. Click "Add Node" (circle tool)
4. Add one node to canvas
5. Watch debug output - you'll see "New nodes detected - rebuilding tree structure"
6. Tree gets rebuilt with potentially different structure
7. Your original edits might be lost

## Severity: CRITICAL (Partially Mitigated)

- ? Load timing improved
- ? No saves during load
- ? Adding nodes still triggers rebuilds
- ? Data loss still possible

**Status**: Partial fix applied, complete solution needed
