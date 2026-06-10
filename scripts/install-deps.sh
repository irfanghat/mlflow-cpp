#!/bin/bash

set -euo pipefail

echo "----------------------------------------------------------------------"
echo ""
echo "* IMPORTANT *"
echo ""
echo "This script will:"
echo "  - Update APT package lists"
echo "  - Update cmake to the latest stable version"
echo "  - Install build tools and compilers"
echo ""
echo "System changes:"
echo "  - Requires sudo privileges"
echo "  - Installs packages via apt"
echo ""
echo "Starting setup..."
echo "----------------------------------------------------------------------"
echo ""

echo ""
echo "CMake version:"
cmake --version

sudo apt update

echo ""
echo "Installing ninja..."
sudo apt install ninja-build
echo ""

echo ""
echo "Installing gRPC and Protobuf dependencies..."
echo ""

echo ""
echo "Updating APT package list (Apache Arrow repos)..."
echo ""
sudo apt update

sudo apt install -y -V \
    valgrind \
    libgtest-dev \
    libgmock-dev

echo ""
echo "Installing Build dependencies..."
sudo apt install build-essential clang g++-14 libstdc++-14-dev -y

echo ""
echo "Installing Dev dependencies..."
sudo apt install -y -V \
    clang-format \
    gcovr \
    llvm-18-tools


echo "All dependencies installed successfully."
echo ""