@Echo off

rem =================== CONFIG ============================
set GENERATOR="Visual Studio 15 2017"
set TOOLSET=v141

set BUILD_DIR=%~dp0%\build\
set SOURCE_DIR=%~dp0%\Source\
rem =======================================================

set BUILDTYPE=Release
if "%~1"=="debug"(set BUILDTYPE=debug)

cmake -S %SOURCE_DIR% -B %BUILD_DIR% -G %GENERATOR% -A x64 -T %TOOLSET%
cmake --build %BUILD_DIR% --config %BUILDTYPE%

pause
