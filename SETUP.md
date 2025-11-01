# Setup and Installation Guide

## System Requirements

### Windows
- Windows 10 or later
- Visual Studio 2019 or later (C++17 support)
- CMake 3.21+
- Git

### macOS
- macOS 10.15 or later
- Xcode 11 or later
- CMake 3.21+
- Git

### Linux (Ubuntu/Debian)
- Ubuntu 18.04 LTS or later
- GCC 7+ or Clang 8+
- CMake 3.21+
- Git

## Step 1: Install Build Tools

### Windows
```powershell
# Install Visual Studio Community (with C++ workload)
# https://visualstudio.microsoft.com/downloads/

# Install CMake
winget install CMake.CMake

# Install Git
winget install Git.Git
```

### macOS
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install CMake and Git
brew install cmake git
```

### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake git
```

## Step 2: Install Dependencies

### Qt 6 Installation

#### Windows
```bash
# Download Qt online installer from https://www.qt.io/download
# Run installer and select:
# - Qt 6.x (latest LTS version)
# - MSVC compiler version matching your Visual Studio
# - Add to PATH during installation
```

#### macOS
```bash
brew install qt@6
```

#### Linux
```bash
sudo apt-get install -y qt6-base-dev qt6-tools-dev qt6-qmake
```

### LLVM/Clang Installation

#### Windows
```bash
# Download from https://github.com/llvm/llvm-project/releases
# Extract to C:\llvm (or preferred location)
# Remember installation path for CMake configuration

# OR use MSYS2/vcpkg
vcpkg install llvm:x64-windows
```

#### macOS
```bash
brew install llvm
# LLVM will be installed to /usr/local/opt/llvm or similar
# Check with: brew --prefix llvm
```

#### Linux
```bash
# Option 1: System package (may be older version)
sudo apt-get install -y llvm-dev clang-dev libclang-dev

# Option 2: Pre-built binaries from llvm.org
wget https://github.com/llvm/llvm-project/releases/download/llvmorg-14.0.0/clang+llvm-14.0.0-x86_64-linux-gnu-ubuntu-18.04.tar.xz
tar -xf clang+llvm-14.0.0-x86_64-linux-gnu-ubuntu-18.04.tar.xz
# Remember extraction path
```

### Boost Installation

#### Windows
```bash
# Download from https://www.boost.org/
# Extract to C:\boost (or preferred location)

# OR use vcpkg
vcpkg install boost:x64-windows
```

#### macOS
```bash
brew install boost
```

#### Linux
```bash
# Option 1: System package
sudo apt-get install -y libboost-all-dev

# Option 2: Build from source
wget https://boostorg.jfrog.io/artifactory/main/release/1.80.0/source/boost_1_80_0.tar.gz
tar -xzf boost_1_80_0.tar.gz
cd boost_1_80_0
./bootstrap.sh
./b2 install --prefix=/usr/local
```

### Graphviz Installation

#### Windows
```bash
# Download from https://graphviz.org/download/
# Run installer
# Add to PATH
```

#### macOS
```bash
brew install graphviz
```

#### Linux
```bash
sudo apt-get install -y graphviz graphviz-dev
```

## Step 3: Clone the Repository

```bash
cd C:\path\to\projects  # or ~/projects on Unix
git clone https://github.com/yourusername/dataviz-uit.git
cd dataviz-uit
```

## Step 4: Configure CMake

### Windows (Visual Studio)

```bash
# Create build directory
mkdir build
cd build

# Configure with explicit paths
cmake -G "Visual Studio 16 2019" `
  -DCMAKE_PREFIX_PATH="C:\Qt\6.x\msvc2019_64;C:\llvm;C:\boost" `
  -DLLVM_DIR="C:\llvm\cmake" `
  -DQt6_DIR="C:\Qt\6.x\msvc2019_64\lib\cmake\Qt6" `
  ..

# Build
cmake --build . --config Release
```

### macOS

```bash
# Create build directory
mkdir build
cd build

# Configure
LLVM_CONFIG=$(brew --prefix llvm)/bin/llvm-config
cmake -DCMAKE_PREFIX_PATH="$(brew --prefix qt@6);$(brew --prefix llvm)" \
  -DLLVM_DIR="$(brew --prefix llvm)/lib/cmake/llvm" \
      ..

# Build
cmake --build . --config Release
```

### Linux

```bash
# Create build directory
mkdir build
cd build

# Configure
cmake -DCMAKE_PREFIX_PATH="/usr/local/lib/cmake/llvm;/usr/lib/x86_64-linux-gnu/cmake/Qt6" \
      -DLLVM_DIR="/usr/lib/llvm-14/cmake" \
 ..

# Build
cmake --build . --config Release -j$(nproc)
```

## Step 5: Verify Installation

```bash
# From the build directory
./dataviz-app  # Windows: dataviz-app.exe

# If successful, the application GUI should open
```

## Environment Setup

### Visual Studio Code

Create `.vscode/settings.json`:
```json
{
    "cmake.configureOnOpen": true,
    "cmake.configureSettings": {
        "CMAKE_PREFIX_PATH": "/path/to/qt;/path/to/llvm",
        "LLVM_DIR": "/path/to/llvm/cmake"
    },
    "C_Cpp.codeAnalysis.enabled": true,
    "C_Cpp.clang_format.fallbackStyle": "LLVM"
}
```

### Visual Studio

1. Open Visual Studio
2. File ? Open ? Folder ? Select `dataviz-uit` directory
3. CMake will auto-configure
4. Select active configuration (Debug/Release)
5. Build ? Build All

### CLion

1. Open project directory
2. Go to Settings ? Build, Execution, Deployment ? CMake
3. Set CMake options:
   ```
   -DCMAKE_PREFIX_PATH=/path/to/qt;/path/to/llvm
   -DLLVM_DIR=/path/to/llvm/cmake
   ```
4. Click "Reload CMake Project"

## Troubleshooting

### Common CMake Errors

**Error**: `Could not find Qt6 package`
```bash
# Solution: Check Qt installation path
find /opt /usr/local -name "Qt6Config.cmake" 2>/dev/null
# Add correct path to CMAKE_PREFIX_PATH
```

**Error**: `LLVM_DIR not found`
```bash
# Solution: Locate LLVM cmake files
find /usr/local /opt -path "*llvm*cmake" 2>/dev/null
# Set LLVM_DIR in CMake configuration
```

**Error**: `Boost libraries not found`
```bash
# Solution: Install Boost development files
sudo apt-get install libboost-all-dev  # Linux
brew install boost  # macOS
```

### Build Issues

**Issue**: Long build time on first configure
- This is normal, wait for CMake to complete
- Subsequent builds will be faster

**Issue**: Out of memory during build
- Use `-j1` flag: `cmake --build . -j1`
- Or set fewer parallel jobs: `-j2`

**Issue**: Linking errors with Qt
- Ensure Qt version matches CMake configuration
- Check that `QT_PATH` environment variable is set correctly

## Verifying Component Installation

### Check Qt Installation
```bash
qmake --version  # Should show Qt version
```

### Check LLVM Installation
```bash
llvm-config --version  # Should show LLVM version
clang --version        # Should show Clang version
```

### Check Boost Installation
```bash
ls /usr/include/boost# Linux
ls /usr/local/include/boost  # macOS
```

### Check Graphviz Installation
```bash
dot -V  # Should show Graphviz version
```

## IDE Configuration

### IntelliSense Setup (Visual Studio Code)

Create `.vscode/c_cpp_properties.json`:
```json
{
    "configurations": [
        {
          "name": "Linux",
   "includePath": [
     "${workspaceFolder}/**",
      "/usr/include/clang",
 "/usr/include/llvm",
       "/usr/include/qt6"
 ],
            "intelliSenseEngine": "Tag Parser"
}
]
}
```

### Clang-Tidy Setup

Create `.clang-tidy`:
```yaml
Checks: '-*,readability-*,performance-*'
WarningsAsErrors: ''
HeaderFilterRegex: '.*'
AnalyzeTemporaryDtors: false
```

## Running Tests

```bash
# From build directory
ctest --verbose  # Run all tests
ctest -R "test_name"  # Run specific test
```

## Next Steps

1. **Choose Your Team**:
   - UI Team: Start with `docs/UI_GUIDE.md`
   - C2V Team: Start with `docs/C2V_GUIDE.md`
   - V2C Team: Start with `docs/V2C_GUIDE.md`

2. **Set Up IDE**:
   - Configure your preferred IDE/editor
   - Set up debugging if needed

3. **Build and Test**:
- Run `cmake --build . --config Release`
   - Verify application launches: `./dataviz-app`

4. **Start Development**:
   - Read your team's implementation guide
 - Create feature branches for development
   - Follow coding standards from `CONTRIBUTING.md`

## Getting Help

- Check official documentation:
  - Qt: https://doc.qt.io/
  - LLVM: https://llvm.org/docs/
  - CMake: https://cmake.org/cmake/help/latest/

- Check project issues and documentation
- Contact your team lead

## Platform-Specific Notes

### Windows
- Use Visual Studio 2019 or 2022 (MSVC)
- CMake integration is built-in
- Qt Creator is optional but recommended
- Ensure Add to System PATH is checked during Qt installation

### macOS
- Use Clang (included with Xcode)
- Homebrew is the easiest package manager
- Qt6 via Homebrew is well-tested
- LLVM from Homebrew is current version

### Linux
- Multiple compiler options available (GCC, Clang)
- Package managers vary by distribution
- Consider using LLVM from official releases for latest version
- Ensure all -dev packages are installed (not just libraries)

## Security Notes

- Keep dependencies updated regularly
- Use stable/LTS versions for production
- Review security advisories for Qt, LLVM, and Boost
- Run with least-privilege when analyzing untrusted code

## Performance Tips

- Use Release build for production: `cmake --build . --config Release`
- Enable optimizations in CMakeLists.txt
- On multi-core systems: `cmake --build . -j$(nproc)`
- Profile with VS Profiler, Xcode Instruments, or perf

---

**Installation Complete!** ??

Your development environment is now ready. Proceed to your team's implementation guide in the `docs/` directory.
