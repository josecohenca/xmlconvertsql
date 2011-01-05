//---------------------------------------------------------------------------

#ifndef ThreadSubmitToDBH
#define ThreadSubmitToDBH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ADODB.hpp>
//---------------------------------------------------------------------------
class TThreadSubmitToDB : public TThread
{            
private:
  bool __fastcall SubmitToOracle();
  bool __fastcall SubmitToSQLServer();
  bool __fastcall SubmitToMySQL();
  int  __fastcall GetRowsForSQLLDR(const AnsiString  & Text);
  int  __fastcall GetRowsForBCP(const AnsiString  & Text); 
protected:
  void __fastcall Execute();
public:
  __fastcall TThreadSubmitToDB(bool CreateSuspended);
  PROCESS_INFORMATION pi;
  HANDLE hReadPipe,hWritePipe;
  AnsiString TempFileName;    //ת����Ļ����ļ���
  AnsiString DataBaseName;    //���ݿ�����(SQLServer��MySQL);
  AnsiString DestTableName;
  int DBType;
  AnsiString DBServer,DBUserName,DBPassword,WorkDir,ErrorMessage;
  TStrings    * MessageTextList;
  TStringList * SQLCommandList;
  int TotalRecordCount;
  TADOConnection * ADOConnection;
  AnsiString ToolPath;        //�ⲿ�������ڵ�·��
  HWND hWnd;  
  AnsiString ServerPort;      //MySQL�Ķ˿ں�
  AnsiString MaxError;
  int MaxMessageLine;
};
//---------------------------------------------------------------------------
#endif
