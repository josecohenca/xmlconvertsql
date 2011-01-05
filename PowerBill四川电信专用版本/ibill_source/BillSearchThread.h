//---------------------------------------------------------------------------

#ifndef BillSearchThreadH
#define BillSearchThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "public.h"
#include "RzListVw.hpp"

#include "MSScriptControl_OCX.h"
#include <OleCtrls.hpp>
//---------------------------------------------------------------------------
#define IMAGE_CUR_ITEM      127
#define IMAGE_SUCCEED_ITEM  29
#define IMAGE_FAILED_ITEM   28
#define IMAGE_RETRYING_ITEM 74


#define MSG_FILE_PROCESSED WM_USER + 9999
class TBillSearchThread : public TThread
{            
private:
protected:
  void __fastcall Execute();
public:
  int ErrorCount;                         //记录出错忽略的文件数
  bool AutoScroll;
  TRzListView * ListView;
  FILE * TempFile;
  AnsiString    ErrorMessage;
  AnsiString    TempFileName;             //存放查找结果的临时文件名
  TBillFile   * BillFile;                 //用于查找的TBillFile对象
  TStringList * FileList;                 //要查找的文件列表
  TStringList * BillNameList;             //要查找的文件的BillName列表
  TStringList * MatchFieldList;           //要查找的字段列表
  TStringList * MatchFieldOperatorList;   //查找时的操作符列表
  TStringList * MatchFieldValueList;      //查找的值列表
  TList *       MatchModeList;            //查找的匹配方式列表
  TList *       AndOrList;                //上下文关系列表
  TBillConfig * BillConfig;
  HANDLE hWnd;
  bool UseScript; //指明是否用VBScript查询
  WideString VBScript;
  //int           MaxError;                 //自动重试的最大次数
  //int           ErrorAction;              //出错时采取什么策略应对
  bool Continue;
  int FoundCount;
  __fastcall TBillSearchThread(bool CreateSuspended);
  void __fastcall QueryByFields();  //按字段查询
  void __fastcall QueryByScript();  //按脚本查询
};
//---------------------------------------------------------------------------

#endif
