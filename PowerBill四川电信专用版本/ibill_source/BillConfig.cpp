


#include "BillConfig.h"
#include <StrUtils.hpp>
/*
*TBillConfig负责管理话单类型定义(Config.xml)
*/
TBillConfig::TBillConfig(AnsiString ConfigFileName)
{
  XMLDocument    = NULL;
  BillNameList   = NULL;
  try
  {
    BillNameList = new TStringList;
    if(ConfigFileName != "")
      ReadBillConfig(ConfigFileName);
  }
  catch(Exception * E)
  {
    if(XMLDocument != NULL)
    {
      if(XMLDocument->Active)
        XMLDocument->Active = false;
      delete XMLDocument;
      XMLDocument    = NULL;
    }
    if(BillNameList != NULL)
    {
      delete BillNameList;
      BillNameList = NULL;
    }
    throw E;
  }
}
TBillConfig::~TBillConfig()
{
  if(BillNameList != NULL)
  {
    delete BillNameList;
  }
  if(XMLDocument != NULL)
  {
    XMLDocument->Active = false;
    delete XMLDocument;
  }
}
void TBillConfig::ReadBillConfig(AnsiString FileName,bool IncludeDisabled)
{
  BillNameList->Clear();
  if(XMLDocument != NULL && XMLDocument->Active)
  {
    XMLDocument->Active = false;
    delete XMLDocument;
    XMLDocument = NULL;
  }
  XMLDocument = LoadXMLDocument(FileName);
  XMLDocument->Active = true;
  XMLDocument->Options = XMLDocument->Options << doNodeAutoIndent;
  BillRootNode = XMLDocument->ChildNodes->FindNode("Bill");
  if(BillRootNode == NULL)
  {
    throw new Exception("配置文件Config.xml不正确,没有关于话单格式的配置信息!");
  }
  _di_IXMLNode BillNode;
  _di_IXMLNode RecordNode;
  AnsiString BillName;
  for(int n = 0;n < BillRootNode->ChildNodes->Count;n++)
  {
    BillNode   = BillRootNode->ChildNodes->Nodes[n];
    BillName   = GetAttributeNodeValue(BillNode,"BillName");
    RecordNode = BillNode->ChildNodes->FindNode("Record");
    if(RecordNode == NULL || RecordNode->ChildNodes->Count == 0||BillName == "")
      continue;
    if(IsDisabled(BillName)&&!IncludeDisabled)
        continue;
    if(BillName.Length() > 80)
      BillName = BillName.SubString(1,80);
    if(BillNameList->IndexOf(BillName) > -1)
      continue;
    BillNameList->Add(BillName);
  }
}
AnsiString TBillConfig::GetAttributeNodeValue(_di_IXMLNode BillNode,AnsiString AttributrName)
{
  _di_IXMLNode AttributeNode = BillNode->ChildNodes->FindNode(AttributrName);
  if(AttributeNode == NULL)
    return "";
  else
    return AttributeNode->Text;
}
_di_IXMLNode TBillConfig::GetBillNode(AnsiString BillName)
{
  for(int n = 0;n < BillRootNode->ChildNodes->Count;n++)
  {
    if(GetAttributeNodeValue(BillRootNode->ChildNodes->Nodes[n],"BillName") == BillName)
      return BillRootNode->ChildNodes->Nodes[n];
  }
  return NULL;
}
_di_IXMLNode TBillConfig::GetSummaryNode(AnsiString BillName)
{
  _di_IXMLNode BillNode = GetBillNode(BillName);
  if(BillNode == NULL)
    return NULL;
  _di_IXMLNode SummaryNode = BillNode->ChildNodes->FindNode("Summary");
  return SummaryNode;
}
_di_IXMLNode TBillConfig::GetRecordNode(AnsiString BillName)
{
  _di_IXMLNode BillNode = GetBillNode(BillName);
  if(BillNode == NULL)
    return NULL;
  _di_IXMLNode Node = BillNode->ChildNodes->FindNode("Record");
  return Node;
}
_di_IXMLNode TBillConfig::GetFieldNode(AnsiString BillName,AnsiString FieldName)
{
  _di_IXMLNode RecordNode = GetRecordNode(BillName);
  if(RecordNode == NULL)
    return NULL;
  FieldName = FieldName.LowerCase();
  for(int n = 0;n < RecordNode->ChildNodes->Count;n++)
  {
    if(GetFieldAttributeValue(RecordNode->ChildNodes->Nodes[n],"FieldName").LowerCase() == FieldName)
      return RecordNode->ChildNodes->Nodes[n];
  }
  return NULL;
}
AnsiString TBillConfig::GetSummaryAttributeValue(AnsiString BillName,AnsiString AttributeName)
{
  _di_IXMLNode Node = GetSummaryNode(BillName);
  if(Node == NULL)
    return "";
  _di_IXMLNode AttributeNode = Node->ChildNodes->FindNode(AttributeName);
  if(AttributeNode == NULL)
    return "";
  else
    return AttributeNode->Text;
}
int TBillConfig::GetFileHeadLength(AnsiString BillName)
{
  AnsiString str = GetSummaryAttributeValue(BillName,"HeadLength");
  if(str == "")
    return 0;
  else
    return StrToInt(str);
}
int TBillConfig::GetBlockHeadLength(AnsiString BillName)
{
  AnsiString str = GetSummaryAttributeValue(BillName,"BlockHeadHength");
  if(str == "")
    return 0;
  else
    return StrToInt(str);
}
int TBillConfig::GetBlockTrailLength(AnsiString BillName)
{
  AnsiString str = GetSummaryAttributeValue(BillName,"BlockTrailLength");
  if(str == "")
    return 0;
  else
    return StrToInt(str);
}
int TBillConfig::GetFileTrailLength(AnsiString BillName)
{
  AnsiString str = GetSummaryAttributeValue(BillName,"TrailLength");
  if(str == "")
    return 0;
  else
    return StrToInt(str);
}
char TBillConfig::GetFieldSeparator(AnsiString BillName)
{
  AnsiString Separator = GetSummaryAttributeValue(BillName,"Separator");
  if(Separator == "")
    return '|';
  else
  {
    Separator = XMLDecodeForField(Separator);
    char chResult;
    memcpy(&chResult,Separator.c_str(),1);
    return chResult;
  }
}
int TBillConfig::TransFilterAction(AnsiString Action)
{
  if(Action == "Skip")
    return RECORD_FILTER_SKIP;
  //else if(Action == "Read")
  //  return RECORD_FILTER_READ;
  return 0;
}
AnsiString TBillConfig::TransActionName(int Action)
{
  switch(Action)
  {
  case RECORD_FILTER_SKIP:
    return "Skip";
  //case RECORD_FILTER_READ:
  //  return "Read";
  default:
    return 0;
  }
}
void TBillConfig::GetRecordFilter(AnsiString BillName,TList * RecordFilterList)
{
  RecordFilter * prf;
  _di_IXMLNode BillNode = GetBillNode(BillName);
  if(BillNode == NULL)
    return;
  _di_IXMLNode FiltersNode = BillNode->ChildNodes->FindNode("RecordFilter");
  if(FiltersNode == NULL)
    return;
  _di_IXMLNode FilterNode;
  int Pos,Is,Action,Length,Width;
  AnsiString Cause;
  for(int n = 0;n < FiltersNode->ChildNodes->Count;n++)
  {
    FilterNode = FiltersNode->ChildNodes->Nodes[n];
    try
    {
      if(!FilterNode->HasAttribute("Pos"))
        continue;
      if(!FilterNode->HasAttribute("Is"))
        continue;
      if(!FilterNode->HasAttribute("Action"))
        continue;
      if(!FilterNode->HasAttribute("Length"))
        continue;
      //if(!FilterNode->HasAttribute("Cause"))
      //  continue;
      Pos    = StrToInt(FilterNode->GetAttribute("Pos"));
      Is     = StrToInt(FilterNode->GetAttribute("Is"));
      Action = TransFilterAction(FilterNode->GetAttribute("Action"));
      Length = StrToInt(FilterNode->GetAttribute("Length"));
      Width  = StrToInt(FilterNode->GetAttribute("Width"));
      if(FilterNode->HasAttribute("Cause"))
        Cause  = FilterNode->GetAttribute("Cause");
      if(Width < 1)
        Width = 1;
      else if(Width > 4)
        Width = 4;
      prf = new RecordFilter;
      prf->Pos    = Pos;
      prf->Is     = Is;
      prf->Action = Action;
      prf->Length = Length;
      prf->Width  = Width;
      strncpy(&prf->Cause[0],Cause.c_str(),100);
      RecordFilterList->Add((TObject *)prf);
    }
    catch(...)
    {
    }
  }
}
int TBillConfig::GetFileRecordLength(AnsiString BillName)
{
  AnsiString str = GetSummaryAttributeValue(BillName,"RecordLength");
  if(str == "")
    return 0;
  else
    return StrToInt(str);
}
int TBillConfig::GetFileBlockLength(AnsiString BillName)
{
  AnsiString str = GetSummaryAttributeValue(BillName,"BlockLength");
  if(str == "")
    return 0;
  else
    return StrToInt(str);
}
int TBillConfig::TransAlignMode(AnsiString AlignName)
{
  if(AlignName == "CENTER")
    return ALIGN_CENTER;
  else if(AlignName == "RIGHT")
    return ALIGN_RIGHT;
  else
    return ALIGN_LEFT;
}
int TBillConfig::TransDisplayFormat(AnsiString DisplayFormatName)
{
  if(DisplayFormatName == "BITS")
    return FIELD_DISPLAY_AS_BITS;
  else
    return FIELD_DISPLAY_AS_TEXT;
}
int TBillConfig::GetFileFormatType(AnsiString BillName)
{
  AnsiString str = GetSummaryAttributeValue(BillName,"FileFormat");
  if(str == ""||!IsNumber(str.c_str()))
  {
    throw new Exception("没有为话单" + BillName + "定义文件类型");
  }
  else
  {
    int FormatType = StrToInt(str);
    if(TransFormatTypeName(FormatType) == "")
      throw new Exception("文件格式类型不在可以接受的范围内!");
    return FormatType;
  }
}
AnsiString TBillConfig::TransFormatTypeName(int FormatType)
{
  switch(FormatType)
  {
  case BIN_BILL_FORMAT:
    return BIN_BILL_FORMAT_STR;
  case TXT_BILL_FORMAT:
    return TXT_BILL_FORMAT_STR;
  default:
    return "";
  }
}
int TBillConfig::TransFormatType(AnsiString FormatTypeName)
{
  if(FormatTypeName == BIN_BILL_FORMAT_STR)
    return BIN_BILL_FORMAT;
  else if(FormatTypeName == TXT_BILL_FORMAT_STR)
    return TXT_BILL_FORMAT;
  else if(FormatTypeName == SR_BILL_FORMAT_STR)
    return SEARCH_RESULT_FORMAT;
  else
    return 0;
}

int TBillConfig::TransFieldDataType(AnsiString DataTypeName)
{
  if(DataTypeName == "LBCD")
    return DATATYPE_LBCD;
  else if(DataTypeName == "RBCD")
    return DATATYPE_RBCD;
  else if(DataTypeName == "STR")
    return DATATYPE_STR;
  else if(DataTypeName == "LBCD_YYYYMMDDHHMMSS")
    return DATATYPE_LBCD_YYYYMMDDHHMMSS;
  else if(DataTypeName == "RBCD_YYYYMMDDHHMMSS")
    return DATATYPE_RBCD_YYYYMMDDHHMMSS;
  else if(DataTypeName == "LBCD_YYMMDDHHMMSS")
    return DATATYPE_LBCD_YYMMDDHHMMSS;
  else if(DataTypeName == "RBCD_YYMMDDHHMMSS")
    return DATATYPE_RBCD_YYMMDDHHMMSS;
  else if(DataTypeName == "BASED_DATETIME")
    return DATATYPE_BASED_DATETIME;
  else if(DataTypeName == "STR_YYYY_MM_DD_HH_MM_SS")
    return DATATYPE_STR_YYYY_MM_DD_HH_MM_SS;
  else if(DataTypeName == "UINT")
    return DATATYPE_UINT;
  else if(DataTypeName == "WORD")
    return DATATYPE_WORD;
  else if(DataTypeName == "CURRENCY")
    return DATATYPE_CURRENCY;
  else if(DataTypeName == "BIT")
    return DATATYPE_BIT;
  else if(DataTypeName == "STR_YYYYMMDDHHMMSS")
    return DATATYPE_STR_YYYYMMDDHHMMSS;
  else if(DataTypeName == "BIN_YYMMDDHHMMSS")
    return DATATYPE_BIN_YYMMDDHHMMSS;
  else if(DataTypeName == "FIXED")
    return DATATYPE_FIXED;
  else if(DataTypeName == "FILENAME")
    return DATATYPE_FILENAME;
  else if(DataTypeName == "BYTE")
    return DATATYPE_BYTE;
  else if(DataTypeName == "BILLNAME")
    return DATATYPE_BILLNAME;
  else if(DataTypeName == "SHORTFILENAME")
    return DATATYPE_SHORTFILENAME;
  else if(DataTypeName == "INT")
    return DATATYPE_INT;
  else if(DataTypeName == "CURRENCY")
    return DATATYPE_CURRENCY;
  else if(DataTypeName == "DURATION")
    return DATATYPE_DURATION;
  else if(DataTypeName == "RECORDPOSITION")
    return DATATYPE_RECORD_POSITION;
  else
    return DATATYPE_STR;
}
int TBillConfig::TransFieldType(AnsiString FieldTypeName)
{
  if(FieldTypeName == "STARTTIME")
    return FIELDTYPE_STARTTIME;
  else if(FieldTypeName == "ENDTIME")
    return FIELDTYPE_ENDTIME;
  else if(FieldTypeName == "RECORDTYPE")
    return FIELDTYPE_RECORDTYPE;
  else
    return FIELDTYPE_NORMAL;
}
int TBillConfig::TransSortType(AnsiString SortTypeName)
{
  if(SortTypeName == "DATETIME")
    return SORT_AS_DATETIME;
  else if(SortTypeName == "NUMBER")
    return SORT_AS_NUMBER;
  else if(SortTypeName == "CURRENCY")
    return SORT_AS_CURRENCY;
  else
    return SORT_AS_STR;
}
void TBillConfig::AddNewBill(
    AnsiString BillName,
    AnsiString Description,
    int  FileFormatType,
    int  HeaderLength,
    int  RecordLength,
    int  TrailLength,
    AnsiString Separator,
    int  BlockLength,
    int  BlockHeadHength,
    int  BlockTrailLength,
    TList * ActionList,
    bool ReadByBlock,
    bool Disabled)
{
  _di_IXMLNode BillNode    = BillRootNode->AddChild("File",BillRootNode->ChildNodes->Count);
  BillNode->AddChild("BillName")->Text    = BillName;
  BillNode->AddChild("Description")->Text = Description;
  _di_IXMLNode SummaryNode = BillNode->AddChild("Summary",BillNode->ChildNodes->Count);
  SummaryNode->AddChild("FileFormat")->Text       = IntToStr(FileFormatType);
  SummaryNode->AddChild("HeadLength")->Text       = IntToStr(HeaderLength);
  SummaryNode->AddChild("RecordLength")->Text     = IntToStr(RecordLength);
  SummaryNode->AddChild("TrailLength")->Text      = IntToStr(TrailLength);
  SummaryNode->AddChild("Separator")->Text        = XMLEncode(Separator);
  SummaryNode->AddChild("BlockLength")->Text      = IntToStr(BlockLength);
  SummaryNode->AddChild("BlockHeadHength")->Text  = IntToStr(BlockHeadHength);
  SummaryNode->AddChild("BlockTrailLength")->Text = IntToStr(BlockTrailLength);
  if(ReadByBlock)
    SummaryNode->AddChild("IsBlockFile")->Text = "1";
  else
    SummaryNode->AddChild("IsBlockFile")->Text = "0";
  if(Disabled)
    SummaryNode->AddChild("Disabled")->Text = "1";
  else
    SummaryNode->AddChild("Disabled")->Text = "0";
  SetBillRecordFilter(BillName,ActionList);
  BillNode->AddChild("Record",BillNode->ChildNodes->Count);
}
bool TBillConfig::IsBlockFile(AnsiString BillName)
{
  return GetSummaryAttributeValue(BillName,"IsBlockFile") == "1";
}
bool TBillConfig::IsDisabled(AnsiString BillName)
{
  return GetSummaryAttributeValue(BillName,"Disabled") == "1";
}
void TBillConfig::ModifyBill(
    AnsiString BillName,
    AnsiString NewBillName,
    AnsiString Description,
    int  FileFormatType,
    int  HeaderLength,
    int  RecordLength,
    int  TrailLength,
    AnsiString Separator,
    int  BlockLength,
    int  BlockHeadHength,
    int  BlockTrailLength,
    TList * ActionList,
    bool ReadByBlock,
    bool Disabled)
{
  _di_IXMLNode BillNode    = GetBillNode(BillName);
  SetChildAttribute(BillNode,"BillName",NewBillName);
  SetChildAttribute(BillNode,"Description",Description);
  _di_IXMLNode SummaryNode = BillNode->ChildNodes->FindNode("Summary");
  if(SummaryNode == NULL)
    SummaryNode = BillNode->AddChild("Summary",BillNode->ChildNodes->Count);  
  SetChildAttribute(SummaryNode,"FileFormat",IntToStr(FileFormatType));
  SetChildAttribute(SummaryNode,"HeadLength",IntToStr(HeaderLength));
  SetChildAttribute(SummaryNode,"RecordLength",IntToStr(RecordLength));
  SetChildAttribute(SummaryNode,"TrailLength",IntToStr(TrailLength));
  SetChildAttribute(SummaryNode,"Separator",XMLEncode(Separator));
  SetChildAttribute(SummaryNode,"BlockLength",IntToStr(BlockLength));
  SetChildAttribute(SummaryNode,"BlockHeadHength",IntToStr(BlockHeadHength));
  SetChildAttribute(SummaryNode,"BlockTrailLength",IntToStr(BlockTrailLength));
  SetBillRecordFilter(NewBillName,ActionList);
  if(ReadByBlock)
    SetChildAttribute(SummaryNode,"IsBlockFile","1");
  else
    SetChildAttribute(SummaryNode,"IsBlockFile","0");
  if(Disabled)
    SetChildAttribute(SummaryNode,"Disabled","1");
  else
    SetChildAttribute(SummaryNode,"Disabled","0");
}
bool TBillConfig::IsModified()
{
  return (XMLDocument != NULL && XMLDocument->Active && XMLDocument->Modified);

}
bool TBillConfig::SaveChanges(bool BakupOldFile)
{
  try
  {
    //备份原有配置
    if(BakupOldFile)
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
void TBillConfig::Rollback()
{
  if(IsModified())
  {
    ReadBillConfig(XMLDocument->FileName);
  }
}
AnsiString TBillConfig::GetFieldAttributeValue(_di_IXMLNode FieldNode,AnsiString AttributeName,AnsiString DefaultValue)
{
	_di_IXMLNode AttributeNode = FieldNode->ChildNodes->FindNode(AttributeName);
  if(AttributeNode == NULL || AnsiString(AttributeNode->Text) == "")
  {
    return DefaultValue;
  }
  else
    return AttributeNode->Text;
}
AnsiString TBillConfig::GetFieldDataTypeNameText(int DataType)
{
  switch(DataType)
  {
  case DATATYPE_LBCD://BCD码左对齐
    return "BCD码左对齐";
  case DATATYPE_RBCD://BCD码右对齐
    return "BCD码右对齐";
  case DATATYPE_LBCD_YYYYMMDDHHMMSS://BCD码左对齐日期时间(四位年份).
    return "BCD码左对齐日期时间(四位年份)";
  case DATATYPE_RBCD_YYYYMMDDHHMMSS://BCD码右对齐日期时间(四位年份).
    return "BCD码右对齐日期时间(四位年份)";
  case DATATYPE_LBCD_YYMMDDHHMMSS://BCD码左对齐日期时间(两位年份).
    return "BCD码左对齐日期时间(两位年份)";
  case DATATYPE_RBCD_YYMMDDHHMMSS://BCD码右对齐日期时间(两位年份).
    return "BCD码右对齐日期时间(两位年份)";
  case DATATYPE_BASED_DATETIME://有基准时间的日期时间格式
    return "有基准时间的日期时间格式";
  case DATATYPE_STR_YYYY_MM_DD_HH_MM_SS://文本文件日期时间字段.格式为:YYYY-MM-DD HH:MM:SS
    return "文本文件日期时间字段(YYYY-MM-DD HH:MM:SS)";
  case DATATYPE_UINT:// 无符号长整数(4字节)
    return "无符号长整数(4字节)";
  case DATATYPE_WORD://无符号短整数(2字节)
    return "无符号短整数(2字节)";
  case DATATYPE_BYTE://字节(1Byte),统一为unsigned char
    return "字节(Byte)";
  case DATATYPE_BIT://BIT位
    return "BIT位";
  case DATATYPE_CURRENCY://货币(精确到小数点后两位).
    return "货币(精确到小数点后两位)";
  case DATATYPE_STR_YYYYMMDDHHMMSS://文本文件日期格式字段(四位年份).
    return "文本文件日期格式字段(YYYYMMDDHHMMSS)";
  case DATATYPE_BIN_YYMMDDHHMMSS://二进制日期时间
    return "二进制日期时间";
  case DATATYPE_FIXED://固定值
    return "固定值";
  case DATATYPE_FILENAME://取记录所在的文件名
    return "带路径话单文件名";
  case DATATYPE_BILLNAME://取话单名称
    return "话单名称";
  case DATATYPE_SHORTFILENAME://不带路径的文件名
    return "不带路径话单文件名";
  case DATATYPE_INT://有符号长整数
    return "有符号长整数";
  case DATATYPE_DURATION://通话时长
    return "通话时长";
  case DATATYPE_RECORD_POSITION:
    return "记录位置";
  default://字符串
    return "字符串";
  }
}
AnsiString TBillConfig::GetDataTypeName(int DataType)
{
  switch(DataType)
  {
  case DATATYPE_LBCD://BCD码左对齐
    return "LBCD";
  case DATATYPE_RBCD://BCD码右对齐
    return "RBCD";
  case DATATYPE_LBCD_YYYYMMDDHHMMSS://BCD码左对齐日期时间(四位年份).
    return "LBCD_YYYYMMDDHHMMSS";
  case DATATYPE_RBCD_YYYYMMDDHHMMSS://BCD码右对齐日期时间(四位年份).
    return "RBCD_YYYYMMDDHHMMSS";
  case DATATYPE_LBCD_YYMMDDHHMMSS://BCD码左对齐日期时间(两位年份).
    return "LBCD_YYMMDDHHMMSS";
  case DATATYPE_RBCD_YYMMDDHHMMSS://BCD码右对齐日期时间(两位年份).
    return "RBCD_YYMMDDHHMMSS";
  case DATATYPE_BASED_DATETIME://有基准时间的日期时间格式
    return "BASED_DATETIME";
  case DATATYPE_STR_YYYY_MM_DD_HH_MM_SS://文本文件日期时间字段.格式为:YYYY-MM-DD HH:MM:SS
    return "STR_YYYY_MM_DD_HH_MM_SS";
  case DATATYPE_UINT:// 无符号长整数(4字节)
    return "UINT";
  case DATATYPE_WORD://无符号短整数(2字节)
    return "WORD";
  case DATATYPE_BYTE://字节(1Byte),统一为unsigned char
    return "BYTE";
  case DATATYPE_BIT://BIT位
    return "BIT";
  case DATATYPE_CURRENCY://货币(精确到小数点后两位).
    return "CURRENCY";
  case DATATYPE_STR_YYYYMMDDHHMMSS://文本文件日期格式字段(四位年份).
    return "STR_YYYYMMDDHHMMSS";
  case DATATYPE_BIN_YYMMDDHHMMSS://二进制日期时间
    return "BIN_YYMMDDHHMMSS";
  case DATATYPE_FIXED://固定值
    return "FIXED";
  case DATATYPE_FILENAME://取记录所在的文件名
    return "FILENAME";
  case DATATYPE_BILLNAME://取话单名称
    return "BILLNAME";
  case DATATYPE_SHORTFILENAME://不带路径的文件名
    return "SHORTFILENAME";
  case DATATYPE_INT://有符号长整数
    return "INT";
  case DATATYPE_DURATION://通话时长
    return "DURATION";
  case DATATYPE_RECORD_POSITION:
    return "RECORDPOSITION";
  default://字符串
    return "STR";
  }
}
void TBillConfig::AddNewField(
    AnsiString BillName,
    AnsiString FieldName,
    bool Disabled,
    AnsiString Label,
    int Offset,
    int Length,
    int FieldType,
    int DataType,
    int BitPosition,
    int BitWidth,
    AnsiString EndChar,
    AnsiString FixedValue,
    TDateTime BaseDateTime,
    int SortType,
    bool Import,
    bool CreateIndex,
    bool DisplayInList,
    int DisplayWidth,
    int Align,
    int DisplayMode,
    int ExportLength,
    TStringList * Trans
    )
{
  _di_IXMLNode BillNode = GetRecordNode(BillName);
  _di_IXMLNode FieldNode = BillNode->AddChild("Field",BillNode->ChildNodes->Count);
  FieldNode->AddChild("FieldName")->Text = FieldName;
  FieldNode->AddChild("Label")->Text     = Label;
  FieldNode->AddChild("Offset")->Text    = IntToStr(Offset);
  FieldNode->AddChild("FieldLength")->Text    = IntToStr(Length);
  if(Disabled)
    FieldNode->AddChild("Disabled")->Text = "1";
  else
    FieldNode->AddChild("Disabled")->Text = "0";
  switch(FieldType)
  {
  case FIELDTYPE_STARTTIME:
    FieldNode->AddChild("FieldType")->Text = "STARTTIME";
    break;
  case FIELDTYPE_ENDTIME:
    FieldNode->AddChild("FieldType")->Text = "ENDTIME";
    break;
  case FIELDTYPE_RECORDTYPE:
    FieldNode->AddChild("FieldType")->Text = "RECORDTYPE";
    break;
  default:
    FieldNode->AddChild("FieldType")->Text = "NORMAL";
    break;
  }
  FieldNode->AddChild("DataType")->Text      = GetDataTypeName(DataType);
  FieldNode->AddChild("BitPosition")->Text   = IntToStr(BitPosition);
  FieldNode->AddChild("BitWidth")->Text      = IntToStr(BitWidth);
  FieldNode->AddChild("EndChar")->Text       = EndChar;
  FieldNode->AddChild("FixedValue")->Text    = FixedValue;
  FieldNode->AddChild("BaseDateTime")->Text  = FormatDateTime("yyyy-mm-dd hh:nn:ss",BaseDateTime);
  switch(SortType)
  {
  case SORT_AS_DATETIME:
    FieldNode->AddChild("SortType")->Text  = "DATETIME";
    break;
  case SORT_AS_NUMBER:
    FieldNode->AddChild("SortType")->Text  = "NUMBER";
    break;
  case SORT_AS_CURRENCY:
    FieldNode->AddChild("SortType")->Text  = "CURRENCY";
    break;
  default:
    FieldNode->AddChild("SortType")->Text  = "STR";
    break;
  }
  if(Import)
    FieldNode->AddChild("Import")->Text  = "1";
  else
    FieldNode->AddChild("Import")->Text  = "0";
  if(CreateIndex)
    FieldNode->AddChild("CreateIndex")->Text  = "1";
  else
    FieldNode->AddChild("CreateIndex")->Text  = "0";
  if(DisplayInList)
    FieldNode->AddChild("DisplayInList")->Text  = "1";
  else
    FieldNode->AddChild("DisplayInList")->Text  = "0";
  FieldNode->AddChild("DisplayWidth")->Text  = IntToStr(DisplayWidth);
  switch(Align)
  {
  case ALIGN_CENTER:
    FieldNode->AddChild("DisplayAlign")->Text  = "CENTER";
    break;
  case ALIGN_RIGHT:
    FieldNode->AddChild("DisplayAlign")->Text  = "RIGHT";
    break;
  default:
    FieldNode->AddChild("DisplayAlign")->Text  = "LEFT";
    break;
  }
  switch(DisplayMode)
  {
  case FIELD_DISPLAY_AS_BITS:
    FieldNode->AddChild("DisplayFormat")->Text  = "BITS";
    break;
  default:
    FieldNode->AddChild("DisplayFormat")->Text  = "TEXT";
    break;
  }
  if(ExportLength <= 0)
    FieldNode->AddChild("ExportLength")->Text  = "255";
  else
    FieldNode->AddChild("ExportLength")->Text  = IntToStr(ExportLength);
  SetFieldTrans(BillName,FieldName,Trans);
}
void TBillConfig::SetChildAttribute(_di_IXMLNode ParentNode,AnsiString ChildNodeName,AnsiString Text)
{
  _di_IXMLNode Node = ParentNode->ChildNodes->FindNode(ChildNodeName);
  if(Node == NULL)
    ParentNode->AddChild(ChildNodeName)->Text = Text;
  else
    Node->Text = Text;
}
void TBillConfig::ModifyField(
    AnsiString BillName,
    AnsiString FieldName,
    AnsiString NewFieldName,
    bool Disabled,
    AnsiString Label,
    int Offset,
    int Length,
    int FieldType,
    int DataType,
    int BitPosition,
    int BitWidth,
    AnsiString EndChar,
    AnsiString FixedValue,
    TDateTime BaseDateTime,
    int SortType,
    bool Import,
    bool CreateIndex,
    bool DisplayInList,
    int DisplayWidth,
    int Align,
    int DisplayMode,
    int ExportLength,
    TStringList * Trans
    )
{
  _di_IXMLNode FieldNode = GetFieldNode(BillName,FieldName);
  SetChildAttribute(FieldNode,"FieldName",NewFieldName);
  SetChildAttribute(FieldNode,"Label",Label);
  SetChildAttribute(FieldNode,"Offset",IntToStr(Offset));
  SetChildAttribute(FieldNode,"FieldLength",IntToStr(Length));
  if(Disabled)
    SetChildAttribute(FieldNode,"Disabled","1");
  else
    SetChildAttribute(FieldNode,"Disabled","0");

  switch(FieldType)
  {
  case FIELDTYPE_STARTTIME:
    SetChildAttribute(FieldNode,"FieldType","STARTTIME");
    break;
  case FIELDTYPE_ENDTIME:
    SetChildAttribute(FieldNode,"FieldType","ENDTIME");
    break;
  default:
    SetChildAttribute(FieldNode,"FieldType","NORMAL");
    break;
  }
  SetChildAttribute(FieldNode,"DataType",GetDataTypeName(DataType));
  SetChildAttribute(FieldNode,"BitPosition",IntToStr(BitPosition));
  SetChildAttribute(FieldNode,"BitWidth",IntToStr(BitWidth));
  SetChildAttribute(FieldNode,"EndChar",EndChar);
  SetChildAttribute(FieldNode,"FixedValue",FixedValue);
  SetChildAttribute(FieldNode,"BaseDateTime",FormatDateTime("yyyy-mm-dd hh:nn:ss",BaseDateTime));
  switch(SortType)
  {
  case SORT_AS_DATETIME:
    SetChildAttribute(FieldNode,"SortType","DATETIME");
    break;
  case SORT_AS_NUMBER:
    SetChildAttribute(FieldNode,"SortType","NUMBER");
    break;
  case SORT_AS_CURRENCY:
    SetChildAttribute(FieldNode,"SortType","CURRENCY");
    break;
  default:
    SetChildAttribute(FieldNode,"SortType","STR");
    break;
  }
  if(Import)
    SetChildAttribute(FieldNode,"Import","1");
  else
    SetChildAttribute(FieldNode,"Import","0");
  if(CreateIndex)
    SetChildAttribute(FieldNode,"CreateIndex","1");
  else
    SetChildAttribute(FieldNode,"CreateIndex","0");
  if(DisplayInList)
    SetChildAttribute(FieldNode,"DisplayInList","1");
  else
    SetChildAttribute(FieldNode,"DisplayInList","0");
  SetChildAttribute(FieldNode,"DisplayWidth",IntToStr(DisplayWidth));
  switch(Align)
  {
  case ALIGN_CENTER:
    SetChildAttribute(FieldNode,"DisplayAlign","CENTER");
    break;
  case ALIGN_RIGHT:
    SetChildAttribute(FieldNode,"DisplayAlign","RIGHT");
    break;
  default:
    SetChildAttribute(FieldNode,"DisplayAlign","LEFT");
    break;
  }
  switch(DisplayMode)
  {
  case FIELD_DISPLAY_AS_BITS:
    SetChildAttribute(FieldNode,"DisplayFormat","BITS");
    break;
  default:
    SetChildAttribute(FieldNode,"DisplayFormat","TEXT");
    break;
  }
  if(ExportLength <= 0)
    SetChildAttribute(FieldNode,"ExportLength","255");
  else
    SetChildAttribute(FieldNode,"ExportLength",IntToStr(ExportLength));
  SetFieldTrans(BillName,NewFieldName,Trans);
}
void TBillConfig::DeleteField(AnsiString BillName,AnsiString FieldName)
{
  _di_IXMLNode FieldNode  = NULL;
  _di_IXMLNode RecordNode = GetRecordNode(BillName);
  if(RecordNode == NULL)
    return;
  for(int n = RecordNode->ChildNodes->Count - 1;n > -1 ;n--)
  {
    FieldNode = RecordNode->ChildNodes->Nodes[n];
    if(GetFieldAttributeValue(FieldNode,"FieldName") == FieldName)
      RecordNode->ChildNodes->Delete(n);  
  }
}
void TBillConfig::DeleteBill(AnsiString BillName)
{
  _di_IXMLNode BillNode  = NULL;
  for(int n = BillRootNode->ChildNodes->Count - 1;n > -1 ;n--)
  {
    BillNode = BillRootNode->ChildNodes->Nodes[n];
    if(GetFieldAttributeValue(BillNode,"BillName") == BillName)
      BillRootNode->ChildNodes->Delete(n);  
  }
}
void TBillConfig::GetFieldTrans(AnsiString BillName,AnsiString FieldName,TStringList * TransList)
{
  TransList->Clear();
  _di_IXMLNode ValuesNode = GetFieldNode(BillName,FieldName)->ChildNodes->FindNode("Values");
  if(ValuesNode == NULL)
    return;
  _di_IXMLNode Node;
  OleVariant v;
  AnsiString Value,Context;
  for(int n = 0;n < ValuesNode->ChildNodes->Count;n++)
  {
    Node = ValuesNode->ChildNodes->Nodes[n];
    if(AnsiString(Node->NodeName) != "Trans")
      continue;
    v = Node->GetAttribute("Value");
    if(v.IsNull())
      continue;
    Value = v;
    v = Node->GetAttribute("Context");
    if(v.IsNull())
      continue;
    Context = v;
    TransList->AddObject(Context,(TObject *)StrToInt(Value));
  }
}
int TBillConfig::TransReplacePosition(AnsiString Position)
{
  if(Position == "RIGHT")
    return REPLACE_RIGHT;
  else if(Position == "ANY")
    return REPLACE_ANY;
  else
    return REPLACE_LEFT;
}
AnsiString TBillConfig::GetFieldReplaceRules(AnsiString BillName,AnsiString FieldName)
{
  _di_IXMLNode FieldNode       = GetFieldNode(BillName,FieldName);
  _di_IXMLNode ReplaceRuleNode = FieldNode->ChildNodes->FindNode("ReplaceRule");
  if(ReplaceRuleNode == NULL)
    return "";
  AnsiString Result = "";
  _di_IXMLNode RuleNode;
  AnsiString ReplaceFrom,ReplaceTo,Position,Count;
  OleVariant v;
  for(int n = 0; n < ReplaceRuleNode->ChildNodes->Count;n++)
  {
    RuleNode    = ReplaceRuleNode->ChildNodes->Nodes[n];
    if(AnsiString(RuleNode->NodeName) != "Rule")
      continue; 
    v = RuleNode->GetAttribute("From");
    if(v.IsNull())
      continue;
    ReplaceFrom = v;
    v = RuleNode->GetAttribute("To");
    if(v.IsNull())
      continue;
    ReplaceTo   = v;
    v = RuleNode->GetAttribute("Position");
    if(v.IsNull())
      continue;
    Position    = v;
    v = RuleNode->GetAttribute("Count");
    if(v.IsNull())
      continue;
    Count       = v;
    Result += ReplaceFrom + "|" + ReplaceTo + "|" + Position + "|" + Count + "|";
  }
  return Result;
}
void TBillConfig::SetBillRecordFilter(AnsiString BillName,TList * ActionList)
{
  _di_IXMLNode BillNode = GetBillNode(BillName);
  _di_IXMLNode FilterNode = BillNode->ChildNodes->FindNode("RecordFilter");
  if(FilterNode == NULL)
    FilterNode = BillNode->AddChild("RecordFilter");
  else
    FilterNode->ChildNodes->Clear();
  RecordFilter * prf;
  _di_IXMLNode Node;
  for(int n = 0;n < ActionList->Count;n++)
  {
    prf = (RecordFilter *)ActionList->Items[n]; 
    Node = FilterNode->AddChild("Filter",FilterNode->ChildNodes->Count);
    Node->SetAttribute("Pos",prf->Pos);
    Node->SetAttribute("Width",prf->Width);
    Node->SetAttribute("Is",prf->Is);
    Node->SetAttribute("Action",TransActionName(prf->Action));
    Node->SetAttribute("Length",prf->Length);
    Node->SetAttribute("Cause",AnsiString(prf->Cause));
  }
}
void TBillConfig::SetFiledReplaceRule(AnsiString BillName,AnsiString FieldName,AnsiString ReplaceReuls)
{
  _di_IXMLNode FieldNode = GetFieldNode(BillName,FieldName);
  _di_IXMLNode ReplaceRuleNode = FieldNode->ChildNodes->FindNode("ReplaceRule");
  if(ReplaceRuleNode == NULL)
    ReplaceRuleNode = FieldNode->AddChild("ReplaceRule");
  else
    ReplaceRuleNode->ChildNodes->Clear();
  TStringList * List = new TStringList();
  Split(ReplaceReuls,"|",List);
  _di_IXMLNode Node;
  for(int n = 0;n < List->Count;n++)
  {
    Node = ReplaceRuleNode->AddChild("Rule",ReplaceRuleNode->ChildNodes->Count);
    Node->SetAttribute("From",List->Strings[n]);
    Node->SetAttribute("To",List->Strings[++n]);
    Node->SetAttribute("Position",List->Strings[++n]);
    Node->SetAttribute("Count",List->Strings[++n]);
  }
  delete List;
}
void TBillConfig::SetFieldTrans(AnsiString BillName,AnsiString FieldName,TStringList * TransList)
{
  _di_IXMLNode FieldNode = GetFieldNode(BillName,FieldName);
  _di_IXMLNode ValuesNode = FieldNode->ChildNodes->FindNode("Values");
  if(ValuesNode == NULL)
    ValuesNode = FieldNode->AddChild("Values");
  else
    ValuesNode->ChildNodes->Clear();
  _di_IXMLNode Node;
  for(int n = 0;n < TransList->Count;n++)
  {
    Node = ValuesNode->AddChild("Trans",ValuesNode->ChildNodes->Count);
    Node->SetAttribute("Value",IntToStr((int)(TransList->Objects[n])));
    Node->SetAttribute("Context",TransList->Strings[n]);
  }
}

