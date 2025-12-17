# ?? Quick Troubleshooting Guide for Teammates

## Your project won't build? Follow this guide step-by-step!

---

## ? Pre-Build Checklist

Before trying to build, verify you have:

- [ ] Visual Studio 2022 installed with C++ development tools
- [ ] Qt 6.x installed (any version 6.5.0 or higher)
- [ ] Noted your Qt installation path (e.g., `C:\Qt\6.10.0\msvc2022_64`)
- [ ] Closed Visual Studio and all terminals (to pick up environment changes)

---

## ?? Quick Fix (Try This First!)

### Method 1: One-Command Setup ? **EASIEST**

```bash
# 1. Run setup script
.\setup_environment.bat

# 2. CLOSE this terminal (important!)

# 3. Open NEW terminal

# 4. Build
.\build.bat
```

---

### Method 2: Manual Environment Setup

**If the setup script doesn't work:**

1. **Set Qt path in current session:**
   ```bash
   set QT_DIR=C:\Qt\6.10.0\msvc2022_64
   ```
   ?? **Replace with YOUR actual Qt path!**

2. **Set license bypass:**
   ```bash
   set QTFRAMEWORK_BYPASS_LICENSE_CHECK=1
   ```

3. **Build with explicit path:**
   ```bash
   cmake -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="%QT_DIR%"
   cmake --build build --config Release
   ```

---

## ?? Diagnostic Commands

Run these to check your setup:

```bash
# Check if Qt exists
dir "C:\Qt\6.10.0\msvc2022_64"

# Check environment variables
echo %QT_DIR%
echo %QTFRAMEWORK_BYPASS_LICENSE_CHECK%

# Check Visual Studio
where cl.exe

# Check CMake
cmake --version
```

---

## ? Common Errors & Fixes

### Error 1: "Qt6 not found"

**Full error:**
```
CMake Error at CMakeLists.txt:XX (find_package):
  Could not find a configuration file for package "Qt6"
```

**Fix Options (try in order):**

**A. Quick fix (current session only):**
```bash
set QT_DIR=C:\Qt\6.10.0\msvc2022_64
cmake -B build -DCMAKE_PREFIX_PATH="%QT_DIR%" -G "Visual Studio 17 2022" -A x64
```

**B. Permanent fix:**
```bash
# Set permanent environment variable
setx QT_DIR "C:\Qt\6.10.0\msvc2022_64"

# CLOSE terminal and open NEW one

# Try build again
.\build.bat
```

**C. Check Qt installation:**
```bash
# Verify qmake exists
dir "C:\Qt\6.10.0\msvc2022_64\bin\qmake.exe"
```

If qmake.exe doesn't exist ? **You need to install Qt!**

---

### Error 2: "Architecture mismatch" (32-bit vs 64-bit)

**Full error:**
```
CMake uses Hostx86/x86 but Qt is 64-bit
```

**Fix:**

**Option A: Use x64 command prompt** ? **BEST**
1. Press Windows key
2. Search: "x64 Native Tools Command Prompt for VS 2022"
3. Open it
4. Navigate to project: `cd path\to\dataviz-uit`
5. Run: `.\build.bat`

**Option B: Force x64 architecture**
```bash
cmake -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="C:/Qt/6.10.0/msvc2022_64"
cmake --build build --config Release
```

---

### Error 3: Build succeeds but app won't run (Missing DLLs)

**Full error:**
```
The code execution cannot proceed because Qt6Core.dll was not found
```

**Fix:**
```bash
cd build\bin\Release
windeployqt dataviz-uit.exe
cd ..\..\..
```

This copies all Qt DLLs to the executable directory.

---

### Error 4: "CMake not found"

**Fix:**
1. Open Visual Studio Installer
2. Modify Visual Studio 2022
3. Check "C++ CMake tools for Windows"
4. Install
5. Restart terminal

---

### Error 5: "Permission denied" when setting environment variables

**Fix:**
1. Right-click Command Prompt
2. "Run as Administrator"
3. Run setup script again:
   ```bash
   .\setup_environment.bat
   ```

---

## ?? Step-by-Step Build Process (Detailed)

If everything else fails, follow this EXACT process:

### Step 1: Find Your Qt Installation

```bash
# Check common locations
dir C:\Qt\6.10.0\msvc2022_64
dir C:\Qt\6.8.0\msvc2022_64
dir C:\Qt\6.7.0\msvc2022_64
```

**Note the path that exists!** Example: `C:\Qt\6.8.0\msvc2022_64`

### Step 2: Open x64 Native Tools Command Prompt

1. Press `Windows` key
2. Type: `x64 Native`
3. Click: "x64 Native Tools Command Prompt for VS 2022"

### Step 3: Navigate to Project

```bash
cd C:\Users\YourName\source\repos\dataviz-uit
```

### Step 4: Set Environment Variables

```bash
# Replace with YOUR Qt path from Step 1
set QT_DIR=C:\Qt\6.8.0\msvc2022_64
set CMAKE_PREFIX_PATH=%QT_DIR%
set QTFRAMEWORK_BYPASS_LICENSE_CHECK=1
```

### Step 5: Clean Previous Build (if exists)

```bash
rmdir /s /q build
```

### Step 6: Configure with CMake

```bash
cmake -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="%QT_DIR%"
```

**Check output!** Should see:
```
-- ? Using Qt from...
-- Qt Version: 6.x.x
```

### Step 7: Build

```bash
cmake --build build --config Release
```

### Step 8: Deploy Qt Dependencies

```bash
cd build\bin\Release
windeployqt dataviz-uit.exe
```

### Step 9: Run

```bash
.\dataviz-uit.exe
```

---

## ?? Clean Start (Nuclear Option)

If nothing works, start completely fresh:

```bash
# 1. Delete build directory
rmdir /s /q build

# 2. Delete CMake cache
del CMakeCache.txt

# 3. Close ALL Visual Studio and terminal windows

# 4. Open NEW x64 Native Tools Command Prompt

# 5. Run setup
.\setup_environment.bat

# 6. CLOSE terminal, open NEW one

# 7. Build
.\build.bat -Clean
```

---

## ?? Contact Team Lead

If you still can't build after trying everything, message the team lead with:

```
1. Your Qt version: [version]
2. Your Qt path: [path]
3. Your Windows version: [version]
4. Visual Studio version: [version]
5. Output of: cmake --version
6. Output of: echo %QT_DIR%
7. Error message screenshot
8. Full CMake output (copy/paste)
```

---

## ? Success Checklist

You've successfully built if you see:

```
[100%] Built target dataviz-uit
? Qt dependencies deployed successfully!
Build completed successfully!
```

And you can run:
```bash
.\build\bin\Release\dataviz-uit.exe
```

---

## ?? Build Successful!

Once building works:
1. Commit your local changes (if any)
2. Don't commit `build/` directory (it's in .gitignore)
3. Help your teammates if they have issues!

---

**Last Updated:** After CMakeLists.txt portability fix
**For more help:** See `CMAKE_PORTABILITY_FIX.md`
