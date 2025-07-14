Attribute VB_Name = "CsvSaverModule"
'Copyright (c) 2011-2025, Andreas Gau
'SPDX-License-Identifier: BSD-3-Clause

Option Explicit

'saves the passed workbook to csv files
'advantages:
'   save all sheets not just the first
'   prevent the workbook from changing it's type while saving
'   can be hooked into BeforeSave event to save the csv files conveniently when saving the workbook
'each sheet is saved to a separate csv file
'the csv files are saved in the folder where the workbook is located
'naming for the first csv file/sheet: [workbookname].csv
'naming for the next csv files/sheets: [workbookname].[sheetname].csv
'if the sheetname contains special chars they are replaced with '_'
'WARNING, ANY FILES WITH THE NAME OF THE PRODUCED CSV FILES IN THE WORKBOOKS FOLDER ARE OVERWRITTEN WITHOUT NOTICE
Public Sub SaveWorkbookToCsvFiles(ByRef myWorkbook As Workbook)
    
    'declare used vars
    Dim fileSystem As Object
    Dim textfile As Object
    Dim sheetIndex As Long
    Dim rowIndex As Long
    Dim columnsIndex As Integer
    Dim temp As String
    Dim filename As String
    Dim postfix As String
    Dim cellText As String
    Dim rowCount As Long
    Dim columnCount As Long
    
    'if the workbook has not been saved abort CSV-file saving
    If myWorkbook.Path = "" Then
        Exit Sub
    End If
    
    'remove .xls postfix from workbook name
    temp = StrReverse(myWorkbook.Name)
    filename = StrReverse(Right(temp, Len(temp) - InStr(temp, ".")))
    temp = ""
   
    'create filesystem object
    Set fileSystem = CreateObject("Scripting.FileSystemObject")
        
    'iterate over the workbook sheets an save each to a csv file
    For sheetIndex = 1 To myWorkbook.Sheets.Count

        'attach table name to all but the first sheet
        If sheetIndex > 1 Then
            postfix = "." & RemoveSpecialChars(myWorkbook.Sheets(sheetIndex).Name)
        End If
   
        'create a text new file
        Set textfile = fileSystem.CreateTextFile(myWorkbook.Path & "\" & filename & postfix & ".csv", True)
        
        'get row and column count
        columnCount = myWorkbook.Sheets(sheetIndex).UsedRange.Columns.Count + myWorkbook.Sheets(sheetIndex).UsedRange.Columns.Column - 1
        rowCount = myWorkbook.Sheets(sheetIndex).UsedRange.Rows.Count + myWorkbook.Sheets(sheetIndex).UsedRange.Rows.Row - 1
        
        'for all rows
        For rowIndex = 1 To rowCount
           'for all columns
           For columnsIndex = 1 To columnCount
              'get cell text
              cellText = myWorkbook.Sheets(sheetIndex).Cells(rowIndex, columnsIndex)
              'put in double quotes if newline, semicolon, or double quote is contained in the cell text
              If InStr(cellText, ";") Or InStr(cellText, "" + Chr(10)) Or InStr(cellText, """") Then
                  temp = temp & """" & Replace(Replace(cellText, """", """"""), vbLf, vbCrLf) & """"
              Else
                  temp = temp & cellText
              End If
              'add the delimiter if not last column
              If columnsIndex < columnCount Then
                  temp = temp & ";"
              End If
          'next columns
          Next columnsIndex
          'if line not empty write it
          If temp <> "" Then
             textfile.writeline temp
             temp = ""
          End If
        'next row
        Next rowIndex
        
        'free used objects
        Set textfile = Nothing
    Next sheetIndex

    'free used objects
    Set fileSystem = Nothing
End Sub


'Same as SaveWorkbookToCsvFiles, but stores the text as UTF8
'WARNING, ANY FILES WITH THE NAME OF THE PRODUCED CSV FILES IN THE WORKBOOKS FOLDER ARE OVERWRITTEN WITHOUT NOTICE
Public Sub SaveWorkbookToCsvFilesUtf8(ByRef myWorkbook As Workbook)
    
    'declare used vars
    Dim fileSystem As Object
    Dim textfile As Object
    Dim sheetIndex As Long
    Dim rowIndex As Long
    Dim columnsIndex As Integer
    Dim temp As String
    Dim filename As String
    Dim postfix As String
    Dim cellText As String
    Dim rowCount As Long
    Dim columnCount As Long
    Dim stream As Object
    
    'if the workbook has not been saved abort CSV-file saving
    If myWorkbook.Path = "" Then
        Exit Sub
    End If
    
    'remove .xls postfix from workbook name
    temp = StrReverse(myWorkbook.Name)
    filename = StrReverse(Right(temp, Len(temp) - InStr(temp, ".")))
    temp = ""
   
    'create filesystem object
    Set fileSystem = CreateObject("Scripting.FileSystemObject")
        
    'iterate over the workbook sheets and save each to a csv file
    For sheetIndex = 1 To myWorkbook.Sheets.Count

        'attach table name to all but the first sheet
        If sheetIndex > 1 Then
            postfix = "." & RemoveSpecialChars(myWorkbook.Sheets(sheetIndex).Name)
        End If

        ' Create a new ADODB Stream for UTF-8 encoding
        Set stream = CreateObject("ADODB.Stream")
        stream.Type = 2 ' Specify stream type - we want to write text
        stream.Charset = "utf-8" ' Specify charset
        stream.Open
        
        'get row and column count
        columnCount = myWorkbook.Sheets(sheetIndex).UsedRange.Columns.Count + myWorkbook.Sheets(sheetIndex).UsedRange.Columns.Column - 1
        rowCount = myWorkbook.Sheets(sheetIndex).UsedRange.Rows.Count + myWorkbook.Sheets(sheetIndex).UsedRange.Rows.Row - 1
        
        'for all rows
        For rowIndex = 1 To rowCount
           'reset temp for each row
           temp = ""
           'for all columns
           For columnsIndex = 1 To columnCount
              'get cell text
              cellText = myWorkbook.Sheets(sheetIndex).Cells(rowIndex, columnsIndex)
              'put in double quotes if newline, semicolon, or double quote is contained in the cell text
              If InStr(cellText, ";") Or InStr(cellText, vbLf) Or InStr(cellText, """") Then
                  temp = temp & """" & Replace(Replace(cellText, """", """"""), vbLf, vbCrLf) & """"
              Else
                  temp = temp & cellText
              End If
              'add the delimiter if not last column
              If columnsIndex < columnCount Then
                  temp = temp & ";"
              End If
          'next columns
          Next columnsIndex
          
          'if line not empty write it with \r\n line ending
          If temp <> "" Then
             stream.WriteText temp & vbCrLf
          End If
        'next row
        Next rowIndex
        
        ' Save the stream to a file
        stream.SaveToFile myWorkbook.Path & "\" & filename & postfix & ".csv", 2 ' 2 = overwrite
        
        'free used objects
        stream.Close
        Set stream = Nothing
    Next sheetIndex

    'free used objects
    Set fileSystem = Nothing
End Sub


'replaces all chars forbidden by the windows filesystem with '_'
Private Function RemoveSpecialChars(filename As String) As String
    Const specialChars As String = "<>:""/\|?*"
    Dim i As Integer
    RemoveSpecialChars = filename
    'loop over the special characters and replace them
    For i = 1 To Len(specialChars)
        RemoveSpecialChars = Replace(RemoveSpecialChars, Mid(specialChars, i, 1), "_")
    Next i
End Function


'save active workbook, usable for toolbar command button
Public Sub SaveActiveWorkbookToCsvFiles()
    SaveWorkbookToCsvFiles ActiveWorkbook
End Sub


'save active workbook, usable for toolbar command button
Public Sub SaveActiveWorkbookToCsvFilesUtf8()
    SaveWorkbookToCsvFilesUtf8 ActiveWorkbook
End Sub

