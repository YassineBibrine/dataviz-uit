#!/bin/bash

################################################################################
# DataViz-UIT: Linux (Fedora/RHEL) Dependency Installation Script
################################################################################
# This script installs all necessary dependencies for building DataViz-UIT
# on Fedora/RHEL using dnf package manager
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

# Check if running as root
check_root() {
    if [[ $EUID -ne 0 ]]; then
        print_error "This script must be run as root or with sudo"
        echo "Please run: sudo ./install_dependencies_fedora.sh"
        exit 1
    fi
}

# Main script
main() {
    print_header "DataViz-UIT: Fedora/RHEL Dependency Installation"
    
    echo ""
    
    # Check root
    check_root
    
    # Update package manager
    print_info "Updating package manager..."
    dnf update -y
    print_success "Package manager updated"
    
    echo ""
    print_info "Installing dependencies..."
    echo ""
    
    # Build essentials
    print_info "Installing build essentials..."
    dnf install -y gcc-c++ gcc
    dnf install -y make
    print_success "Build essentials installed"
    
    # CMake
    print_info "Installing CMake..."
    dnf install -y cmake
    print_success "CMake installed"
    
    # Git
    print_info "Installing Git..."
    dnf install -y git
    print_success "Git installed"
    
    # Qt6 Base Libraries
    print_info "Installing Qt6 development libraries..."
    dnf install -y qt6-qtbase-devel
    dnf install -y qt6-qttools-devel
print_success "Qt6 libraries installed"
    
    # Graphviz
    print_info "Installing Graphviz..."
    dnf install -y graphviz
  dnf install -y graphviz-devel
    print_success "Graphviz installed"
    
    # Development tools
    print_info "Installing development tools..."
    dnf install -y clang-tools-extra
    dnf install -y cppcheck
    print_success "Development tools installed"
    
    # Ninja Build System (optional)
    print_info "Installing Ninja build system..."
dnf install -y ninja-build
    print_success "Ninja installed"
    
    # pkg-config (useful for Qt detection)
    print_info "Installing pkg-config..."
    dnf install -y pkgconfig
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
    if pkg-config --exists Qt6Core 2>/dev/null; then
        echo "Qt6 found: $(pkg-config --modversion Qt6Core)"
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
    echo "mkdir build"
    echo "   cd build"
    echo "   cmake .."
    echo "   cmake --build . --config Release"
    echo ""
    echo "3. Run the application:"
    echo "   ./dataviz"
    echo ""
    echo "4. (Optional) Create system-wide link:"
    echo " sudo cp dataviz /usr/local/bin/"
    echo ""
    
    echo ""
    print_header "Setup Complete"
    echo ""
    print_success "All dependencies installed successfully!"
    echo ""
}

# Run main function
main "$@"
