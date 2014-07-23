@ECHO OFF
REM This script searchs for the tools neded to build the Kata Tournament Runner, e.g. CMake, Visual Studio and Cucumber
REM and further enpacks the dependencies

SET PROGRAM_FILES=%PROGRAMFILES(X86)%
IF NOT EXIST "%PROGRAM_FILES%" SET PROGRAM_FILES=%PROGRAMFILES%
SET CMAKE_PATH="%PROGRAM_FILES%"

REM Search cmake
IF NOT EXIST %CMAKE_PATH% GOTO cmake_path_error
PUSHD %CMAKE_PATH%

FOR /f "delims=" %%A IN ('dir /b /s cmake.exe') DO SET F=%%A
IF EXIST "%F%" (
  SET CMAKE="%F%"
) ELSE (
  GOTO cmake_missing
)
POPD

REM Search for Visual studio and set the environment for using MS Visual Studio tools
SET VISUAL_STUDIO_ENV="%PROGRAM_FILES%\Microsoft Visual Studio 12.0\VC\vcvarsall.bat"

IF NOT EXIST %VISUAL_STUDIO_ENV% GOTO visual_studio_missing
CALL %VISUAL_STUDIO_ENV%

REM Enpack the external dependencies, e.g. boost, googlemock, cucumber-cpp
PUSHD externals
@ECHO.
@ECHO ======================================================================================
@ECHO -- Clean up

IF EXIST include DEL /S/Q include
IF EXIST lib DEL /S/Q lib

@ECHO.
@ECHO ======================================================================================
@ECHO -- Unzip external headers and libraries --
@ECHO ------------------------------------------
.\win_externals_include.exe -y

.\win_externals_lib-msvc12.exe -y

@ECHO.
POPD

GOTO end

:cmake_path_error
@ECHO Path to cmake does not exist.
SET FAILURE_CODE=1
GOTO :end

:cmake_missing
@ECHO Error: Could not find cmake. Please provide the path to cmake.
@ECHO Usage: configure.bat --cmake_path [path to cmake.exe]
SET FAILURE_CODE=1
GOTO :end
:visual_studio_missing
ECHO Error: Could not find Visual Studio 2013.
SET FAILURE_CODE=1
GOTO :end
:end
IF ERRORLEVEL 1 SET FAILURE_CODE=%ERRORLEVEL%
EXIT /B %FAILURE_CODE%
