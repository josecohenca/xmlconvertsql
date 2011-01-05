//---------------------------------------------------------------------------

#ifndef EditTransFrmH
#define EditTransFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RzButton.hpp"
#include "RzCmboBx.hpp"
#include "RzEdit.hpp"
#include "RzPanel.hpp"
#include "RzSpnEdt.hpp"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TfrmEditTrans : public TForm
{
__published:	// IDE-managed Components
  TRzGroupBox *RzGroupBox1;
  TLabel *Label4;
  TLabel *Label1;
  TRzEdit *txtValue;
  TRzEdit *txtContext;
  TRzBitBtn *btnCancel;
  TRzBitBtn *btnOk;
  void __fastcall btnOkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  AnsiString Value,Context;
  __fastcall TfrmEditTrans(TComponent* Owner,AnsiString AValue,AnsiString AContext);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmEditTrans *frmEditTrans;
//---------------------------------------------------------------------------
#endif
