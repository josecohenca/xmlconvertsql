//---------------------------------------------------------------------------

#ifndef FilterRptFrmH
#define FilterRptFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RzPanel.hpp"
#include <ExtCtrls.hpp>
#include "RzButton.hpp"
#include "RzEdit.hpp"
#include <Mask.hpp>
#include "RzListVw.hpp"
#include <ComCtrls.hpp>
#include "BillFile.h"
//---------------------------------------------------------------------------
class TfrmFilterRpt : public TForm
{
__published:	// IDE-managed Components
  TRzGroupBox *RzGroupBox1;
  TRzBitBtn *btnClose;
  TLabel *Label1;
  TRzEdit *txtBillName;
  TLabel *Label2;
  TRzEdit *txtFileName;
  TRzListView *lvReport;
  TLabel *Label3;
  TRzBitBtn *btnCapy;
  void __fastcall btnCapyClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmFilterRpt(TComponent* Owner,TBillFile * ABillFile,AnsiString AFileName);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmFilterRpt *frmFilterRpt;
//---------------------------------------------------------------------------
#endif
