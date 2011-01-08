@echo off
xcopy DataConverter.exe ..\..\..\Asn1Editor\Bin\Debug /y
if errorlevel 1 goto CSharpReportError
goto CSharpEnd
:CSharpReportError
echo Project error: A tool returned an error code from the build event
exit 1
:CSharpEnd