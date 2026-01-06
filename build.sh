#!/bin/bash
# ===================================================================
# DataViz-UIT Build Script for Linux/macOS
# ===================================================================
# Usage: ./build.sh [Release|Debug] [clean]
# ===================================================================

set -e

BUILD_TYPE="${1:-Release}"
CLEAN_BUILD=0

if [[ "$1" == "clean" ]] || [[ "$2" == "clean" ]]; then
    CLEAN_BUILD=1
fi

echo ""
echo "================================================================================"
echo "  DataViz-UIT Build Script"
echo "================================================================================"
echo "  Build Type: $BUILD_TYPE"
echo "================================================================================"
echo ""

# Check for Qt
if [[ -z "$QT_DIR" ]]; then
    echo "QT_DIR not set, searching for Qt..."
    
    # Common Qt installation paths
    QT_SEARCH_PATHS=(
        "$HOME/Qt/6.8.0/gcc_64"
        "$HOME/Qt/6.7.0/gcc_64"
        "$HOME/Qt/6.6.0/gcc_64"
        "$HOME/Qt/6.8.0/macos"
        "$HOME/Qt/6.7.0/macos"
        "$HOME/Qt/6.6.0/macos"
        "/opt/Qt/6.8.0/gcc_64"
        "/opt/Qt/6.7.0/gcc_64"
   "/opt/Qt/6.6.0/gcc_64"
        "/opt/homebrew/opt/qt@6"
        "/usr/local/opt/qt@6"
   "/opt/homebrew/opt/qt"
        "/usr/local/opt/qt"
        "/usr/lib/qt6"
    )
    
    for qt_path in "${QT_SEARCH_PATHS[@]}"; do
        if [[ -d "$qt_path" ]]; then
            export QT_DIR="$qt_path"
       echo "Found Qt at: $QT_DIR"
          break
        fi
    done
    
    if [[ -z "$QT_DIR" ]]; then
        echo "WARNING: Qt not found automatically."
        echo "Please set QT_DIR environment variable or install Qt."
        echo ""
 echo "For Ubuntu/Debian: sudo apt install qt6-base-dev"
        echo "For Fedora: sudo dnf install qt6-qtbase-devel"
        echo "For macOS: brew install qt@6"
        echo "Or download from: https://www.qt.io/download-qt-installer"
echo ""
    fi
fi

# Build directory
BUILD_DIR="build"

if [[ $CLEAN_BUILD -eq 1 ]]; then
    echo "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Detect generator
CMAKE_GENERATOR=""
if command -v ninja &> /dev/null; then
    CMAKE_GENERATOR="-G Ninja"
    echo "Using Ninja generator"
else
    echo "Using default CMake generator (Make)"
fi

# Configure
echo ""
echo "Configuring CMake..."
CMAKE_ARGS="$CMAKE_GENERATOR -DCMAKE_BUILD_TYPE=$BUILD_TYPE"

if [[ -n "$QT_DIR" ]]; then
    CMAKE_ARGS="$CMAKE_ARGS -DCMAKE_PREFIX_PATH=$QT_DIR"
fi

cmake $CMAKE_ARGS ..

if [[ $? -ne 0 ]]; then
    echo ""
    echo "ERROR: CMake configuration failed!"
    exit 1
fi

# Build
echo ""
echo "Building..."
cmake --build . --config "$BUILD_TYPE" --parallel $(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

if [[ $? -ne 0 ]]; then
    echo ""
    echo "ERROR: Build failed!"
    exit 1
fi

cd ..

echo ""
echo "================================================================================"
echo "  Build Successful!"
echo "================================================================================"
echo "  Executable: build/bin/dataviz-uit"
echo ""
echo "  To run: ./build/bin/dataviz-uit"
echo "================================================================================"
echo ""
