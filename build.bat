@echo off
REM ===================================================================
REM DataViz-UIT Build Script for Windows
REM ===================================================================
REM Usage: build.bat [Release|Debug] [clean]
REM ===================================================================

setlocal EnableDelayedExpansion

set BUILD_TYPE=%1
if "%BUILD_TYPE%"=="" set BUILD_TYPE=Release

set CLEAN_BUILD=0
if /i "%1"=="clean" set CLEAN_BUILD=1
if /i "%2"=="clean" set CLEAN_BUILD=1

echo.
echo ================================================================================
echo   DataViz-UIT Build Script
echo ================================================================================
echo   Build Type: %BUILD_TYPE%
echo ================================================================================
echo.

REM Check for Qt
if not defined QT_DIR (
  echo QT_DIR not set, searching for Qt...
    for %%V in (6.10.0 6.9.0 6.8.3 6.8.2 6.8.1 6.8.0 6.7.0 6.6.0 6.5.0) do (
        for %%C in (msvc2022_64 msvc2019_64 mingw_64) do (
     if exist "C:\Qt\%%V\%%C\bin\qmake.exe" (
     set QT_DIR=C:\Qt\%%V\%%C
          echo Found Qt at: !QT_DIR!
     goto :QT_FOUND
   )
        )
    )
  echo ERROR: Qt not found! Please run setup_environment.bat first.
    exit /b 1
)
:QT_FOUND

REM Create build directory
set BUILD_DIR=build

if %CLEAN_BUILD%==1 (
    echo Cleaning build directory...
    if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
)

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"

REM Detect available generator
set CMAKE_GENERATOR=
where ninja >nul 2>&1
if %ERRORLEVEL%==0 (
    set CMAKE_GENERATOR=-G "Ninja"
    echo Using Ninja generator
) else (
    REM Try to find Visual Studio
    if exist "C:\Program Files\Microsoft Visual Studio\2022" (
        set CMAKE_GENERATOR=-G "Visual Studio 17 2022" -A x64
 echo Using Visual Studio 2022 generator
  ) else if exist "C:\Program Files\Microsoft Visual Studio\2019" (
  set CMAKE_GENERATOR=-G "Visual Studio 16 2019" -A x64
        echo Using Visual Studio 2019 generator
 ) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019" (
      set CMAKE_GENERATOR=-G "Visual Studio 16 2019" -A x64
        echo Using Visual Studio 2019 generator
    ) else (
        echo Using default CMake generator
 )
)

REM Configure
echo.
echo Configuring CMake...
cmake %CMAKE_GENERATOR% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_PREFIX_PATH="%QT_DIR%" ..

if %ERRORLEVEL% neq 0 (
  echo.
    echo ERROR: CMake configuration failed!
   cd ..
    exit /b 1
)

REM Build
echo.
echo Building...
cmake --build . --config %BUILD_TYPE% --parallel

if %ERRORLEVEL% neq 0 (
    echo.
    echo ERROR: Build failed!
    cd ..
    exit /b 1
)

cd ..

echo.
echo ================================================================================
echo   Build Successful!
echo ================================================================================
echo   Executable: build\bin\dataviz-uit.exe
echo.
echo   To run: run.bat
echo ================================================================================
echo.

endlocal
