# DataViz UIT - Data Structures Visualization

A Qt-based application for visualizing data structures and algorithms.

## ?? Quick Start for New Team Members

### Prerequisites

1. **Visual Studio 2022** with C++ development tools
2. **Qt 6.x** (6.5.0 or higher) - MSVC 2022 64-bit build
3. **CMake 3.16+** (usually included with Visual Studio)
4. **Graphviz** (optional, for advanced graph layouts)

---

## ?? Setup Instructions

### Step 1: Install Qt

1. Download Qt Online Installer: https://www.qt.io/download-qt-installer
2. During installation, select:
   - ? Qt 6.x.x ? MSVC 2022 64-bit
   - ? Qt 6.x.x ? Qt Debug Information Files (optional)
   - ? Developer and Designer Tools ? CMake (if not installed)
3. **Remember your installation path** (e.g., `C:\Qt\6.10.0\msvc2022_64`)

### Step 2: Configure Environment

**Option A: Automated Setup (Recommended)** ?
```bash
# Run the setup script
.\setup_environment.bat
```
Follow the prompts to configure Qt and Graphviz paths.

**Option B: Manual Configuration**

Set environment variables (Windows ? System ? Environment Variables ? System variables):

| Variable | Value | Required |
|----------|-------|----------|
| `QT_DIR` | `C:\Qt\6.10.0\msvc2022_64` | ? Yes |
| `GRAPHVIZ_ROOT` | `C:\Program Files\Graphviz` | ?? Optional |
| `QTFRAMEWORK_BYPASS_LICENSE_CHECK` | `1` | ? Yes |

**?? IMPORTANT:** Restart your terminal/IDE after setting environment variables!

### Step 3: Clone Repository

```bash
git clone https://github.com/YassineBibrine/dataviz-uit.git
cd dataviz-uit
```

### Step 4: Build

**Easy Build (Recommended):**
```bash
.\build.bat
```

**With Options:**
```bash
# Clean build
.\build.bat -Clean

# Build and run
.\build.bat -Run

# Debug build
.\build.bat -Config Debug
```

**Manual Build:**
```bash
# Configure
cmake -B build -G "Visual Studio 17 2022" -A x64

# Build
cmake --build build --config Release

# Deploy Qt DLLs
cd build\bin\Release
windeployqt dataviz-uit.exe

# Run
.\dataviz-uit.exe
```

---

## ?? Troubleshooting

### ? Error: "Qt6 not found"

**Symptoms:**
```
CMake Error at CMakeLists.txt:XX (find_package):
  Could not find a configuration file for package "Qt6"
```

**Solutions:**

1. **Check Qt installation:**
   ```bash
   # Verify Qt directory exists
   dir "C:\Qt\6.10.0\msvc2022_64"
   ```

2. **Set QT_DIR environment variable:**
   ```bash
   # In command prompt:
   set QT_DIR=C:\Qt\6.10.0\msvc2022_64
   
   # Or permanently:
   setx QT_DIR "C:\Qt\6.10.0\msvc2022_64"
   ```

3. **Pass path to CMake directly:**
   ```bash
   cmake -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.10.0/msvc2022_64" -G "Visual Studio 17 2022" -A x64
   ```

4. **Re-run setup script:**
   ```bash
   .\setup_environment.bat
   ```

### ? Error: "Architecture mismatch" (32-bit vs 64-bit)

**Symptoms:**
```
CMake uses Hostx86/x86 but Qt is 64-bit
```

**Solutions:**

1. **Use Visual Studio x64 tools:**
   - Open "x64 Native Tools Command Prompt for VS 2022"
   - Navigate to project directory
   - Run build commands

2. **Specify architecture in CMake:**
   ```bash
   cmake -B build -G "Visual Studio 17 2022" -A x64 ..
   ```

3. **Check Qt version:**
   - Ensure you installed `msvc2022_64` (not `msvc2022`)

### ? Error: App builds but won't run (missing DLLs)

**Symptoms:**
```
The code execution cannot proceed because Qt6Core.dll was not found
```

**Solution:**
```bash
cd build\bin\Release
windeployqt dataviz-uit.exe
```

This copies all required Qt DLLs to the executable directory.

### ? Warning: "Graphviz not found"

**Not Critical:** The application will work with fallback graph layout.

**To fix (optional):**
1. Install Graphviz: https://graphviz.org/download/
2. Set environment variable:
   ```bash
   setx GRAPHVIZ_ROOT "C:\Program Files\Graphviz"
   ```
3. Rebuild project

### ?? Qt License Warning

If you see license warnings, ensure this environment variable is set:
```bash
setx QTFRAMEWORK_BYPASS_LICENSE_CHECK "1"
```

---

## ?? Project Structure

```
dataviz-uit/
??? src/
?   ??? algorithms/ # Algorithm implementations
?   ??? core/    # Core data structures
?   ??? codegen/# Code generation & parsing
?   ??? orchestration/ # Algorithm management
?   ??? ui/ # User interface
?   ??? visualization/       # Rendering and animation
? ??? main.cpp
??? CMakeLists.txt      # Build configuration
??? setup_environment.bat    # Environment setup script ?
??? build.bat           # Build wrapper
??? README.md   # This file
```

---

## ?? Features

- **Data Structure Visualization**: Array, Linked List, Binary Tree, Graph
- **Algorithm Animation**: Sorting, Searching, Traversal algorithms
- **Code Generator**: Generate C++ code from visualized structures
- **Code Parser**: Create structures from C++ code
- **Interactive Controls**: Play, Pause, Step Forward/Backward
- **Real-time Metrics**: Performance statistics
- **Customizable Speed**: Adjust animation playback

---

## ?? Usage

1. **Select Data Structure**: Choose from the Structure Selector
2. **Configure**: Set size and parameters
3. **Select Algorithm**: Pick an algorithm to visualize
4. **Control Playback**: Use play/pause/step controls
5. **Generate Code**: Tools ? Code Generator & Parser

---

## ??? Development

**Built with:**
- Language: C++17
- GUI Framework: Qt 6.10.0
- Build System: CMake 3.16+
- Compiler: MSVC 2022 (Visual Studio 17)
- Graphics: Graphviz (optional)

**Build Requirements:**
- CMake minimum: 3.16
- C++ Standard: C++17
- Qt minimum: 6.5.0 (6.10.0 recommended)

---

## ?? Additional Documentation

- **Code Generator Guide**: `CODE_GENERATOR_GUIDE.md`
- **Multi-Structure Guide**: `MULTI_STRUCTURE_GUIDE.md`
- **Build Fixes**: `BUILD_FIXES.md`
- **CMake Portability**: `CMAKE_PORTABILITY_FIX.md`

---

## ?? Contributing

When making changes that affect the build:
1. ? Never hardcode absolute paths in CMakeLists.txt
2. ? Use environment variables for external dependencies
3. ? Test on a clean machine before committing
4. ? Update documentation
5. ? Run `.\build.bat -Clean` to verify clean builds

---

## ?? Common Build Commands

```bash
# Quick build
.\build.bat

# Clean + build
.\build.bat -Clean

# Debug build
.\build.bat -Config Debug

# Build and run
.\build.bat -Run

# Manual CMake configure
cmake -B build -G "Visual Studio 17 2022" -A x64

# Manual CMake build
cmake --build build --config Release

# Run tests (if available)
ctest --test-dir build -C Release
```

---

## ?? Still Having Issues?

1. **Check your Qt version matches requirements**
2. **Verify environment variables are set** (run `echo %QT_DIR%`)
3. **Try clean build** (`.\build.bat -Clean`)
4. **Check CMake output** for specific error messages
5. **Open an issue** on GitHub with:
   - CMake output
   - Your Qt version and installation path
   - Windows version
   - Visual Studio version

---

## ?? License

[Add your license information here]

## ?? Contributors

[Add contributor information here]
