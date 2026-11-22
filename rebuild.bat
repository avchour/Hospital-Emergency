@echo off
setlocal
echo ==============================================
echo  Hospital System - Auto Clean, Config, Build
echo ==============================================

set "PATH=C:\Qt\Tools\mingw1510_64\bin;C:\Qt\6.12.0\mingw_64\bin;%SystemRoot%\system32;%SystemRoot%;%PATH%"

echo [1/3] Cleaning previous build artifacts...
mingw32-make.exe clean
if errorlevel 1 (
    echo [Warning] Clean completed with warnings or no previous artifacts.
)

echo.
echo [2/3] Configuring with qmake...
C:\Qt\6.12.0\mingw_64\bin\qmake.exe hospital.pro
if errorlevel 1 (
    echo [Error] QMake configuration failed!
    exit /b 1
)

echo.
echo [3/3] Compiling and linking project...
mingw32-make.exe -j4
if errorlevel 1 (
    echo [Error] Build failed!
    exit /b 1
)

echo.
echo ==============================================
echo  Build Succeeded: release\HospitalSystem.exe
echo ==============================================
endlocal
