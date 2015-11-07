@echo off
rem  Copyright (c) 2011-2015 Andreas Gau
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

set PROJECT_DIRECTORY=..\implementation
set PROJECT_NAME=implementation
set USE_GENERATOR=1

if defined USE_GENERATOR if not defined CCK_ROOT echo Environment variable CCK_ROOT is not set
if defined USE_GENERATOR if not defined CCK_ROOT exit /b 1

call :CreateFile[] > "%PROJECT_DIRECTORY%\%PROJECT_NAME%.mpc"
exit /b %ERRORLEVEL%

:CreateFile[]
echo project(*) : base, boost_base, tccmd {

if defined USE_GENERATOR echo    CCK_Text_Compiler_Command_Files {
if defined USE_GENERATOR echo        conditional(nmake, vc6, vc7, vc71, vc8, vc9, vc10, vc11) {
if defined USE_GENERATOR call :ListCommandFiles[]
if defined USE_GENERATOR echo        }
if defined USE_GENERATOR echo     }
echo }
exit /b %ERRORLEVEL% 

:ListCommandFiles[]
for /F "usebackq" %%i in (`dir /b "%PROJECT_DIRECTORY%\*.tccmd"`) do "%CCK_ROOT%\bin\ccktc.exe" "%PROJECT_DIRECTORY%\%%i" -d mpc & echo. 
exit /b %ERRORLEVEL%
