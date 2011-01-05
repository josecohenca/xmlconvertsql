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
  int ErrorCount;                         //��¼������Ե��ļ���
  bool AutoScroll;
  TRzListView * ListView;
  FILE * TempFile;
  AnsiString    ErrorMessage;
  AnsiString    TempFileName;             //��Ų��ҽ������ʱ�ļ���
  TBillFile   * BillFile;                 //���ڲ��ҵ�TBillFile����
  TStringList * FileList;                 //Ҫ���ҵ��ļ��б�
  TStringList * BillNameList;             //Ҫ���ҵ��ļ���BillName�б�
  TStringList * MatchFieldList;           //Ҫ���ҵ��ֶ��б�
  TStringList * MatchFieldOperatorList;   //����ʱ�Ĳ������б�
  TStringList * MatchFieldValueList;      //���ҵ�ֵ�б�
  TList *       MatchModeList;            //���ҵ�ƥ�䷽ʽ�б�
  TList *       AndOrList;                //�����Ĺ�ϵ�б�
  TBillConfig * BillConfig;
  HANDLE hWnd;
  bool UseScript; //ָ���Ƿ���VBScript��ѯ
  WideString VBScript;
  //int           MaxError;                 //�Զ����Ե�������
  //int           ErrorAction;              //����ʱ��ȡʲô����Ӧ��
  bool Continue;
  int FoundCount;
  __fastcall TBillSearchThread(bool CreateSuspended);
  void __fastcall QueryByFields();  //���ֶβ�ѯ
  void __fastcall QueryByScript();  //���ű���ѯ
};
//---------------------------------------------------------------------------

#endif
