//---------------------------------------------------------------------------

#ifndef FTPWatchThreadH
#define FTPWatchThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>
//---------------------------------------------------------------------------
class TFTPWatchThread : public TThread
{            
private:
protected:
  void __fastcall Execute();
public:
  TDateTime LastStartTime;
  void * FTPFile;
  bool Continue;
  int  Timeout;
  __fastcall TFTPWatchThread(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
