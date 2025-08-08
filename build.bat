@echo off
if not exist int mkdir int
windres resource.rc -o int\resource.o
if not exist bin mkdir bin
pushd bin
if exist ppmloaderold.exe del ppmloaderold.exe
rename ppmloader.exe ppmloaderold.exe
popd
echo Compiling:
dir /b src\*.cpp
g++ src\*.cpp int\resource.o -lgdi32 -mwindows -o bin\ppmloader.exe
if exist bin\ppmloader.exe echo Compiled to bin\ppmloader.exe.
pause