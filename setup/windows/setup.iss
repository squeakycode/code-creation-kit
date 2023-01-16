;  Copyright (c) 2011-2023 Andreas Gau
;  All rights reserved.
;
;  Redistribution and use in source and binary forms, with or without
;  modification, are permitted provided that the following conditions are met:
;      * Redistributions of source code must retain the above copyright
;        notice, this list of conditions and the following disclaimer.
;      * Redistributions in binary form must reproduce the above copyright
;        notice, this list of conditions and the following disclaimer in the
;        documentation and/or other materials provided with the distribution.
;      * Neither the name of the copyright holder nor the
;        names of contributors may be used to endorse or promote products
;        derived from this software without specific prior written permission.
;
;  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
;  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
;  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
;  DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
;  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
;  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
;  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
;  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
;  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
;  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#define PackageName "code-creation-kit"
#define PackageCopyright ReadIni(AddBackslash(SourcePath) + "..\..\version\version.gen.ini", "version", "copyrightinfo")
#define PackageVersion ReadIni(AddBackslash(SourcePath) + "..\..\version\version.gen.ini", "version", "versiontext")
#define PackageVersionRaw ReadIni(AddBackslash(SourcePath) + "..\..\version\version.gen.ini", "version", "versiontextraw")
#define PackageReleaseType ReadIni(AddBackslash(SourcePath) + "..\..\version\version.gen.ini", "version", "versiontype")

[Messages]
SetupAppTitle={#PackageName} {#PackageVersion}

[Setup]
OutputDir=current_release
VersionInfoVersion={#PackageVersionRaw}
VersionInfoCopyright={#PackageCopyright}
Compression=lzma/ultra
VersionInfoProductName={#PackageName}
VersionInfoProductVersion={#PackageVersionRaw}
AppCopyright={#PackageCopyright}
AppName={#PackageName}
LicenseFile=..\..\license\modified_bsd\modified_bsd_license_for_setup.txt
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
;license
Source: ..\..\license\modified_bsd\modified_bsd_license.txt; DestDir: {app}; Flags: ignoreversion uninsremovereadonly; DestName: LICENSE.TXT; Components: program
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

;add instant template file association
Root: HKCR; Subkey: .itpl; ValueType: string; ValueName: ; ValueData: CodeCreationKit.TextCompiler.InstantTemplateFile; Flags: uninsdeletevalue; Components: program
Root: HKCR; Subkey: CodeCreationKit.TextCompiler.InstantTemplateFile; ValueType: string; ValueName: ; ValueData: code-creation-kit Instant Template File; Flags: uninsdeletekey; Components: program
Root: HKCR; Subkey: CodeCreationKit.TextCompiler.InstantTemplateFile\DefaultIcon; ValueType: string; ValueName: ; ValueData: {app}\bin\tccmd.ico; Components: program
Root: HKCR; Subkey: CodeCreationKit.TextCompiler.InstantTemplateFile\shell\open\command; ValueType: string; ValueName: ; ValueData: """{app}\bin\ccktc.exe"" -t ""%1"" -p"; Components: program
Root: HKCR; Subkey: CodeCreationKit.TextCompiler.InstantTemplateFile\shell\edit\command; ValueType: string; ValueName: ; ValueData: """{win}\notepad.exe"" ""%1"""; Components: program


[Icons]
Name: {group}\CCK Help; Filename: {app}\html\index.html; Components: help
Name: {group}\CCK Examples; Filename: {app}\examples; Components: examples
Name: {group}\code-creation-kit Project Home; FileName: "http://sourceforge.net/projects/cck";
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
