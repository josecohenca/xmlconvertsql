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
  AnsiString TempFileName;    //转换后的话单文件名
  AnsiString DataBaseName;    //数据库名称(SQLServer和MySQL);
  AnsiString DestTableName;
  int DBType;
  AnsiString DBServer,DBUserName,DBPassword,WorkDir,ErrorMessage;
  TStrings    * MessageTextList;
  TStringList * SQLCommandList;
  int TotalRecordCount;
  TADOConnection * ADOConnection;
  AnsiString ToolPath;        //外部工具所在的路径
  HWND hWnd;  
  AnsiString ServerPort;      //MySQL的端口号
  AnsiString MaxError;
  int MaxMessageLine;
};
//---------------------------------------------------------------------------
#endif
