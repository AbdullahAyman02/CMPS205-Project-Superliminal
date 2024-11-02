# To run this script:
# 1. Open a PowerShell terminal
# 2. Navigate to the root directory of the project
# 3. Run the following command: .\scripts\run-game.ps1

# Check if the build directory exists
if (-Not (Test-Path -Path "build" -PathType Container)) {
    New-Item -ItemType Directory -Path "build"
}

# Navigate to the build directory
Set-Location -Path "build"

# Run CMake to generate build files
cmake ..

# Build the project
cmake --build .

# Navigate back to the root directory
Set-Location -Path ".."

# Run the executable
Start-Process -FilePath "./bin/GAME_APPLICATION.exe"
