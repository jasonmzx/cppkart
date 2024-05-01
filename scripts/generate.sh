#!/bin/bash

# Get the absolute path of the script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/../build"

echo "Generating build files..."

# Check if the build directory exists
if [ -d "$BUILD_DIR" ]; then
    # Remove all content inside the build directory
    rm -rf "${BUILD_DIR:?}"/*
else
    # Create the build directory if it doesn't exist
    mkdir -p "$BUILD_DIR"
fi

# Change to the build directory
cd "$BUILD_DIR" || exit

# Run CMake and Make
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..