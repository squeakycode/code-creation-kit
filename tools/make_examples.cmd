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

echo Creating workspace vc8
pushd ..\output\examples\mpc_integration
call create_workspace.cmd vc8 nowait
popd
if %errorlevel% neq 0 goto :exit_failure

echo Creating workspace for test build
pushd ..\output\examples_build_test\mpc_integration
call create_workspace.cmd vc14 nowait
popd
if %errorlevel% neq 0 goto :exit_failure

echo Examples successfully created...
exit /b 0

:exit_failure
rem if not disabled show failed state and wait
if "%1" equ "" echo Example creation failed...
if "%1" equ "" pause
exit /b 1
