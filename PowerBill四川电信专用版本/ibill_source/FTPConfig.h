//FTP服务器管理类

#ifndef FTPCONFIG_H
#define FTPCONFIG_H

#include "public.h"
#include <Classes.hpp>
#include <msxmldom.hpp>
#include <XMLDoc.hpp>
#include <xmldom.hpp>
#include <XMLIntf.hpp>
#include <windows.h>
//const char FTP_PASSWORD_KEY[] = "APPLICATION_198705#_*@#$(*&$(*@&!($*&#*&(*&(*^123521@(*)!";
class TFTPConfig
{
private:
  //AnsiString ConfigFileName;
  _di_IXMLDocument XMLDocument;
  void __fastcall CloseConfigFile();
  _di_IXMLNode __fastcall GetAttributeNode(AnsiString FTPName,AnsiString AttributeName);
  _di_IXMLNode __fastcall AddAttributeNode(AnsiString FTPName,AnsiString AttributeName);
public:
  AnsiString EncryFTPString(AnsiString Str);
  _di_IXMLNode  RootNode;
  _di_IXMLNode __fastcall GetFTPNode(AnsiString FTPName);
  AnsiString LastErrorMessage;
  AnsiString ConfigFileName;
  TFTPConfig(AnsiString ConfigFileName = "");
  ~TFTPConfig();
  bool __fastcall LoadConfig(AnsiString  AConfigFileName);
  int  __fastcall GetFTPCount();
  AnsiString __fastcall GetFTPName(int Index); 
  AnsiString __fastcall GetFTPAttributeValue(AnsiString FTPName,AnsiString AttributeName,bool NeedDecry = false);
  bool __fastcall SetFTPAttributeValue(AnsiString FTPName,AnsiString AttributeName,AnsiString Value,bool NeedEncry = false);
  bool __fastcall DeleteFTPNode(AnsiString FTPName);
  bool __fastcall IsModified();
  bool __fastcall SaveChanges();
  void __fastcall Rollback();
  bool __fastcall IsFTPNameExists(AnsiString FTPName);
  void __fastcall AddNewFTPServer(
    AnsiString FTPName,
    AnsiString Server,
    int Port,
    AnsiString UserName,
    AnsiString Password,
    AnsiString Mode//,int TimeOut
    );
};

#endif
 