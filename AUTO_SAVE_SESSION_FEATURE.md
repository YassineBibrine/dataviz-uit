# Auto-Save Session Feature

## Overview
The application now supports **optional automatic session saving** instead of mandatory saving with a manual clear option.

## What Changed

### Before:
- ? Session was **always saved** on application exit
- ? Users had to manually click "Clear Session" to reset
- ? No choice in session persistence

### After:
- ? Users can **toggle auto-save** on/off
- ? Preference is **remembered** between sessions
- ? Clear messaging about current state

## How to Use

### Accessing the Feature
1. Open the application
2. Go to **File > Auto-save Session**
3. Check/uncheck to enable/disable

### When Auto-Save is ENABLED (Default):
- ? Session is automatically saved on exit
- ? Previous structures are restored on next launch
- ? Checkmark appears next to "Auto-save Session" in menu

### When Auto-Save is DISABLED:
- ? Session is NOT saved on exit
- ? Application starts fresh each time
- ? No checkmark appears in menu

## Technical Implementation

### Files Modified:

#### 1. `src/ui/main_window.h`
```cpp
// Added new member variables
QAction* autoSaveAction = nullptr;
bool autoSaveEnabled = true;  // Default: enabled
```

#### 2. `src/ui/main_window.cpp`

**Constructor:**
- Loads auto-save preference from `QSettings`
- Conditionally restores session based on preference

**createMenuBar():**
- Replaced "Clear Session" button with "Auto-save Session" toggle
- Saves preference to `QSettings` when changed
- Shows informative message on toggle

**closeEvent():**
- Only saves session if `autoSaveEnabled` is true
- Logs action for debugging

### Settings Storage
- Setting key: `"autoSaveSession"`
- Storage: `QSettings` (persistent across sessions)
- Default value: `true` (enabled)

## User Benefits

1. **Control**: Users decide if they want session persistence
2. **Flexibility**: Perfect for both:
   - Regular users who want continuity
   - Testing/demo users who want fresh starts
3. **Transparency**: Clear indication of current setting
4. **Persistence**: Preference saved between sessions

## Example Scenarios

### Scenario 1: Developer Testing
```
1. Disable auto-save (File > Auto-save Session)
2. Create test structures
3. Close app
4. Reopen ? Fresh start, no old structures
```

### Scenario 2: Regular Use
```
1. Keep auto-save enabled (default)
2. Create complex structures
3. Close app
4. Reopen ? All structures restored automatically
```

## Debug Messages

When closing the application:
```
// Auto-save enabled:
"Session saved on exit (auto-save enabled)"

// Auto-save disabled:
"Session NOT saved (auto-save disabled)"
```

## Future Enhancements

Potential improvements:
- Manual "Save Session Now" button
- Session export/import
- Multiple named sessions
- Session backup on overwrite

## Testing Checklist

- [x] Toggle auto-save on/off
- [x] Verify preference persists after restart
- [x] Verify session saves when enabled
- [x] Verify session doesn't save when disabled
- [x] Check informative messages appear
- [x] Confirm build compiles successfully
