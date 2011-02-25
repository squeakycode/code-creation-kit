Imports System

Class [ENTRY]["Class Name"]

    'declaration
    Public m_[ENTRY]["Name"] As [ENTRY.]["[ENTRY]["Type"]"] = [ENTRY]["Default"] '[ENTRY]["Description"][ANY]
    
    Public Function Clip() As UInteger

        'clip max
[MACRO_BEGIN]        If m_[ENTRY]["Name"] > [ENTRY]["Max"] Then
            m_[ENTRY]["Name"] = [ENTRY]["Max"]
            Clip += 1
        End If
[MACRO_END]

        'clip min
        [MACRO_BEGIN][TRIM]
        If m_[ENTRY]["Name"] < [ENTRY]["Min"] Then
            m_[ENTRY]["Name"] = [ENTRY]["Min"]
            Clip += 1
        End If
        [MACRO_END][TRIM]
        
    End Function

End Class
