
Module ModXml
    Public Sub populateTreeControl(ByVal document As System.Xml.XmlNode, ByVal nodes As System.Windows.Forms.TreeNodeCollection)
        Dim node As System.Xml.XmlNode
        For Each node In document.ChildNodes
            Dim [text] As String
            If node.Value <> Nothing Then
                [text] = node.Value
            Else
                If Not node.Attributes Is Nothing And node.Attributes.Count > 0 Then
                    [text] = node.Attributes(1).Value
                Else : [text] = node.Name
                End If
            End If
            Dim new_child As New TreeNode([text])
            nodes.Add(new_child)
            populateTreeControl(node, new_child.Nodes)
        Next node
    End Sub
    Public Function HexToByte(ByVal HexNum As String) As Byte
        HexToByte = CByte(Val("&H" & HexNum))
    End Function
    Public Function HexToBin(ByVal H As String) As String
        Select Case UCase(H)
            Case "0"
                HexToBin = "0000"
            Case "1"
                HexToBin = "0001"
            Case "2"
                HexToBin = "0010"
            Case "3"
                HexToBin = "0011"
            Case "4"
                HexToBin = "0100"
            Case "5"
                HexToBin = "0101"
            Case "6"
                HexToBin = "0110"
            Case "7"
                HexToBin = "0111"
            Case "8"
                HexToBin = "1000"
            Case "9"
                HexToBin = "1001"
            Case "A"
                HexToBin = "1010"
            Case "B"
                HexToBin = "1011"
            Case "C"
                HexToBin = "1100"
            Case "D"
                HexToBin = "1101"
            Case "E"
                HexToBin = "1110"
            Case "F"
                HexToBin = "1111"
            Case Else
                HexToBin = ""
        End Select
    End Function
    Public Function Hex2Bin(ByVal sHex As String) As String
        Const s1 = "0000101001101111000", s2 = "0125A4936DB7FEC8"
        Dim i As Integer, sBin As String
        sHex = UCase(sHex)
        sBin = vbNullString
        For i = 1 To Len(sHex)
            sBin = sBin & Mid(s1, InStr(1, s2, Mid(sHex, i, 1)), 4)
        Next i
        Hex2Bin = sBin
    End Function
End Module
