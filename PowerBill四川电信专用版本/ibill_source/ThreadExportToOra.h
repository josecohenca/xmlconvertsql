//---------------------------------------------------------------------------

#ifndef ThreadExportToOraH
#define ThreadExportToOraH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class TThreadExportToOra : public TThread
{            
private:
protected:
  void __fastcall Execute();
public:
  __fastcall TThreadExportToOra(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
