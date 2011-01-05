//---------------------------------------------------------------------------

#ifndef SelBillNameFrmH
#define SelBillNameFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RzPanel.hpp"
#include <ExtCtrls.hpp>
#include "RzButton.hpp"
#include "RzListVw.hpp"
#include <ComCtrls.hpp>
#include "RzRadChk.hpp"
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TfrmSelBillName : public TForm
{
__published:	// IDE-managed Components
  TRzGroupBox *RzGroupBox1;
  TRzBitBtn *btnOk;
  TRzBitBtn *btnCancel;
  TLabel *Label1;
  TRzListView *lvBillTypes;
  TImage *Image1;
  void __fastcall lvBillTypesChange(TObject *Sender, TListItem *Item,
          TItemChange Change);
  void __fastcall btnOkClick(TObject *Sender);
private:	// User declarations
  AnsiString FileName;
public:		// User declarations
  __fastcall TfrmSelBillName(TComponent* Owner,AnsiString AFileName);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSelBillName *frmSelBillName;
//---------------------------------------------------------------------------
#endif
