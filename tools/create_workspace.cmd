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

set IDE_TYPE=%~1

set MPC_DEPENDENCY_COMBINED_STATIC_LIBRARY=Yes

set PrgFiles=%ProgramFiles%
if defined ProgramFiles(x86) set PrgFiles=%ProgramFiles(x86)%

if not defined IDE_TYPE if exist "%PrgFiles%\Microsoft Visual Studio 14.0" set IDE_TYPE=vc14
if not defined IDE_TYPE if exist "%PrgFiles%\Microsoft Visual Studio 12.0" set IDE_TYPE=vc12
if not defined IDE_TYPE if exist "%PrgFiles%\Microsoft Visual Studio 11.0" set IDE_TYPE=vc11
if not defined IDE_TYPE if exist "%PrgFiles%\Microsoft Visual Studio 10.0" set IDE_TYPE=vc10
if not defined IDE_TYPE if exist "%PrgFiles%\Microsoft Visual Studio 9.0" set IDE_TYPE=vc9
if not defined IDE_TYPE if exist "%PrgFiles%\Microsoft Visual Studio 8" set IDE_TYPE=vc8

pushd ..
%MPC_ROOT%\mwc.pl -type %IDE_TYPE% -include mpc -features boost=1 -value_template add_references=1 -static cck.mwc
if %errorlevel% neq 0 popd & goto :exit_failure
popd

echo Create workspace successful...
exit /b 0

:exit_failure
echo Create workspace failed...
if "%1" equ "" pause
exit /b %errorlevel%