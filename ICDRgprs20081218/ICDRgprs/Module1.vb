Imports System.Management
Module ModWmi
    Public Sub GetWmi()
        Dim disk As ManagementBaseObject
        Dim strResult As String
        Dim diskClass = New ManagementClass("Win32_LogicalDisk")
        Dim disks As ManagementObjectCollection
        disks = diskClass.GetInstances()
        For Each disk In disks
            strResult = ""
            strResult += "设备ID:" & disk("DeviceID") & vbCrLf
            strResult += "磁盘名称:" & disk("Name") & vbCrLf
            strResult += "磁盘卷标:" & disk("VolumeName") & vbCrLf
            If disk("FileSystem") <> "" Then strResult += "文件系统:" & disk("FileSystem") & vbCrLf
            strResult += "磁盘描述:" & disk("Description") & vbCrLf
            If System.Convert.ToInt64(disk("Size")) > 0 Then
                strResult += "磁盘大小:" & System.Convert.ToInt64(disk("Size").ToString()) & vbCrLf
                strResult += "磁盘类型:" & System.Convert.ToInt16(disk("DriveType").ToString())
            End If
            MsgBox(strResult)
        Next
    End Sub
End Module
