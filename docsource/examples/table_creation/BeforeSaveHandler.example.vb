Private Sub Workbook_BeforeSave(ByVal SaveAsUI As Boolean, Cancel As Boolean)
    SaveWorkbookToCsvFiles ThisWorkbook
End Sub
