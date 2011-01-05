//---------------------------------------------------------------------------

#ifndef ReplaceRultFrmH
#define ReplaceRultFrmH
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
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TfrmReplaceRule : public TForm
{
__published:	// IDE-managed Components
  TRzGroupBox *RzGroupBox1;
  TRzBitBtn *btnOk;
  TRzBitBtn *btnCancel;
  TLabel *Label4;
  TRzEdit *txtBillName;
  TLabel *Label1;
  TRzEdit *txtFieldName;
  TLabel *Label2;
  TRzListView *lvReplaceRules;
  TPopupMenu *PopupMenu1;
  TMenuItem *menuAddRule;
  TMenuItem *menuEditRule;
  TMenuItem *menuDeleteRule;
  void __fastcall PopupMenu1Popup(TObject *Sender);
private:	// User declarations
  AnsiString ReplayRules;
public:		// User declarations
  __fastcall TfrmReplaceRule(TComponent* Owner,AnsiString BillName,AnsiString FieldName,AnsiString AReplayRules);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmReplaceRule *frmReplaceRule;
//---------------------------------------------------------------------------
#endif
