@echo off
REM Run the built DataViz UIT application

if not exist "build\bin\Release\dataviz-uit.exe" (
    echo Error: Application not built yet!
    echo Please run build.bat first.
    pause
    exit /b 1
)

echo Starting DataViz UIT...
start "" "build\bin\Release\dataviz-uit.exe"
