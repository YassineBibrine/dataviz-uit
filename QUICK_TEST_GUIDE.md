# Quick Start: Testing Algorithm Animation

## ?? Quick Test (5 minutes)

### 1. Build
```bash
cd "C:\Users\User\source\repos\YassineBibrine\dataviz-uit"
cmake --build build --config Debug
```

### 2. Run
```bash
.\build\bin\dataviz-uit.exe
```

### 3. Create Test Data
- Click **"Create Sample Structures"** 
- Wait for dialog showing "Successfully created 4 sample structure(s)"

### 4. Test Insertion Sort
1. **Right panel** ? Select "Sample Array" from list
2. **Algorithm** ? Select "InsertionSort"
3. Click **Play** button

### Expected Result
? Array elements change colors as algorithm sorts:
- ?? Blue ? Start
- ?? Yellow ? Current key
- ?? Green ? Sorted portion
- ?? Red ? Comparing
- ?? Orange ? Shifting
- ?? Cyan ? Inserting
- ?? All green ? Complete!

---

## ?? Controls During Animation

| Control | Action |
|---------|--------|
| **Play** | Start animation |
| **Pause** | Pause animation |
| **Reset** | Go back to start |
| **>** Step | Go to next frame |
| **<** Step | Go to previous frame |
| **Speed Slider** | 1 (slow) to 100 (fast) |

---

## ?? Debugging

### Check Console Output
```
InsertionSort generated 47 animation frames
PlaybackController: Loaded 47 frames
PlaybackController: Starting playback with interval 500 ms (speed: 1.0x)
Frame: Select Key - Iteration 1
Frame: Compare - Comparing: arr[0]=5 > key=3 ? YES
...
PlaybackController: Animation complete
```

### Common Issues

**Problem:** No colors showing
- ? Check console for frame generation messages
- ? Verify animation frames were created (should see "generated X frames")
- ? Check PlaybackController logs in console

**Problem:** Colors static (not animating)
- ? Click Play button - timer should start
- ? Check speed slider value (should be >1)
- ? Look for "Starting playback" message in console

**Problem:** Crashes when clicking Play
- ? Make sure structure is selected (highlighted in list)
- ? Make sure algorithm is selected in dropdown
- ? Check Output window for crash details

---

## ?? What You'll See

### Initial State (Frame 0)
```
[5] [3] [8] [1] [6] [2]
 ??  ??  ??  ??  ??  ??
All blue - ready to sort
```

### During Iteration 1 (Frames 1-5)
```
Frame 1: Select Key
[5] [3] [8] [1] [6] [2]
 ??  ??  ?  ?  ?  ?
Selected: 3 (yellow), Sorted: 5 (green)

Frame 2: Compare
[5] [3] [8] [1] [6] [2]
 ??  ??  ?  ?  ?  ?
Comparing: 5 > 3? YES, will shift

Frame 3: Shift
[5] [3] [8] [1] [6] [2]
 ??  ??  ?  ?  ?  ?
Shifting 5 to the right

Frame 4: Insert
[3] [5] [8] [1] [6] [2]
 ??  ??  ?  ?  ?  ?
Inserted 3, now positions 0-1 sorted

Frame 5: Sorted Portion
[3] [5] [8] [1] [6] [2]
 ??  ??  ?  ?  ?  ?
Ready for next iteration
```

### After All Iterations (Final Frame)
```
[1] [2] [3] [5] [6] [8]
 ??  ??  ??  ??  ??  ??
All green - sorted!
```

---

## ?? Specific Test Cases

### Test Case 1: Already Sorted
1. Create array manually with [1,2,3,4,5,6]
2. Run InsertionSort
3. Should show mostly green nodes (minimal work)

### Test Case 2: Reverse Sorted
1. Create array manually with [6,5,4,3,2,1]
2. Run InsertionSort
3. Should show lots of red (comparisons) and orange (shifts)

### Test Case 3: Speed Control
1. Start animation at speed 50 (1x normal)
2. Pause
3. Change to speed 100 (2x fast)
4. Resume - should move faster

### Test Case 4: Step Controls
1. Play animation
2. Pause after 5-10 frames
3. Click **>** (Step Forward) 5 times
4. Click **<** (Step Backward) 5 times
5. Click **Reset** to restart

---

## ?? Information Shown

### On Console (Debug Output)
- Frame count: "InsertionSort generated X frames"
- Playback status: "Starting playback", "Paused", "Animation complete"
- Frame details: "Comparing: arr[0]=5 > key=3 ? YES"

### In Visualization
- **Colors** - Show algorithm state
- **Node labels** - Show array values
- **No overlay text** - Currently, annotations only in console

### In Console Output Window
```
Algorithm selected: InsertionSort
Loaded 47 animation frames
Starting playback with interval 500 ms (speed: 1.0x)
Frame: Select Key - Iteration 1: Selected key = 3
Frame: Compare - Comparing: arr[0]=5 > key=3 ? YES - Shift needed
Frame: Shift Right - Shifting arr[0]=5 to position 1
Frame: Insert - Inserting key=3 at position 0
...
Animation complete
```

---

## ? Success Criteria

You'll know it's working when:

1. ? **Create Sample** - Button works without errors
2. ? **Select Array** - Array appears in visualization with numbers
3. ? **Select Algorithm** - "InsertionSort" available in dropdown
4. ? **Click Play** - Animation starts (colors change)
5. ? **Colors Animate** - See blue?yellow?green?red transitions
6. ? **Speed Slider** - Changing speed affects animation speed
7. ? **Pause Works** - Animation freezes on current frame
8. ? **Step Works** - Can advance 1 frame at a time
9. ? **Reset Works** - Goes back to blue initial state
10. ? **Complete** - Animation ends with all green nodes

---

## ?? Troubleshooting

### Animation doesn't start
- [ ] Did you click **Play** button?
- [ ] Did you select an algorithm from dropdown?
- [ ] Did you select a structure from the list?
- [ ] Check console for error messages

### Colors not showing
- [ ] Right-click canvas ? Check if it updates
- [ ] Try different structure
- [ ] Rebuild project (`cmake --build build --config Debug`)

### Only static colors, no animation
- [ ] Check console - should see "generated X frames"
- [ ] Check console - should see "Starting playback"
- [ ] If not, frames might not be created properly

### App crashes
- [ ] Make sure structure is selected before clicking Play
- [ ] Check Output window (Debug pane) for crash details
- [ ] Try with different sample array

---

## ?? Build & Clean

### Full Rebuild
```bash
cd "C:\Users\User\source\repos\YassineBibrine\dataviz-uit"
rmdir /s /q build  # Remove old build
cmake -B build -G "Visual Studio 17 2022"
cmake --build build --config Debug
```

### Quick Rebuild (after code changes)
```bash
cd "C:\Users\User\source\repos\YassineBibrine\dataviz-uit"
cmake --build build --config Debug
```

### Run Application
```bash
.\build\bin\dataviz-uit.exe
```

---

## ?? Test Checklist

- [ ] App launches without errors
- [ ] Sample structures created successfully
- [ ] Array displays in visualization with correct values
- [ ] InsertionSort selected from algorithm dropdown
- [ ] Play button works (animation starts)
- [ ] Nodes change colors during animation
- [ ] Speed slider affects animation speed
- [ ] Pause button stops animation
- [ ] Step forward button advances 1 frame
- [ ] Step backward button goes back 1 frame
- [ ] Reset button returns to initial state
- [ ] Animation completes with all nodes green
- [ ] Console shows animation frames being generated
- [ ] No crashes or errors during playback

**Once all checkmarks are complete, animation system is working!** ?

