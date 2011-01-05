/*
* Version 2.0
*本类负责管理FTP服务器配置
*
*
*/
#include "FTPConfig.h"

TFTPConfig::TFTPConfig(AnsiString ConfigFileName)
{
  XMLDocument = NULL;
  RootNode    = NULL;
  
  LastErrorMessage = "";
  if(ConfigFileName != "")
    LoadConfig(ConfigFileName);
}
TFTPConfig::~TFTPConfig()
{
  CloseConfigFile();
}
bool __fastcall TFTPConfig::LoadConfig(AnsiString AConfigFileName)
{
  CloseConfigFile();
  try
  {
    XMLDocument = LoadXMLDocument(AConfigFileName);
    XMLDocument->Active  = true;
    XMLDocument->Options = XMLDocument->Options << doNodeAutoIndent;
    ConfigFileName = AConfigFileName;
  }
  catch(Exception * E)
  {
    LastErrorMessage = E->Message;
    return false;
  }
  RootNode = XMLDocument->ChildNodes->FindNode("FTPConfig");
  if(RootNode == NULL)
  {
    LastErrorMessage = "配置文件中没有找到要求的FTPConfig根节点.";
    CloseConfigFile();
  }
  return true;
}
void __fastcall TFTPConfig::CloseConfigFile()
{
  if(XMLDocument != NULL)
  {
    if(XMLDocument->Active)
      XMLDocument->Active = false;
    delete XMLDocument;
    XMLDocument = NULL;
  }
  RootNode = NULL;
}
int __fastcall TFTPConfig::GetFTPCount()
{
  return RootNode->ChildNodes->Count;
}
AnsiString __fastcall TFTPConfig::GetFTPAttributeValue(AnsiString FTPName,AnsiString AttributeName,bool NeedDecry)
{
  _di_IXMLNode AttributeNode = GetAttributeNode(FTPName,AttributeName);
  if(AttributeNode == NULL)
    return "";
  else
  {
    AnsiString Value = AttributeNode->Text;
    if(NeedDecry && Value != "")
    {
      TWCDESComp * WCDESComp = new TWCDESComp(NULL);
      Value = WCDESComp->DecryStrHex(Value,FTP_PASSWORD_KEY);
      delete WCDESComp;
    }
    return Value;
  }
}
AnsiString TFTPConfig::EncryFTPString(AnsiString Str)
{
  TWCDESComp * WCDESComp = new TWCDESComp(NULL);
  AnsiString Value = WCDESComp->EncryStrHex(Str,FTP_PASSWORD_KEY);
  delete WCDESComp;
  return Value;
}
bool __fastcall TFTPConfig::SetFTPAttributeValue(AnsiString FTPName,AnsiString AttributeName,AnsiString Value,bool NeedEncry)
{
  _di_IXMLNode AttributeNode;
  AttributeNode = GetAttributeNode(FTPName,AttributeName);
  if(AttributeNode == NULL)
  {
    AttributeNode = AddAttributeNode(FTPName,AttributeName);
    if(AttributeNode == NULL)
    {
      return false;
    }
  }
  if(NeedEncry && Value != "")
  {
    Value = EncryFTPString(Value);
  }
  AttributeNode->Text = Value;
  return true;
}
_di_IXMLNode __fastcall TFTPConfig::GetFTPNode(AnsiString FTPName)
{
  _di_IXMLNode AttributeNode;
  FTPName = UpperCase(FTPName);
  for(int n = 0;n < RootNode->ChildNodes->Count;n++)
  {
    AttributeNode = RootNode->ChildNodes->Nodes[n]->ChildNodes->FindNode("ServerName");
    if(AttributeNode != NULL && UpperCase(AttributeNode->Text) == FTPName)
      return RootNode->ChildNodes->Nodes[n];
  }
  return NULL;
}
_di_IXMLNode __fastcall TFTPConfig::GetAttributeNode(AnsiString FTPName,AnsiString AttributeName)
{
  _di_IXMLNode FTPNode = GetFTPNode(FTPName);
  if(FTPNode == NULL)
    return NULL;
  else
    return FTPNode->ChildNodes->FindNode(AttributeName);
}

AnsiString __fastcall TFTPConfig::GetFTPName(int Index)
{
  if(XMLDocument == NULL || !XMLDocument->Active || Index >= RootNode->ChildNodes->Count)
    return "";
  _di_IXMLNode AttributeNode = RootNode->ChildNodes->Nodes[Index]->ChildNodes->FindNode("ServerName");
  if(AttributeNode == NULL)
    return "";
  else
    return AttributeNode->Text;
}

_di_IXMLNode __fastcall TFTPConfig::AddAttributeNode(AnsiString FTPName,AnsiString AttributeName)
{
  _di_IXMLNode AttributeNode;
  AttributeNode = GetAttributeNode(FTPName,AttributeName);
  if(AttributeNode != NULL)
    return AttributeNode;
  else
  {
    _di_IXMLNode FTPNode = GetFTPNode(FTPName);
    if(FTPNode == NULL)
    {
      LastErrorMessage = "配置文件中找不到对于" + FTPName + "的定义.";
      return NULL;
    }
    AttributeNode = FTPNode->AddChild(AttributeName);
    if(AttributeNode == NULL)
      LastErrorMessage = "无法添加子节点.";
    return AttributeNode;
  }
}
bool __fastcall TFTPConfig::IsModified()
{
  return (XMLDocument != NULL && XMLDocument->Active && XMLDocument->Modified);

}
bool __fastcall TFTPConfig::SaveChanges()
{
  try
  {
    CopyFile(AnsiString(XMLDocument->FileName).c_str(),(AnsiString(XMLDocument->FileName) + FormatDateTime("yyyymmddhhnnss",Now()) + "的备份.xml").c_str(),false);
    XMLDocument->SaveToFile(XMLDocument->FileName);
    return true;
  }
  catch(Exception * E)
  {
    LastErrorMessage = E->Message;
    return false;
  }
}
bool __fastcall TFTPConfig::IsFTPNameExists(AnsiString FTPName)
{
  return GetFTPNode(FTPName) != NULL;
}
void __fastcall TFTPConfig::Rollback()
{
  if(IsModified())
  {
    AnsiString FileName = XMLDocument->FileName;
    CloseConfigFile();
    LoadConfig(FileName);
  }
}
bool __fastcall TFTPConfig::DeleteFTPNode(AnsiString FTPName)
{
  _di_IXMLNode FTPNode = GetFTPNode(FTPName);
  if(FTPNode == NULL)
    return true;
  int Index = RootNode->ChildNodes->IndexOf(FTPNode); 
  return RootNode->ChildNodes->Delete(Index) > -1;
}
void __fastcall TFTPConfig::AddNewFTPServer(
    AnsiString FTPName,
    AnsiString Server,
    int Port,
    AnsiString UserName,
    AnsiString Password,
    AnsiString Mode//,int TimeOut
  )
{
  _di_IXMLNode FTPNode = RootNode->AddChild("FTP",RootNode->ChildNodes->Count);
  FTPNode->AddChild("ServerName")->Text = FTPName;  
  FTPNode->AddChild("Server")->Text   = Server;
  FTPNode->AddChild("Port")->Text     = IntToStr(Port);
  FTPNode->AddChild("UserName")->Text = UserName;  
  FTPNode->AddChild("Password")->Text = "";  
  FTPNode->AddChild("Mode")->Text     = Mode;
  //FTPNode->AddChild("TimeOut")->Text  = IntToStr(TimeOut);
  SetFTPAttributeValue(FTPName,"Password",Password,true);  
}



 