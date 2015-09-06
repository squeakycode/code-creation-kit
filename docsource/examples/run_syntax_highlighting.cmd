@echo on
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

set EXAMPLE_HOME=%~1
set SAVED_ERROR_LEVEL=0
if not defined EXAMPLE_HOME exit /b 1

set PrgFiles=%ProgramFiles%
if defined ProgramFiles(x86) set PrgFiles=%ProgramFiles(x86)%

pushd "%EXAMPLE_HOME%"
for /F "usebackq" %%i in (`dir /s /b CDPlayerStatemachine.h`) do call :RunTplHighlighter[] %%i
for /F "usebackq" %%i in (`dir /s /b *.tpl.h`) do call :RunTplHighlighter[] %%i
for /F "usebackq" %%i in (`dir /s /b *.tpl.vb`) do call :RunTplHighlighter[] %%i
for /F "usebackq" %%i in (`dir /s /b *.gen.h`) do call :RunHighlighter[] %%i
for /F "usebackq" %%i in (`dir /s /b *.gen.vb`) do call :RunHighlighter[] %%i
for /F "usebackq" %%i in (`dir /s /b *.example.vb`) do call :RunHighlighter[] %%i
for /F "usebackq" %%i in (`dir /s /b *.tccmd`) do call :RunTcCmdHighlighter[] %%i
popd

exit /b %SAVED_ERROR_LEVEL%

:RunTplHighlighter[]
    "%PrgFiles%\Highlight\highlight" -i "%~1" -o "%~1.html" --inline-css --fragment --style=tpl --syntax=tpl
    if %errorlevel% neq 0 set SAVED_ERROR_LEVEL=%errorlevel%
    "%PrgFiles%\Highlight\highlight" -i "%~1" -o "%~1.rtf" --style=tpl --syntax=tpl --out-format=rtf
    if %errorlevel% neq 0 set SAVED_ERROR_LEVEL=%errorlevel%
    exit /b 0
    
:RunHighlighter[]
    "%PrgFiles%\Highlight\highlight" -i "%~1" -o "%~1.html" --inline-css --fragment --style=edit-msvs2008
    if %errorlevel% neq 0 set SAVED_ERROR_LEVEL=%errorlevel%
    "%PrgFiles%\Highlight\highlight" -i "%~1" -o "%~1.rtf" --style=edit-msvs2008 --out-format=rtf
    if %errorlevel% neq 0 set SAVED_ERROR_LEVEL=%errorlevel%
    exit /b 0
    
:RunTcCmdHighlighter[]
    "%PrgFiles%\Highlight\highlight" -i "%~1" -o "%~1.html" --inline-css --fragment --style=tpl --syntax=tccmd
    if %errorlevel% neq 0 set SAVED_ERROR_LEVEL=%errorlevel%
    "%PrgFiles%\Highlight\highlight" -i "%~1" -o "%~1.rtf" --style=tpl --syntax=tccmd --out-format=rtf
    if %errorlevel% neq 0 set SAVED_ERROR_LEVEL=%errorlevel%
    exit /b 0
    