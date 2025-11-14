@echo off
REM Test launch script for DataViz-UIT
REM This will display the application window

title DataViz-UIT - Algorithm Visualization

echo ========================================
echo DataViz-UIT Application Launcher
echo ========================================
echo.
echo Building application path...
cd /d "C:\Users\User\source\repos\YassineBibrine\dataviz-uit\build\Release"

echo.
echo Checking if executable exists...
if not exist dataviz.exe (
    echo ERROR: dataviz.exe not found at:
    echo %CD%\dataviz.exe
    echo.
  echo Please build the project first using:
    echo   cd C:\Users\User\source\repos\YassineBibrine\dataviz-uit\build
    echo   cmake .. -DQt6_DIR="C:\Qt\6.10.0\msvc2022_64\lib\cmake\Qt6"
    echo   cmake --build . --config Release
 echo.
    pause
 exit /b 1
)

echo Found dataviz.exe at: %CD%\dataviz.exe
echo.
echo ========================================
echo Launching DataViz-UIT...
echo ========================================
echo.

REM Launch the application
dataviz.exe

echo.
echo Application closed.
pause
