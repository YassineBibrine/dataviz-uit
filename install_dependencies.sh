#!/bin/bash

################################################################################
# DataViz-UIT: macOS Dependency Installation Script
################################################################################
# This script installs all necessary dependencies for building DataViz-UIT
# on macOS using Homebrew package manager
################################################################################

set -e

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Functions
print_header() {
    echo -e "${BLUE}================================================================================${NC}"
    echo -e "${BLUE}$1${NC}"
 echo -e "${BLUE}================================================================================${NC}"
}

print_success() {
    echo -e "${GREEN}[+]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[!]${NC} $1"
}

print_info() {
    echo -e "${BLUE}[*]${NC} $1"
}

# Main script
main() {
    print_header "DataViz-UIT: macOS Dependency Installation"
    
    echo ""
    
    # Check for Homebrew
    print_info "Checking for Homebrew..."
    if ! command -v brew &> /dev/null; then
        print_warning "Homebrew not found. Installing..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
        print_success "Homebrew installed"
    else
        print_success "Homebrew is installed"
    fi
    
    echo ""
    
    # Update Homebrew
    print_info "Updating Homebrew..."
  brew update
    print_success "Homebrew updated"
    
    echo ""
    print_info "Installing dependencies..."
  echo ""
    
    # CMake
    print_info "Installing CMake..."
    if brew list cmake &>/dev/null; then
  print_warning "CMake already installed"
     brew upgrade cmake || true
    else
      brew install cmake
    fi
    print_success "CMake ready"
    
    # Git
    print_info "Installing Git..."
    if brew list git &>/dev/null; then
        print_warning "Git already installed"
    else
        brew install git
    fi
    print_success "Git ready"
    
    # Xcode Command Line Tools
    print_info "Checking Xcode Command Line Tools..."
if ! xcode-select -p &> /dev/null; then
   print_warning "Xcode Command Line Tools not found. Installing..."
        xcode-select --install
    print_success "Xcode Command Line Tools installed"
    else
        print_success "Xcode Command Line Tools found"
    fi
    
    # Qt6
    print_info "Installing Qt6..."
    if brew list qt6 &>/dev/null; then
 print_warning "Qt6 already installed"
        brew upgrade qt6 || true
    else
        brew install qt6
    fi
    print_success "Qt6 ready"
    
    # Graphviz
    print_info "Installing Graphviz..."
    if brew list graphviz &>/dev/null; then
        print_warning "Graphviz already installed"
   brew upgrade graphviz || true
    else
        brew install graphviz
    fi
    print_success "Graphviz ready"
    
    # Clang Format
    print_info "Installing Clang Format..."
    if brew list clang-format &>/dev/null; then
      print_warning "Clang Format already installed"
  else
   brew install clang-format
    fi
    print_success "Clang Format ready"
    
    # Ninja (optional)
    print_info "Installing Ninja Build System..."
 if brew list ninja &>/dev/null; then
      print_warning "Ninja already installed"
    else
        brew install ninja
    fi
print_success "Ninja ready"
    
    echo ""
  print_header "Verification"
    
    echo ""
    
    # Verify installations
    print_info "Verifying CMake..."
 cmake --version | head -1
    print_success "CMake verified"
    
    print_info "Verifying Git..."
 git --version
  print_success "Git verified"
    
    print_info "Verifying Clang..."
    clang --version | head -1
    print_success "Clang verified"
    
    print_info "Verifying Qt6..."
    if brew list qt6 &>/dev/null; then
        QT_PATH=$(brew --prefix qt6)
        echo "Qt6 path: $QT_PATH"
        print_success "Qt6 verified"
    fi
    
    print_info "Verifying Graphviz..."
    dot -V 2>&1 | head -1
    print_success "Graphviz verified"
    
    echo ""
    print_header "Next Steps"
    
    echo ""
    echo "1. Clone the repository:"
    echo "   git clone https://github.com/YassineBibrine/dataviz-uit.git"
    echo "   cd dataviz-uit"
    echo "   git checkout yb1"
    echo ""
    echo "2. Build the project:"
    echo "   mkdir build"
    echo "   cd build"
    echo "   cmake .."
    echo "   cmake --build . --config Release"
    echo ""
    echo "3. Run the application:"
    echo "   ./dataviz"
    echo ""
    
    echo ""
    print_header "Setup Complete"
    echo ""
    print_success "All dependencies installed successfully!"
    echo ""
}

# Run main function
main "$@"
