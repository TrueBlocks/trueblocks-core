:: NOTE: THIS FILE IS NOT MAINTAINED AND IS LIKELY BROKEN
:: NO WE DON'T SUPPORT WINDOWS

@echo off
setlocal

:: Get the directory where the script is located
set SCRIPT_DIR_BACKSLASH=%~dp0
set SCRIPT_DIR=%script_dir_backslash:~0,-1%
set ROOT_DIR=%SCRIPT_DIR%\..

:: Regenerate the go.work file
set GO_WORK_FILE=%ROOT_DIR%\go.work
echo // Go Version > "%GO_WORK_FILE%"
echo go 1.22 >> "%GO_WORK_FILE%"

:: Find all go.mod files in the src directory and add their directories to go.work
cd %ROOT_DIR% && go work use .\examples\balanceChart
cd %ROOT_DIR% && go work use .\examples\cancelContext
cd %ROOT_DIR% && go work use .\examples\findFirst
cd %ROOT_DIR% && go work use .\examples\nameManager
cd %ROOT_DIR% && go work use .\examples\simple
cd %ROOT_DIR% && go work use .\examples\withStreaming
cd %ROOT_DIR% && go work use .\src\apps\chifra
cd %ROOT_DIR% && go work use .\src\node
cd %ROOT_DIR% && go work use .\src\dev_tools\goMaker
cd %ROOT_DIR% && go work use .\src\dev_tools\sdkFuzzer
cd %ROOT_DIR% && go work use .\src\dev_tools\testRunner

:: Run go work sync
cd /d %ROOT_DIR% && go work sync

:: Display the contents of go.work
echo Created go.work with these contents.
type "%GO_WORK_FILE%"

:: Run go-mod-tidy script
call "%SCRIPT_DIR%\go-mod-tidy.bat"

:: Return to the original directory and exit
cd /d "%SCRIPT_DIR%"
exit /b
