@echo off
REM ================================================================================
REM DataViz-UIT Environment Setup Script
REM ================================================================================
REM This script helps configure Qt and other dependencies for building the project
REM Run this ONCE before your first build
REM ================================================================================

setlocal enabledelayedexpansion

echo.
echo ================================================================================
echo   DataViz-UIT Environment Setup
echo ================================================================================
echo.
echo This script will configure your environment for building DataViz-UIT
echo.

REM ===================================================================
REM Step 1: Qt Configuration
REM ===================================================================
echo [1/3] Qt Configuration
echo --------------------------------------------------------------------------------
echo.
echo Qt is required to build this project.
echo.
echo Where is your Qt MSVC 2022 64-bit installation?
echo Common locations:
echo   - C:\Qt\6.10.0\msvc2022_64
echo   - C:\Qt\6.8.0\msvc2022_64
echo   - C:\Qt\6.7.0\msvc2022_64
echo.

REM Try to auto-detect Qt
set QT_AUTO_DETECTED=0
set QT_AUTO_PATH=

for %%V in (6.10.0 6.8.0 6.7.0 6.6.0 6.5.0) do (
    if exist "C:\Qt\%%V\msvc2022_64" (
        set QT_AUTO_PATH=C:\Qt\%%V\msvc2022_64
 set QT_AUTO_DETECTED=1
        goto qt_found
    )
)

:qt_found
if %QT_AUTO_DETECTED%==1 (
    echo Auto-detected Qt at: !QT_AUTO_PATH!
    echo.
    set /p USE_AUTO="Use this path? (Y/N): "
  
    if /i "!USE_AUTO!"=="Y" (
      set QT_PATH=!QT_AUTO_PATH!
        goto qt_configured
    )
)

:qt_manual
echo.
set /p QT_PATH="Enter full Qt path: "

if "!QT_PATH!"=="" (
    echo ERROR: Qt path cannot be empty
    goto qt_manual
)

if not exist "!QT_PATH!" (
 echo.
    echo ERROR: Path does not exist: !QT_PATH!
  echo.
    echo Please check the path and try again.
    goto qt_manual
)

if not exist "!QT_PATH!\bin\qmake.exe" (
    echo.
    echo WARNING: qmake.exe not found in !QT_PATH!\bin
 echo This may not be a valid Qt installation path.
    echo.
    set /p CONTINUE="Continue anyway? (Y/N): "
    if /i not "!CONTINUE!"=="Y" goto qt_manual
)

:qt_configured
echo.
echo Setting QT_DIR environment variable...
setx QT_DIR "!QT_PATH!" >nul 2>&1
if errorlevel 1 (
    echo ERROR: Failed to set environment variable
    echo You may need to run this script as Administrator
    pause
    exit /b 1
)

echo ? Qt configured: !QT_PATH!
echo.

REM ===================================================================
REM Step 2: Graphviz Configuration (Optional)
REM ===================================================================
echo [2/3] Graphviz Configuration (Optional)
echo --------------------------------------------------------------------------------
echo.
echo Graphviz provides advanced graph layout capabilities.
echo The application will work without it (using fallback layout).
echo.
set /p HAS_GRAPHVIZ="Do you have Graphviz installed? (Y/N): "

if /i "!HAS_GRAPHVIZ!"=="Y" (
    echo.
    echo Common Graphviz locations:
    echo   - C:\Program Files\Graphviz
    echo   - C:\Program Files (x86)\Graphviz
    echo.
    
    REM Try to auto-detect Graphviz
    if exist "C:\Program Files\Graphviz" (
        echo Auto-detected: C:\Program Files\Graphviz
  set /p USE_AUTO_GV="Use this path? (Y/N): "
        
        if /i "!USE_AUTO_GV!"=="Y" (
        set GRAPHVIZ_PATH=C:\Program Files\Graphviz
   goto graphviz_set
        )
)
    
    set /p GRAPHVIZ_PATH="Enter Graphviz root directory: "
    
    :graphviz_set
    if exist "!GRAPHVIZ_PATH!" (
 setx GRAPHVIZ_ROOT "!GRAPHVIZ_PATH!" >nul 2>&1
    echo ? Graphviz configured: !GRAPHVIZ_PATH!
    ) else (
        echo WARNING: Path does not exist: !GRAPHVIZ_PATH!
 echo Graphviz configuration skipped
    )
) else (
    echo Graphviz configuration skipped (optional)
)
echo.

REM ===================================================================
REM Step 3: Qt License Bypass
REM ===================================================================
echo [3/3] Qt License Configuration
echo --------------------------------------------------------------------------------
echo.
echo Setting Qt license bypass...
setx QTFRAMEWORK_BYPASS_LICENSE_CHECK "1" >nul 2>&1
echo ? Qt license check bypass enabled
echo.

REM ===================================================================
REM Summary
REM ===================================================================
echo ================================================================================
echo   Setup Complete!
echo ================================================================================
echo.
echo Environment variables configured:
echo   QT_DIR = !QT_PATH!
if defined GRAPHVIZ_PATH echo   GRAPHVIZ_ROOT = !GRAPHVIZ_PATH!
echo   QTFRAMEWORK_BYPASS_LICENSE_CHECK = 1
echo.
echo ?  IMPORTANT: These variables are set for NEW terminal sessions.
echo    Please CLOSE this window and open a NEW command prompt or PowerShell.
echo.
echo Next steps:
echo   1. Close this window
echo2. Open a NEW command prompt in the project directory
echo   3. Run: build.bat
echo.
echo ================================================================================
echo.
pause

endlocal
