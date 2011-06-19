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

rem set root to tools folder to use defined text compiler
rem set CCK_ROOT=%~dp0

echo Creating version info
call create_version_info.cmd nowait
if %errorlevel% neq 0 goto :exit_failure

echo Creating workspace
call create_workspace.cmd "" nowait
if %errorlevel% neq 0 goto :exit_failure

echo Building solution
call build_solution.cmd ..\cck.sln
if %errorlevel% neq 0 goto :exit_failure

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