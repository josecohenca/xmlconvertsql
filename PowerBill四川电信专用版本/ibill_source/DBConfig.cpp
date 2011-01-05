

#include "DBConfig.h"

TDBConfig::TDBConfig(AnsiString ConfigFileName)
{
  XMLDocument = NULL;
  RootNode    = NULL;
  
  LastErrorMessage = "";
  if(ConfigFileName != "")
    LoadConfig(ConfigFileName);
}
TDBConfig::~TDBConfig()
{
  CloseConfigFile();
}
bool __fastcall TDBConfig::LoadConfig(AnsiString AConfigFileName)
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
  RootNode = XMLDocument->ChildNodes->FindNode("DBConfig");
  if(RootNode == NULL)
  {
    LastErrorMessage = "配置文件中没有找到要求的DBConfig根节点.";
    CloseConfigFile();
  }
  return true;
}
void __fastcall TDBConfig::CloseConfigFile()
{
  if(XMLDocument != NULL)
  {
    if(XMLDocument->Active)
      XMLDocument->Active = false;
    delete XMLDocument;
    XMLDocument = NULL;
  }
  RootNode = NULL;
  ConfigFileName = "";
}
int __fastcall TDBConfig::GetDBCount()
{
  return RootNode->ChildNodes->Count;
}
AnsiString __fastcall TDBConfig::GetDBAttributeValue(AnsiString DBName,AnsiString AttributeName,bool NeedDecry)
{
  _di_IXMLNode AttributeNode = GetAttributeNode(DBName,AttributeName);
  if(AttributeNode == NULL)
    return "";
  else
  {
    AnsiString Value = AttributeNode->Text;
    if(NeedDecry && Value != "")
    {
      TWCDESComp * WCDESComp = new TWCDESComp(NULL);
      Value = WCDESComp->DecryStrHex(Value,PASSWORD_KEY);
      delete WCDESComp;
    }
    return Value;
  }
}
bool __fastcall TDBConfig::SetDBAttributeValue(AnsiString DBName,AnsiString AttributeName,AnsiString Value,bool NeedEncry)
{
  _di_IXMLNode AttributeNode;
  AttributeNode = GetAttributeNode(DBName,AttributeName);
  if(AttributeNode == NULL)
  {
    AttributeNode = AddAttributeNode(DBName,AttributeName);
    if(AttributeNode == NULL)
    {
      return false;
    }
  }
  if(NeedEncry && Value != "")
  {
    TWCDESComp * WCDESComp = new TWCDESComp(NULL);
    Value = WCDESComp->EncryStrHex(Value,PASSWORD_KEY);
    delete WCDESComp;
  }
  AttributeNode->Text = Value;
  return true;
}
_di_IXMLNode __fastcall TDBConfig::GetDBNode(AnsiString DBName)
{
  _di_IXMLNode AttributeNode;
  DBName = UpperCase(DBName);
  for(int n = 0;n < RootNode->ChildNodes->Count;n++)
  {
    AttributeNode = RootNode->ChildNodes->Nodes[n]->ChildNodes->FindNode("DBName");
    if(AttributeNode != NULL && UpperCase(AttributeNode->Text) == DBName)
      return RootNode->ChildNodes->Nodes[n];
  }
  return NULL;
}
_di_IXMLNode __fastcall TDBConfig::GetAttributeNode(AnsiString DBName,AnsiString AttributeName)
{
  _di_IXMLNode DBNode = GetDBNode(DBName);
  if(DBNode == NULL)
    return NULL;
  else
    return DBNode->ChildNodes->FindNode(AttributeName);
}

AnsiString __fastcall TDBConfig::GetDBName(int Index)
{
  if(XMLDocument == NULL || !XMLDocument->Active || Index >= RootNode->ChildNodes->Count)
    return "";
  _di_IXMLNode AttributeNode = RootNode->ChildNodes->Nodes[Index]->ChildNodes->FindNode("DBName");
  if(AttributeNode == NULL)
    return "";
  else
    return AttributeNode->Text;
}

_di_IXMLNode __fastcall TDBConfig::AddAttributeNode(AnsiString DBName,AnsiString AttributeName)
{
  _di_IXMLNode AttributeNode;
  AttributeNode = GetAttributeNode(DBName,AttributeName);
  if(AttributeNode != NULL)
    return AttributeNode;
  else
  {
    _di_IXMLNode DBNode = GetDBNode(DBName);
    if(DBNode == NULL)
    {
      LastErrorMessage = "配置文件中找不到对于" + DBName + "的定义.";
      return NULL;
    }
    AttributeNode = DBNode->AddChild(AttributeName);
    if(AttributeNode == NULL)
      LastErrorMessage = "无法添加子节点.";
    return AttributeNode;
  }
}
//---------------------------------------------------------------------------
AnsiString __fastcall TDBConfig::GetDBConnectionString(
    int DBType,
    AnsiString DBServer,
    AnsiString UserName,
    AnsiString Password,
    AnsiString DataBase,
    AnsiString Port)
{
  switch(DBType)
  {
  case DB_SQLSERVER:
     return "Provider=SQLOLEDB.1;"\
      "Password=" + Password + ";"\
      "Persist Security Info=True;"\
      "User ID=" + UserName + ";"\
      "Initial Catalog=" + DataBase + ";"\
      "Data Source= " + DBServer;
  case DB_ORACLE:
      return "Provider=OraOLEDB.Oracle.1;"\
      "Password=" + Password + ";"\
      "Persist Security Info=True;"\
      "User ID=" + UserName + ";"\
      "Data Source=" + DBServer;
  case DB_MYSQL:
      return "Provider=MSDASQL.1;"\
      "Password=\"" + Password + "\";"\
      "Persist Security Info=True;"\
      "User ID=" + UserName + ";"\
      "Extended Properties=\"DATABASE=" + DataBase + ";"\
      "DRIVER={MySQL ODBC 3.51 Driver};"\
      "OPTION=0;PORT=" + Port + ";SERVER=" + DBServer + ";"\
      "UID=" + UserName + "\"";
  default:
    return "";
  }
}
bool __fastcall TDBConfig::IsModified()
{
  return (XMLDocument != NULL && XMLDocument->Active && XMLDocument->Modified);

}
bool __fastcall TDBConfig::SaveChanges()
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
bool __fastcall TDBConfig::IsDBNameExists(AnsiString DBName)
{
  return GetDBNode(DBName) != NULL;
}
void __fastcall TDBConfig::Rollback()
{
  if(IsModified())
  {
    AnsiString FileName = XMLDocument->FileName;
    CloseConfigFile();
    LoadConfig(FileName);
  }
}
bool __fastcall TDBConfig::DeleteDBNode(AnsiString DBName)
{
  _di_IXMLNode DBNode = GetDBNode(DBName);
  if(DBNode == NULL)
    return true;
  int Index = RootNode->ChildNodes->IndexOf(DBNode); 
  return RootNode->ChildNodes->Delete(Index) > -1;
}
void __fastcall TDBConfig::AddNewDBServer(
  AnsiString DBName,
  AnsiString DBType,
  AnsiString Server,
  AnsiString UserName,
  AnsiString Password,
  AnsiString Database,
  AnsiString Port
  )
{
  _di_IXMLNode DBNode = RootNode->AddChild("DB",RootNode->ChildNodes->Count);
  DBNode->AddChild("DBName")->Text = DBName;  
  DBNode->AddChild("DBType")->Text = DBType;  
  DBNode->AddChild("Server")->Text = Server;  
  DBNode->AddChild("UserName")->Text = UserName;  
  DBNode->AddChild("Password")->Text = "";
  DBNode->AddChild("DataBase")->Text = Database;  
  DBNode->AddChild("Port")->Text = Port;
  SetDBAttributeValue(DBName,"Password",Password,true);  
}


