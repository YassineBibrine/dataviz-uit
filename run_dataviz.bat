@echo off
REM Launch DataViz-UIT Application
REM This script properly launches the Qt6 GUI application

echo Starting DataViz-UIT Application...
echo.

REM Navigate to build directory
cd /d "C:\Users\User\source\repos\YassineBibrine\dataviz-uit\build\Release"

REM Check if executable exists
if not exist dataviz.exe (
    echo ERROR: dataviz.exe not found!
    echo Please build the project first with CMake
  pause
    exit /b 1
)

REM Run the application
echo Launching dataviz.exe...
start "" dataviz.exe

REM Give it a moment to start
timeout /t 2 /nobreak

echo.
echo Application launched! Check your taskbar if window is not visible.
echo To close the application, close the window or use Ctrl+C in any console window.
