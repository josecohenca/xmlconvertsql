@echo on 
@echo xsd.bat

cd F:\Program Files\Microsoft Visual Studio 10.0\VC
F:
vcvarsall.bat

@echo cd G:\xmlconvertsql\XmlToSqlXSD
G:

xsd Asn1OutXml.xml

xsd Asn1OutXml.xsd /CLASSES