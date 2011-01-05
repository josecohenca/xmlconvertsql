//---------------------------------------------------------------------------

#ifndef OpenFileThreadH
#define OpenFileThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "BillFile.h"
//---------------------------------------------------------------------------
class TOpenFileThread : public TThread
{            
private:
protected:
  void __fastcall Execute();
public:
  TBillFile * BillFile;
  AnsiString ErrorMessage;
  AnsiString BillName;
  AnsiString FileName;
  HANDLE hWnd;
  bool Continue;
  __fastcall TOpenFileThread(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
