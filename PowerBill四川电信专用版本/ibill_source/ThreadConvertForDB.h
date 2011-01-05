//---------------------------------------------------------------------------

#ifndef ThreadConvertForDBH
#define ThreadConvertForDBH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "public.h"
#include "BillConfig.h"
#include "BillFile.h"
#include "BillRecord.h"
#include "BillField.h"
//---------------------------------------------------------------------------
class TThreadConvertForDB : public TThread
{            
private:
  int __fastcall ConvertToOracleFormat();
  int __fastcall ConvertToSQLServerFormat();
  int __fastcall ConvertToMySQL();
  int __fastcall ConvertCSVFile(FILE * file);
protected:
  void __fastcall Execute();
public:
  FILE * file;
  __fastcall TThreadConvertForDB(bool CreateSuspended);
  AnsiString TempFileName;    //ת�����Ŀ���ļ�λ��
  TStringList * BillFileList; //Ҫת���Ļ����ļ��б�
  TStringList * BillNameList; //ת���Ļ����ļ��Ļ��������б�������BillFileList������һһ��Ӧ
  
  int  DestFileFormat;        //ת����Ŀ���ļ���ʽ����,�����ƻ��ı��ļ�
  int  DestDBType;            //ת����Ŀ�����ݿ�����
  bool TransFieldValue;       //ָʾ�Ƿ�Config.xml�ж�����ֶκ��巭���ֶε�ֵ
  HWND hWnd;                  //������Ϣ�Ĵ��ھ��
  AnsiString TempDirectory;
  AnsiString ErrorMessage;    //���ת�������еĴ�����Ϣ
  TBillConfig * BillConfig;
  AnsiString DestTableName;   //Ŀ�����
  TBillFile * BillFile;
  AnsiString BillName;        //Ŀ�껰���ļ�����������
  AnsiString CreateTableSQL;  //���ɽ���������
  TStrings * MessageTextList;
  TStringList * SQLCommandList;
  int TotalRecordCount;
};
//---------------------------------------------------------------------------
#endif
