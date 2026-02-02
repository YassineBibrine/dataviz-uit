# Binary Tree State Preservation Fix

## Problem Description
When adding nodes to a binary tree manually and then switching to another structure, the tree state (nodes, edges, and positions) was not being preserved. Upon returning to the tree, it would either:
1. Regenerate with a different layout
2. Lose manually added nodes
3. Reset positions to the initial generated state

## Root Cause
The issue was caused by two problems:

### 1. No Save Before Switch
When selecting a different structure, the current structure's canvas state was not being saved. The `onStructureSelected` function would immediately switch structures without persisting the current state.

### 2. Trees Always Ignoring Saved Positions
In `loadStructureIntoCanvas`, there was code that explicitly prevented trees from using saved positions:

```cpp
bool useSavedPositions = false;
if (structureType != "Tree" && structureType != "Binary Tree" && structureType != "BinaryTree") {
    useSavedPositions = structure->hasAnyPositions();
}
```

This meant trees would ALWAYS recalculate their layout even when positions were already saved.

## Solution

### Fix 1: Save Before Switch (src/ui/main_window.cpp)
Modified `onStructureSelected` to save the current structure state before switching:

```cpp
void MainWindow::onStructureSelected(QString structureId) {
    std::string id = structureId.toStdString();

    if (dataModelManager) {
     // **FIX**: Save current structure state before switching
    std::string previousId = dataModelManager->getSelectedStructureId();
        if (!previousId.empty() && previousId != id) {
            // Save the current canvas state to the previous structure
    auto* interactionMgr = visualizationPane->getInteractionManager();
 if (interactionMgr && interactionMgr->isSyncEnabled()) {
        qDebug() << "Saving state of previous structure:" << QString::fromStdString(previousId);
      interactionMgr->saveToCurrentStructure();
        interactionMgr->saveNodePositionsToStructure();
     }
        }

        dataModelManager->selectStructure(id);
        // ... rest of the function
    }
}
```

### Fix 2: Allow Trees to Use Saved Positions (src/ui/main_window.cpp)
Modified the position usage logic to check for saved positions for ALL structure types, including trees:

```cpp
// For trees, check if positions exist - if yes, use them!
bool useSavedPositions = structure->hasAnyPositions();
```

## How It Works

### Workflow:
1. **User adds nodes to Tree A manually**
   - User creates nodes, draws edges, positions them
 - InteractionManager syncs changes to backend (saveToCurrentStructure)
   - Node positions saved to TreeStructure (saveNodePositionsToStructure)

2. **User switches to Structure B**
   - `onStructureSelected` is called with Structure B's ID
   - **BEFORE** switching:
 - Gets previous structure ID
     - Saves Tree A's canvas state (nodes, edges, positions)
   - **AFTER** saving:
     - Selects Structure B
     - Loads Structure B into canvas

3. **User returns to Tree A**
   - `onStructureSelected` saves Structure B's state
   - Loads Tree A from backend
   - `layoutTreeHierarchically` checks `structure->hasAnyPositions()`
   - **Returns early** with saved positions instead of recalculating
   - Tree A appears exactly as user left it

### Key Components:

#### InteractionManager (src/visualization/interaction_manager.cpp)
- `saveToCurrentStructure()`: Syncs canvas nodes/edges to backend structure
- `saveNodePositionsToStructure()`: Saves node positions for persistence
- Already had proper tree handling, just needed to be called at the right time

#### TreeStructure (src/core/tree_structure.h/cpp)
- Inherits `nodePositions` map from `DataStructure`
- `hasAnyPositions()`: Checks if positions are saved
- `getNodePosition()`: Retrieves saved position for a node
- `setNodePosition()`: Saves position for a node

#### layoutTreeHierarchically (src/ui/main_window.cpp)
- First checks `hasSavedPositions`
- If positions exist, uses them directly
- If not, calculates new hierarchical layout
- Saves calculated positions for future use

## Testing

### Test Case 1: Manual Tree Editing
1. Generate a binary tree
2. Add new nodes manually using toolbox
3. Draw edges between nodes
4. Move nodes to custom positions
5. Switch to another structure
6. Switch back to tree
7. **Expected**: Tree appears with all manually added nodes and positions intact

### Test Case 2: Generated Tree Preservation
1. Generate a binary tree (auto-layout)
2. Move some nodes to different positions
3. Switch to another structure
4. Switch back to tree
5. **Expected**: Tree appears with modified positions, not original generated layout

### Test Case 3: Multiple Switches
1. Create Tree A, add nodes, position them
2. Switch to Array B, modify it
3. Switch to Tree A - verify state preserved
4. Switch back to Array B - verify state preserved
5. Switch to Tree A again - verify still preserved

## Files Modified

### src/ui/main_window.cpp
- `onStructureSelected()`: Added state saving before switch
- `loadStructureIntoCanvas()`: Allow trees to use saved positions

## Benefits

1. **User Experience**: Manual edits are preserved across structure switches
2. **Data Integrity**: No loss of user work when navigating between structures
3. **Consistency**: Trees behave like other structures (arrays, lists, graphs)
4. **Session Persistence**: Saved positions persist across application restarts (via existing session system)

## Technical Notes

### Why Trees Were Special
Trees were initially treated differently because:
- Generated trees use hierarchical layout algorithm
- We wanted newly generated trees to have proper layout
- Concern about mixing manual and generated layouts

### How We Balanced This
- First-time generated trees still get hierarchical layout
- Once positions are saved (either from generation or manual editing), they're preserved
- User can always regenerate a tree if they want the default layout back

### Node ID Mapping
The `canvasToStructureNodeId` map in InteractionManager ensures that:
- Canvas node IDs (n1, n2, n3...) map to structure node IDs (tree_0, tree_1, tree_2...)
- Positions are saved using structure IDs
- When reloading, positions are retrieved using structure IDs
- This mapping survives structure switches and app restarts

## Conclusion
The fix ensures that binary trees maintain their complete state (nodes, edges, positions) when switching between structures, providing a consistent and predictable user experience. All manual edits are now preserved, making the application more reliable for complex tree construction and manipulation.
