#!/bin/bash

# To run this script:
# 1. Open a git bash terminal
# 2. Navigate to the root directory of the project
# 3. Run the following command: bash run.bash

# Check if the build directory exists
if [ ! -d "build" ]; then
    mkdir build
fi

# Navigate to the build directory
cd build

# Run CMake to generate build files
cmake ..

# Build the project
cmake --build .

# Navigate back to the root directory
cd ..

# Run the executable
./bin/GAME_APPLICATION.exe