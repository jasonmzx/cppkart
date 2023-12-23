#!/bin/bash

# Navigate to the build directory
cd "$(dirname "$0")"/build

# Run CMake and Make
cmake .. && make

# Check if the build was successful
if [ $? -eq 0 ]; then
    echo "Build successful. Running cppkart..."
    ./cppkart
else
    echo "Build failed."
fi
