;   Copyright (C) 2011, Andreas Gau
;
;   This file is part of the code-creation-kit.
;
;   The code-creation-kit is free software: you can redistribute it and/or modify
;   it under the terms of the GNU General Public License as published by
;   the Free Software Foundation, either version 2 of the License, or
;   (at your option) any later version.
;
;   The code-creation-kit is distributed in the hope that it will be useful,
;   but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;   GNU General Public License for more details.
;
;   You should have received a copy of the GNU General Public License
;   along with the code-creation-kit. If not, see <http://www.gnu.org/licenses/>.

#define PackageName "code-creation-kit"
#define PackageCopyright ReadIni("..\..\version\version.gen.ini", "version", "copyrightinfo")
#define PackageVersion ReadIni("..\..\version\version.gen.ini", "version", "versiontext")
#define PackageReleaseType ReadIni("..\..\version\version.gen.ini", "version", "versiontype")

[Messages]
SetupAppTitle={#PackageName} {#PackageVersion}

[Setup]
OutputDir=current_release
VersionInfoVersion={#PackageVersion}
VersionInfoCopyright={#PackageCopyright}
Compression=lzma/ultra
VersionInfoProductName={#PackageName}
VersionInfoProductVersion={#PackageVersion}
AppCopyright={#PackageCopyright}
AppName={#PackageName}
LicenseFile=..\..\license\gpl\gpl-2.0.txt
ChangesAssociations=true
ChangesEnvironment=true
AppVerName={#PackageName} {#PackageReleaseType} {#PackageVersion}
DefaultDirName={pf}\{#PackageName}
DefaultGroupName={#PackageName}
OutputBaseFilename={#PackageName}_{#PackageVersion}_x86
AppPublisher=Andreas Gau
AppVersion={#PackageVersion}
UninstallDisplayIcon={app}\bin\tccmd.ico

[Types]
Name: full; Description: Full installation
Name: compact; Description: Compact installation
Name: custom; Description: Custom installation; Flags: iscustom

[Components]
Name: program; Description: Program Files; Types: full compact custom; Flags: fixed
Name: help; Description: Help Files; Types: full compact custom
Name: excel_addin; Description: CSV-Saver Excel Add-In; Types: full custom
Name: examples; Description: Examples; Types: full custom
Name: support; Description: Support Files; Types: full custom
;Name: pspad; Description: PSPad Syntax Highlighter; Types: full custom

[Files]
;bin
Source: ..\..\output\bin\ccktc.exe; DestDir: {app}\bin; Flags: ignoreversion overwritereadonly uninsremovereadonly; Components: program
Source: ..\..\icons\explosion.ico; DestDir: {app}\bin; Flags: ignoreversion uninsremovereadonly; DestName: tccmd.ico; Components: program
;html
Source: ..\..\output\documentation\*.*; DestDir: {app}\html; Flags: ignoreversion overwritereadonly uninsremovereadonly recursesubdirs; Components: help
;add_ins
Source: ..\..\addins\Code-Creation-Kit_CSV-Saver.xla; DestDir: {app}\add_ins; Flags: ignoreversion overwritereadonly uninsremovereadonly; Components: excel_addin
Source: ..\..\addins\ExcelAddinInstaller.exe; DestDir: {app}\add_ins; Flags: ignoreversion overwritereadonly uninsremovereadonly; Components: excel_addin
;mpc
Source: ..\..\output\mpc\*.*; DestDir: {app}\mpc; Flags: ignoreversion overwritereadonly uninsremovereadonly; Components: support or examples
;tables
Source: ..\..\output\tables\*.*; DestDir: {app}\tables; Flags: ignoreversion overwritereadonly uninsremovereadonly; Components: support or examples
;examples
Source: ..\..\output\examples\*.*; DestDir: {app}\examples; Flags: ignoreversion overwritereadonly uninsremovereadonly recursesubdirs; Components: examples
;pspad highlighter
;Source: ..\..\output\examples\create_syntax_highlighter\*.ini; DestDir: {app}\PSPad editor\Syntax; Components: pspad

[Registry]
;set environment
Root: HKLM; Subkey: SYSTEM\CurrentControlSet\Control\Session Manager\Environment; ValueType: string; ValueName: CCK_ROOT; ValueData: {app}; Flags: uninsdeletevalue dontcreatekey; Components: program

;add text compiler command file association
Root: HKCR; Subkey: .tccmd; ValueType: string; ValueName: ; ValueData: CodeCreationKit.TextCompiler.CommandFile; Flags: uninsdeletevalue; Components: program
Root: HKCR; Subkey: CodeCreationKit.TextCompiler.CommandFile; ValueType: string; ValueName: ; ValueData: code-creation-kit Text Compiler Command File; Flags: uninsdeletekey; Components: program
Root: HKCR; Subkey: CodeCreationKit.TextCompiler.CommandFile\DefaultIcon; ValueType: string; ValueName: ; ValueData: {app}\bin\tccmd.ico; Components: program
Root: HKCR; Subkey: CodeCreationKit.TextCompiler.CommandFile\shell\open\command; ValueType: string; ValueName: ; ValueData: """{app}\bin\ccktc.exe"" ""%1"" -p"; Components: program
Root: HKCR; Subkey: CodeCreationKit.TextCompiler.CommandFile\shell\edit\command; ValueType: string; ValueName: ; ValueData: """{win}\notepad.exe"" ""%1"""; Components: program
Root: HKCR; Subkey: CodeCreationKit.TextCompiler.CommandFile\shell\Dependencies\command; ValueType: string; ValueName: ; ValueData: """{app}\bin\ccktc.exe"" ""%1"" -p -d vs"; Components: program

[Icons]
Name: {group}\CCK Help; Filename: {app}\html\index.html; Components: help
Name: {group}\Examples; Filename: {app}\examples; Components: examples
Name: {group}\{cm:UninstallProgram, {#PackageName}}; Filename: {uninstallexe}

[Run]
Filename: {app}\add_ins\ExcelAddinInstaller.exe; Parameters: -Code-Creation-Kit CSV-Saver; Flags: runminimized runhidden; Components: excel_addin
Filename: {app}\add_ins\ExcelAddinInstaller.exe; Parameters: +{app}\add_ins\Code-Creation-Kit_CSV-Saver.xla; Flags: runminimized runhidden; Components: excel_addin

[UninstallRun]
Filename: {app}\add_ins\ExcelAddinInstaller.exe; Parameters: -Code-Creation-Kit CSV-Saver; Flags: runminimized runhidden; Components: excel_addin

[Code]
//checks for installed versions
function InitializeSetup(): Boolean;
var
  uninstaller: String;
  ErrorCode: Integer;
begin
	Result := true;
	if RegKeyExists(HKEY_LOCAL_MACHINE, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{#PackageName}_is1') then
    begin

        if MsgBox('A previous version of {#PackageName} is already installed on your computer. Do you wish to uninstall the previous setup and continue this installation?', mbConfirmation, MB_YESNO) = IDNO then
            begin
               Result := False;
            end
        else
			begin
                RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{#PackageName}_is1', 'UninstallString', uninstaller);

                ShellExec('open', uninstaller, '/SILENT', '', SW_HIDE, ewWaitUntilTerminated, ErrorCode);

                if RegKeyExists(HKEY_LOCAL_MACHINE, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{#PackageName}_is1') then
					begin
					Result := false;
					end
                else
					begin
					Result := true;
					end
			end
    end;
end;
