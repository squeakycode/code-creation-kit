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

echo Creating version info
call create_version_info.cmd nowait
if %errorlevel% neq 0 goto :exit_failure

echo Creating workspace
call create_workspace.cmd "" nowait
if %errorlevel% neq 0 goto :exit_failure

echo Building solution
call build_solution.cmd ..\cck.sln
if %errorlevel% neq 0 goto :exit_failure

rem set root to tools folder to use defined text compiler
set CCK_ROOT=%~dp0\..\output

echo Creating and installing syntax highlighter language definition
rem The subfolders themes and langDefs of %ProgramFiles%\WinHighlight
rem (see highlight above) need to writable under Vista and Windows 7, because
rem the syntax definitions of the text compiler need to be placed there.
rem This is a problem with activated User Account Control (UAC).
rem A possible solution for this is to change the access rights to these folders.
call create_and_install_sytaxhighlighter_langdef.cmd nowait
if %errorlevel% neq 0 goto :exit_failure

echo Rebuilding documentation
pushd ..\docsource
call _rebuild_documentation.cmd nowait
if %errorlevel% neq 0 popd & goto :exit_failure
popd

echo Making Tables
call make_tables.cmd nowait
if %errorlevel% neq 0 goto :exit_failure

echo Making Examples
call make_examples.cmd nowait
if %errorlevel% neq 0 goto :exit_failure

echo Making Setup
call make_setup.cmd
if %errorlevel% neq 0 goto :exit_failure

echo Building examples to check for errors
call build_solution.cmd ..\output\examples_build_test\examples.sln
if %errorlevel% neq 0 goto :exit_failure

echo Make Release successful...
exit /b 0

:exit_failure
echo Make Release failed...
if "%1" equ "" pause
exit /b %errorlevel%