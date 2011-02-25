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

call create_test_mpcs.cmd implementation
if %errorlevel% neq 0 goto :exit_failure
call create_project_mpc.cmd ..\implementation implementation use_generator
if %errorlevel% neq 0 goto :exit_failure

pushd ..
%MPC_ROOT%\mwc.pl -type vc9 -include mpc -features boost=1 -static cck.mwc
if %errorlevel% neq 0 popd & goto :exit_failure
rem %MPC_ROOT%\mwc.pl -type make -include mpc -features boost=1 -static cck.mwc
popd

echo Create workspace successful...
exit /b 0

:exit_failure
echo Create workspace failed...
if "%1" equ "" pause
exit /b %errorlevel%