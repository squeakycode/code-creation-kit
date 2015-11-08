Attribute VB_Name = "MainModule"
Option Explicit On
' Copyright (c) 2011-2015 Andreas Gau
' All rights reserved.
'
' Redistribution And use in source And binary forms, with Or without
' modification, are permitted provided that the following conditions are met:
'     * Redistributions of source code must retain the above copyright
'       notice, this list of conditions And the following disclaimer.
'     * Redistributions in binary form must reproduce the above copyright
'       notice, this list of conditions And the following disclaimer in the
'       documentation And/Or other materials provided with the distribution.
'     * Neither the name of the copyright holder nor the
'       names of contributors may be used to endorse Or promote products
'       derived from this software without specific prior written permission.
'
' THIS SOFTWARE Is PROVIDED BY THE COPYRIGHT HOLDERS And CONTRIBUTORS "AS IS" And
' ANY EXPRESS Or IMPLIED WARRANTIES, INCLUDING, BUT Not LIMITED TO, THE IMPLIED
' WARRANTIES OF MERCHANTABILITY And FITNESS FOR A PARTICULAR PURPOSE ARE
' DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
' DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, Or CONSEQUENTIAL DAMAGES
' (INCLUDING, BUT Not LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS Or SERVICES;
' LOSS OF USE, DATA, Or PROFITS; Or BUSINESS INTERRUPTION) HOWEVER CAUSED And
' ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, Or TORT
' (INCLUDING NEGLIGENCE Or OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
' SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


Private Sub DisplayUsage()
    Con.WriteLine Format("Copyright by Andreas Gau 2010 [Version 1.0]"), True, conStandardError
    Con.WriteLine Format(""), True, conStandardError
    Con.WriteLine Format("Usage:"), True, conStandardError
    Con.WriteLine Format(""), True, conStandardError
    Con.WriteLine Format("Excel AddIn Installation:"), True, conStandardError
    Con.WriteLine Format("+Addin Location"), True, conStandardError
    Con.WriteLine Format("Example: +C:\Program Files\MyApp\MyAddIn.xla"), True, conStandardError
    Con.WriteLine Format(""), True, conStandardError
    Con.WriteLine Format("Excel AddIn Deinstallation:"), True, conStandardError
    Con.WriteLine Format("-Addin Name"), True, conStandardError
    Con.WriteLine Format("Example: -MyAddIn"), True, conStandardError
End Sub

' It is mandatory to call "LINK.EXE /EDIT /SUBSYSTEM:CONSOLE ExcelAddinInstaller.exe"
' before this program can be excuted from the commandline
Public Sub Main()
    
    Dim ExitCode As Integer
    Dim executeQuit As Boolean
    
    Con.Initialize
    
    If Len(Command) > 1 Then
       
       Dim prefix As String
       Dim payload As String
       Dim oXL As Object
       Dim oAddin As Object
       
       prefix = Mid(Command, 1, 1)
       payload = Mid(Command, 2, Len(Command) - 1)

       ' argument given
       If prefix = "+" Or prefix = "-" Then
            
            On Error Resume Next
            
            'try to get the the running excel instance
            Set oXL = GetObject(, "Excel.Application")
            If Err.Number <> 0 Then
                'clear error for creating an instance
                Err.Number = 0
                'create excel instance if excel is not running
                Set oXL = CreateObject("Excel.Application")
                executeQuit = True
            End If
            If Err.Number = 0 Then
                
                'install
                If prefix = "+" Then
                    'add a workbook to make add in manager available if needed
                    If oXL.Workbooks.Count = 0 Then
                        oXL.Workbooks.Add
                    End If
                    If Err.Number = 0 Then
                        
                        'add the addin
                        Set oAddin = oXL.AddIns.Add(payload, True)
                        If Err.Number = 0 Then
                            
                            'set the addin installed
                            oAddin.Installed = True
                            If Err.Number <> 0 Then
                                Con.WriteLine Format("Failed to install AddIn"), True, conStandardError
                                ExitCode = 5
                            End If
                        Else
                            Con.WriteLine Format("Failed to add AddIn"), True, conStandardError
                            ExitCode = 4
                        End If
                        Set oAddin = Nothing
                    Else
                        Con.WriteLine Format("Failed to add Workbook"), True, conStandardError
                        ExitCode = 3
                    End If
                Else ' remove addin
                    oXL.AddIns(payload).Installed = False
                    If Err.Number <> 0 Then
                        Con.WriteLine Format("Failed to remove AddIn"), True, conStandardError
                        ExitCode = 5
                        GoTo exit_label:
                    End If
                End If
            Else
                Con.WriteLine Format("Failed to create Excel Instance"), True, conStandardError
                ExitCode = 2
            End If
            
            'quit excel
            Err.Number = 0
            If (ExitCode > 2 Or ExitCode = 0) And executeQuit Then
                oXL.Quit
            End If
            If Err.Number <> 0 Then
                Con.WriteLine Format("Failed to destroy Excel Instance"), True, conStandardError
                ExitCode = 6
                GoTo exit_label:
            End If
            
            'release object
            Set oXL = Nothing
       Else
            DisplayUsage
       End If
    
    Else
        DisplayUsage
        ExitCode = 1
    End If
    
exit_label:
    Con.ExitCode = 0
End Sub

