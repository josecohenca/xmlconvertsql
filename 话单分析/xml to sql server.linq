<Query Kind="Expression">
  <Connection>
    <ID>f2f6e2b4-0f40-48df-b2c7-82584a77c393</ID>
    <AttachFileName>&lt;ApplicationData&gt;\LINQPad\Nutshell.mdf</AttachFileName>
    <Server>.\SQLEXPRESS</Server>
    <AttachFile>true</AttachFile>
    <UserInstance>true</UserInstance>
  </Connection>
  <Reference>&lt;ProgramFiles&gt;\Microsoft Visual Studio 9.0\Common7\IDE\PublicAssemblies\Microsoft.VisualStudio.QualityTools.UnitTestFramework.dll</Reference>
</Query>

Using XSD Tool to Generate Classes from XML
Home » XML » Using XSD Tool to Generate Classes from XML
XSD is a simple tool that can be used to generate a C# class from a given XML document. 
This class can then be used to deserialize and process the XML within your code.
 

 

To get started you will need an XML file, in this example I will again use the cd catalog XML from W3 Schools.

You will need to run the XSD.EXE tool from the Visual Studio Command Prompt
(Start -> Visual Studio 2005 -> Visual Studio Tools -> Visual Studio Command Prompt) as there is no graphical front end for the tool.

The first step is to generate an XML schema from the file (skip this if you already have one).

On the command prompt type:

xsd cd_catalog.xmlThis will generate a schema for the file, which we can use to generate our classes.

You should now have something like this on the screen:

C:\Sharper\>xsd cd_catalog.xml
Microsoft (R) Xml Schemas/DataTypes support utility
[Microsoft (R) .NET Framework, Version 2.0.50727.42]
Copyright (C) Microsoft Corporation. All rights reserved.
Writing file 'C:\Sharper\cd_catalog.xsd'.

C:\Sharper>The XSD schema will look similar to the XML file, but it will not contain any data. It is used to define the structure and the type of data that the XML will hold.

<?xml version="1.0" encoding="utf-8"?>
<xs:schema id="CATALOG" xmlns="" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:msdata="urn:schemas-microsoft-com:xml-msdata">
  <xs:element name="CATALOG" msdata:IsDataSet="true" msdata:Locale="en-US">
	<xs:complexType>
	  <xs:choice minOccurs="0" maxOccurs="unbounded">
		<xs:element name="CD">
		  <xs:complexType>
			<xs:sequence>
			  <xs:element name="TITLE" type="xs:string" minOccurs="0" />
			  <xs:element name="ARTIST" type="xs:string" minOccurs="0" />
			  <xs:element name="COUNTRY" type="xs:string" minOccurs="0" />
			  <xs:element name="COMPANY" type="xs:string" minOccurs="0" />
			  <xs:element name="PRICE" type="xs:string" minOccurs="0" />
			  <xs:element name="YEAR" type="xs:string" minOccurs="0" />
			</xs:sequence>
		  </xs:complexType>
		</xs:element>
	  </xs:choice>
	</xs:complexType>
  </xs:element>
</xs:schema>Next we can use the XSD file to generate our class(es). The generated XSD can (and often do) contain multiple classes, so it would be better to use /classes switch which tells xsd.exe to generate all the code. The default language is C#, however if you wish to have the code generated in VB.Net simply add the switch /language:vb.

C:\Sharper>xsd cd_catalog.xsd /classes
Microsoft (R) Xml Schemas/DataTypes support utility
[Microsoft (R) .NET Framework, Version 2.0.50727.42]
Copyright (C) Microsoft Corporation. All rights reserved.
Writing file 'C:\Sharper\cd_cataloge.cs'.

This will then generate a .cs source file that you can incorporate into your project(s).

Xml Deserialization
In order to use the original XML data in our project, all we need to do is deserialize the XML file using the class we just generated. By creating a XML schema and adding it to a project, Visual Studio will create a strongly typed DataSet for you as well. Anyway the deserialization code is show below.

using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Xml.Serialization;
 
namespace ConsoleApplication1
{
  class Program
  {
	static void Main(string[] args)
	{
	  StreamReader str = new StreamReader("cd_catalog.xml");
	  XmlSerializer xSerializer = new XmlSerializer(typeof(CATALOG));
 
	  CATALOG myCdCatalogue = (CATALOG)xSerializer.Deserialize(str);
 
	  foreach (CATALOG.CDRow cd in myCdCatalogue.CD)
	  {
		Console.WriteLine(cd.TITLE);
		Console.WriteLine(cd.ARTIST);
		Console.WriteLine(cd.COUNTRY);
		Console.WriteLine(cd.COMPANY);
		Console.WriteLine(cd.PRICE);
		Console.WriteLine(cd.YEAR);
		Console.WriteLine();
	  }
	  str.Close();
	}
  }
} 

 

 

