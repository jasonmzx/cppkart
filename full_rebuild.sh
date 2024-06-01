#!/bin/bash

# Print the starting message
echo "Starting the build process..."

# Remove the build directory if it exists
if [ -d "build" ]; then
    echo "Removing existing 'build' directory..."
    rm -r build
else
    echo "'build' directory does not exist. Skipping removal step..."
fi

# Create a new build directory
echo "Creating new 'build' directory..."
mkdir build

# Change into the build directory
echo "Changing into the 'build' directory..."
cd build

# Run CMake to generate build files
echo "Running CMake to generate build files..."
cmake ..

# Check if CMake succeeded
if [ $? -ne 0 ]; then
    echo "CMake configuration failed. Exiting..."
    exit 1
fi

# Build the project using make
echo "Building the project using make..."
make -j8

# Check if make succeeded
if [ $? -ne 0 ]; then
    echo "Build process failed. Exiting..."
    exit 1
fi

# Print the finishing message
echo "Build process completed successfully!"
