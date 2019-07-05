@echo off
rem  Copyright (c) 2011-2019 Andreas Gau
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
