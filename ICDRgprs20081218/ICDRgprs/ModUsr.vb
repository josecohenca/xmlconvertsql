Option Strict On
Imports System.Security.Principal
Imports System.Threading
Imports System.IO
Imports System.Text
Imports System.Management
Imports System.Xml
Imports System
Imports System.Security.Cryptography
Module Users
    Dim strxml As String
    Public Function IsLoginXml(ByVal strName As String, ByVal strPassword As String) As Boolean
        Dim dsUsers As New DataSet()
        Dim drRows() As DataRow
        Try
            'dsUsers.ReadXml(Application.StartupPath & "\Users.xml")
            strxml = decrypttext(My.Computer.FileSystem.ReadAllText(Application.StartupPath & "\users.xml", System.Text.Encoding.GetEncoding("utf-8")))
            Dim stringReader As New StringReader(strxml)
            Dim xmlTextReader As New XmlTextReader(stringReader)
            dsUsers.ReadXml(xmlTextReader)
            drRows = dsUsers.Tables(0).Select("name = '" & strName & "' and password = '" & strPassword & "'")
            If drRows.Length > 0 Then
                Return True
            Else
                Return False
            End If
        Catch e As FileNotFoundException
            MsgBox(strmName() & "," & strmPassword())
            End
        End Try
    End Function
    Public Function GetLogin(ByVal strName As String, ByVal strPassword As String) As GenericPrincipal
        Dim dsUsers As New DataSet()
        Dim drRows() As DataRow
        Try
            'dsUsers.ReadXml(Application.StartupPath & "\Users.xml")
            strxml = decrypttext(My.Computer.FileSystem.ReadAllText(Application.StartupPath & "\users.xml", System.Text.Encoding.GetEncoding("utf-8")))
            Dim stringReader As New StringReader(strxml)
            Dim xmlTextReader As New XmlTextReader(stringReader)
            dsUsers.ReadXml(xmlTextReader)
            drRows = dsUsers.Tables(0).Select("name = '" & strName & "' and password = '" & strPassword & "'")
        Catch e As FileNotFoundException
            MsgBox(strmName() & "," & strmPassword())
            End
        End Try
        Dim GenIdentity As New GenericIdentity(strName)
        Dim Roles() As String = {CStr(drRows(0).Item("Role")), ""}
        Dim GenPrincipal As New GenericPrincipal(GenIdentity, Roles)
        Return GenPrincipal
    End Function
    Public Function IsLoginCpu(ByVal strName As String, ByVal strPassword As String) As Boolean
        Dim intLoginAttempts As Integer
        Dim GenPrincipal As GenericPrincipal
        If IsLoginXml(strName, strPassword) Then
            GenPrincipal = GetLogin(strName, strPassword)
            Thread.CurrentPrincipal = GenPrincipal
            Return True
        Else
            intLoginAttempts += 1
            If intLoginAttempts >= 3 Then
                End
            End If
        End If
    End Function
    Public Function strmPassword() As String
        Dim strResult As String = Nothing
        Dim search As New ManagementObjectSearcher("SELECT * FROM Win32_OperatingSystem")
        Dim info As ManagementObject
        For Each info In search.Get()
            strResult = info("SerialNumber").ToString()
        Next
        Return strResult
    End Function
    Public Function strmName() As String
        Dim strResult As String = Nothing
        Dim search As New ManagementObjectSearcher("SELECT * FROM Win32_ComputerSystem")
        Dim info As ManagementObject
        For Each info In search.Get()
            strResult = info("model").ToString()
        Next
        Return strResult
    End Function
    Function getMd5Hash(ByVal input As String) As String
        Dim md5Hasher As New MD5CryptoServiceProvider()
        Dim data As Byte() = md5Hasher.ComputeHash(Encoding.Default.GetBytes(input))
        Dim sBuilder As New StringBuilder()
        Dim i As Integer
        For i = 0 To data.Length - 1
            sBuilder.Append(data(i).ToString("x2"))
        Next i
        Return sBuilder.ToString()
    End Function
    Function verifyMd5Hash(ByVal input As String, ByVal hash As String) As Boolean
        Dim hashOfInput As String = getMd5Hash(input)
        Dim comparer As StringComparer = StringComparer.OrdinalIgnoreCase
        If 0 = comparer.Compare(hashOfInput, hash) Then
            Return True
        Else
            Return False
        End If
    End Function
    Function encrypttext(ByVal strtext As String) As String
        Return encrypt(strtext, "&%#@?,:*")
    End Function
    Function decrypttext(ByVal strtext As String) As String
        Return decrypt(strtext, "&%#@?,:*")
    End Function
    Function encrypt(ByVal strtext As String, ByVal strencrkey As String) As String
        Dim bykey() As Byte = {}
        Dim iv() As Byte = {&H12, &H34, &H56, &H78, &H90, &HAB, &HCD, &HEF}
        Try
            bykey = System.Text.Encoding.UTF8.GetBytes(Left(strencrkey, 8))
            Dim des As New DESCryptoServiceProvider()
            Dim inputbytearray() As Byte = Encoding.UTF8.GetBytes(strtext)
            Dim ms As New memorystream()
            Dim cs As New CryptoStream(ms, des.CreateEncryptor(bykey, iv), CryptoStreamMode.Write)
            cs.Write(inputbytearray, 0, inputbytearray.Length)
            cs.FlushFinalBlock()
            Return Convert.ToBase64String(ms.toarray())
        Catch ex As Exception
            Return ex.Message
        End Try
    End Function
    Function decrypt(ByVal strtext As String, ByVal sdecrkey As String) As String
        Dim bykey() As Byte = {}
        Dim iv() As Byte = {&H12, &H34, &H56, &H78, &H90, &HAB, &HCD, &HEF}
        Dim inputbytearray(strtext.Length) As Byte
        Try
            bykey = System.Text.Encoding.UTF8.GetBytes(Left(sdecrkey, 8))
            Dim des As New DESCryptoServiceProvider()
            inputbytearray = Convert.FromBase64String(strtext)
            Dim ms As New memorystream()
            Dim cs As New CryptoStream(ms, des.CreateDecryptor(bykey, iv), CryptoStreamMode.Write)
            cs.Write(inputbytearray, 0, inputbytearray.Length)
            cs.FlushFinalBlock()
            Dim encoding As System.Text.Encoding = System.Text.Encoding.UTF8
            Return encoding.GetString(ms.toarray())
        Catch ex As Exception
            Return ex.Message
        End Try
    End Function
End Module