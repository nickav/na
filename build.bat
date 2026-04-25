@echo off

:: Setup
set script_path=%~dp0%
set project_root=%script_path%

:: Config
set build_folder=%project_root%\build\
set src_file=%project_root%\test\main.cpp
set exe_name=na.exe

:: Build executable
if not exist %build_folder% (mkdir %build_folder%)

pushd %build_folder%
  cl /MD -DDEBUG=1 /Od -nologo -Zo -Z7 %project_root%\test\main.cpp /link -subsystem:console -incremental:no -opt:ref -OUT:%exe_name%
  IF %errorlevel% NEQ 0 (popd && goto end)
  .\%exe_name%

  cl /MD -DDEBUG=1 /Od -nologo -Zo -Z7 %project_root%\test\window.c /link -subsystem:windows -incremental:no -opt:ref -OUT:window.exe
  IF %errorlevel% NEQ 0 (popd && goto end)
  .\window.exe
popd

:end

exit /B %errorlevel%
