

#include "BillField.h"
#include <StrUtils.hpp>
#include <math.h>

//当前正在进行比较的字段
TBillField * CompareField;


/******************************************************************************
*C全局函数,为qsort提供
******************************************************************************/
//文本排序规则
int CompareBinStrFieldValue(const void *arg1, const void *arg2)
{
  AnsiString str1,str2;
  CompareField->DataBillFile->ManualSetRecordBuffer((const char *)arg1);
  str1 = CompareField->GetFieldValue(false);
  CompareField->DataBillFile->ManualSetRecordBuffer((const char *)arg2);
  str2 = CompareField->GetFieldValue(false);
  return strcmp(str1.c_str(),str2.c_str()) * CompareField->CompareDirection;
}
//日期时间排序规则
int CompareBinDateTimeFieldValue(const void *arg1, const void *arg2)
{
  try
  {
    TDateTime T1,T2;
    CompareField->DataBillFile->ManualSetRecordBuffer((const char *)arg1);
    T1 = StrToDateTime(CompareField->GetFieldValue(false));
    CompareField->DataBillFile->ManualSetRecordBuffer((const char *)arg2);
    T2 = StrToDateTime(CompareField->GetFieldValue(false));
    int Result = 0;
    if(T1 < T2)
      Result = -1;
    else if(T1 > T2)
      Result = 1;
    return Result * CompareField->CompareDirection;
  }
  catch(...)
  {
    return -1;
  }  
}
int CompareBinNumberFieldValue(const void *arg1, const void *arg2)
{
  try
  {
    int T1,T2;
    CompareField->DataBillFile->ManualSetRecordBuffer((const char *)arg1);
    T1 = atoi(CompareField->GetFieldValue(false).c_str());//StrToInt(CompareField->GetFieldValue(false));
    CompareField->DataBillFile->ManualSetRecordBuffer((const char *)arg2);
    T2 = atoi(CompareField->GetFieldValue(false).c_str());//StrToInt(CompareField->GetFieldValue(false));
    int Result = 0;
    if(T1 < T2)
      Result = -1;
    else if(T1 > T2)
      Result = 1;
    return Result * CompareField->CompareDirection;
  }
  catch(...)
  {
    return -1;
  }  
}
/*
int __fastcall CompareTxtStrFieldValue(TStringList* RecordList, int Index1, int Index2)
{
  AnsiString str1,str2;
  CompareField->BillFile->SetRecordBuffer(RecordList->Strings[Index1].c_str());
  str1 = CompareField->GetFieldValue();
  CompareField->BillFile->SetRecordBuffer(RecordList->Strings[Index2].c_str());
  str2 = CompareField->GetFieldValue();
  return strcmp(str1.c_str(),str2.c_str()) * CompareField->CompareDirection;
}
//文本文件的日期时间排序规则
int __fastcall CompareTxtDateTimeFieldValue(TStringList* RecordList, int Index1, int Index2)
{
  try
  {
    TDateTime T1,T2;
    CompareField->BillFile->SetRecordBuffer(RecordList->Strings[Index1].c_str());
    T1 = StrToDateTime(CompareField->GetFieldValue());
    CompareField->BillFile->SetRecordBuffer(RecordList->Strings[Index2].c_str());
    T2 = StrToDateTime(CompareField->GetFieldValue());
    int Result = 0;
    if(T1 < T2)
      Result = -1;
    else if(T1 > T2)
      Result = 1;
    return Result * CompareField->CompareDirection;
  }
  catch(...)
  {
    return -1;
  }
}
int __fastcall CompareTxtNumberFieldValue(TStringList* RecordList, int Index1, int Index2)
{
  try
  {
    Currency T1,T2;
    CompareField->BillFile->SetRecordBuffer(RecordList->Strings[Index1].c_str());
    T1 = StrToCurr(CompareField->GetFieldValue());
    CompareField->BillFile->SetRecordBuffer(RecordList->Strings[Index2].c_str());
    T2 = StrToCurr(CompareField->GetFieldValue());
    int Result = 0;
    if(T1 < T2)
      Result = -1;
    else if(T1 > T2)
      Result = 1;
    return Result * CompareField->CompareDirection;
  }
  catch(...)
  {
    return -1;
  }
}
*/

/******************************************************************************
*字段内容匹配,查找记录时使用
*如果str1 < str2 返回-1
*如果str1 > str2 返回 1
*如果str1 = str2 返回 0
*Str1是要查找的字符串
*Str2是被查找的字符串
******************************************************************************/
int __fastcall MatchStrFieldValue(const AnsiString & Str1,const AnsiString & Str2,int Param)
{
  return strncmp(Str1.c_str(),Str2.c_str(),Param); 
}
int __fastcall MatchDateTimeFieldValue(const AnsiString & Str1,const AnsiString & Str2,int Param)
{
  try
  {
    TDateTime T1(Str1);
    TDateTime T2(Str2);
    if(T1 < T2)
      return -1;
    else if(T1 > T2)
      return 1;
    else
      return 0;
  }
  catch(...)
  {
    return -1;
  }
}
int __fastcall MatchNumberFieldValue(const AnsiString & Str1,const AnsiString & Str2,int Param)
{
  try
  {
    int T1 = StrToInt(Str1);
    int T2 = StrToInt(Str2);
    if(T1 < T2)
      return -1;
    else if(T1 > T2)
      return 1;
    else
      return 0;
  }
  catch(...)
  {
    return -1;
  }
}
int __fastcall MatchCurrencyFieldValue(const AnsiString & Str1,const AnsiString & Str2,int Param)
{
  try
  {
    int T1 = StrToCurr(Str1);
    int T2 = StrToCurr(Str2);
    if(T1 < T2)
      return -1;
    else if(T1 > T2)
      return 1;
    else
      return 0;
  }
  catch(...)
  {
    return -1;
  }
}
/****************************************************************************
*话单字段类
*如果FieldNode==NULL,表示调用类要自已初始化TBillField.
****************************************************************************/
TBillField::TBillField(TBillFile * ABillFile,_di_IXMLNode FieldNode)
{
  DateSeparator   = '-';
  ShortDateFormat = "yyyy-m-d";

  ValueTransList    = NULL;
  BillFile          = ABillFile;
  DataBillFile      = ABillFile;
  //Tag               = 0;
  ReplaceRuleList   = NULL;
  ConvertData       = NULL;
  CompareFieldData  = NULL;
  GetFieldValueFunc = NULL;
  TBillConfig * BillConfig = BillFile->BillConfig;
  int DataType;
  if(FieldNode != NULL)
  {
    AnsiString str;
    FieldName = BillConfig->GetFieldAttributeValue(FieldNode,"FieldName");
    if(FieldName == "")
      throw new Exception("没有为字段定义名称");
    str = BillConfig->GetFieldAttributeValue(FieldNode,"DataType");
    if(str == "")
      throw new Exception("没有为字段" + FieldName + "定义数据类型");
    DataType = BillConfig->TransFieldDataType(str);
    if(DataType == DATATYPE_BASED_DATETIME)
    {
      AnsiString s = BillConfig->GetFieldAttributeValue(FieldNode,"BaseDateTime","1900-01-01 00:00:00");
      if(s == "")
        throw new Exception("没有为字段" + FieldName + "定义基准时间");
      try
      {
        BaseDateTime = StrToDateTime(s);
      }
      catch(...)
      {
        throw new Exception("为字段" + FieldName + "定义的基准时间不正确");
      }
    }
    Label   = BillConfig->GetFieldAttributeValue(FieldNode,"Label",FieldName);
    str = BillConfig->GetFieldAttributeValue(FieldNode,"EndChar");
    if(str == "")
      EndChar = 0x0f;
    else
    {
      memcpy(&EndChar,str.c_str(),1);
      if(EndChar >= 'a' && EndChar <= 'f')
        EndChar -= 87;
      else if(EndChar >= 'A' && EndChar <= 'F')
        EndChar -= 55;
      else
        EndChar = 0x0f;
    }
    str = BillConfig->GetFieldAttributeValue(FieldNode,"DisplayWidth","0");
    if(!IsNumber(str.c_str()))
      DisplayWidth = 0;
    else
      DisplayWidth   = StrToInt(str);
    DisplayInList  = BillConfig->GetFieldAttributeValue(FieldNode,"DisplayInList","1") != "0";
    DisplayAlign = BillConfig->TransAlignMode(BillConfig->GetFieldAttributeValue(FieldNode,"DisplayAlign","LEFT"));
    DisplayFormat = BillConfig->TransDisplayFormat(BillConfig->GetFieldAttributeValue(FieldNode,"DisplayFormat","TEXT"));
    CreateIndex = BillConfig->GetFieldAttributeValue(FieldNode,"CreateIndex","0") == "1";
    if(DataType == DATATYPE_BIT)
    {
      str = BillConfig->GetFieldAttributeValue(FieldNode,"BitPosition","0");
      if(IsNumber(str.c_str()))
        BitPosition = StrToInt(str);
      else
        BitPosition = 0;
      if(BitPosition < 0 || BitPosition > 7)
        throw new Exception("为字段" + FieldName + "定义的BIT位置[" + IntToStr(BitPosition) + "]不正确.");
      str = BillConfig->GetFieldAttributeValue(FieldNode,"BitWidth");
      if(IsNumber(str.c_str()))
        BitWidth = StrToInt(str);
      else
        BitWidth = 0;
      if(BitWidth < 1 || BitWidth > 8)
        throw new Exception("为字段" + FieldName + "定义的BIT宽度[" + IntToStr(BitWidth) + "]不正确.");
    }
    FixedValue     = BillConfig->GetFieldAttributeValue(FieldNode,"FixedValue");
    FieldType = BillConfig->TransFieldType(BillConfig->GetFieldAttributeValue(FieldNode,"FieldType"));
    SortType  = BillConfig->TransSortType(BillConfig->GetFieldAttributeValue(FieldNode,"SortType","STR"));
    switch(SortType)
    {
    case SORT_AS_DATETIME:
      ExportLength = 20;
      break;
    case SORT_AS_NUMBER:
      ValueTransList = new TStringList;
      BillConfig->GetFieldTrans(BillFile->GetBillName(),FieldName,ValueTransList);
      if(ValueTransList->Count == 0)
      {
        delete ValueTransList;
        ValueTransList = NULL;
      }
    case SORT_AS_CURRENCY:
      ExportLength = 10;
      break;
    default:
      str = BillConfig->GetFieldAttributeValue(FieldNode,"ExportLength");
      if(str == "" || !IsNumber(str.c_str()))
      {
        ExportLength = 255;
      }
      else
      {
        ExportLength = StrToInt(str);
        if(ExportLength > 255 || ExportLength < 1)
          ExportLength = 255;
      }
      break;
    }
    if(BillFile->FileType == TXT_BILL_FORMAT)
    {
      FieldLength = 0;
    }
    else
    {
      str = BillConfig->GetFieldAttributeValue(FieldNode,"FieldLength");
      if(IsNumber(str.c_str()))
      {
        FieldLength = StrToInt(str);
      }
      else
      {
        throw new Exception("为字段" + FieldName + "定义的字段长度[" + str + "]超出了范围.");
      }
    }
    if(DataType == DATATYPE_DURATION ||
       DataType == DATATYPE_FIXED    ||
       DataType == DATATYPE_FILENAME ||
       DataType == DATATYPE_BILLNAME ||
       DataType == DATATYPE_SHORTFILENAME ||
       DataType == DATATYPE_RECORD_POSITION)
    {
      Offset      = -1;
      FieldLength = 0;
    }
    else
    {
      str = BillConfig->GetFieldAttributeValue(FieldNode,"Offset","0");
      if(IsNumber(str.c_str()))
      {
        Offset = StrToInt(str);
      }
      else
      {
        throw new Exception("为字段" + FieldName + "定义的偏移位置[" + str + "]超出了范围.");
      }
    }
    ImportToDB = BillConfig->GetFieldAttributeValue(FieldNode,"Import","1") != 0;
    //读取替换规则
    AnsiString ReplaceRules = BillConfig->GetFieldReplaceRules(BillFile->GetBillName(),FieldName);
    TStringList * StringList = new TStringList;
    Split(ReplaceRules,"|",StringList);
    AnsiString ReplaceFrom;
    AnsiString ReplaceTo;
    if(StringList->Count > 0)
      ReplaceRuleList = new TList;
    for(int n = 0;n < StringList->Count;n++)
    {
      ReplaceRule * AReplaceRule = new ReplaceRule;
      ReplaceFrom = XMLDecodeForField(StringList->Strings[n]);
      ReplaceTo   = XMLDecodeForField(StringList->Strings[++n]);
      strncpy(&AReplaceRule->chFrom[0],ReplaceFrom.c_str(),256);
      strncpy(&AReplaceRule->chTo[0],ReplaceTo.c_str(),256);
      AReplaceRule->FromLength = ReplaceFrom.Length();
      AReplaceRule->ToLength   = ReplaceTo.Length();
      AReplaceRule->Position = BillConfig->TransReplacePosition(StringList->Strings[++n]);
      AReplaceRule->Count = StrToInt(StringList->Strings[++n]);
      if(AReplaceRule->Count <= 0)
        AReplaceRule->Count = 9999999999;
      ReplaceRuleList->Add((TObject *)AReplaceRule);
    }
    if(GetValueTransCount() > 0)
    {
      SortType     = SORT_AS_STR;
      ExportLength = 255;
    }
    SetDataType(DataType);
/*
    if(GetValueTransCount() > 0)
    {
      CompareFieldData = CompareBinStrFieldValue;
      MatchFieldData   = MatchStrFieldValue;
    }
*/
  }
}
TBillField::~TBillField()
{
  if(ValueTransList != NULL)
  {
    delete ValueTransList;
    ValueTransList = NULL;
  }
  if(ReplaceRuleList != NULL)
  {
    for(int n = 0;n < ReplaceRuleList->Count;n++)
    {
      delete ((ReplaceRule *)ReplaceRuleList->Items[n]);
    }
    delete ReplaceRuleList;
    ReplaceRuleList = NULL;
  }
}
void TBillField::AddValueTrans(int Value,AnsiString Trans)
{
  if(ValueTransList == NULL)
    ValueTransList = new TStringList;
  else
  {
    if(ValueTransList->IndexOfObject((TObject *)Value) > -1)
      return;
  }
  ValueTransList->AddObject(Trans,(TObject *)Value);
}
//读取字段的原始数据
const char * TBillField::GetFieldOriData()
{
  switch(FieldType)
  {
  case FIELDTYPE_SEARCH_RESULT_POS:
  case FIELDTYPE_SEARCH_RESULT_FILENAME:
    return GetFieldValue().c_str();
  default:
    return BillFile->BillRecord->GetBuffer(Offset);
  }
}
void TBillField::GetFieldOriData(char chDest[],int BufferLen)
{
  const char * Buffer = BillFile->BillRecord->GetBuffer(Offset);
  if(BufferLen > FieldLength)
    memcpy(&chDest,Buffer,FieldLength);
  else
    memcpy(&chDest,Buffer,BufferLen);
}
//根据字段的数据类型设置字段的解码和比较函数
void TBillField::SetDataType(int ADataType)
{
  DataType = ADataType;
  switch(DataType)
  {
  case DATATYPE_BIT:
    GetFieldValueFunc = GetFieldValue_DATATYPE_BIT;
    break;
  case DATATYPE_DURATION:
    GetFieldValueFunc = GetFieldValue_DATATYPE_DURATION;
    FieldLength = 0;
    break;
  case DATATYPE_LBCD:
    ConvertData   = LeftBCDToStr;
    break;
  case DATATYPE_RBCD:
    ConvertData      = RightBCDToStr;
    break;
  case DATATYPE_BASED_DATETIME:
    ConvertData      = ConvertBasedDateTime;
    GetFieldValueFunc  = GetFieldValue_DATATYPE_BASED_DATETIME;
    break;
  case DATATYPE_WORD:
    ConvertData      = ConvertToWord;
    break;
  case DATATYPE_RBCD_YYYYMMDDHHMMSS:
    ConvertData      = ConvertRBCDYYYYMMDDHHMMSS;
    break;
  case DATATYPE_RBCD_YYMMDDHHMMSS:
    ConvertData      = ConvertRBCDYYMMDDHHMMSS;
    break;
  case DATATYPE_STR:
    ConvertData      = ConvertStr;
    break;
  case DATATYPE_STR_YYYYMMDDHHMMSS:
    ConvertData      = ConvertSTRYYYYMMDDHHMMSS;
    break;
  case DATATYPE_BIN_YYMMDDHHMMSS:
    ConvertData      = ConvertBINYYMMDDHHMMSS;
    break;
  case DATATYPE_UINT:
    ConvertData      = ConvertToUINT;
    break;
  case DATATYPE_BYTE:
     GetFieldValueFunc = GetFieldValue_DATATYPE_BYTE;
    break;
  case DATATYPE_INT:
    ConvertData    = ConvertToInt;
    break;
  //以下几种数据类型的FieldLength = 0;
  case DATATYPE_FIXED:
    GetFieldValueFunc    = GetFieldValue_DATATYPE_FIXED;
    FieldLength = 0;
    break;
  case DATATYPE_FILENAME:
    GetFieldValueFunc    = GetFieldValue_DATATYPE_FILENAME;
    FieldLength = 0;
    break;
  case DATATYPE_BILLNAME:
    GetFieldValueFunc    = GetFieldValue_DATATYPE_BILLNAME;
    FieldLength = 0;
    break;
  case DATATYPE_SHORTFILENAME:
    GetFieldValueFunc    = GetFieldValue_DATATYPE_SHORTFILENAME;
    FieldLength = 0;
    break;
  case DATATYPE_RECORD_POSITION:
    GetFieldValueFunc    = GetFieldValue_DATATYPE_RECORD_POSITION;
    FieldLength = 0;
    break;
  default:
    ConvertData    = ConvertStr;
    break;
  }
  switch(FieldType)
  {
  case FIELDTYPE_SEARCH_RESULT_FILENAME:
    GetFieldValueFunc = GetFieldValue_FIELDTYPE_SEARCH_RESULT_FILENAME;
    break;
  case FIELDTYPE_SEARCH_RESULT_POS:
    GetFieldValueFunc = GetFieldValue_FIELDTYPE_SEARCH_RESULT_POS;
    break;
  }
  if(DisplayFormat == FIELD_DISPLAY_AS_BITS)
  {
    GetFieldValueFunc = GetFieldValue_FIELD_DISPLAY_AS_BITS;
  }
  if(GetFieldValueFunc == NULL)
  {
    if(BillFile->FileType == BIN_BILL_FORMAT)
      GetFieldValueFunc = GetFieldValue_NORMAL;
    else
      GetFieldValueFunc = GetFieldValue_TEXT_NORMAL;
  }
  switch(SortType)
  {
  case SORT_AS_DATETIME:
    CompareFieldData = CompareBinStrFieldValue;
    //MatchFieldData      = MatchDateTimeFieldValue;
    MatchFieldData   = MatchStrFieldValue;//MatchDateTimeFieldValue;
    break;
  case SORT_AS_CURRENCY:
    CompareFieldData  = CompareBinNumberFieldValue;
    MatchFieldData    = MatchCurrencyFieldValue;
  case SORT_AS_NUMBER:
    CompareFieldData  = CompareBinNumberFieldValue;
    MatchFieldData    = MatchNumberFieldValue;
    break;
  default:
    CompareFieldData = CompareBinStrFieldValue;
    MatchFieldData   = MatchStrFieldValue;
    break;
  }
/*  else if(BillFile->FileType == TXT_BILL_FORMAT && ConvertData != NULL && DataType != DATATYPE_STR)
  {
    Result = ConvertData(Result.c_str(),Result.Length(),0,0);
  }*/  
}
AnsiString GetFieldValue_DATATYPE_RECORD_POSITION(TBillField * Field)
{
  return Field->BillFile->GetCurRecordPosition() + 1;
}
AnsiString GetFieldValue_FIELDTYPE_SEARCH_RESULT_FILENAME(TBillField * Field)
{
  return AnsiString(((TSearchResultBillFile *)Field->BillFile)->srd.OriFileName);
}
AnsiString GetFieldValue_FIELDTYPE_SEARCH_RESULT_POS(TBillField * Field)
{
  return AnsiString(((TSearchResultBillFile *)Field->BillFile)->srd.OriDataIndex + 1);  
}
AnsiString GetFieldValue_DATATYPE_DURATION(TBillField * Field)
{
  return Field->BillFile->CalcDuration();
}
AnsiString GetFieldValue_DATATYPE_FILENAME(TBillField * Field)
{
  return Field->BillFile->GetFileName();
}
AnsiString GetFieldValue_DATATYPE_FIXED(TBillField * Field)
{
  return Field->FixedValue;
}
AnsiString GetFieldValue_DATATYPE_BILLNAME(TBillField * Field)
{
  return Field->BillFile->GetBillName();
}
AnsiString GetFieldValue_DATATYPE_SHORTFILENAME(TBillField * Field)
{
  return Field->BillFile->GetShortFileName();
}
AnsiString GetFieldValue_FIELD_DISPLAY_AS_BITS(TBillField * Field)
{
  const char * Buffer = Field->BillFile->BillRecord->GetBuffer(Field->Offset);
  char ch[9] = {0};
  int len = Field->FieldLength * 8 + 1;
  char * chResult = new char[len];
  memset(chResult,'0',len);
  char cha;
  int j;
  for(int n = 0;n < Field->FieldLength;n++)
  {
    cha = *(Buffer + n);
    for(j = 0;j < 8;j++)
      ch[j] = ((cha >> 7 - j) & 0x01) + 48;
    ch[8] = 0;
    memcpy((void *)((chResult + n * 8) + 8 - strlen(&ch[0])),&ch[0],strlen(&ch[0]));
  }
  * (chResult + len - 1) = 0;
  AnsiString Result = chResult;
  delete[] chResult;
  return Result;
}
AnsiString GetFieldValue_DATATYPE_BIT(TBillField * Field)
{
  unsigned char ch1 = *(Field->BillFile->BillRecord->GetBuffer(Field->Offset));
  unsigned char ch2 = (ch1 >> Field->BitPosition);// & 0x0f;
  ch2 = ch2 << (8 - Field->BitWidth);
  ch2 = ch2 >> (8 - Field->BitWidth);
  return IntToStr(ch2);
}
AnsiString GetFieldValue_DATATYPE_BASED_DATETIME(TBillField * Field)
{
  return Field->ConvertData(Field->BillFile->BillRecord->GetBuffer(Field->Offset),Field->FieldLength,(int)(&Field->BaseDateTime),0);
}
AnsiString GetFieldValue_DATATYPE_BYTE(TBillField * Field)
{
  unsigned char Byte = * (Field->BillFile->BillRecord->GetBuffer(Field->Offset));
  return IntToStr((int)Byte);
}
AnsiString GetFieldValue_NORMAL(TBillField * Field)
{
  return Field->ConvertData(Field->BillFile->BillRecord->GetBuffer(Field->Offset),Field->FieldLength,Field->EndChar,0);
}
AnsiString GetFieldValue_TEXT_NORMAL(TBillField * Field)
{
  AnsiString Result = Field->BillFile->BillRecord->GetBuffer(Field->Offset);
  //if(Field->ConvertData != NULL)
  return Field->ConvertData(Result.c_str(),Result.Length(),0,0);
  //else
  //  return Result;
}

AnsiString TBillField::GetFieldValue(bool Trans)
{
  AnsiString Result = GetFieldValueFunc(this);
  ReplaceFieldValue(Result);
  if(Trans)
    TransValue(Result);
  return Result;
}
void TBillField::ReplaceFieldValue(AnsiString & OriString)
{
  if(ReplaceRuleList != NULL)
  {
    ReplaceRule * AReplaceRule;
    int Length;
    for(int n = 0;n < ReplaceRuleList->Count;n++)
    {
      AReplaceRule = (ReplaceRule *)ReplaceRuleList->Items[n];
      if(StrLScan(OriString.c_str(),&AReplaceRule->chFrom[0],AReplaceRule->FromLength) > -1)
      {
        Length = OriString.Length();
        switch(AReplaceRule->Position)
        {
        case REPLACE_RIGHT:
          for(int n = 1; n <= AReplaceRule->Count;n++)
          {
            if(strcmp(OriString.SubString(Length - AReplaceRule->FromLength + 1,AReplaceRule->FromLength).c_str(),&AReplaceRule->chFrom[0]) == 0)
            {
              OriString = OriString.SubString(1,Length - AReplaceRule->FromLength) + AnsiString(AReplaceRule->chTo);
              Length = OriString.Length();
            }
            else
              break;
          }
          break;
        case REPLACE_ANY:
          OriString = AnsiReplaceStr(OriString,AReplaceRule->chFrom,AReplaceRule->chTo);
          break;
        default:
          for(int n = 1; n <= AReplaceRule->Count;n++)
          {
            if(strcmp(OriString.SubString(1,AReplaceRule->FromLength).c_str(),&AReplaceRule->chFrom[0]) == 0)
              OriString = AnsiString(AReplaceRule->chTo) + OriString.SubString(AReplaceRule->FromLength + 1,Length - AReplaceRule->FromLength);
            else
              break;
          }
          break;
        }
      }
    }
  }
}
void TBillField::TransValue(AnsiString & OriString)
{
  if(ValueTransList != NULL)
  {
    if(OriString != "" && IsNumber(OriString.c_str()))
    {
      try
      {
        int Index = ValueTransList->IndexOfObject((TObject *)StrToInt(OriString));
        if(Index > -1)
        {
          OriString = ValueTransList->Strings[Index];
        }
      }
      catch(...)
      {
        return;
      }
    }
  }
}
int TBillField::GetValueTransCount()
{
  if(ValueTransList == NULL)
    return 0;
  else
    return ValueTransList->Count;
}


 