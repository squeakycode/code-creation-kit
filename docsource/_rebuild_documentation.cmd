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

@echo off
setlocal enableextensions
setlocal enabledelayedexpansion

if not defined CCK_ROOT echo Environment variable CCK_ROOT is not set
if not defined CCK_ROOT goto :exit_failure

echo Clearing temp folder
if exist temp rmdir /s /q temp
mkdir temp
if %errorlevel% neq 0 goto :exit_failure

echo Copying example tree into temp
mkdir temp\examples
if %errorlevel% neq 0 goto :exit_failure
xcopy /s /e /q examples temp\examples\
if %errorlevel% neq 0 goto :exit_failure
xcopy /s /e /q ..\examples\getting_started_simple temp\examples\getting_started_simple\
if %errorlevel% neq 0 goto :exit_failure
xcopy /s /e /q ..\examples\getting_started_simple_extended temp\examples\getting_started_simple_extended\
if %errorlevel% neq 0 goto :exit_failure
xcopy /s /e /q ..\examples\documented_state_machine temp\examples\documented_state_machine\
if %errorlevel% neq 0 goto :exit_failure

echo Removing everything from documentation folder
if not exist ..\output mkdir ..\output
if not exist ..\output\documentation mkdir ..\output\documentation
if not exist ..\output\documentation\boost mkdir ..\output\documentation\boost
del /q /f ..\output\documentation\*
del /q /f ..\output\documentation\boost\*
if %errorlevel% neq 0 goto :exit_failure

echo Copying boost book resource files to documentation folder
xcopy /q ..\templates\documentation\boostbook\resources\* ..\output\documentation\boost
if %errorlevel% neq 0 goto :exit_failure

echo Triggering generation of examples
call ..\tools\run_example_generation.cmd temp\examples 
if %errorlevel% neq 0 goto :exit_failure

echo Triggering syntax highlighting
call temp\examples\run_syntax_highlighting.cmd temp\examples 
if %errorlevel% neq 0 goto :exit_failure

echo Triggering generation of pages
"%CCK_ROOT%\bin\ccktc.exe" generate_temp_files.tccmd
if %errorlevel% neq 0 goto :exit_failure
"%CCK_ROOT%\bin\ccktc.exe" temp\generate_pages.gen.tccmd
if %errorlevel% neq 0 goto :exit_failure

echo Copying resource files to documentation folder
copy logo.png ..\output\documentation
if %errorlevel% neq 0 goto :exit_failure
copy overview.png ..\output\documentation
if %errorlevel% neq 0 goto :exit_failure
copy state_graph.png ..\output\documentation
if %errorlevel% neq 0 goto :exit_failure

echo Documentation successfully created... 
exit /b 0

:exit_failure
rem if not disabled show failed state and wait
if "%1" equ "" echo Documentation creation failed...
if "%1" equ "" pause
exit /b 1 