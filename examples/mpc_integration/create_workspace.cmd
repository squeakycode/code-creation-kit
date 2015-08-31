@echo off
rem This file is of interest to MPC users only.
rem It demonstrates the use of the Makefile, Project and Workspace Creator - MPC
rem together with the code-creation-kit text compiler 
rem and is used to create the example solution and projects.
rem See http://www.ociweb.com/products/mpc for more information on MPC

if not defined MPC_ROOT (
    echo Requires MPC - The Makefile, Project and Workspace Creator
    pause
    exit /b 1
)

set IDE_TYPE=%~1

if not defined IDE_TYPE if exist "%ProgramFiles%\Microsoft Visual Studio 9.0" set IDE_TYPE=vc9
if not defined IDE_TYPE if exist "%ProgramFiles%\Microsoft Visual Studio 8" set IDE_TYPE=vc8

pushd ..
 
call :create_project_mpc getting_started_simple getting_started_simple ""
call :create_project_mpc getting_started_simple_extended getting_started_simple_extended "" VB
call :create_project_mpc documented_state_machine\cd_player cd_player ..
call :create_project_mpc documented_state_machine\cd_player_extended cd_player_extended ..
call :create_project_mpc documented_state_machine\cd_player_inline cd_player_inline ""
call :create_project_mpc command_line_parser command_line_parser ""

%MPC_ROOT%\mwc.pl -type %IDE_TYPE% -include ../mpc examples.mwc

popd
if "%1" equ "" pause
exit /b %ERRORLEVEL%

:create_project_mpc

set PROJECT_DIRECTORY=%~1
set PROJECT_NAME=%~2
set SUB_DIR_INCLUDE=%~3
set SPECIAL_FILES=%~4

call :CreateFile[] > "%PROJECT_DIRECTORY%\%PROJECT_NAME%.mpc"
exit /b %ERRORLEVEL%

:CreateFile[]
echo.project(*) : tccmd {
echo.
echo.    specific(nmake, vc6, vc7, vc71, vc8, vc9) {
echo.       warning_level = 4
echo.    }
echo.
echo.    CCK_Text_Compiler_Command_Files {
call :list_cck_text_compiler_command_files
echo.    }
echo.
if defined SPECIAL_FILES call :add_special_files
echo.    Define_Custom(Table) {
echo.    }
echo.
echo.    Table_Files {
dir /b "%PROJECT_DIRECTORY%\*.xls";"%PROJECT_DIRECTORY%\*.csv" 2> nul:
echo.    }
echo.
echo.    Header_Files {
dir /b "%PROJECT_DIRECTORY%\*.h"
if defined SUB_DIR_INCLUDE for /F "usebackq" %%i in (`dir /b %PROJECT_DIRECTORY%\%SUB_DIR_INCLUDE%\*.h`) do echo %SUB_DIR_INCLUDE%\%%i
echo.    }
echo.
echo.    Source_Files {
dir /b "%PROJECT_DIRECTORY%\*.cpp" 2> nul:
echo.    }
echo.}
exit /b %ERRORLEVEL% 

:add_special_files
echo.    Define_Custom(%SPECIAL_FILES%) {
echo.    }
echo.
echo.    %SPECIAL_FILES%_Files {
dir /b "%PROJECT_DIRECTORY%\*.%SPECIAL_FILES%" 2> nul:
echo.    }
echo.
exit /b %ERRORLEVEL%

:list_cck_text_compiler_command_files
for /F "usebackq" %%i in (`dir /b "%PROJECT_DIRECTORY%\*.tccmd"`) do "%CCK_ROOT%\bin\ccktc.exe" "%PROJECT_DIRECTORY%\%%i" -d mpc & echo. 
exit /b %ERRORLEVEL%

