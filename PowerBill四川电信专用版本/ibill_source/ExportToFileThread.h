//---------------------------------------------------------------------------

#ifndef ExportToFileThreadH
#define ExportToFileThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "public.h"
#include "BillFile.h"
//---------------------------------------------------------------------------
class TExportToFileThead : public TThread
{            
private:
protected:
  void __fastcall Execute();
public:
  __fastcall TExportToFileThead(bool CreateSuspended);
  TBillFile     * BillFile;
  TStringList   * FileList;
  TStringList   * BillNameList;
  FILE * DestFile;
  AnsiString ErrorMessage;
  AnsiString DestFileName;
  int DestFileType;
  //int RetryCount;
};
//---------------------------------------------------------------------------
#endif
