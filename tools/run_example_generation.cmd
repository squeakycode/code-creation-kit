@echo off
rem   Copyright (C) 2011-2015, Andreas Gau
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

rem check preconditions
if not defined CCK_ROOT (
    echo The environment variable CCK_ROOT is not set
    exit /b 1
)

rem check parameter
set EXAMPLE_HOME=%~1
if not defined EXAMPLE_HOME exit \b 1

rem var used for error level tracking
set SAVED_ERROR_LEVEL=0

rem go through the example directory and run the command files
pushd "%EXAMPLE_HOME%"
for /F "usebackq" %%i in (`dir /s /b *.tccmd`) do call :RunGenerator[] %%i
popd

exit /b %SAVED_ERROR_LEVEL%

:RunGenerator[]
    "%CCK_ROOT%\bin\ccktc.exe" "%~1"
    if %errorlevel% neq 0 set SAVED_ERROR_LEVEL=%errorlevel% 
    exit /b 0
