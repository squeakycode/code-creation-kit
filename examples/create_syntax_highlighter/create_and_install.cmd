@echo off
rem Creates and installs language definitions for PSPad editor
rem The language definition files need to be activated
rem Consult PSPad help on how to do this

if not exist "%ProgramFiles%\PSPad editor" (
    echo PSPad editor installation not found
    goto :exit_failure
)

"%CCK_ROOT%\bin\ccktc.exe" pspad.tccmd
if %errorlevel% neq 0 goto :exit_failure

copy /Y "CCK TC-Commands.INI" "%ProgramFiles%\PSPad editor\Syntax"
if %errorlevel% neq 0 goto :exit_failure

copy /Y "CCK Template.INI" "%ProgramFiles%\PSPad editor\Syntax"
if %errorlevel% neq 0 goto :exit_failure

echo Create and install PSPad syntax highlighter language definition successful...
pause
exit /b 0

:exit_failure
echo Create and install PSPad syntax highlighter language definition failed...
pause
exit /b %errorlevel%