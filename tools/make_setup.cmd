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

set PrgFiles=%ProgramFiles%
if defined ProgramFiles(x86) set PrgFiles=%ProgramFiles(x86)%

pushd ..\setup\windows
"%PrgFiles%\Inno Setup 5\iscc.exe" setup.iss
if %errorlevel% neq 0 popd & exit /b %errorlevel%
popd
exit /b %errorlevel%