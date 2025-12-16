# DataViz UIT - Data Structures Visualization

A Qt-based application for visualizing data structures and algorithms.

## Prerequisites

- Visual Studio 2022 (with C++ development tools)
- CMake 3.16 or higher
- Qt 6.10.0 (or compatible version) installed at `C:/Qt/`
  - MSVC 2022 64-bit build
- Graphviz (optional, for advanced graph layouts)

## Building the Project

### Quick Build

Use the provided build script:

```bash
# Clean build
.\build.bat -Clean

# Build only
.\build.bat

# Build and run
.\build.bat -Run

# Debug build
.\build.bat -Config Debug
```

### Manual Build

1. **Configure with CMake:**
   ```bash
   mkdir build
   cd build
   set QTFRAMEWORK_BYPASS_LICENSE_CHECK=1
   set CMAKE_PREFIX_PATH=C:/Qt/6.10.0/msvc2022_64
   cmake -G "Visual Studio 17 2022" -A x64 ..
   ```

2. **Build:**
 ```bash
   cmake --build . --config Release
   ```

3. **Deploy Qt Dependencies:**
   ```bash
   cd bin\Release
   windeployqt dataviz-uit.exe
 ```

4. **Run:**
   ```bash
   .\dataviz-uit.exe
   ```

## Project Structure

```
dataviz-uit/
??? src/
?   ??? algorithms/          # Algorithm implementations
?   ?   ??? frame_recorder.h/cpp
?   ?   ??? algorithm_runner.h/cpp
?   ?   ??? execution_state.h
?   ??? core/ # Core data structures
?   ?   ??? data_model_manager.h/cpp
?   ?   ??? data_structure.h
?   ?   ??? array_structure.h/cpp
?   ?   ??? list_structure.h/cpp
?   ?   ??? tree_structure.h/cpp
?   ?   ??? graph_structure.h/cpp
?   ??? orchestration/       # Algorithm management
?   ?   ??? algorithm_manager.h/cpp
?   ?   ??? sorting_algorithm_factory.cpp
?   ?   ??? ...
?   ??? ui/          # User interface
?   ?   ??? main_window.h/cpp
?   ?   ??? control_panel.h/cpp
?   ?   ??? visualization_pane.h/cpp
?   ?   ??? metrics_panel.h/cpp
?   ?   ??? toolbox_panel.h/cpp
? ??? visualization/       # Rendering and animation
?   ?   ??? visualization_renderer.h
?   ?   ??? animation_frame.h/cpp
?   ?   ??? playback_controller.h
?   ?   ??? GraphvizLayoutEngine.cpp
?   ??? main.cpp
??? CMakeLists.txt
??? build.ps1      # PowerShell build script
??? build.bat         # Batch file wrapper

```

## Common Issues and Solutions

### Qt Not Found

**Error:** `Could not find a configuration file for package "Qt6"`

**Solution:** 
- Make sure Qt is installed at `C:/Qt/6.10.0/msvc2022_64`
- Or set `CMAKE_PREFIX_PATH` to your Qt installation path
- The CMakeLists.txt has been configured to automatically detect common Qt paths

### 32-bit vs 64-bit Architecture Mismatch

**Error:** CMake uses 32-bit compiler but Qt is 64-bit

**Solution:** 
- Use Visual Studio generator with `-A x64` flag
- Or open x64 Native Tools Command Prompt for VS 2022

### Qt License Warning

**Error:** `Could not initialize license client`

**Solution:** 
- Set environment variable: `set QTFRAMEWORK_BYPASS_LICENSE_CHECK=1`
- This is automatically set by the build script

### Missing Graphviz

**Warning:** `Graphviz headers not found`

**Solution:**
- Optional: Install Graphviz from https://graphviz.org/download/
- The application will use fallback layout if Graphviz is not available

## Features

- **Data Structure Visualization**: Array, Linked List, Binary Tree, Graph
- **Algorithm Animation**: Sorting, Searching, Traversal algorithms
- **Interactive Controls**: Play, Pause, Step Forward/Backward
- **Real-time Metrics**: Performance statistics and algorithm details
- **Customizable Speed**: Adjust animation playback speed

## Usage

1. **Select Data Structure**: Choose from the control panel
2. **Configure Size**: Set the initial data size
3. **Select Algorithm**: Pick an algorithm to visualize
4. **Control Playback**: Use play/pause/step controls
5. **Interact**: Use toolbox to manipulate the visualization

## Development

Built with:
- **Language**: C++17
- **GUI Framework**: Qt 6.10.0
- **Build System**: CMake 3.16+
- **Compiler**: MSVC 2022 (Visual Studio 17)
- **Graphics**: Graphviz (optional)

## License

[Add your license information here]

## Contributors

[Add contributor information here]
