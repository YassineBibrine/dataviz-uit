# Canvas Clearing on Structure Selection - Fix

## Problem
When modifying mock structures (interactive nodes/edges drawn on canvas), clicking on a different structure in the Structure Selector automatically clears the canvas, losing all the work.

## Root Cause
The `onStructureSelected()` method in `MainWindow` always calls `updateVisualizationForStructure()`, which creates a new `AnimationFrame` and renders it, completely replacing whatever was on the canvas - including any interactive mock structures being worked on.

**Problem Flow:**
```
User drawing nodes on canvas
  ?
User clicks another structure in list
  ?
onStructureSelected() called
  ?
updateVisualizationForStructure() called
  ?
Creates new frame from stored structure
  ?
Renders new frame
  ?
? Canvas cleared! Interactive work lost!
```

## Solution
Add a check to preserve interactive work in progress. Only update the visualization to show a stored structure if there's NO active interactive work on the canvas.

### Code Changes

**File: `src/ui/main_window.cpp`**

**Method: `onStructureSelected()`**

```cpp
void MainWindow::onStructureSelected(QString structureId) {
    std::string id = structureId.toStdString();

    if (dataModelManager) {
        dataModelManager->selectStructure(id);

        auto structures = dataModelManager->getAllStructures();
        for (const auto& meta : structures) {
       if (meta.id == id) {
    if (toolboxPanel) {
         QString structureType = QString::fromStdString(meta.type);
             toolboxPanel->updateTools(structureType);
        qDebug() << "Toolbox updated for structure type:" << structureType;
                }
          
         // ? NEW: Only update visualization if there's no active interactive work
           bool hasInteractiveWork = false;
       if (visualizationPane && visualizationPane->getInteractionManager()) {
hasInteractiveWork = visualizationPane->getInteractionManager()->hasInteractiveData();
        }
              
          // If user is actively working on interactive nodes/edges, DON'T replace them
   // Only show stored structure visualization if there's no active work
       if (!hasInteractiveWork) {
      updateVisualizationForStructure(id);
        } else {
     qDebug() << "Structure selected but preserving interactive work in canvas";
    }
          
             break;
            }
  }
    qDebug() << "Structure selected:" << structureId;
    }
}
```

## How It Works

### Check for Interactive Data
```cpp
bool hasInteractiveWork = visualizationPane->getInteractionManager()->hasInteractiveData();
```

The `hasInteractiveData()` method returns `true` if there are any nodes in the `InteractionManager`:
```cpp
// In InteractionManager
bool hasInteractiveData() const { 
    return !nodes.empty(); 
}
```

### Conditional Visualization Update
```cpp
if (!hasInteractiveWork) {
    updateVisualizationForStructure(id);  // Show stored structure
} else {
    // Keep interactive work visible
    qDebug() << "Preserving interactive work";
}
```

## New Workflow

### Scenario 1: No Interactive Work
```
User clicks structure in list
  ?
onStructureSelected() called
?
Check: hasInteractiveData() ? false
  ?
updateVisualizationForStructure() ? ? Shows stored structure
```

### Scenario 2: Interactive Work in Progress
```
User drawing nodes/edges on canvas
  ?
User clicks different structure in list
  ?
onStructureSelected() called
  ?
Check: hasInteractiveData() ? true
  ?
Skip updateVisualizationForStructure()
  ?
? Canvas preserved! Work not lost!
  ?
Structure still selected in backend
Toolbox still updates
User can continue working or finalize
```

## User Benefits

### Before Fix ?
1. User draws 5 nodes and connects them
2. User accidentally clicks on another structure
3. **Canvas clears - all work lost!**
4. User has to redraw everything

### After Fix ?
1. User draws 5 nodes and connects them
2. User accidentally clicks on another structure
3. **Canvas stays intact - work preserved!**
4. User can continue working
5. When ready, user can:
   - Click "Create Structure" to finalize
   - OR click "Clear Canvas" to start fresh
   - OR view stored structures (manual action required)

## Additional Considerations

### When to Show Stored Structures?

**Option 1: Manual Button** (Recommended)
Add a "View Structure" button that explicitly shows the selected structure:
```cpp
void MainWindow::on ViewStructureClicked() {
    // Always show, even if it clears interactive work
    if (!dataModelManager->getSelectedStructureId().empty()) {
        // Warn user if they have unsaved work
        if (hasInteractiveWork) {
    auto reply = QMessageBox::question(...);
            if (reply == Yes) {
         clearInteractive();
    updateVisualizationForStructure(...);
            }
   } else {
            updateVisualizationForStructure(...);
    }
    }
}
```

**Option 2: Clear Canvas First**
User must explicitly clear canvas before viewing stored structures:
1. User clicks "Clear Canvas" button
2. Interactive data cleared
3. Now clicking structures will show them

**Option 3: Status Indicator**
Show a warning when structure is selected but not displayed:
```cpp
if (hasInteractiveWork) {
    statusBar()->showMessage("Structure selected but not displayed (interactive work in progress)", 3000);
}
```

## Testing

### Test Case 1: Drawing Preserved
```
1. Draw 3 nodes on canvas
2. Connect them with edges
3. Click on "Sample Array" in structure list
4. ? Nodes still visible
5. ? Edges still visible
6. ? Can continue modifying
```

### Test Case 2: Empty Canvas Shows Structure
```
1. Canvas is empty
2. Click on "Sample Array" in structure list
3. ? Array structure displayed
4. ? Shows [5][12][8][3][15][10]
```

### Test Case 3: After Finalization
```
1. Draw nodes and finalize as "My Graph"
2. Canvas clears (because finalization completes the work)
3. Click on "My Graph" in list
4. ? Shows the finalized graph
```

### Test Case 4: Switching Back
```
1. Draw some nodes
2. Click different structure (nodes preserved)
3. Decide to view the stored structure
4. Click "Clear Canvas" button
5. Click same structure again
6. ? Now shows the stored structure
```

## Implementation Notes

### Why This Approach?
- **Safe**: Never loses user work
- **Simple**: One boolean check
- **Intuitive**: Users maintain control
- **Flexible**: Can be extended with warnings/confirmations

### Alternative Approaches Considered

**Approach A: Always Replace**
- ? Loses user work
- ? Frustrating UX

**Approach B: Always Preserve**
- ? Can never view stored structures while drawing
- ? Confusing - structure selected but not shown

**Approach C: Ask Every Time** (Current - best balance)
- ? Preserves work automatically
- ? Clear intent - user controls when to view
- ?? Might need explicit "View" button for discoverability

## Future Enhancements

### 1. Tabbed Canvas
```
[Interactive Drawing] [Structure: Array 1] [Structure: My Graph]
```
- Switch between workspaces
- Never lose work
- Multiple structures visible

### 2. History/Undo
```
Ctrl+Z ? Restore cleared canvas
```

### 3. Auto-Save Interactive Work
```
Background save of interactive nodes
Recover if accidentally cleared
```

### 4. Visual Indicator
```
[Canvas: Interactive Mode] ??
[Canvas: Viewing "Sample Array"] ???
```

## Files Modified

- `src/ui/main_window.cpp` - `onStructureSelected()` method
  - Added `hasInteractiveData()` check
  - Conditional visualization update
  - Debug logging for clarity

## Build & Test

```bash
# Build
.\build.bat

# Test
# 1. Run application
# 2. Draw some nodes
# 3. Click different structures
# 4. Verify nodes are preserved
```

## Documentation

- This file: `CANVAS_CLEARING_FIX.md`
- Related: `STRUCTURE_DESIGN_FIX.md`
- Related: `STRUCTURE_VISUALIZATION_FIX.md`

---

**Status:** ? Fix Implemented
**Priority:** High (prevents data loss)
**User Impact:** Positive - prevents frustration

The canvas now intelligently preserves interactive work while still allowing structure selection and toolbox updates!
