Module ModSql
    Public conn As New ADODB.Connection
    Public rs As New ADODB.Recordset
    Public addFlag As Boolean
    Public Function OpenCn(ByVal Cip As String) As Boolean
        conn = New ADODB.Connection
        conn.ConnectionTimeout = 25
        conn.Provider = "sqloledb"
        conn.Properties("data source").Value = Cip '服务器的名字
        conn.Properties("initial catalog").Value = "iCDR" '库名"
        conn.Properties("integrated security").Value = "SSPI" '登陆类型
        conn.Properties("user id").Value = "sa"
        conn.Properties("password").Value = "1"
        conn.Open()
    End Function
    Public Function OpenCnAcess(ByVal Cip As String) As Boolean
        conn = New ADODB.Connection
        conn.ConnectionTimeout = 25
        conn.Provider = "Microsoft.Jet.OLEDB.4.0;Data Source=" & Cip
        conn.Open()
    End Function
    Public Function OpenCnAcess2007(ByVal Cip As String) As Boolean
        conn = New ADODB.Connection
        conn.ConnectionTimeout = 25
        conn.Provider = "Microsoft.ACE.OLEDB.12.0;Data Source=" & Cip
        conn.Open()
    End Function
    Public Sub cloCn()
        On Error Resume Next
        If conn.State <> ADODB.ObjectStateEnum.adStateClosed Then conn.Close()
        conn = Nothing
    End Sub
    Public Function openRs(ByVal strsql As String) As Boolean '连接数据库记录集
        rs = New ADODB.Recordset
        With rs
            .let_ActiveConnection(conn)
            .CursorLocation = ADODB.CursorLocationEnum.adUseClient
            .CursorType = ADODB.CursorTypeEnum.adOpenKeyset
            .LockType = ADODB.LockTypeEnum.adLockOptimistic
            .Open(strsql)
        End With
    End Function
    Public Sub cloRs()
        On Error Resume Next
        If rs.State <> ADODB.ObjectStateEnum.adStateClosed Then rs.Clone()
        rs = Nothing
    End Sub
End Module



