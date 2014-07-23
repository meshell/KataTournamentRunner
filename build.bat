@ECHO OFF
@ECHO Build and run all tests
SETLOCAL
SET BUILD_DIR=build

CALL configure.bat %*
IF ERRORLEVEL 1 GOTO error
@ECHO.
@ECHO ======================================================================================
@ECHO -- Clean up
IF EXIST %BUILD_DIR% DEL /S/Q %BUILD_DIR%
@ECHO ======================================================================================
MKDIR %BUILD_DIR%
PUSHD %BUILD_DIR%
@ECHO.
@ECHO ======================================================================================
@ECHO -- Build with cmake --
@ECHO ----------------------
%CMAKE% -G "NMake Makefiles" ..
IF ERRORLEVEL 1 GOTO error

nmake
IF ERRORLEVEL 1 GOTO error

@ECHO.
@ECHO ======================================================================================
@ECHO -- Run unittests --
@ECHO -------------------
.\tests\unit\unittests.exe --gtest_shuffle --gtest_output="xml:..\reports\tests\unittests.xml"
IF ERRORLEVEL 1 GOTO error
@ECHO ======================================================================================
@ECHO. 
@ECHO -- Run Feature tests --
@ECHO -----------------------
start .\tests\feature\features.exe

PUSHD ..\tests\feature
call cucumber -p ci
IF ERRORLEVEL 1 GOTO featureerror

POPD
POPD
@ECHO ======================================================================================
@ECHO -- Create Documentation --
@ECHO --------------------------
%CMAKE% --build %BUILD_DIR% --target doc 

GOTO end
:featureerror
POPD
:error
POPD
PAUSE
SET FAILURE_CODE=%ERRORLEVEL%
:end
ENDLOCAL
EXIT /B %FAILURE_CODE%
