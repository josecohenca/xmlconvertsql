//话单文件类

#ifndef BILLFILE_H
#define BILLFILE_H

#include "public.h"
#include "BillConfig.h"
#include "BillRecord.h"
#include "BillField.h"
#include "FTPFile.h"

//#pragma pack(1)
//DBF文件头
typedef struct DBFHead
{
  unsigned char FileType;       //文件类型
  unsigned char LastModify[3];  //上次修改时间(YYMMDD)
  long int      RecordCount;    //表中的记录数
  short int     HeadLength;     //文件头(包含字段定义)的长度
  short int     RecordLength;   //每条记录的长度
  char          Reserved1[16];  //保留
  unsigned char TableFlag;      //表的标记          
  char          CodePage;       //代码页
  unsigned char Reserved2[2];   //保留
}DBFHead;
//DBF字段
typedef struct DBFField
{
  char          FieldName[11];    //字段名
  char          FieldType;        //字段数据类型
  int           Offset;           //记录中该字段的偏移量
  unsigned char Length;           //字段长度
  unsigned char Decimal;          //浮点数整数部分长度
  char          Reserved1[2];     //保留
  unsigned char FieldFlag;        //字段标记
  char          Reserved2[10];    //保留
  char          ProductionIndex;  //
}DBFField;

//记录索引结构
typedef struct RecordIndex
{
  int RecordPosition;
  int RecordLength; //在二进制文件中未使用,始终为TBillFile的RecordLength
}TxtFileRecord,BinRecordIndex;



//话单文件的基类
class TBillFile
{
protected:
  unsigned char * Buffer;
  AnsiString TempDirectory;
  AnsiString TempFileName;
  AnsiString    BillName;
  AnsiString    FileName;
  AnsiString    ShortFileName;
public:
  bool AbortOperation;
  RecordIndex ri;
  int SkipRecordCount;            //记录跳过的记录数
  int FileBlockLength;            //
  bool IsBlockBinFile;            //标识是否是二进制流文件
  TList * FilterList;
  TBillField  * AnswerTimeField;
  TBillField  * EndTimeField;
  TStringList * FieldList;
  int FileHeadLength;
  int FileTrailLength;
  HANDLE hWnd;
  TFTPFile    * FTPFile;
  TBillRecord * BillRecord;
  TBillConfig * BillConfig;
  char Separator;
  int  FileType;
  int  RecordLength;
  int  FieldCount;
  int  BlockHeadLength;   //块头长度
  int  BlockTrailLength;  //块尾长度
  bool FileLoaded;
  TBillField * GetBillField(int Index);
  TBillField * GetBillField(AnsiString FieldName);

  int CalcDuration(); //计算通话时长

  AnsiString __fastcall TransFileName(AnsiString AFileName);
  void WriteDBFHead(FILE * file,TList * DBFFieldList,TList * BillFieldList);        //生成DBF文件头
  void ExportDataToDBFFile(FILE * file,TList * DBFFieldList,TList * BillFieldList); //将话单数据写出入到DBF文件
  void EndOfDBFFile(FILE * file,int RecordCount);
  void TermCurOperation();
  bool IsTermBuUser(){return AbortOperation;};

  void ExportDataToTxtFile(FILE * file,AnsiString Separator);//将话单文件写入到文本文件
  virtual void ExportDataToOriFile(FILE * file);             //将话单文件写入到文本文件

  TBillFile(AnsiString ABillName,TBillConfig * ABillConfig,HANDLE hParentWnd,AnsiString ATempDirectory);
  virtual ~TBillFile();

  //将文件转换为可以导入到DBMS的格式
  virtual int  ConvertToDBFile(FILE * file,AnsiString FieldTermChar, bool TransFieldValue);
  virtual bool ReadSingleRecord(int RecordIndex);  //读取一条的记录
  virtual int  GetRecordCount();
  virtual bool OpenBillFile(AnsiString AFileName,AnsiString ABillName);
  virtual void CloseBillFile(){};
  virtual int  GetFormatType(){return FileType;};
  virtual void SortFile(TBillField * BillField,int Direction){}; //对文件进行排序.如果Direction指示排序的方向
  virtual int  SetRecordBuffer(const char * chSrc,int Length){return 0;};
  virtual void SaveRecordToFile(FILE * file){};   //将BillFile的当前记录写入到一个文件中
  virtual void SaveFileHeadToFile(FILE * file){}; //写文件头
  virtual void SaveFileTrailToFile(FILE * file){};//写文件尾
  virtual int  ManualSetRecordBuffer(const char * ABuffer){return 0;};
  virtual int  GetCurRecordPosition(){return -1;};
  virtual int  GetCurRecordLength(){return -1;};
  virtual AnsiString GetBillName(){return BillName;};
  virtual AnsiString GetShortFileName(){return ShortFileName;};
  virtual AnsiString GetOriShortFileName(){return ShortFileName;};
  virtual AnsiString GetFileName(){return FileName;};
  virtual void * GetRecordBuffer();
  virtual void SetOwnerWnd(HANDLE hNewWnd);
};


//二进制文件操作类
class TBinBillFile : public TBillFile
{
private:
  TMemoryStream * FileStream;
  TMemoryStream * IndexStream;
  char * EmptyRecord; //空记录,用于比对,其长度=RecordLength
public:
  int  SetRecordBuffer(const char * chSrc,int Length);
  bool ReadSingleRecord(int RecordIndex);           //读取一条的记录
  int  GetRecordCount();
  TBinBillFile(AnsiString ABillName,TBillConfig * ABillConfig,HANDLE hParentWnd,AnsiString ATempDirectory);
  ~TBinBillFile();
  bool OpenBillFile(AnsiString AFileName,AnsiString ABillName);
  void CloseBillFile();
  void SortFile(TBillField * BillField,int Direction);
  void SaveRecordToFile(FILE * file);
  void SaveFileHeadToFile(FILE * file);
  void SaveFileTrailToFile(FILE * file);
  int  ManualSetRecordBuffer(const char * ABuffer);
  int  GetCurRecordPosition();
  int  GetCurRecordLength();
};

//文本文件操作类
class TTxtBillFile : public TBillFile
{
private:
  TMemoryStream * FileStream;
  TMemoryStream * IndexStream;
public:
  int  GetRecordCount();
  bool ReadSingleRecord(int RecordIndex);
  TTxtBillFile(AnsiString ABillName,TBillConfig * ABillConfig,HANDLE hParentWnd,AnsiString ATempDirectory);
  ~TTxtBillFile();
  bool OpenBillFile(AnsiString AFileName,AnsiString ABillName);
  void CloseBillFile();
  void SortFile(TBillField * BillField,int Direction);
  int  SetRecordBuffer(const char * chSrc,int Length);
  void SaveRecordToFile(FILE * file);
  void SaveFileHeadToFile(FILE * file);
  int  ManualSetRecordBuffer(const char * ABuffer);
  int  GetCurRecordPosition();
  int  GetCurRecordLength();
};
/******************************************************************************
*查询结果话单类型,在对混合话单进行操作时用到
*该类除包含原有的话单结构类型外,还包含两个字段:
*1-原始话单文件名
*2-记录在原始话单中的位置
*查询结果是一个二进制文件
******************************************************************************/
//文件头部是一个SearchResultFileHead结构
typedef struct SearchResultFileHead
{
  char BillName[256];
  int  RecordCount;
  int  IndexAreaLength;
}SearchResultFileHead;
//该结构描述了一条记录在结果文件中的起始位置
typedef struct SearchResultDataIndex
{
  int RecordIndex;
  int Position;
}SearchResultDataIndex;
//该结构位于每一条记录的前端,描述了一条记录的长度、原始文件名和其在原始文件中的基于0的记录序号
typedef struct SearchResultData
{
  int  DataLength;
  char OriFileName[256];
  int  OriDataIndex;
}SearchResultData;

class TSearchResultBillFile : public TBillFile
{
private:
  TBillFile     * RealBillFile;
  TMemoryStream * FileStream;
  SearchResultFileHead FileHead;
  int LastRecordLength;
public:
  SearchResultData srd;
  AnsiString GetBillName();
  int  GetRecordCount();
  bool ReadSingleRecord(int RecordIndex);
  TSearchResultBillFile(AnsiString AFileName,TBillConfig * ABillConfig,HANDLE hParentWnd,AnsiString ATempDirectory);
  ~TSearchResultBillFile();
  bool OpenBillFile(AnsiString AFileName,AnsiString ABillName);
  void SortFile(TBillField * BillField,int Direction);
  int  SetRecordBuffer(const char * chSrc,int Length);
  bool ReadBlock();
  void SaveRecordToFile(FILE * file);
  void SaveFileTrailToFile(FILE * file);
  void SaveFileHeadToFile(FILE * file);
  int  ManualSetRecordBuffer(const char * ABuffer);
  int  GetCurRecordPosition();
  int  GetCurRecordLength();
  AnsiString GetShortFileName();
  void * GetRecordBuffer();
};
#endif
 