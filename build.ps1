# Build script for DataViz UIT project

param(
    [switch]$Clean,
    [switch]$Run,
    [string]$Config = "Release"
)

$ErrorActionPreference = "Stop"

# Set Qt license bypass
$env:QTFRAMEWORK_BYPASS_LICENSE_CHECK = "1"

$buildDir = "build"
$projectRoot = $PSScriptRoot

Write-Host "=== DataViz UIT Build Script ===" -ForegroundColor Cyan

# Clean build if requested
if ($Clean -and (Test-Path $buildDir)) {
 Write-Host "Cleaning build directory..." -ForegroundColor Yellow
Remove-Item -Recurse -Force $buildDir
}

# Create build directory
if (-not (Test-Path $buildDir)) {
    Write-Host "Creating build directory..." -ForegroundColor Green
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

# Configure with CMake
Write-Host "Configuring project with CMake..." -ForegroundColor Green
Push-Location $buildDir
try {
    # Set Qt path if not in CMAKE_PREFIX_PATH
    if (-not $env:CMAKE_PREFIX_PATH) {
        if (Test-Path "C:/Qt/6.10.0/msvc2022_64") {
  $env:CMAKE_PREFIX_PATH = "C:/Qt/6.10.0/msvc2022_64"
 }
    }

    # Use Visual Studio generator for better compatibility
    cmake -G "Visual Studio 17 2022" -A x64 ..
    if ($LASTEXITCODE -ne 0) {
        throw "CMake configuration failed"
    }

    # Build the project
    Write-Host "Building project ($Config)..." -ForegroundColor Green
    cmake --build . --config $Config
    if ($LASTEXITCODE -ne 0) {
        throw "Build failed"
    }

    Write-Host "Build completed successfully!" -ForegroundColor Green

    # Deploy Qt dependencies
    $exePath = "bin\$Config\dataviz-uit.exe"
    if (Test-Path $exePath) {
 Write-Host "Deploying Qt dependencies..." -ForegroundColor Green
        Push-Location "bin\$Config"
 try {
            windeployqt dataviz-uit.exe --release | Out-Null
        Write-Host "Qt dependencies deployed successfully!" -ForegroundColor Green
        }
        finally {
 Pop-Location
        }

  # Run if requested
        if ($Run) {
       Write-Host "Launching application..." -ForegroundColor Cyan
       $exeFullPath = Join-Path $projectRoot (Join-Path $buildDir (Join-Path "bin" (Join-Path $Config "dataviz-uit.exe")))
       Start-Process -FilePath $exeFullPath
  }
    }
}
finally {
    Pop-Location
}

Write-Host "`n=== Build Complete ===" -ForegroundColor Cyan
Write-Host "Executable: build\bin\$Config\dataviz-uit.exe" -ForegroundColor Gray
