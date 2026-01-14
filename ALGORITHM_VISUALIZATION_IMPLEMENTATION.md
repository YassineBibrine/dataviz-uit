# Algorithm Visualization Implementation Plan

## Problem Analysis

Currently, when users click "Play" to execute an algorithm:
- ? Algorithm executes immediately (synchronous)
- ? Only final result is shown
- ? No step-by-step animation
- ? No visual highlighting of current operations
- ? Playback controls (pause, step, speed) don't work
- ? `FrameRecorder` exists but algorithms don't use it
- ? `PlaybackController` exists but isn't integrated

## Required Changes

### 1. **FrameRecorder Integration**
Algorithms must:
- Create `AnimationFrame` for each step
- Record frames using `FrameRecorder`
- Return frame collection to MainWindow

### 2. **PlaybackController Integration in MainWindow**
- Create `PlaybackController` instance
- Connect it to `VisualizationPane`
- Handle play/pause/step/speed controls
- Display current frame number

### 3. **Algorithm Base Class Enhancement**
Add virtual method:
```cpp
virtual std::vector<AnimationFrame> executeWithFrames() = 0;
```

### 4. **Update Each Algorithm**
Every algorithm needs to:
- Record "before" state
- Record comparison/swap operations
- Record "after" state
- Generate meaningful annotations

### 5. **Visualization Pane Enhancement**
- Accept `AnimationFrame` for rendering
- Highlight nodes/edges based on frame data
- Show annotations on canvas
- Update smoothly between frames

## Implementation Steps

### Phase 1: Core Infrastructure (Priority: HIGH)
1. ? Update `Algorithm` base class with `executeWithFrames()`
2. ? Integrate `PlaybackController` in `MainWindow`
3. ? Connect playback signals to control panel
4. ? Update `VisualizationPane::renderFrame()`

### Phase 2: Algorithm Updates (Priority: HIGH)
5. ? Update `InsertionSort` with frame recording
6. ? Update `SelectionSort` with frame recording
7. ? Update Tree traversal algorithms
8. ? Update Graph algorithms (BFS, DFS)

### Phase 3: UI Enhancements (Priority: MEDIUM)
9. ? Show frame counter in control panel
10. ? Enable/disable controls based on playback state
11. ? Add animation speed slider integration
12. ? Display algorithm annotations on canvas

### Phase 4: Polish (Priority: LOW)
13. ? Add smooth interpolation between frames
14. ? Color-code different operation types
15. ? Add visual effects (fade in/out, pulse)
16. ? Performance metrics display

## Architecture Overview

```
MainWindow
??? DataModelManager (structure data)
??? AlgorithmManager (algorithm selection)
??? PlaybackController (NEW: animation playback)
?   ??? frames: vector<AnimationFrame>
?   ??? currentFrame: int
?   ??? timer: QTimer
??? VisualizationPane (rendering)
?   ??? renderFrame(AnimationFrame) (UPDATED)
??? ControlPanel (playback controls)
    ??? signals ? MainWindow ? PlaybackController
```

## Example: InsertionSort with Frames

```cpp
std::vector<AnimationFrame> InsertionSort::executeWithFrames() {
    FrameRecorder recorder;
    auto& data = arr->getData();
    
    // Frame 0: Initial state
    AnimationFrame initial;
    initial.operationType = "Initial State";
    initial.addAnnotation("Starting Insertion Sort");
    // ... add all nodes ...
 recorder.recordFrame(initial);
    
    for (int i = 1; i < n; i++) {
        // Frame N: Highlight key element
        AnimationFrame selectKey;
        selectKey.operationType = "Select Key";
     selectKey.addHighlightedNode("arr_" + std::to_string(i), "yellow");
        selectKey.addAnnotation("Key = " + std::to_string(data[i]));
        recorder.recordFrame(selectKey);
        
    // Frame N+1: Compare elements
   for (int j = i - 1; j >= 0; j--) {
    AnimationFrame compare;
     compare.operationType = "Compare";
     compare.addHighlightedNode("arr_" + std::to_string(j), "red");
compare.addHighlightedNode("arr_" + std::to_string(j+1), "blue");
     compare.addAnnotation(std::to_string(data[j]) + " vs " + std::to_string(key));
        recorder.recordFrame(compare);
            
 if (data[j] > key) {
             // Frame N+2: Swap
                AnimationFrame swap;
     swap.operationType = "Swap";
          swap.addHighlightedNode("arr_" + std::to_string(j), "green");
     swap.addHighlightedNode("arr_" + std::to_string(j+1), "green");
                recorder.recordFrame(swap);
   
          std::swap(data[j], data[j+1]);
            }
        }
    }
    
    // Final frame: Sorted state
    AnimationFrame final;
    final.operationType = "Sorted";
    final.addAnnotation("Array is now sorted!");
    for (int i = 0; i < n; i++) {
        final.addHighlightedNode("arr_" + std::to_string(i), "green");
    }
    recorder.recordFrame(final);
    
    return recorder.getAllFrames();
}
```

## Key Design Decisions

### 1. **Two-Phase Execution**
- Phase 1: Algorithm runs, generates frames (fast)
- Phase 2: Frames play back with timing (visual)

### 2. **Frame Recording Strategy**
- Record frames during algorithm execution
- Don't modify original algorithm logic significantly
- Add frame recording at key decision points

### 3. **Backward Compatibility**
- Keep `execute()` method for non-animated execution
- Add new `executeWithFrames()` for animated mode
- MainWindow chooses which to call based on context

### 4. **Performance Considerations**
- Generate all frames upfront (not real-time)
- Allows for pause/step/rewind
- Maximum 1000 frames per algorithm (prevent memory issues)

## Testing Strategy

### Unit Tests
- Each algorithm generates correct frame sequence
- Frames contain valid node/edge references
- Annotations are meaningful

### Integration Tests
- PlaybackController cycles through frames correctly
- Speed control affects playback timing
- Pause/resume works without frame loss

### UI Tests
- Nodes highlight correctly during playback
- Annotations display properly
- Frame counter updates accurately
- Controls enable/disable appropriately

## File Modifications Required

### New Files
- None (all infrastructure exists)

### Modified Files
1. `src/orchestration/algorithm.h` - Add `executeWithFrames()`
2. `src/ui/main_window.h` - Add `PlaybackController` member
3. `src/ui/main_window.cpp` - Integrate playback system
4. `src/algorithms/insertion_sort.h` - Add frame recording
5. `src/algorithms/selection_sort.h` - Add frame recording
6. `src/algorithms/tree_inorder.h` - Add frame recording
7. `src/algorithms/graph_algorithms.cpp` - Add frame recording
8. `src/ui/visualization_pane.cpp` - Enhanced frame rendering
9. `src/algorithms/frame_recorder.h` - Add `recordFrame()` method
10. `src/ui/control_panel.cpp` - Update frame counter

## Timeline Estimate

- **Phase 1** (Core Infrastructure): 2-3 hours
- **Phase 2** (Algorithm Updates): 4-5 hours
- **Phase 3** (UI Enhancements): 2-3 hours
- **Phase 4** (Polish): 2-3 hours
- **Testing & Bug Fixes**: 2-3 hours

**Total**: ~15-20 hours of development

## Success Criteria

? User clicks "Play" ? Algorithm animates step-by-step  
? User can pause/resume during execution  
? User can step forward/backward through frames  
? Speed slider controls animation speed  
? Current step is visually highlighted  
? Annotations explain what's happening  
? Frame counter shows current position  
? All 10+ algorithms support animation  

## Next Steps

1. Start with Phase 1: Core infrastructure
2. Implement one example algorithm (InsertionSort) fully
3. Test thoroughly
4. Replicate pattern to other algorithms
5. Polish and optimize

---

**Ready to implement?** Let's start with Phase 1!
