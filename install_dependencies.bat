@echo off
REM ============================================================================
REM DataViz-UIT: Windows Dependency Installation Script
REM ============================================================================
REM This script installs all necessary dependencies for building DataViz-UIT
REM on Windows using Chocolatey package manager
REM ============================================================================

setlocal enabledelayedexpansion
color 0A
title DataViz-UIT - Windows Dependency Installer

REM Check if running as administrator
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo.
    echo ERROR: This script must be run as Administrator!
    echo Please right-click and select "Run as administrator"
    echo.
    pause
    exit /b 1
)

echo.
echo ============================================================================
echo DataViz-UIT: Windows Dependency Installation
echo ============================================================================
echo.

REM Check if Chocolatey is installed
echo [*] Checking for Chocolatey...
choco --version >nul 2>&1
if errorLevel 1 (
    echo [!] Chocolatey not found. Installing...
    echo [*] Downloading Chocolatey installer...
    
  powershell -NoProfile -InputFormat None -ExecutionPolicy Bypass -Command "[System.Net.ServicePointManager]::SecurityProtocol = 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))"
    
    if errorLevel 1 (
     echo [ERROR] Failed to install Chocolatey
        pause
        exit /b 1
    )
    echo [+] Chocolatey installed successfully
) else (
    echo [+] Chocolatey is installed
)

echo.
echo [*] Starting dependency installation...
echo.

REM Install CMake
echo [*] Installing CMake...
choco install cmake -y --no-progress
if errorLevel 1 (
    echo [ERROR] Failed to install CMake
) else (
    echo [+] CMake installed
)

REM Install Git
echo [*] Installing Git...
choco install git -y --no-progress
if errorLevel 1 (
    echo [ERROR] Failed to install Git
) else (
    echo [+] Git installed
)

REM Install Visual Studio Build Tools
echo [*] Installing Visual Studio 2022 Build Tools...
choco install visualstudio2022-workload-nativedesktop -y --no-progress
if errorLevel 1 (
    echo [WARNING] Visual Studio installation may require manual setup
    echo Please visit: https://visualstudio.microsoft.com/vs/
) else (
    echo [+] Visual Studio Build Tools installed
)

REM Install Graphviz
echo [*] Installing Graphviz...
choco install graphviz -y --no-progress
if errorLevel 1 (
    echo [ERROR] Failed to install Graphviz
) else (
    echo [+] Graphviz installed
)

REM Optional: Install development tools
echo.
echo [*] Installing optional development tools...

echo [*] Installing Ninja Build System...
choco install ninja -y --no-progress

echo [*] Installing 7-Zip...
choco install 7zip -y --no-progress

REM Verify installations
echo.
echo ============================================================================
echo Verification
echo ============================================================================
echo.

set "all_ok=1"

echo [*] Verifying CMake...
cmake --version >nul 2>&1
if errorLevel 1 (
    echo [ERROR] CMake verification failed
    set "all_ok=0"
) else (
    for /f "tokens=3" %%i in ('cmake --version') do (
        echo [+] CMake version: %%i
        goto :cmake_ok
    )
    :cmake_ok
)

echo [*] Verifying Git...
git --version >nul 2>&1
if errorLevel 1 (
  echo [ERROR] Git verification failed
    set "all_ok=0"
) else (
    echo [+] Git is installed
)

echo [*] Verifying Graphviz...
dot -V >nul 2>&1
if errorLevel 1 (
    echo [ERROR] Graphviz verification failed
    set "all_ok=0"
) else (
    echo [+] Graphviz is installed
)

echo.
echo ============================================================================
echo Next Steps
echo ============================================================================
echo.
echo 1. Install Qt 6 from: https://www.qt.io/download
echo    - Or use: choco install qt (requires manual setup)
echo.
echo 2. Clone the repository:
echo    git clone https://github.com/YassineBibrine/dataviz-uit.git
echo    cd dataviz-uit
echo    git checkout yb1
echo.
echo 3. Build the project:
echo    mkdir build
echo    cd build
echo cmake ..
echo    cmake --build . --config Release
echo.
echo ============================================================================
echo.

if "!all_ok!"=="1" (
 echo [SUCCESS] All dependencies installed successfully!
    color 0A
) else (
echo [WARNING] Some dependencies may not be installed
 color 0C
)

echo.
pause
