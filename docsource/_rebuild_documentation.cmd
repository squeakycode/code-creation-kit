@echo off
rem  Copyright (c) 2011-2023 Andreas Gau
rem  All rights reserved.
rem
rem  Redistribution and use in source and binary forms, with or without
rem  modification, are permitted provided that the following conditions are met:
rem      * Redistributions of source code must retain the above copyright
rem        notice, this list of conditions and the following disclaimer.
rem      * Redistributions in binary form must reproduce the above copyright
rem        notice, this list of conditions and the following disclaimer in the
rem        documentation and/or other materials provided with the distribution.
rem      * Neither the name of the copyright holder nor the
rem        names of contributors may be used to endorse or promote products
rem        derived from this software without specific prior written permission.
rem
rem  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
rem  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
rem  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
rem  DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
rem  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
rem  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
rem  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
rem  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
rem  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
rem  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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