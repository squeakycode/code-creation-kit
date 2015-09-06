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

rem change path back to where the cmd file resides when run as admin
if /I "%CD%" EQU "%SystemRoot%\System32" cd /d "%~dp0"

set PrgFiles=%ProgramFiles%
if defined ProgramFiles(x86) set PrgFiles=%ProgramFiles(x86)%

"%CCK_ROOT%\bin\ccktc.exe" syntaxhighlighter.tccmd
if %errorlevel% neq 0 goto :exit_failure

move /Y syntaxhighlighter.gen.lang "%PrgFiles%\Highlight\langDefs\tpl.lang"
if %errorlevel% neq 0 goto :exit_failure

move /Y syntaxhighlighter_tccmd.gen.lang "%PrgFiles%\Highlight\langDefs\tccmd.lang"
if %errorlevel% neq 0 goto :exit_failure

rem adds some keywords to the standard vb.lang language definition of Highlight
copy /Y vb.lang "%PrgFiles%\Highlight\langDefs\vb.lang"
if %errorlevel% neq 0 goto :exit_failure

copy /Y syntaxhighlighter.theme "%PrgFiles%\Highlight\themes\tpl.theme"
if %errorlevel% neq 0 goto :exit_failure

echo Create and install syntax highlighter language definition successful...
exit /b 0

:exit_failure
echo Create and install syntax highlighter language definition failed...
if "%1" equ "" pause
exit /b %errorlevel%