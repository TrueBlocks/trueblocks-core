:: NOTE: THIS FILE IS NOT MAINTAINED AND IS LIKELY BROKEN
:: NO WE DON'T SUPPORT WINDOWS

@echo off
setlocal

:: Get the directory where the script is located
set SCRIPT_DIR_BACKSLASH=%~dp0
set SCRIPT_DIR=%script_dir_backslash:~0,-1%
set ROOT_DIR=%SCRIPT_DIR%\..

:: Find all go.mod files and tidy them
cd /d %ROOT_DIR%\examples\balanceChart      && go mod tidy
cd /d %ROOT_DIR%\examples\cancelContext     && go mod tidy
cd /d %ROOT_DIR%\examples\findFirst         && go mod tidy
cd /d %ROOT_DIR%\examples\nameManager       && go mod tidy
cd /d %ROOT_DIR%\examples\simple            && go mod tidy
cd /d %ROOT_DIR%\examples\withStreaming     && go mod tidy
cd /d %ROOT_DIR%\src\apps\chifra            && go mod tidy
cd /d %ROOT_DIR%\src\node                   && go mod tidy
cd /d %ROOT_DIR%\src\dev_tools\goMaker      && go mod tidy
cd /d %ROOT_DIR%\src\dev_tools\indexManager && go mod tidy
cd /d %ROOT_DIR%\src\dev_tools\sdkFuzzer    && go mod tidy
cd /d %ROOT_DIR%\src\dev_tools\testRunner   && go mod tidy

:: Return to the original directory and exit
exit /b
