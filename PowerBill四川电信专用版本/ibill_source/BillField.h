//�����ֶ���

#ifndef BILLFIELD_H
#define BILLFIELD_H

#include <stdlib.h>
#include "public.h"
#include "BillFile.h"
//�滻����ṹ
typedef struct ReplaceRule
{
  char chFrom[256];
  int  FromLength;
  char chTo[256];
  int  ToLength;
  int  Position;
  int  Count;
}ReplaceRule;

//�Ƚ��ֶ�ֵ
typedef int (* CompareFieldValue)(const void *arg1, const void *arg2);
typedef int __fastcall (* MatchFieldValue)(const AnsiString & Str1,const AnsiString & Str2,int Param);
typedef AnsiString (* GetFieldValueFunction)(TBillField * Field);

//�ֶ�����ƥ��,���Ҽ�¼ʱʹ��
//���str1 < str2 ����-1
//���str1 > str2 ���� 1
//���str1 = str2 ���� 0
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
//�����ֶ�����
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
  TStringList * ValueTransList; //�����ֶεĺ����б�
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
  int  FieldType;             //�ֶ�����,2.0�汾����
  AnsiString Label;
  AnsiString FieldName;
  AnsiString FixedValue;
  AnsiString GetFieldValue(bool Trans = true);
  GetFieldValueFunction GetFieldValueFunc;
  int  CompareDirection; //��ǰ�ıȽϷ�ʽ.1��ʾ����;-1��ʾ����
  int  CompareLen;       //���ڱȽϵ��ֽڳ���
  CompareFieldValue CompareFieldData;
  MatchFieldValue   MatchFieldData;
  void SetDataType(int ADataType);
  int  GetValueTransCount();
  void   GetFieldOriData(char chDest[],int BufferLen);
  const char * GetFieldOriData();
  TBillField(TBillFile * ABillFile,_di_IXMLNode FieldNode);
  ~TBillField();
};
extern TBillField * CompareField;     //��ǰ���ڽ��бȽϵ��ֶ�

#endif
 