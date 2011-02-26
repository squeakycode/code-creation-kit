@echo off
rem   Copyright (C) 2011, Andreas Gau
rem
rem   This file is part of the code-creation-kit.
rem
rem   The code-creation-kit is free software: you can redistribute it and/or modify
rem   it under the terms of the GNU General Public License as published by
rem   the Free Software Foundation, either version 2 of the License, or
rem   (at your option) any later version.
rem
rem   The code-creation-kit is distributed in the hope that it will be useful,
rem   but WITHOUT ANY WARRANTY; without even the implied warranty of
rem   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
rem   GNU General Public License for more details.
rem
rem   You should have received a copy of the GNU General Public License
rem   along with the code-creation-kit. If not, see <http://www.gnu.org/licenses/>.

setlocal enableextensions
setlocal enabledelayedexpansion

set PROJECT_DIRECTORY=%~1
set PROJECT_NAME=%~2
set USE_GENERATOR=%~3

if defined USE_GENERATOR if not defined CCK_ROOT echo Environment variable CCK_ROOT is not set
if defined USE_GENERATOR if not defined CCK_ROOT exit /b 1

call :CreateFile[] > "%PROJECT_DIRECTORY%\%PROJECT_NAME%.mpc"
exit /b %ERRORLEVEL%

:CreateFile[]
echo project(*) : base, boost_base, tccmd {
if defined USE_GENERATOR (
echo     CCK_Text_Compiler_Command_Files {
call :ListCommandFiles[]
echo    }
)
echo }
exit /b %ERRORLEVEL% 

:ListCommandFiles[]
for /F "usebackq" %%i in (`dir /b "%PROJECT_DIRECTORY%\*.tccmd"`) do "%CCK_ROOT%\bin\ccktc.exe" "%PROJECT_DIRECTORY%\%%i" -d mpc & echo. 
exit /b %ERRORLEVEL%
