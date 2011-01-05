//话单定义管理类

#ifndef BILLCONFIG_H
#define BILLCONFIG_H

#include "public.h"

typedef struct RecordFilter
{
  int  Pos;
  int  Width;
  int  Is;
  int  Action;
  int  Length;
  char Cause[101];
  int SkipCount;  //用于记录跳过的记录数
}RecordFilter;

class TBillConfig
{
private:
public:
  _di_IXMLDocument XMLDocument;
  _di_IXMLNode BillRootNode;
  void ReadBillConfig(AnsiString FileName,bool IncludeDisabled = false);
  AnsiString LastErrorMessage;
  
  AnsiString   GetAttributeNodeValue(_di_IXMLNode BillNode,AnsiString AttributrName);

  _di_IXMLNode GetBillNode(AnsiString BillName);
  _di_IXMLNode GetSummaryNode(AnsiString BillName);
  _di_IXMLNode GetRecordNode(AnsiString BillName);
  _di_IXMLNode GetFieldNode(AnsiString BillName,AnsiString FieldName);

  AnsiString   GetSummaryAttributeValue(AnsiString BillName,AnsiString AttributeName);
  
  int  GetFileHeadLength(AnsiString BillName);
  int  GetFileTrailLength(AnsiString BillName);
  int  GetFileRecordLength(AnsiString BillName);
  int  GetFileFormatType(AnsiString BillName);
  int  GetFileBlockLength(AnsiString BillName);
  int  GetBlockHeadLength(AnsiString BillName);
  int  GetBlockTrailLength(AnsiString BillName);

  int  TransFieldType(AnsiString FieldTypeName);
  int  TransFieldDataType(AnsiString DataTypeName);
  int  TransFormatType(AnsiString FormatTypeName);
  int  TransSortType(AnsiString SortTypeName);
  int  TransAlignMode(AnsiString AlignName);
  int  TransDisplayFormat(AnsiString DisplayFormatName);
  int  TransReplacePosition(AnsiString Position);
  int  TransFilterAction(AnsiString Action);
  bool IsBlockFile(AnsiString BillName);
  bool IsDisabled(AnsiString BillName);
  char GetFieldSeparator(AnsiString BillName);
  void GetFieldTrans(AnsiString BillName,AnsiString FieldName,TStringList * TransList);
  void GetRecordFilter(AnsiString BillName,TList * RecordFilterList);

  void SetChildAttribute(_di_IXMLNode ParentNode,AnsiString ChildNodeName,AnsiString Text);
  AnsiString TransActionName(int Action);
  AnsiString GetDataTypeName(int DataType);
  void SetBillRecordFilter(AnsiString BillName,TList * ActionList);
  //添加一种话单类型
  void AddNewBill(
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
    bool Disabled
    );
  void ModifyBill(
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
    bool Disabled
    );    
  //添加一个字段
  void AddNewField(
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
    );
  //修改字段定义
  void ModifyField(
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
    );
  void SetFiledReplaceRule(AnsiString BillName,AnsiString FieldName,AnsiString ReplaceReuls);
  void SetFieldTrans(AnsiString BillName,AnsiString FieldName,TStringList * TransList);
  void DeleteField(AnsiString BillName,AnsiString FieldName);
  void DeleteBill(AnsiString BillName);
  bool IsModified();
  bool SaveChanges(bool BakupOldFile);
  void Rollback();    
    

  AnsiString GetFieldAttributeValue(_di_IXMLNode FieldNode,AnsiString AttributeName,AnsiString DefaultValue = "");
  AnsiString GetFieldDataTypeNameText(int DataType);
  AnsiString GetFieldReplaceRules(AnsiString BillName,AnsiString FieldName);
  TStringList * BillNameList;
  AnsiString TransFormatTypeName(int FormatType);
  TBillConfig(AnsiString ConfigFileName = "");
  ~TBillConfig();
};

#endif
 