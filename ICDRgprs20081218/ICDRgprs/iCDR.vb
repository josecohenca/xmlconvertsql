Imports System.IO
Imports System.Xml
Imports System.Text
Imports System.Management
Imports System.Web.Security
Public Class iCDR
    Dim dts As DataTable
    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        '加密模块开始 
        Dim path As String = Application.StartupPath & "\LAY3-T.lic"
        If Not File.Exists(path) Then
            If Not File.Exists(path) Then
                Me.Close()
                Return
            End If
        End If
        '加密模块检查序列号
        Dim reader As New StreamReader(Path, System.Text.Encoding.GetEncoding("utf-8"))
        serialNum = reader.ReadLine()
        machineNum = reader.ReadLine()
        regNum = reader.ReadLine()
        reader.Close()
        If Not CheckRegHead(machineNum, regNum) Then
            '这儿会对序列号正确性做简单校验，不合格的会有提示。以后再校验出问题就判定为恶意破解，不再有提示，直接修改系统运行结果。 
            MessageBox.Show("序列号不正确，系统将退出，请与软件供应商联系！")
            Me.Close()
        Else
            Me.WindowState = FormWindowState.Maximized
        End If
        '加载数据
        OpenCnAcess2007(Application.StartupPath & "\iCDR.accdb")
        Dim doc As New XmlDocument
        doc.Load(Application.StartupPath & "\CongfigSql.xml")
        Dim node = doc.DocumentElement.SelectSingleNode("//Variable")
        Dim attributeValue As String = node.Attributes(0).Value
        Dim document As New XmlDocument
        document.Load(Application.StartupPath & "\CdrAppTreeSql.xml")
        populateTreeControl(document.DocumentElement, TreeView1.Nodes)
        TreeView1.ExpandAll()
        RadioButton2.Checked = 1
        RadioButton7.Checked = 1
        RadioButton9.Checked = 1
        Me.WindowState = FormWindowState.Maximized
    End Sub
    Private Sub TreeView1_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles TreeView1.Click
        cloRs()
        Try
            Dim Node1 As TreeNode = Me.TreeView1.GetNodeAt(Me.TreeView1.PointToClient(Control.MousePosition))
            TextBox1.Text = Node1.Index + 1
            TextBox2.Text = Node1.Text
            Dim document As New XmlDocument
            document.Load(Application.StartupPath & "\CdrAppTreeSql.xml")
            Dim nodes As XmlNodeList = document.GetElementsByTagName("CdrApp")
            Dim node As XmlNode
            node = Nothing
            For i = 0 To nodes.Count - 1
                If nodes.ItemOf(i).Attributes.ItemOf(0).Value = Node1.Index + 1 Then
                    node = nodes.ItemOf(i)
                End If
            Next i
            If Not Node1 Is Nothing Then
                openRs(node.Attributes.ItemOf(2).Value.ToString)
                dts = RecordsetToDataTable(rs, "CdrApp")
                With Me.DataGridView3
                    .AutoGenerateColumns = True
                    .DataSource = dts.DefaultView
                    .AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells
                    .AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.DisplayedCellsExceptHeaders
                    .BorderStyle = BorderStyle.Fixed3D
                    .EditMode = DataGridViewEditMode.EditOnEnter
                    .GridColor = Color.Blue
                End With
            End If
        Catch exp As Exception
            MessageBox.Show(exp.ToString())
        Finally
            cloRs()
        End Try
    End Sub

    Private Sub RadioButton3_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton3.CheckedChanged
        ToolStripTextBox1.Text = "SELECT TOP 100 * FROM SGSNMO"
    End Sub
    Private Sub RadioButton4_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton4.CheckedChanged
        ToolStripTextBox1.Text = "SELECT TOP 100 * FROM SGSNMT"
    End Sub
    Private Sub RadioButton5_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton5.CheckedChanged
        ToolStripTextBox1.Text = "SELECT TOP 100 * FROM SGSNPDP"
    End Sub
    Private Sub RadioButton6_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton6.CheckedChanged
        ToolStripTextBox1.Text = "SELECT TOP 100 * FROM SGSNPDP"
    End Sub
    Private Sub ListBox1_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ListBox1.SelectedIndexChanged
        '检验注册
        If bRegOK = False Then
            Exit Sub
        End If
        'Asn->Xml
        ToolStripStatusLabel3.Text = "Asn To Xml......"
        Dim DosAsn2Xml As String
        DosAsn2Xml = Application.StartupPath & "\Asn2Xml.exe " _
        & System.IO.Path.GetFullPath(ListBox1.SelectedItem) _
        & "  -o " & Application.StartupPath & "\Asn1OutXml.xml"
        Shell(DosAsn2Xml, AppWinStyle.Hide, True, 60000)
    End Sub
    Private Sub ToolStripButton1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ToolStripButton1.Click
        For i = 1 To ListBox1.Items.Count - 1
            'asn->xml
            ListBox1.SelectedIndex = i
            'xml->csv
            ToolStripButton3.PerformClick()
            'csv->sql
            ToolStripButton4.PerformClick()
        Next i
    End Sub
    Dim serialNum As String
    Dim machineNum As String
    Dim regNum As String
    Dim bRegOK As Boolean = True
    Private Sub 打开OToolStripButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles 打开OToolStripButton.Click
        '检验注册号，把检验结果找一内存保存起来。不要在校验结果代码附近做任何影响程序正常工作的处理，这样不易被跟踪。
        Dim regTail3 As String = GetRegTail3ByMac(serialNum)
        If String.Compare(regTail3, 0, regNum, 12, 17) = 0 Then
            bRegOK = True
        Else
            bRegOK = False
        End If
        '选中相应的工作空间
        话单导入ToolStripMenuItem.PerformClick()
        '把相应文件夹的文件列出
        FolderBrowserDialog1.RootFolder = Environment.SpecialFolder.MyComputer
        FolderBrowserDialog1.SelectedPath = Environment.SpecialFolder.Recent
        FolderBrowserDialog1.Description = "Open ASN.1 File"
        If Me.FolderBrowserDialog1.ShowDialog() = DialogResult.OK Then
            Me.ListBox1.DataSource = IO.Directory.GetFiles(Me.FolderBrowserDialog1.SelectedPath)
        End If
        ToolStripStatusLabel1.Text = "合计" & ListBox1.Items.Count & "文件"
    End Sub

    Private Sub ToolStripButton2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ToolStripButton2.Click
        '选中相应的工作空间
        字段格式ToolStripMenuItem.PerformClick()
        '执行相应的查询
        Dim ds As New DataSet
        Dim fs9 As New FileStream((Application.StartupPath & "\CdrFieldName.xml"), FileMode.Open)
        Dim fs10 As New FileStream((Application.StartupPath & "\CdrAppTreeSql.xml"), FileMode.Open)
        Dim fs11 As New FileStream((Application.StartupPath & "\CdrFieldContent.xml"), FileMode.Open)
        Select Case True
            Case RadioButton9.Checked
                ds.ReadXml(fs9)
            Case RadioButton10.Checked
                ds.ReadXml(fs10)
            Case RadioButton11.Checked
                ds.ReadXml(fs11)
            Case Else
        End Select
        With Me.DataGridView1
            .AutoGenerateColumns = True
            .DataSource = New DataView(ds.Tables(1))
            .AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells
            .AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.DisplayedCellsExceptHeaders
            .BorderStyle = BorderStyle.Fixed3D
            .EditMode = DataGridViewEditMode.EditOnEnter
            .GridColor = Color.Blue
        End With
        fs9.Close()
        fs10.Close()
        fs11.Close()
    End Sub

    Private Sub ToolStripButton3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ToolStripButton3.Click
        '检验注册
        If bRegOK = False Then
            Exit Sub
        End If
        'Xml->Csv
        ToolStripStatusLabel3.Text = "Xml To Csv......"
        Dim DosXml2Txt As String
        DosXml2Txt = Application.StartupPath + "\Xml2Csv Asn1OutXml.xml SGSNPDPRecord.csv SGSNPDPRecord.txt"
        Shell(DosXml2Txt, AppWinStyle.Hide, True, 60000)
        DosXml2Txt = Application.StartupPath + "\Xml2Csv Asn1OutXml.xml GGSNPDPRecord.csv GGSNPDPRecord.txt"
        Shell(DosXml2Txt, AppWinStyle.Hide, True, 60000)
        DosXml2Txt = Application.StartupPath + "\Xml2Csv Asn1OutXml.xml SGSNSMORecord.csv SGSNSMORecord.txt"
        Shell(DosXml2Txt, AppWinStyle.Hide, True, 60000)
        DosXml2Txt = Application.StartupPath + "\Xml2Csv Asn1OutXml.xml SGSNSMTRecord.csv SGSNSMTRecord.txt"
        Shell(DosXml2Txt, AppWinStyle.Hide, True, 60000)
    End Sub

    Private Sub ToolStripButton4_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ToolStripButton4.Click
        '检验注册
        If bRegOK = False Then
            Exit Sub
        End If
        'Txt->Acc
        ToolStripStatusLabel3.Text = "Csv To Sql......"
        Dim i, strsql As Object
        Dim III, JJJ As Short
        Dim JetTemp As String
        Dim aa() As String
        Dim vv(50) As Object
        Dim v(50) As Object
        Dim fd(50) As Object
        Dim fl(3) As Object
        Dim n, j As Integer
        fl(0) = "\SGSNPDPRecord.csv"
        fl(1) = "\GGSNPDPRecord.csv"
        fl(2) = "\SGSNSMORecord.csv"
        fl(3) = "\SGSNSMTRecord.csv"
        On Error Resume Next
        For j = 0 To 3
            If Dir(Application.StartupPath & fl(j)) = "" Then GoTo LnextFile
            III = FreeFile()
            FileOpen(III, Application.StartupPath & fl(j), OpenMode.Input)
            n = 0
            Do While Not EOF(III)
                JetTemp = LineInput(III)
                ToolStripStatusLabel2.Text = "正在处理" & n & "行"
                n = n + 1
                System.Windows.Forms.Application.DoEvents()
                aa = Split(JetTemp, ",")
                Select Case Len(aa(0))
                    Case Is <= 5
                        strsql = ""
                        '把每行数据格式化
                        For i = LBound(aa) To UBound(aa)
                            Select Case vv(i)
                                Case "hextodec"
                                    v(i) = hextodec(aa(i)) 'dec
                                Case "hextostr"
                                    v(i) = hextostr(aa(i)) 'str
                                Case "hextoip"
                                    v(i) = hextoip(aa(i)) 'ip
                                Case "hextonip"
                                    v(i) = hextonip(aa(i)) 'nip
                                Case "hextotbcd"
                                    v(i) = hextotbcd(aa(i)) 'tbcd
                                Case "hextobcd"
                                    v(i) = hextobcd(aa(i)) 'bcd
                                Case "hextotime"
                                    v(i) = hextotime(aa(i)) 'time
                                Case "hextohex"
                                    v(i) = hextohex(aa(i)) 'hex
                                Case "hextoimsi"
                                    v(i) = hextoimsi(aa(i)) 'imsi
                                Case "hextomsdn"
                                    v(i) = hextomsdn(aa(i)) 'msdn
                                Case "hextoqos"
                                    v(i) = hextoqos(aa(i)) 'qos
                                Case "hextootectstr1"
                                    v(i) = hextootectstr1(aa(i)) 'octect string size1
                                Case "hextootectstr2"
                                    v(i) = hextootectstr2(aa(i)) 'octect string size2
                                Case Else
                                    v(i) = aa(i)
                            End Select
                            If i < UBound(aa) Then
                                strsql = strsql & "'" & v(i) & "',"
                            Else
                                strsql = strsql & "'" & v(i) & "'"
                            End If
                        Next i
                        '把每行数据格式化后写入数据库
                        Select Case j
                            Case 0
                                strsql = "insert into sgsnpdp values(" & strsql & ")"
                            Case 1
                                strsql = "insert into ggsnpdp values(" & strsql & ")"
                            Case 2
                                strsql = "insert into sgsnmo values(" & strsql & ")"
                            Case 3
                                strsql = "insert into sgsnmt values(" & strsql & ")"
                        End Select
                        JJJ = JJJ + 1
                        openRs(strsql)
                        cloRs()
                    Case Is > 5
                        strsql = ""
                        '创建数据表字段
                        For i = LBound(aa) To UBound(aa)
                            '字段名称
                            Dim document As New XmlDocument
                            document.Load(Application.StartupPath & "\CdrFieldName.xml")
                            Dim nodes As XmlNodeList = document.GetElementsByTagName("GsnFieldName")
                            Dim node As XmlNode
                            node = Nothing
                            For x = 0 To nodes.Count - 1
                                If nodes.ItemOf(x).Attributes.ItemOf(0).Value = aa(i) Then
                                    node = nodes.ItemOf(x)
                                End If
                            Next x
                            '字段名称
                            If node Is Nothing Then
                                fd(i) = "F" & aa(i)
                                vv(i) = "hextohex"
                            Else
                                fd(i) = node.Attributes.ItemOf(3).Value.ToString
                                vv(i) = node.Attributes.ItemOf(2).Value.ToString
                            End If
                            If i < UBound(aa) Then
                                'strsql = strsql & "[" & fd(i) & "]" & " [nvarchar](255),"
                                strsql = strsql & "[" & fd(i) & "]" & " TEXT(100),"
                            Else
                                'strsql = strsql & "[" & fd(i) & "]" & " [nvarchar](255)"
                                strsql = strsql & "[" & fd(i) & "]" & " TEXT(100)"
                            End If
                        Next i
                        '创建数据表
                        If Me.RadioButton1.Checked = 1 Or Me.RadioButton2.Checked = 0 Then
                            Select Case j
                                Case 0
                                    'openRs("IF EXISTS (SELECT * FROM sys.tables WHERE  sys.tables.name = N'sgsnpdp') drop table sgsnpdp")
                                    'strsql = "create table sgsnpdp (" & strsql & " ) ON [PRIMARY]"
                                    strsql = "create table sgsnpdp (" & strsql & " ) "
                                Case 1
                                    'openRs("IF EXISTS (SELECT * FROM sys.tables WHERE  sys.tables.name = N'ggsnpdp') drop table ggsnpdp")
                                    'strsql = "create table ggsnpdp (" & strsql & " ) ON [PRIMARY]"
                                    strsql = "create table ggsnpdp (" & strsql & " ) "
                                Case 2
                                    'openRs("IF EXISTS (SELECT * FROM sys.tables WHERE  sys.tables.name = N'sgsnpdp') drop table sgsnmo")
                                    'strsql = "create table sgsnmo (" & strsql & " ) ON [PRIMARY]"
                                    strsql = "create table sgsnmo (" & strsql & " ) "
                                Case 3
                                    'openRs("IF EXISTS (SELECT * FROM sys.tables WHERE  sys.tables.name = N'sgsnpdp') drop table sgsnmt")
                                    'strsql = "create table sgsnmt (" & strsql & " ) ON [PRIMARY]"
                                    strsql = "create table sgsnmt (" & strsql & " ) "
                            End Select
                        End If
                        openRs(strsql)
                        cloRs()
                    Case Else
                End Select
            Loop
            FileClose(III)
LnextFile:
        Next j

        If RadioButton7.Checked Then
            For i = 0 To 3
                Dim xFile1 As New FileInfo(Application.StartupPath & fl(i))
                xFile1.Delete()
            Next i
        End If

    End Sub
    Private Sub ToolStripButton5_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ToolStripButton5.Click
        '选中相应的工作空间
        话单分析ToolStripMenuItem.PerformClick()
        '执行查询填写数据到表
        cloRs()
        Try
            openRs(ToolStripTextBox1.Text)
            dts = RecordsetToDataTable(rs, 2)
            With Me.DataGridView2
                .AutoGenerateColumns = True
                .DataSource = dts.DefaultView
                .AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells
                .AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.DisplayedCellsExceptHeaders
                .BorderStyle = BorderStyle.Fixed3D
                .EditMode = DataGridViewEditMode.EditOnEnter
                .GridColor = Color.Blue
            End With
        Catch exp As Exception
            MessageBox.Show(exp.ToString())
        Finally
            cloRs()
        End Try
    End Sub
    Private Sub ToolStripButton6_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ToolStripButton6.Click
        '选中相应的工作空间
        字段设置SToolStripMenuItem.PerformClick()
        '打开话单导入字段设置
        Dim dlgOpenfile As New OpenFileDialog
        Dim strFileFullName As String
        dlgOpenfile.Filter = "txt   files   (*.txt)|*.txt|All   files   (*.*)|*.*"
        dlgOpenfile.Title = "Open"
        dlgOpenfile.ShowDialog()
        dlgOpenfile.RestoreDirectory = True
        If dlgOpenfile.FileName <> "" Then
            strFileFullName = dlgOpenfile.FileName
            RichTextBox1.LoadFile(strFileFullName, RichTextBoxStreamType.PlainText)
        End If
    End Sub
    Private Sub ToolStripMenuItem1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ToolStripMenuItem1.Click
        打开OToolStripButton.PerformClick()
    End Sub
    Private Sub ToolStripMenuItem2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ToolStripMenuItem2.Click
        ToolStripButton1.PerformClick()
    End Sub
    Private Sub ToolStripMenuItem3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ToolStripMenuItem3.Click
        ToolStripButton3.PerformClick()
    End Sub
    Private Sub ToolStripMenuItem4_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ToolStripMenuItem4.Click
        ToolStripButton4.PerformClick()
    End Sub
    Private Sub ToolStripMenuItem5_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ToolStripMenuItem5.Click
        ToolStripButton5.PerformClick()
    End Sub
    Private Sub ToolStripMenuItem6_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ToolStripMenuItem6.Click
        ToolStripButton2.PerformClick()
    End Sub
    Private Sub 打开xmlcsv设置ToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles 打开xmlcsv设置ToolStripMenuItem.Click
        ToolStripButton6.PerformClick()
    End Sub
    Private Sub 退出系统EToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles 退出系统EToolStripMenuItem.Click
        Process.GetCurrentProcess().CloseMainWindow()
    End Sub

    Private Sub 话单导入ToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles 话单导入ToolStripMenuItem.Click
        For i = 0 To 4
            If TabControl1.TabPages(i).Name = "话单导入" Then
                TabControl1.SelectedIndex = i
            End If
        Next
    End Sub

    Private Sub 话单分析ToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles 话单分析ToolStripMenuItem.Click
        For i = 0 To 4
            If TabControl1.TabPages(i).Name = "话单分析" Then
                TabControl1.SelectedIndex = i
            End If
        Next
    End Sub

    Private Sub 话单应用ToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles 话单应用ToolStripMenuItem.Click
        For i = 0 To 4
            If TabControl1.TabPages(i).Name = "话单应用" Then
                TabControl1.SelectedIndex = i
            End If
        Next
    End Sub

    Private Sub 字段格式ToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles 字段格式ToolStripMenuItem.Click
        For i = 0 To 4
            If TabControl1.TabPages(i).Name = "字段格式" Then
                TabControl1.SelectedIndex = i
            End If
        Next
    End Sub

    Private Sub 字段设置SToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles 字段设置SToolStripMenuItem.Click
        For i = 0 To 4
            If TabControl1.TabPages(i).Name = "字段设置" Then
                TabControl1.SelectedIndex = i
            End If
        Next
    End Sub

    '加密模块开始 
    '#region 真正的注册号检验代码，直接拷贝到需要校验的类中做为私有变量，不要单独打包成dll和做为公共变量。 
    ''' <summary> 
    ''' 校验注册码头两端是否正确 
    ''' </summary> 
    ''' <param name="machineCode">机器码</param> 
    ''' <param name="regCode">注册码</param> 
    ''' <returns></returns> 
    Private Function CheckRegHead(ByVal machineCode As String, ByVal regCode As String) As Boolean
        Dim regHead2 As String = GetRegHead2(machineCode)
        Return String.Compare(regHead2, 0, regCode, 0, 9) = 0
    End Function

    ''' <summary> 
    ''' 通过网卡号和序列号重新获取注册码后三段。此函数给程序用来抗破解之用 
    ''' </summary> 
    ''' <returns></returns> 
    Private Function GetRegTail3ByMac(ByVal seriesNum As String) As String
        Dim machineCode As String = GetMachineCode(seriesNum)
        Return GetRegTail3(machineCode)
    End Function

    ''' <summary> 
    ''' 获取注册码的后三段。此函数不能流失出去 
    ''' </summary> 
    ''' <param name="machineCode"></param> 
    ''' <returns></returns> 
    Private Function GetRegTail3(ByVal machineCode As String) As String
        Dim regTail3 As String
        regTail3 = FormsAuthentication.HashPasswordForStoringInConfigFile(machineCode, "MD5")
        regTail3 = FormsAuthentication.HashPasswordForStoringInConfigFile(regTail3, "SHA1")
        regTail3 = FormsAuthentication.HashPasswordForStoringInConfigFile(regTail3, "SHA1")
        regTail3 = FormsAuthentication.HashPasswordForStoringInConfigFile(regTail3, "MD5")
        regTail3 = FormsAuthentication.HashPasswordForStoringInConfigFile(regTail3, "SHA1")
        regTail3 = FormsAuthentication.HashPasswordForStoringInConfigFile(regTail3, "MD5")
        regTail3 = FormsAuthentication.HashPasswordForStoringInConfigFile(regTail3, "MD5")

        regTail3 = GetCode(regTail3, 3)
        Return regTail3
    End Function
    ''' <summary> 
    ''' 获取注册码的头两端。用来做注册码正确性的简单判断，判断失败会给提示。 
    ''' </summary> 
    ''' <param name="machineCode"></param> 
    ''' <returns></returns> 
    Private Function GetRegHead2(ByVal machineCode As String) As String
        Dim regHead2 As String = FormsAuthentication.HashPasswordForStoringInConfigFile(machineCode, "MD5")
        regHead2 = GetCode(regHead2, 2)
        Return regHead2
    End Function

    ''' <summary> 
    ''' 获取机器码 
    ''' </summary> 
    ''' <param name="seriesNum"></param> 
    ''' <returns>带“-”的四段数字</returns> 
    Private Function GetMachineCode(ByVal seriesNum As String) As String
        Dim macAdd As String = GetMacAddress()
        Dim md5Str As String = FormsAuthentication.HashPasswordForStoringInConfigFile(macAdd + seriesNum, "MD5")
        Return GetCode(md5Str, 4)
    End Function

    ''' <summary> 
    ''' 得到以“-”分隔，每段5个字母的编码 
    ''' </summary> 
    ''' <param name="src"></param> 
    ''' <param name="num"></param> 
    ''' <returns></returns> 
    Private Function GetCode(ByVal src As String, ByVal num As Integer) As String
        Dim re As String = ""
        For i As Integer = 0 To num - 1
            Dim tem As String = src.Substring(i * num, 5)
            re += tem
            If i < num - 1 Then
                re += "-"
            End If
        Next
        Return re
    End Function
    Private Function GetMacAddress() As String
        Try
            Dim mac As String = ""
            Dim mc As New ManagementClass("Win32_NetworkAdapterConfiguration")
            Dim moc As ManagementObjectCollection = mc.GetInstances()
            For Each mo As ManagementObject In moc
                If CBool(mo("IPEnabled")) = True Then
                    mac = mo("MacAddress").ToString()
                    Exit For
                End If
            Next
            moc = Nothing
            mc = Nothing
            Return mac
        Catch
            Return "unknow"
        Finally
        End Try
    End Function
    '#End Region
    '加密模块结束 
End Class

