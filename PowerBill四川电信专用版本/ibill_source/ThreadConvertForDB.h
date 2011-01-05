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
  AnsiString TempFileName;    //转换后的目标文件位置
  TStringList * BillFileList; //要转换的话单文件列表
  TStringList * BillNameList; //转换的话单文件的话单类型列表。必须与BillFileList的序数一一对应
  
  int  DestFileFormat;        //转换的目标文件格式类型,二进制或文本文件
  int  DestDBType;            //转换的目标数据库类型
  bool TransFieldValue;       //指示是否按Config.xml中定义的字段含义翻译字段的值
  HWND hWnd;                  //接收消息的窗口句柄
  AnsiString TempDirectory;
  AnsiString ErrorMessage;    //存放转换过程中的错误信息
  TBillConfig * BillConfig;
  AnsiString DestTableName;   //目标表名
  TBillFile * BillFile;
  AnsiString BillName;        //目标话单文件的类型名称
  AnsiString CreateTableSQL;  //生成建立表的语句
  TStrings * MessageTextList;
  TStringList * SQLCommandList;
  int TotalRecordCount;
};
//---------------------------------------------------------------------------
#endif
