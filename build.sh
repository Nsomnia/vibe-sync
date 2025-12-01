#!/bin/bash
# Vibe-Sync Build Script
# This script attempts to build Vibe-Sync with available dependencies

set -e

echo "=============================================="
echo "  Vibe-Sync Build Script"
echo "=============================================="
echo ""

# Check for required tools
check_command() {
    if command -v "$1" >/dev/null 2>&1; then
        echo "✓ $1 found"
        return 0
    else
        echo "✗ $1 not found"
        return 1
    fi
}

# Check basic build tools
echo "Checking build tools..."
HAS_CMAKE=false
HAS_MAKE=false

if check_command cmake; then
    HAS_CMAKE=true
fi

if check_command make; then
    HAS_MAKE=true
fi

check_command g++
check_command gcc

echo ""

# Check for optional dependencies
echo "Checking optional dependencies..."
check_command pkg-config

echo ""

# Try to find Qt6
echo "Checking for Qt6..."
QT6_VERSION=""
if pkg-config --exists qt6-core 2>/dev/null; then
    QT6_VERSION=$(pkg-config --modversion qt6-core 2>/dev/null || echo "unknown")
    echo "✓ Qt6 found: $QT6_VERSION"
else
    echo "✗ Qt6 not found"
fi

echo ""

# Try to install missing packages (if possible)
echo "Attempting to install missing dependencies..."
INSTALL_PACKAGES=()

# Check if we can use apt
if command -v apt >/dev/null 2>&1; then
    echo "Using apt package manager..."
    
    # Try to update package list
    if sudo apt update >/dev/null 2>&1; then
        echo "✓ Package list updated"
        
        # Install build essentials
        if sudo apt install -y build-essential cmake pkg-config >/dev/null 2>&1; then
            echo "✓ Build essentials installed"
            HAS_CMAKE=true
            HAS_MAKE=true
        fi
        
        # Try to install Qt6
        if [ -z "$QT6_VERSION" ]; then
            if sudo apt install -y qt6-base-dev qt6-multimedia-dev >/dev/null 2>&1; then
                echo "✓ Qt6 development packages installed"
                QT6_VERSION=$(pkg-config --modversion qt6-core 2>/dev/null || echo "installed")
            fi
        fi
        
        # Try to install other dependencies
        sudo apt install -y libsdl2-dev libasound2-dev libfreetype6-dev \
                           libavcodec-dev libavformat-dev libavutil-dev \
                           libtag1-dev >/dev/null 2>&1 || true
        
        echo "✓ Additional packages installation attempted"
    else
        echo "⚠ Cannot update package list (permission denied)"
    fi
else
    echo "⚠ apt not available"
fi

echo ""

# Choose build method
BUILD_METHOD=""
if [ "$HAS_CMAKE" = true ]; then
    echo "Using CMake build system..."
    BUILD_METHOD="cmake"
elif [ "$HAS_MAKE" = true ]; then
    echo "Using Makefile build system..."
    BUILD_METHOD="make"
else
    echo "ERROR: No build system available (cmake or make required)"
    exit 1
fi

echo ""

# Clean previous build
echo "Cleaning previous build..."
rm -rf build
mkdir build
cd build

# Build the project
echo "Building Vibe-Sync..."
if [ "$BUILD_METHOD" = "cmake" ]; then
    cmake ..
    make -j$(nproc)
else
    # Use the Makefile in the parent directory
    make -f ../Makefile
fi

echo ""
echo "=============================================="
echo "  Build Complete!"
echo "=============================================="

# Test the build
echo ""
echo "Testing build..."
./vibe-sync --version

echo ""
echo "Build successful! You can now run:"
echo "  ./vibe-sync --help"
echo "  ./vibe-sync --check-deps"
echo "  ./vibe-sync --build-info"

echo ""
echo "For full functionality, install:"
echo "  - Qt6 development packages"
echo "  - ProjectM libraries"
echo "  - FFmpeg development libraries"
echo ""
echo "Then rebuild with: $0"