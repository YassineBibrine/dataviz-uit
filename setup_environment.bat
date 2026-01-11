@echo off
REM ================================================================================
REM DataViz-UIT Environment Setup Script for Windows
REM ================================================================================
REM This script helps configure your environment for building the project.
REM Run this before building if you haven't set up Qt paths manually.
REM ================================================================================

setlocal EnableDelayedExpansion

echo.
echo ================================================================================
echo   DataViz-UIT Environment Setup
echo ================================================================================
echo.

REM Check if QT_DIR is already set
if defined QT_DIR (
    echo Found existing QT_DIR: %QT_DIR%
    if exist "%QT_DIR%\bin\qmake.exe" (
     echo Qt installation verified!
goto :SETUP_COMPLETE
    ) else (
        echo WARNING: QT_DIR is set but qmake not found. Searching for Qt...
    )
)

REM Search for Qt installation
echo Searching for Qt installation...

set QT_FOUND=0
set QT_PATH=

REM Search common Qt installation paths (newest versions first)
for %%V in (6.10.0 6.9.0 6.8.3 6.8.2 6.8.1 6.8.0 6.7.3 6.7.2 6.7.1 6.7.0 6.6.3 6.6.2 6.6.1 6.6.0 6.5.3 6.5.2 6.5.1 6.5.0) do (
    for %%C in (msvc2022_64 msvc2019_64 mingw_64) do (
 if exist "C:\Qt\%%V\%%C\bin\qmake.exe" (
    set QT_PATH=C:\Qt\%%V\%%C
            set QT_FOUND=1
            goto :QT_SEARCH_DONE
     )
    )
)

REM Also check Qt5
for %%V in (5.15.2 5.15.1 5.15.0 5.14.2 5.12.12) do (
    for %%C in (msvc2019_64 msvc2017_64 mingw73_64) do (
        if exist "C:\Qt\%%V\%%C\bin\qmake.exe" (
    set QT_PATH=C:\Qt\%%V\%%C
         set QT_FOUND=1
   goto :QT_SEARCH_DONE
        )
    )
)

:QT_SEARCH_DONE

if %QT_FOUND%==0 (
    echo.
    echo ERROR: Qt installation not found!
    echo.
    echo Please install Qt from: https://www.qt.io/download-qt-installer
    echo.
    echo After installation, either:
    echo   1. Run this script again
    echo   2. Set QT_DIR environment variable manually:
    echo      set QT_DIR=C:\Qt\6.x.x\msvc2022_64
    echo.
    pause
    exit /b 1
)

echo Found Qt at: %QT_PATH%

REM Set environment variable for current session
set QT_DIR=%QT_PATH%
echo Set QT_DIR=%QT_PATH%

REM Ask user if they want to set it permanently
echo.
set /p SETPERM="Set QT_DIR permanently for your user account? (Y/N): "
if /i "%SETPERM%"=="Y" (
    setx QT_DIR "%QT_PATH%"
    echo QT_DIR has been set permanently.
)

:SETUP_COMPLETE

REM Check for Graphviz (optional)
echo.
echo Checking for Graphviz (optional)...

set GRAPHVIZ_FOUND=0
for %%P in ("C:\Program Files\Graphviz" "C:\Program Files (x86)\Graphviz" "C:\Graphviz") do (
    if exist "%%~P\bin\dot.exe" (
        echo Found Graphviz at: %%~P
    set GRAPHVIZ_ROOT=%%~P
        set GRAPHVIZ_FOUND=1
  goto :GRAPHVIZ_DONE
    )
)

:GRAPHVIZ_DONE

if %GRAPHVIZ_FOUND%==0 (
    echo Graphviz not found - This is OK, the app will use fallback layouts.
 echo To enable advanced graph layouts, install Graphviz from:
    echo https://graphviz.org/download/
)

REM Check for CMake
echo.
echo Checking for CMake...
where cmake >nul 2>&1
if %ERRORLEVEL%==0 (
    for /f "tokens=3" %%i in ('cmake --version ^| findstr /i "version"') do (
        echo Found CMake version: %%i
    )
) else (
    echo WARNING: CMake not found in PATH!
    echo Please install CMake from: https://cmake.org/download/
)

REM Check for Visual Studio / MSVC
echo.
echo Checking for C++ compiler...
where cl >nul 2>&1
if %ERRORLEVEL%==0 (
    echo Found MSVC compiler
) else (
  echo MSVC not in PATH - Make sure to run from Developer Command Prompt
    echo or use Visual Studio / VS Code with CMake Tools extension
)

echo.
echo ================================================================================
echo   Setup Complete!
echo ================================================================================
echo.
echo Environment configured. You can now build the project:
echo.
echo   Option 1 - Using build script:
echo   build.bat
echo.
echo   Option 2 - Manual CMake:
echo     mkdir build
echo     cd build
echo     cmake ..
echo     cmake --build . --config Release
echo.
echo   Option 3 - Visual Studio:
echo     Open the folder in Visual Studio (CMake project)
echo.
echo ================================================================================
echo.

endlocal & set QT_DIR=%QT_PATH%
pause
