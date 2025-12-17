@echo off
REM Simple build and run script for DataViz UIT

echo === Building DataViz UIT ===
powershell -ExecutionPolicy Bypass -File "%~dp0build.ps1" %*
