//话单字段类

#ifndef BILLFIELD_H
#define BILLFIELD_H

#include <stdlib.h>
#include "public.h"
#include "BillFile.h"
//替换规则结构
typedef struct ReplaceRule
{
  char chFrom[256];
  int  FromLength;
  char chTo[256];
  int  ToLength;
  int  Position;
  int  Count;
}ReplaceRule;

//比较字段值
typedef int (* CompareFieldValue)(const void *arg1, const void *arg2);
typedef int __fastcall (* MatchFieldValue)(const AnsiString & Str1,const AnsiString & Str2,int Param);
typedef AnsiString (* GetFieldValueFunction)(TBillField * Field);

//字段内容匹配,查找记录时使用
//如果str1 < str2 返回-1
//如果str1 > str2 返回 1
//如果str1 = str2 返回 0
int __fastcall MatchStrFieldValue(const AnsiString & Str1,const AnsiString & Str2,int Param);
int __fastcall MatchDateTimeFieldValue(const AnsiString & Str1,const AnsiString & Str2,int Param);
int __fastcall MatchNumberFieldValue(const AnsiString & Str1,const AnsiString & Str2,int Param);

AnsiString GetFieldValue_TEXT_NORMAL(TBillField * Field);
AnsiString GetFieldValue_NORMAL(TBillField * Field);
AnsiString GetFieldValue_DATATYPE_BYTE(TBillField * Field);
AnsiString GetFieldValue_DATATYPE_BASED_DATETIME(TBillField * Field);
AnsiString GetFieldValue_DATATYPE_BIT(TBillField * Field);
AnsiString GetFieldValue_FIELD_DISPLAY_AS_BITS(TBillField * Field);
AnsiString GetFieldValue_DATATYPE_SHORTFILENAME(TBillField * Field);
AnsiString GetFieldValue_DATATYPE_BILLNAME(TBillField * Field);
AnsiString GetFieldValue_DATATYPE_FIXED(TBillField * Field);
AnsiString GetFieldValue_DATATYPE_FILENAME(TBillField * Field);
AnsiString GetFieldValue_DATATYPE_DURATION(TBillField * Field);
AnsiString GetFieldValue_FIELDTYPE_SEARCH_RESULT_POS(TBillField * Field);
AnsiString GetFieldValue_FIELDTYPE_SEARCH_RESULT_FILENAME(TBillField * Field);
AnsiString GetFieldValue_DATATYPE_RECORD_POSITION(TBillField * Field);
//话单字段字义
class TBillField
{
private:
  TList * ReplaceRuleList;
  void AddValueTrans(int Value,AnsiString Trans);
  void ReplaceFieldValue(AnsiString & OriString);
  void TransValue(AnsiString & OriString);
public:
  ConvertFieldData ConvertData;
  TDateTime BaseDateTime;
  TStringList * ValueTransList; //话单字段的含意列表
  TBillFile   * BillFile;
  TBillFile   * DataBillFile;
  int  DataType;
  int  Offset;
  int  FieldIndex;
  int  FieldLength;
  int  DisplayWidth;
  int  DisplayFormat;
  int  BitPosition;
  int  BitWidth;
  int  SortType;
  char EndChar;
  bool DisplayInList;
  int  DisplayAlign;
  bool CreateIndex;
  bool ImportToDB;
  int  ExportLength;
  int  FieldType;             //字段类型,2.0版本新增
  AnsiString Label;
  AnsiString FieldName;
  AnsiString FixedValue;
  AnsiString GetFieldValue(bool Trans = true);
  GetFieldValueFunction GetFieldValueFunc;
  int  CompareDirection; //当前的比较方式.1表示升序;-1表示降序
  int  CompareLen;       //用于比较的字节长度
  CompareFieldValue CompareFieldData;
  MatchFieldValue   MatchFieldData;
  void SetDataType(int ADataType);
  int  GetValueTransCount();
  void   GetFieldOriData(char chDest[],int BufferLen);
  const char * GetFieldOriData();
  TBillField(TBillFile * ABillFile,_di_IXMLNode FieldNode);
  ~TBillField();
};
extern TBillField * CompareField;     //当前正在进行比较的字段

#endif
 