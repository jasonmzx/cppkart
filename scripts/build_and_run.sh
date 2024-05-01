#!/bin/bash

# Get the absolute path of the script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/../build"

# Check if the build directory exists
if ! [ -d "$BUILD_DIR" ]; then
    $SCRIPT_DIR/./generate.sh
fi

# Change to the build directory
cd "$BUILD_DIR" || exit

# Run CMake and Make
make -j$(nproc)

# Check if the build was successful
if [ $? -eq 0 ]; then
    echo "Build successful. Running cppkart..."
    ./cppkart
else
    echo "Build failed."
fi