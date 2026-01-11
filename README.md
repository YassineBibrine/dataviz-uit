# DataViz UIT - Data Structures Visualization

A cross-platform Qt-based application for visualizing data structures and algorithms.

## ?? Quick Start

### Prerequisites

| Component | Minimum Version | Recommended |
|-----------|-----------------|-------------|
| **C++ Compiler** | C++17 support | MSVC 2022 / GCC 11+ / Clang 14+ |
| **CMake** | 3.14 | 3.20+ |
| **Qt** | 5.15 or 6.4 | Qt 6.6+ |
| **Graphviz** | Any | Latest (optional) |

### Supported Platforms

- ? Windows 10/11 (MSVC, MinGW)
- ? macOS 11+ (Clang)
- ? Linux (GCC, Clang)

---

## ?? Installation & Build

### Windows

**Option 1: Automated Setup (Recommended)**
```batch
:: Run setup script to auto-detect Qt
setup_environment.bat

:: Build the project
build.bat
```

**Option 2: Manual Build**
```batch
:: Set Qt path (adjust version as needed)
set QT_DIR=C:\Qt\6.8.0\msvc2022_64

:: Create build directory and configure
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..

:: Build
cmake --build . --config Release

:: Run
bin\dataviz-uit.exe
```

### macOS

```bash
# Install dependencies
brew install qt@6 cmake

# Clone and build
git clone https://github.com/YassineBibrine/dataviz-uit.git
cd dataviz-uit

# Build (auto-detects Qt)
chmod +x build.sh
./build.sh

# Or manual
export QT_DIR=$(brew --prefix qt@6)
mkdir build && cd build
cmake ..
cmake --build .
```

### Linux (Ubuntu/Debian)

```bash
# Install dependencies
sudo apt update
sudo apt install qt6-base-dev cmake build-essential

# Optional: Graphviz for advanced layouts
sudo apt install libgraphviz-dev

# Clone and build
git clone https://github.com/YassineBibrine/dataviz-uit.git
cd dataviz-uit
chmod +x build.sh
./build.sh
```

### Linux (Fedora)

```bash
# Install dependencies
sudo dnf install qt6-qtbase-devel cmake gcc-c++

# Optional: Graphviz
sudo dnf install graphviz-devel

# Build
./build.sh
```

---

## ?? Qt Installation Guide

### Windows

1. Download Qt Online Installer: https://www.qt.io/download-qt-installer
2. Select components:
   - ? Qt 6.x.x ? MSVC 2022 64-bit
   - ? Qt 6.x.x ? MinGW 64-bit (alternative)
3. Set environment variable:
   ```batch
   setx QT_DIR "C:\Qt\6.8.0\msvc2022_64"
   ```

### macOS

```bash
# Using Homebrew (recommended)
brew install qt@6

# Set environment variable (add to ~/.zshrc or ~/.bash_profile)
export QT_DIR=$(brew --prefix qt@6)
```

### Linux

```bash
# Ubuntu/Debian - system packages
sudo apt install qt6-base-dev

# Or using Qt Installer for newer versions
# Download from qt.io and set QT_DIR accordingly
export QT_DIR=$HOME/Qt/6.8.0/gcc_64
```

---

## ?? Build Options

### CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `CMAKE_BUILD_TYPE` | Release | Build type (Debug/Release) |
| `CMAKE_PREFIX_PATH` | Auto | Qt installation path |
| `USE_QT5` | OFF | Force Qt5 instead of Qt6 |
| `USE_GRAPHVIZ` | ON | Enable Graphviz support |

### Examples

```bash
# Force Qt5
cmake -B build -DUSE_QT5=ON ..

# Disable Graphviz
cmake -B build -DUSE_GRAPHVIZ=OFF ..

# Specify Qt path explicitly
cmake -B build -DCMAKE_PREFIX_PATH="/path/to/Qt/6.8.0/gcc_64" ..

# Debug build with Ninja
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug ..
```

---

## ??? Troubleshooting

### Qt Not Found

**Error:**
```
CMake Error: Could not find a configuration file for package "Qt6"
```

**Solutions:**

1. **Set QT_DIR environment variable:**
   ```bash
   # Windows
   set QT_DIR=C:\Qt\6.8.0\msvc2022_64
   
   # Linux/macOS
   export QT_DIR=$HOME/Qt/6.8.0/gcc_64
   ```

2. **Pass path to CMake:**
   ```bash
   cmake -B build -DCMAKE_PREFIX_PATH="/path/to/Qt" ..
   ```

3. **Run setup script (Windows):**
   ```batch
   setup_environment.bat
   ```

### Missing Qt DLLs (Windows)

**Error:** Application won't start, missing Qt6Core.dll

**Solution:**
```batch
cd build\bin
windeployqt dataviz-uit.exe
```

The build script does this automatically, but you may need to run it manually.

### Graphviz Not Found

**Warning:** "Graphviz not found - Using fallback layout"

This is **not critical** - the app works without Graphviz using a built-in fallback layout engine.

**To enable Graphviz (optional):**

```bash
# Windows - Install from https://graphviz.org/download/
# Set environment variable:
setx GRAPHVIZ_ROOT "C:\Program Files\Graphviz"

# macOS
brew install graphviz

# Linux
sudo apt install libgraphviz-dev  # Debian/Ubuntu
sudo dnf install graphviz-devel   # Fedora
```

### Architecture Mismatch (Windows)

**Error:** x86/x64 mismatch

**Solution:** Ensure you're using matching architectures:
- Use "x64 Native Tools Command Prompt for VS 2022"
- Install Qt MSVC 64-bit version
- Use `-A x64` flag with CMake

---

## ?? Project Structure

```
dataviz-uit/
??? src/
?   ??? algorithms/         # Sorting, searching algorithms
?   ??? core/       # Data structures (Array, List, Tree, Graph)
?   ??? codegen/    # Code generation & parsing
?   ??? orchestration/      # Algorithm management
?   ??? ui/        # Qt UI components
?   ??? visualization/      # Rendering, animation, layout
??? CMakeLists.txt          # Cross-platform build config
??? build.bat               # Windows build script
??? build.sh   # Linux/macOS build script
??? setup_environment.bat   # Windows environment setup
??? README.md
```

---

## ? Features

- **Data Structures**: Array, Linked List, Binary Tree, Graph
- **Algorithms**: Sorting (Bubble, Quick, Merge), Graph traversal (BFS, DFS)
- **Interactive Visualization**: Step-by-step animation with play/pause
- **Code Generator**: Generate C++ code from structures
- **Code Parser**: Create structures from C++ code
- **Cross-Platform**: Works on Windows, macOS, and Linux

---

## ?? Development

### Build Commands

```bash
# Windows
build.bat  # Release build
build.bat Debug   # Debug build
build.bat clean    # Clean build

# Linux/macOS
./build.sh       # Release build
./build.sh Debug# Debug build
./build.sh clean             # Clean build
```

### IDE Setup

**Visual Studio Code:**
1. Install "CMake Tools" extension
2. Open folder
3. Select kit (compiler)
4. Press F7 to build

**Visual Studio:**
1. File ? Open ? Folder
2. CMake auto-configures
3. Build ? Build All

**Qt Creator:**
1. File ? Open File or Project
2. Select CMakeLists.txt
3. Configure and build

---

## ?? Documentation

| Document | Description |
|----------|-------------|
| `CODE_GENERATOR_GUIDE.md` | Using the code generator |
| `MULTI_STRUCTURE_GUIDE.md` | Managing multiple structures |
| `ARCHITECTURE_DIAGRAM.md` | System architecture |
| `BUILD_FIXES.md` | Build troubleshooting |

---

## ?? Contributing

1. Fork the repository
2. Create feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open Pull Request

### Guidelines

- ? Don't hardcode absolute paths
- ? Test on clean build (`build.bat clean`)
- ? Update documentation for new features
- ? Follow existing code style

---

## ?? License

[Add license information]

## ?? Contributors

[Add contributor information]
