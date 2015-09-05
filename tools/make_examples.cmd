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

echo Removing everything from output examples folder
if exist ..\output\examples rmdir /s /q ..\output\examples
if exist ..\output\examples_build_test rmdir /s /q ..\output\examples_build_test
if not exist ..\output mkdir ..\output
if not exist ..\output\examples mkdir ..\output\examples

echo Copying example files
xcopy /s /e /q ..\examples ..\output\examples\
if %errorlevel% neq 0 goto :exit_failure
copy ..\output\examples\documented_state_machine\cd_player\run_doxygen.cmd ..\output\examples\documented_state_machine\cd_player_extended
if %errorlevel% neq 0 goto :exit_failure


echo Removing everything from output mpc folder
if not exist ..\output\mpc mkdir ..\output\mpc

echo Copying mpc files
copy ..\mpc\tccmd.mpb ..\output\mpc\tccmd.mpb
if %errorlevel% neq 0 goto :exit_failure

echo Triggering generation of examples
call run_example_generation.cmd ..\output\examples 
if %errorlevel% neq 0 goto :exit_failure

echo Remove syntax highlighter output
if not exist "..\output\examples\create_syntax_highlighter\CCK TC-Commands.INI" goto :exit_failure
del "..\output\examples\create_syntax_highlighter\CCK TC-Commands.INI"
if %errorlevel% neq 0 goto :exit_failure
if not exist "..\output\examples\create_syntax_highlighter\CCK Template.INI" goto :exit_failure
del "..\output\examples\create_syntax_highlighter\CCK Template.INI"
if %errorlevel% neq 0 goto :exit_failure

echo Copying example files for test build
xcopy /s /e /q ..\output\examples ..\output\examples_build_test\

echo Creating workspace vc71
pushd ..\output\examples\mpc_integration
call create_workspace.cmd vc71 nowait
popd
if %errorlevel% neq 0 goto :exit_failure

echo Creating workspace for test build
pushd ..\output\examples_build_test\mpc_integration
call create_workspace.cmd vc9 nowait
popd
if %errorlevel% neq 0 goto :exit_failure

echo Examples successfully created...
exit /b 0

:exit_failure
rem if not disabled show failed state and wait
if "%1" equ "" echo Example creation failed...
if "%1" equ "" pause
exit /b 1
