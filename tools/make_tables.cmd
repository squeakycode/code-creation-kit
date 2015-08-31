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

echo Removing everything from output tables folder
if exist ..\output\tables rmdir /s /q ..\output\tables
if not exist ..\output mkdir ..\output
if not exist ..\output\tables mkdir ..\output\tables

echo Generate table files
"%CCK_ROOT%\bin\ccktc.exe" tables.tccmd
if %errorlevel% neq 0 goto :exit_failure

echo Tables successfully created... 
exit /b 0

:exit_failure
rem if not disabled show failed state and wait
if "%1" equ "" echo Example creation failed...
if "%1" equ "" pause
exit /b 1
