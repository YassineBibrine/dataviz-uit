# Binary Tree Edge Preservation and Duplicate Value Support Fix

## Problem Description

Two critical issues were present in the binary tree implementation:

### Issue 1: Edges Not Saved
When switching between structures, the tree edges were not being saved. Upon returning to a tree:
- Only node positions were restored
- Edges were regenerated based on the tree structure (left/right pointers)
- Manually drawn edges that didn't match the tree structure were lost

### Issue 2: Duplicate Values Break Everything
When two or more nodes had the same value:
- Node mapping relied on matching values (`valueToIndex`)
- Duplicate values caused incorrect node mapping
- Some nodes would be lost or incorrectly mapped
- Edges would connect to wrong nodes

## Root Causes

### 1. No Edge Persistence for Trees
In `InteractionManager::saveToCurrentStructure()`:
```cpp
// OLD CODE - edges were NOT saved for trees
structure->clearCustomEdges();
// DO NOT save custom edges for trees - edges are part of tree structure
```

Trees were treated specially - their edges were considered "derived from structure" rather than user-defined, so they weren't saved.

### 2. Value-Based Mapping
The system used node **values** to map canvas nodes to tree nodes:
```cpp
// OLD CODE - mapping by value
std::map<int, int> valueToIndex;  // value -> tree index
// ...
for (const auto& canvasNode : nodes) {
    int value = nodeValues[canvasNode.id];
    auto idxIt = valueToIndex.find(value);// BREAKS with duplicates!
    if (idxIt != valueToIndex.end()) {
        canvasToStructureNodeId[canvasNode.id] = "tree_" + std::to_string(idxIt->second);
    }
}
```

This breaks when multiple nodes have the same value because `valueToIndex` can only store one tree index per value.

## Solution

### Fix 1: Save Custom Edges (interaction_manager.cpp)

Modified `saveToCurrentStructure()` for trees to save edges:

```cpp
// **FIX 3**: Save custom edges for manual trees
structure->clearCustomEdges();
for (const auto& edge : canvasEdges) {
    auto fromIt = canvasToStructureNodeId.find(edge.source);
    auto toIt = canvasToStructureNodeId.find(edge.target);
    
    if (fromIt != canvasToStructureNodeId.end() && toIt != canvasToStructureNodeId.end()) {
        structure->addCustomEdge(fromIt->second, toIt->second);
  qDebug() << "Saved custom edge:" << QString::fromStdString(fromIt->second)
      << "->" << QString::fromStdString(toIt->second);
    }
}
```

### Fix 2: Canvas ID-Based Mapping (interaction_manager.cpp)

Changed from value-based to canvas ID-based mapping using the stored `canvasToStructureNodeId`:

```cpp
// **FIX 1**: Build mapping using CANVAS IDs directly, not values
// This allows duplicate values in the tree
std::map<std::string, TreeNode*> canvasIdToTreeNode;

// Traverse existing tree and match to canvas nodes using STORED MAPPING
TreeNode* existingRoot = const_cast<TreeNode*>(tree->getRoot());

if (existingRoot) {
    // Tree exists - build TreeNode* -> tree_index mapping first
    std::map<TreeNode*, int> nodeToIndex;
    std::queue<TreeNode*> bfsQueue;
    bfsQueue.push(existingRoot);
    int index = 0;

    while (!bfsQueue.empty()) {
        TreeNode* treeNode = bfsQueue.front();
        bfsQueue.pop();
        if (treeNode) {
            nodeToIndex[treeNode] = index++;
     if (treeNode->left) bfsQueue.push(treeNode->left);
            if (treeNode->right) bfsQueue.push(treeNode->right);
   }
    }

    // Now match canvas nodes to tree nodes using the saved mapping
    for (const auto& canvasNode : nodes) {
        auto mappingIt = canvasToStructureNodeId.find(canvasNode.id);
   if (mappingIt != canvasToStructureNodeId.end()) {
          // We have a saved mapping - parse tree_X to get index
          std::string treeNodeId = mappingIt->second;
       if (treeNodeId.find("tree_") == 0) {
     try {
    int treeIndex = std::stoi(treeNodeId.substr(5));
       // Find the TreeNode* with this index
  for (const auto& pair : nodeToIndex) {
   if (pair.second == treeIndex) {
              canvasIdToTreeNode[canvasNode.id] = pair.first;
       break;
               }
              }
} catch (...) {}
  }
 }
    }
}
```

### Fix 3: Restore Custom Edges (main_window.cpp)

Modified `layoutTreeHierarchically()` to restore saved edges:

```cpp
// **FIX**: Restore custom edges if they exist, otherwise use tree structure edges
if (structure->hasCustomEdges()) {
    qDebug() << "Restoring" << structure->getCustomEdges().size() << "custom edges";
    const auto& customEdges = structure->getCustomEdges();
    for (const auto& edge : customEdges) {
        if (oldToNewId.count(edge.from) && oldToNewId.count(edge.to)) {
            interactionMgr->addEdge(oldToNewId[edge.from], oldToNewId[edge.to]);
    qDebug() << "Restored edge:" << QString::fromStdString(edge.from)
         << "->" << QString::fromStdString(edge.to);
        }
  }
}
else {
    // No custom edges - use tree structure edges
    for (const auto& kv : edgesMap) {
     const std::string parentId = kv.first;
        const std::string leftId = kv.second.first;
        const std::string rightId = kv.second.second;
        if (!leftId.empty() && oldToNewId.count(parentId) && oldToNewId.count(leftId)) {
            interactionMgr->addEdge(oldToNewId[parentId], oldToNewId[leftId]);
        }
        if (!rightId.empty() && oldToNewId.count(parentId) && oldToNewId.count(rightId)) {
            interactionMgr->addEdge(oldToNewId[parentId], oldToNewId[rightId]);
        }
    }
}
```

## How It Works

### Workflow with Duplicate Values:

1. **User creates tree with duplicate values:**
   ```
   Node n1: value = 5
   Node n2: value = 5  (duplicate!)
   Node n3: value = 7
```

2. **Initial mapping created (addNodeWithMapping):**
   ```
   canvasToStructureNodeId[n1] = "tree_0"
   canvasToStructureNodeId[n2] = "tree_1"
   canvasToStructureNodeId[n3] = "tree_2"
   ```

3. **User draws edges:**
   ```
   Edge: n1 -> n2
   Edge: n1 -> n3
   ```

4. **Save (saveToCurrentStructure):**
   - Uses `canvasToStructureNodeId` to map canvas IDs to tree IDs
   - Saves custom edges: `{"tree_0" -> "tree_1", "tree_0" -> "tree_2"}`
   - **No longer uses values for mapping!**

5. **Switch to another structure:**
   - State is saved automatically in `onStructureSelected()`

6. **Return to tree:**
   - `layoutTreeHierarchically()` checks `hasSavedPositions`
   - Creates nodes at saved positions
 - **Checks `hasCustomEdges()`**
   - Restores edges from saved list
   - Result: Exact same tree with duplicate values preserved!

### Key Data Structures:

#### canvasToStructureNodeId (InteractionManager)
```cpp
std::map<std::string, std::string> canvasToStructureNodeId;
// Example:
// "n15" -> "tree_0"   (canvas node n15 maps to tree node 0)
// "n16" -> "tree_1"   (canvas node n16 maps to tree node 1)
// "n17" -> "tree_2"   (canvas node n17 maps to tree node 2)
```

#### customEdges (DataStructure)
```cpp
std::vector<DSEdge> customEdges;
// Example:
// {"tree_0", "tree_1"}  (edge from tree node 0 to tree node 1)
// {"tree_0", "tree_2"}  (edge from tree node 0 to tree node 2)
```

#### nodePositions (DataStructure)
```cpp
std::map<std::string, DSNodePosition> nodePositions;
// Example:
// "tree_0" -> {x: 400.0, y: 80.0}
// "tree_1" -> {x: 310.0, y: 200.0}
// "tree_2" -> {x: 490.0, y: 200.0}
```

## Benefits

### 1. Duplicate Values Support
- ? Multiple nodes can have the same value
- ? Each node maintains its unique identity via canvas ID
- ? Mapping is stable across saves/loads
- ? No conflicts or lost nodes

### 2. Edge Preservation
- ? All edges are saved, including manually drawn ones
- ? Edges are restored exactly as drawn
- ? Complex tree structures maintain their connectivity
- ? Works for non-standard tree shapes

### 3. Complete State Preservation
- ? Node positions saved
- ? Node values saved (including duplicates)
- ? Edges saved
- ? Everything persists across structure switches
- ? Everything persists across app restarts (via session system)

## Testing Scenarios

### Test Case 1: Duplicate Values
1. Create a tree
2. Add 3 nodes all with value "5"
3. Draw edges between them
4. Switch to another structure
5. Return to tree
6. **Expected**: All 3 nodes with value 5 preserved, edges intact

### Test Case 2: Complex Manual Tree
1. Create tree with structure:
   ```
        5
    / \
      5   13
     / \   \
    11  2   7
         \
         12
   ```
2. Node 5 appears twice (root and left child)
3. Switch away and back
4. **Expected**: Exact structure preserved

### Test Case 3: Edge Modifications
1. Create generated tree
2. Delete an edge (break parent-child link)
3. Draw new edge (create non-standard connection)
4. Switch away and back
5. **Expected**: Modified edges preserved, not regenerated

## Files Modified

### src/visualization/interaction_manager.cpp
- `saveToCurrentStructure()`: 
  - Changed tree node mapping from value-based to canvas ID-based
  - Added custom edge saving for trees
  - Improved logging for debugging

### src/ui/main_window.cpp
- `layoutTreeHierarchically()`:
  - Added custom edge restoration when loading saved positions
  - Falls back to tree structure edges if no custom edges exist

## Technical Notes

### Why Canvas IDs Work Better

**Canvas IDs** (n1, n2, n3...):
- Unique per node
- Stable across operations
- Don't depend on node values
- Don't change when values change

**Tree IDs** (tree_0, tree_1, tree_2...):
- Assigned via BFS traversal
- Stored in `canvasToStructureNodeId` mapping
- Used for persistence
- Independent of values

### Edge Storage Strategy

**Generated Trees** (no custom edges):
- Edges derived from TreeNode left/right pointers
- Regenerated on load
- Follows binary tree structure

**Manual Trees** (with custom edges):
- Edges explicitly saved in `customEdges` vector
- Restored exactly as drawn
- Can violate binary tree rules

### Backward Compatibility

Old trees without custom edges:
- `hasCustomEdges()` returns false
- Falls back to tree structure edges
- No breaking changes

New trees with custom edges:
- Custom edges take precedence
- Tree structure still maintained internally
- Full flexibility for manual construction

## Conclusion

The fix enables binary trees to properly support:
1. **Multiple nodes with identical values** - Essential for many tree algorithms and use cases
2. **Complete edge preservation** - Manual tree structures maintain their exact shape
3. **Robust state management** - Everything persists correctly across all operations

This brings binary trees to feature parity with other structures (graphs, lists) while maintaining their specialized tree algorithms and hierarchical layout capabilities.
