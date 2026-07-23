@echo off
REM build.bat - Build script for Windows

echo Building Quantum...

where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo Error: CMake not found. Please install CMake and add it to PATH.
    exit /b 1
)

REM if exist build rmdir /s /q build

if not exist build mkdir build
cd build || exit /b 1

cmake .. -G "Visual Studio 17 2022" -A x64 || exit /b 1
cmake --build . --config Release --parallel || exit /b 1
echo Build complete!

set EXE_PATH=bin\Release\hello_triangle.exe
if exist "%EXE_PATH%" (
    echo Running hello_triangle...
    "%EXE_PATH%"
) else (
    echo Error: hello_triangle.exe not found!
    echo Searching for executable...
    dir /s hello_triangle.exe 2>nul || echo No hello_triangle.exe found
    exit /b 1
)