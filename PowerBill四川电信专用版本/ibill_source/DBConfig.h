
//数据库配置管理类

#ifndef DBCONFIG_H
#define DBCONFIG_H

#include "public.h"
#include <Classes.hpp>
#include <msxmldom.hpp>
#include <XMLDoc.hpp>
#include <xmldom.hpp>
#include <XMLIntf.hpp>
#include <windows.h>

//数据库类型
#define DB_SQLSERVER 1   //SQL Server数据库
#define DB_ORACLE    2   //Oracle数据库
#define DB_MYSQL     3   //MySQL数据库

const char PASSWORD_KEY[] = "APPLICATION_198705#_*@#$(*&$(*@&!($*&#@(*)!";
class TDBConfig
{
private:
  //AnsiString ConfigFileName;
  _di_IXMLDocument XMLDocument;
  void __fastcall CloseConfigFile();
  _di_IXMLNode __fastcall GetAttributeNode(AnsiString DBName,AnsiString AttributeName);
  _di_IXMLNode __fastcall AddAttributeNode(AnsiString DBName,AnsiString AttributeName);
public:
  AnsiString ConfigFileName;
  _di_IXMLNode __fastcall GetDBNode(AnsiString DBName);
  _di_IXMLNode  RootNode;
  AnsiString LastErrorMessage;
  TDBConfig(AnsiString ConfigFileName = "");
  ~TDBConfig();
  bool __fastcall LoadConfig(AnsiString  AConfigFileName);
  int  __fastcall GetDBCount();
  AnsiString __fastcall GetDBName(int Index); 
  AnsiString __fastcall GetDBAttributeValue(AnsiString DBName,AnsiString AttributeName,bool NeedDecry = false);
  bool __fastcall SetDBAttributeValue(AnsiString DBName,AnsiString AttributeName,AnsiString Value,bool NeedEncry = false);
  bool __fastcall DeleteDBNode(AnsiString DBName);
  bool __fastcall IsModified();
  bool __fastcall SaveChanges();
  void __fastcall Rollback();
  bool __fastcall IsDBNameExists(AnsiString DBName);
  AnsiString __fastcall GetDBConnectionString(
    int DBType,
    AnsiString DBServer,
    AnsiString UserName,
    AnsiString Password,
    AnsiString DataBase,
    AnsiString Port);
  void __fastcall AddNewDBServer(
    AnsiString DBName,
    AnsiString DBType,
    AnsiString Server,
    AnsiString UserName,
    AnsiString Password,
    AnsiString Database,
    AnsiString Port
    );
};

#endif
 