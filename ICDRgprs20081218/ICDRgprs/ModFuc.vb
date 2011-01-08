
Module ModFuc
    Function RecordsetToDataTable(ByVal adoRS As ADODB.Recordset, ByVal strTable As String)
        adoRS.MoveFirst()
        Dim dt As DataTable
        dt = New DataTable(strTable)
        Dim i As Integer
        Dim strcolname As String
        Dim t As Type
        Dim dr As DataRow
        For i = 0 To adoRS.Fields.Count - 1
            strcolname = adoRS.Fields(i).Name
            t = adoRS.Fields(i).Value.GetType()
            dt.Columns.Add(strcolname, t)
        Next
        While (Not adoRS.EOF)
            dr = dt.NewRow()
            For i = 0 To adoRS.Fields.Count - 1
                dr(i) = adoRS.Fields(i).Value
            Next
            dt.Rows.Add(dr)
            adoRS.MoveNext()
        End While
        Return dt
    End Function
End Module



