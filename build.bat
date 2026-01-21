@echo off
setlocal

if not exist build mkdir build
if exist bin\ppmloader.exe del bin\ppmloader.exe

where cmake >nul 2>nul
if errorlevel 1 (
    echo "CMake not found."
    exit /b 1
)

cmake -S . -B build -DCMAKE_BUILD_TYPE=%CONFIG% >nul 2>nul
if errorlevel 1 (
    echo "CMake build files generation failed."
    exit /b 1
)

cmake --build build --config Release --parallel
if errorlevel 1 (
    echo "CMake build failed."
    exit /b 1
)

bin\ppmloader test\valid.ppm