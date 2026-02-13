@echo off
setlocal
set CONFIG=Release

if not exist build mkdir build
if exist bin\ppmloader.exe del bin\ppmloader.exe

where cmake >nul 2>nul
if errorlevel 1 (
    echo CMake not found.
    exit /b 1
)
pushd build
cmake -S .. -B . -DCMAKE_BUILD_TYPE=%CONFIG% >nul 2>nul
if errorlevel 1 (
    echo CMake build files generation failed.
    exit /b 1
)

cmake --build . --config Release --parallel >error.txt 2>nul
if errorlevel 1 (
    echo CMake build failed.
    type error.txt
    exit /b 1
)
popd
bin\ppmloader test\valid.ppm