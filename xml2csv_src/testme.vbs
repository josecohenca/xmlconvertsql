'----------
  Dim xmlSource
  Dim xmlXForm
  Dim strErr
  Dim strResult
   
  Dim fso , file
  Dim strPath
  Const ForReading = 1
  Const ForWriting = 2
  Const ForAppending = 8
 
  Set xmlSource = CreateObject("MSXML.DOMDocument")
  Set xmlXForm = CreateObject("MSXML.DOMDocument")
 
  xmlSource.validateOnParse = True
  xmlXForm.validateOnParse = True
  xmlSource.async = False
  xmlXForm.async = False
 
  xmlSource.Load "c:\testme.xml"  ' This loads the text that I want to transform
  If Err.Number <> 0 Then
      strErr = Err.Description & vbCrLf
      strErr = strErr & xmlSource.parseError.reason & " line: " & xmlSource.parseError.Line & " col: " & xmlSource.parseError.linepos & " text: " & xmlSource.parseError.srcText
      MsgBox strErr, vbCritical, "Error loading the XML"
  End If
 
  xmlXForm.Load "c:\testme.xsl" ' This loads the XSLT transform
  If Err.Number <> 0 Then
      strErr = Err.Description & vbCrLf
      strErr = strErr & xmlSource.parseError.reason & " line: " & xmlSource.parseError.Line & " col: " & xmlSource.parseError.linepos & " text: " & xmlSource.parseError.srcText
      MsgBox strErr, vbCritical, "Error loading the Transform"
  End If
   
  strResult = xmlSource.transformNode(xmlXForm) ' This transforms the data in xmlSource
  If Err.Number <> 0 Then
      strErr = Err.Description & vbCrLf
      strErr = strErr & xmlSource.parseError.reason & " line: " & xmlSource.parseError.Line & " col: " & xmlSource.parseError.linepos & " text: " & xmlSource.parseError.srcText
      MsgBox strErr, vbCritical, "Error executing the Transform"
  End If
 
  Set fso = CreateObject("Scripting.FileSystemObject")
  strPath = "c:\testme.txt"
  ' open the file
  Set file = fso.opentextfile(strPath, ForWriting, True)
  ' write the info to the file
  file.write strResult
  ' close and clean up
  file.Close
 
  Set file = Nothing
  Set fso = Nothing
  Set xmlSource = Nothing
  Set xmlXForm = Nothing
'----------