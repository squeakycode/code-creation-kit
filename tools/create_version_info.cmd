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

if not defined CCK_ROOT echo Environment variable CCK_ROOT is not set
if not defined CCK_ROOT exit /b 1

SubWCRev .. ..\version\version.svntpl.csv ..\version\version.svngen.csv
if %errorlevel% neq 0 goto :exit_failure
"%CCK_ROOT%\bin\ccktc.exe" ..\version\version.tccmd
if %errorlevel% neq 0 goto :exit_failure

exit /b 0

:exit_failure
echo Version file creation failed...
if "%1" equ "" pause
exit /b %errorlevel%