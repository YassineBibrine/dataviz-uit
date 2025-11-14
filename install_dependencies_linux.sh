#!/bin/bash

################################################################################
# DataViz-UIT: Linux (Ubuntu/Debian) Dependency Installation Script
################################################################################
# This script installs all necessary dependencies for building DataViz-UIT
# on Ubuntu/Debian using apt package manager
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

# Check if running as root for apt-get
check_root() {
if [[ $EUID -ne 0 ]]; then
        print_error "This script must be run as root or with sudo"
        echo "Please run: sudo ./install_dependencies_linux.sh"
 exit 1
    fi
}

# Main script
main() {
    print_header "DataViz-UIT: Linux Dependency Installation"
    
    echo ""
    
  # Check root
    check_root
    
    # Update package manager
    print_info "Updating package manager..."
    apt-get update
    print_success "Package manager updated"
    
    echo ""
    print_info "Installing dependencies..."
    echo ""
    
    # Build essentials
    print_info "Installing build essentials..."
    apt-get install -y build-essential
    apt-get install -y g++ gcc
    print_success "Build essentials installed"
    
    # CMake
 print_info "Installing CMake..."
    apt-get install -y cmake
    print_success "CMake installed"
    
    # Git
    print_info "Installing Git..."
    apt-get install -y git
    print_success "Git installed"
    
    # Qt6 Base Libraries
    print_info "Installing Qt6 development libraries..."
    apt-get install -y qt6-base-dev
    apt-get install -y libqt6core6
    apt-get install -y libqt6gui6
    apt-get install -y libqt6widgets6
    apt-get install -y qt6-tools-dev
    print_success "Qt6 libraries installed"
    
    # Graphviz
    print_info "Installing Graphviz..."
    apt-get install -y graphviz
    apt-get install -y libgraphviz-dev
    print_success "Graphviz installed"
    
    # Development tools
    print_info "Installing development tools..."
    apt-get install -y clang-format
    apt-get install -y clang-tools
    apt-get install -y cppcheck
    print_success "Development tools installed"
    
    # Ninja Build System (optional)
    print_info "Installing Ninja build system..."
 apt-get install -y ninja-build
    print_success "Ninja installed"
    
    # pkg-config (useful for Qt detection)
    print_info "Installing pkg-config..."
    apt-get install -y pkg-config
    print_success "pkg-config installed"
    
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
    
    print_info "Verifying GCC..."
    g++ --version | head -1
    print_success "GCC verified"
    
    print_info "Verifying Qt6..."
    if pkg-config --exists Qt6Core; then
        echo "Qt6 version: $(pkg-config --modversion Qt6Core)"
        print_success "Qt6 verified"
    else
        print_warning "Qt6 may not be properly configured"
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
    echo "cmake --build . --config Release"
    echo ""
    echo "3. Run the application:"
    echo "   ./dataviz"
    echo ""
    echo "4. (Optional) Create desktop shortcut:"
    echo "   sudo cp dataviz /usr/local/bin/"
    echo ""
    
    echo ""
    print_header "Setup Complete"
    echo ""
  print_success "All dependencies installed successfully!"
    echo ""
}

# Run main function
main "$@"
