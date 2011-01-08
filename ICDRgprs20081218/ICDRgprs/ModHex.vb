Imports System.Text
Module ModHex
    Public Function hextotbcd(ByVal strs As String) As String
        Dim bcdtmp As String
        Dim n As Object
        Dim i As Short
        bcdtmp = ""
        For i = 1 To Len(strs) Step 2
            n = Mid(strs, i, 2)
            n = Right(n, 1) & Left(n, 1)
            bcdtmp = bcdtmp & n
        Next i
        hextotbcd = bcdtmp
    End Function
    Public Function hextoimsi(ByVal strs As String) As String
        Dim bcdtmp As String
        Dim n As Object
        Dim i As Short
        bcdtmp = ""
        For i = 1 To Len(strs) Step 2
            n = Mid(strs, i, 2)
            n = Right(n, 1) & Left(n, 1)
            bcdtmp = bcdtmp & n
        Next i
        hextoimsi = Mid(bcdtmp, 1, 15)
    End Function
    Public Function hextomsdn(ByVal strs As String) As String
        Dim bcdtmp As String
        Dim n As Object
        Dim i As Short
        bcdtmp = ""
        For i = 1 To Len(strs) Step 2
            n = Mid(strs, i, 2)
            n = Right(n, 1) & Left(n, 1)
            bcdtmp = bcdtmp & n
        Next i
        hextomsdn = Mid(bcdtmp, 5, 11)
    End Function
    Public Function hextoip(ByVal strs As String) As String
        Dim iptmp As String
        Dim n As Object
        Dim i As Object
        Dim J As Short
        Dim cc() As String
        iptmp = ""
        n = ""
        i = ""
        If Len(strs) < 10 Then
            For i = 1 To Len(strs) Step 2
                n = Mid(strs, i, 2)
                n = hextodec(n)
                If i > 1 Then
                    iptmp = iptmp & "." & n
                Else
                    iptmp = iptmp & n
                End If
            Next i
        Else
            cc = Split(strs, ";")
            For i = 1 To Len(cc(0)) Step 2
                n = Mid(cc(0), i, 2)
                n = hextodec(n)
                If i > 1 Then
                    iptmp = iptmp & "." & n
                Else
                    iptmp = iptmp & n
                End If
            Next i
            iptmp = iptmp & "-"
            For J = 1 To Len(cc(1)) Step 2
                '
                n = Mid(cc(1), J, 2)
                '
                n = hextodec(n)
                If J > 1 Then
                    iptmp = iptmp & "." & n
                Else
                    iptmp = iptmp & n
                End If
            Next J
        End If
        hextoip = iptmp
    End Function
    Public Function hextostr(ByVal strs As String) As String
        Dim n As Object
        Dim i As Short
        Dim tmp As String
        tmp = ""
        For i = 1 To Len(strs) Step 2
            n = Val("&H" & Mid(strs, i, 2))
            If n < 0 Or n > 127 Then
                n = Val("&H" & Mid(strs, i, 4))
                i = i + 2
            End If
            tmp = tmp & Chr(n)
        Next i
        hextostr = tmp
    End Function
    Public Function hextotime(ByVal strs As String) As String
        hextotime = "20" & Mid(strs, 1, 2) & "/" & Mid(strs, 3, 2) & "/" & Mid(strs, 5, 2) & " " & Mid(strs, 7, 2) & ":" & Mid(strs, 9, 2) & ":" & Mid(strs, 11, 2)
    End Function
    Public Function hextoenum(ByVal strs As String) As String
        Dim iptmp As Object
        Dim n As Object
        Dim i As Short
        iptmp = ""
        For i = 1 To Len(strs)
            n = Mid(strs, i, 2)
            iptmp = iptmp & n
        Next i
        hextoenum = iptmp
    End Function
    Public Function hextohex(ByVal strs As String) As String
        hextohex = strs
    End Function
    Public Function hextobcd(ByVal strs As String) As String
        hextobcd = Left(strs, Len(strs) - 1)
    End Function
    Public Function hextonip(ByVal strs As String) As String
        Dim nip() As String
        nip = Split(strs, "E")
        hextonip = hextostr(nip(0) & "E") & hextostr(nip(1) & "E") & hextostr(nip(2) & "E") & hextostr(nip(3))
    End Function
    Public Function hextooctet(ByVal strs As String) As String
        Dim iptmp As Object
        Dim n As Object
        Dim i As Short
        iptmp = ""
        For i = 1 To Len(strs)
            n = Mid(strs, i, 2)
            n = Asc(n)
            iptmp = iptmp & n
        Next i
        hextooctet = iptmp
    End Function

    Public Function hextodec(ByVal Hex_Renamed As String) As Integer      ''用途: 将十六进制转化为十进制
        Dim i As Integer
        Dim B As Integer
        On Error Resume Next
        Hex_Renamed = UCase(Hex_Renamed)
        For i = 1 To Len(Hex_Renamed)
            Select Case Mid(Hex_Renamed, Len(Hex_Renamed) - i + 1, 1)
                Case "0" : B = B + 16 ^ (i - 1) * 0
                Case "1" : B = B + 16 ^ (i - 1) * 1
                Case "2" : B = B + 16 ^ (i - 1) * 2
                Case "3" : B = B + 16 ^ (i - 1) * 3
                Case "4" : B = B + 16 ^ (i - 1) * 4
                Case "5" : B = B + 16 ^ (i - 1) * 5
                Case "6" : B = B + 16 ^ (i - 1) * 6
                Case "7" : B = B + 16 ^ (i - 1) * 7
                Case "8" : B = B + 16 ^ (i - 1) * 8
                Case "9" : B = B + 16 ^ (i - 1) * 9
                Case "A" : B = B + 16 ^ (i - 1) * 10
                Case "B" : B = B + 16 ^ (i - 1) * 11
                Case "C" : B = B + 16 ^ (i - 1) * 12
                Case "D" : B = B + 16 ^ (i - 1) * 13
                Case "E" : B = B + 16 ^ (i - 1) * 14
                Case "F" : B = B + 16 ^ (i - 1) * 15
            End Select
        Next i
        hextodec = B
    End Function
    'OCTET STRING (SIZE(1))
    Public Function hextootectstr1(ByVal str As String) As String
        Dim bytes As String = Nothing
        Dim bytesbit As String
        bytesbit = Hex2Bin(str)
        '取1个8位2进制
        bytes = bytes & "<GEA1>"
        bytes = bytes & Mid(bytesbit, 1, 1)
        bytes = bytes & "<SMdedicatedchannels>"
        bytes = bytes & Mid(bytesbit, 2, 1)
        bytes = bytes & "<SMGPRSchannels>"
        bytes = bytes & Mid(bytesbit, 3, 1)
        bytes = bytes & "<UCS2>"
        bytes = bytes & Mid(bytesbit, 4, 1)
        bytes = bytes & "<SS>"
        bytes = bytes & Mid(bytesbit, 5, 2)
        bytes = bytes & "<SoLSA>"
        bytes = bytes & Mid(bytesbit, 7, 1)
        bytes = bytes & "<Revision>"
        bytes = bytes & Mid(bytesbit, 8, 1)
        'bytes = bytes & "<PFC feature mode: bit>"
        'bytes = bytes & "<Extended GEA bits>"
        'bytes = bytes & "< LCS VA capability: bit >"
        'bytes = bytes & "<Spare bits>"
        hextootectstr1 = bytes
    End Function
    'OCTET STRING (SIZE(2))
    Public Function hextootectstr2(ByVal str As String) As String
        Dim bytes As String
        bytes = Hex2Bin(str)
        '取1个16位2进制
        bytes = Mid(bytes, 1, 16)
        hextootectstr2 = BIN_to_DEC(bytes)
    End Function
    'Quality of service 
    Public Function hextoqos(ByVal str As String) As String
        Dim bytes As String
        Dim qosstr(0 To 10)
        hextoqos = Nothing
        bytes = Hex2Bin(str)
        '取第0个8位Type = 135 (Decimal)
        qosstr(0) = "Type" & BIN_to_DEC(Mid(bytes, 1, 8))
        '取第1个8位Length
        qosstr(1) = "Length" & BIN_to_DEC(Mid(bytes, 9, 8))
        '取第2个8位Allocation/Retention Priority
        qosstr(2) = "Priority" & BIN_to_DEC(Mid(bytes, 17, 8))
        '取第3个8位
        '---------------------------------------------
        '2位spare
        'qosstr(3) = BIN_to_DEC(Mid(bytes, 25, 2))
        qosstr(3) = "spare"
        '3位Delay class
        qosstr(4) = "Delay" & BIN_to_DEC(Mid(bytes, 27, 3))
        '3位Reliability class
        qosstr(5) = "Reliability" & BIN_to_DEC(Mid(bytes, 30, 3))
        '---------------------------------------------
        '取第4个8位
        '---------------------------------------------
        '4位Peak throughput
        qosstr(6) = "Peak" & BIN_to_DEC(Mid(bytes, 33, 4))
        '1位spare
        'qosstr(7) = BIN_to_DEC(Mid(bytes, 37, 1))
        qosstr(7) = "spare"
        '3位Precedence class
        qosstr(8) = "Precedence" & BIN_to_DEC(Mid(bytes, 38, 3))
        '---------------------------------------------
        '取第5个8位
        '---------------------------------------------
        '3位spare
        'qosstr(9) = BIN_to_DEC(Mid(bytes, 41, 3))
        qosstr(9) = "spare"
        '5位Mean throughput
        qosstr(10) = "Mean" & BIN_to_DEC(Mid(bytes, 44, 5))
        '---------------------------------------------
        For i = 0 To 10
            If hextoqos = Nothing Then
                hextoqos = qosstr(i)
            Else
                hextoqos = hextoqos & "-" & qosstr(i)
            End If
        Next i
    End Function
    Public Function BIN_to_DEC(ByVal Bin As String) As Long
        Dim i As Long
        For i = 1 To Len(Bin)
            BIN_to_DEC = BIN_to_DEC * 2 + Val(Mid(Bin, i, 1))
        Next i
    End Function
End Module