//�����ļ���

#ifndef BILLFILE_H
#define BILLFILE_H

#include "public.h"
#include "BillConfig.h"
#include "BillRecord.h"
#include "BillField.h"
#include "FTPFile.h"

//#pragma pack(1)
//DBF�ļ�ͷ
typedef struct DBFHead
{
  unsigned char FileType;       //�ļ�����
  unsigned char LastModify[3];  //�ϴ��޸�ʱ��(YYMMDD)
  long int      RecordCount;    //���еļ�¼��
  short int     HeadLength;     //�ļ�ͷ(�����ֶζ���)�ĳ���
  short int     RecordLength;   //ÿ����¼�ĳ���
  char          Reserved1[16];  //����
  unsigned char TableFlag;      //��ı��          
  char          CodePage;       //����ҳ
  unsigned char Reserved2[2];   //����
}DBFHead;
//DBF�ֶ�
typedef struct DBFField
{
  char          FieldName[11];    //�ֶ���
  char          FieldType;        //�ֶ���������
  int           Offset;           //��¼�и��ֶε�ƫ����
  unsigned char Length;           //�ֶγ���
  unsigned char Decimal;          //�������������ֳ���
  char          Reserved1[2];     //����
  unsigned char FieldFlag;        //�ֶα��
  char          Reserved2[10];    //����
  char          ProductionIndex;  //
}DBFField;

//��¼�����ṹ
typedef struct RecordIndex
{
  int RecordPosition;
  int RecordLength; //�ڶ������ļ���δʹ��,ʼ��ΪTBillFile��RecordLength
}TxtFileRecord,BinRecordIndex;



//�����ļ��Ļ���
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
  int SkipRecordCount;            //��¼�����ļ�¼��
  int FileBlockLength;            //
  bool IsBlockBinFile;            //��ʶ�Ƿ��Ƕ��������ļ�
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
  int  BlockHeadLength;   //��ͷ����
  int  BlockTrailLength;  //��β����
  bool FileLoaded;
  TBillField * GetBillField(int Index);
  TBillField * GetBillField(AnsiString FieldName);

  int CalcDuration(); //����ͨ��ʱ��

  AnsiString __fastcall TransFileName(AnsiString AFileName);
  void WriteDBFHead(FILE * file,TList * DBFFieldList,TList * BillFieldList);        //����DBF�ļ�ͷ
  void ExportDataToDBFFile(FILE * file,TList * DBFFieldList,TList * BillFieldList); //����������д���뵽DBF�ļ�
  void EndOfDBFFile(FILE * file,int RecordCount);
  void TermCurOperation();
  bool IsTermBuUser(){return AbortOperation;};

  void ExportDataToTxtFile(FILE * file,AnsiString Separator);//�������ļ�д�뵽�ı��ļ�
  virtual void ExportDataToOriFile(FILE * file);             //�������ļ�д�뵽�ı��ļ�

  TBillFile(AnsiString ABillName,TBillConfig * ABillConfig,HANDLE hParentWnd,AnsiString ATempDirectory);
  virtual ~TBillFile();

  //���ļ�ת��Ϊ���Ե��뵽DBMS�ĸ�ʽ
  virtual int  ConvertToDBFile(FILE * file,AnsiString FieldTermChar, bool TransFieldValue);
  virtual bool ReadSingleRecord(int RecordIndex);  //��ȡһ���ļ�¼
  virtual int  GetRecordCount();
  virtual bool OpenBillFile(AnsiString AFileName,AnsiString ABillName);
  virtual void CloseBillFile(){};
  virtual int  GetFormatType(){return FileType;};
  virtual void SortFile(TBillField * BillField,int Direction){}; //���ļ���������.���Directionָʾ����ķ���
  virtual int  SetRecordBuffer(const char * chSrc,int Length){return 0;};
  virtual void SaveRecordToFile(FILE * file){};   //��BillFile�ĵ�ǰ��¼д�뵽һ���ļ���
  virtual void SaveFileHeadToFile(FILE * file){}; //д�ļ�ͷ
  virtual void SaveFileTrailToFile(FILE * file){};//д�ļ�β
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


//�������ļ�������
class TBinBillFile : public TBillFile
{
private:
  TMemoryStream * FileStream;
  TMemoryStream * IndexStream;
  char * EmptyRecord; //�ռ�¼,���ڱȶ�,�䳤��=RecordLength
public:
  int  SetRecordBuffer(const char * chSrc,int Length);
  bool ReadSingleRecord(int RecordIndex);           //��ȡһ���ļ�¼
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

//�ı��ļ�������
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
*��ѯ�����������,�ڶԻ�ϻ������в���ʱ�õ�
*���������ԭ�еĻ����ṹ������,�����������ֶ�:
*1-ԭʼ�����ļ���
*2-��¼��ԭʼ�����е�λ��
*��ѯ�����һ���������ļ�
******************************************************************************/
//�ļ�ͷ����һ��SearchResultFileHead�ṹ
typedef struct SearchResultFileHead
{
  char BillName[256];
  int  RecordCount;
  int  IndexAreaLength;
}SearchResultFileHead;
//�ýṹ������һ����¼�ڽ���ļ��е���ʼλ��
typedef struct SearchResultDataIndex
{
  int RecordIndex;
  int Position;
}SearchResultDataIndex;
//�ýṹλ��ÿһ����¼��ǰ��,������һ����¼�ĳ��ȡ�ԭʼ�ļ���������ԭʼ�ļ��еĻ���0�ļ�¼���
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
 