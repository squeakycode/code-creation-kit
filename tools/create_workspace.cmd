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