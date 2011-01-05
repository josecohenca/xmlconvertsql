//---------------------------------------------------------------------------

#ifndef ReplaceRuleFrmH
#define ReplaceRuleFrmH
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
  void __fastcall menuEditRuleClick(TObject *Sender);
  void __fastcall btnOkClick(TObject *Sender);
  void __fastcall menuDeleteRuleClick(TObject *Sender);
  void __fastcall menuAddRuleClick(TObject *Sender);
  void __fastcall lvReplaceRulesDragOver(TObject *Sender, TObject *Source,
          int X, int Y, TDragState State, bool &Accept);
  void __fastcall lvReplaceRulesDragDrop(TObject *Sender, TObject *Source,
          int X, int Y);
  bool __fastcall FormHelp(WORD Command, int Data, bool &CallHelp);
private:	// User declarations
public:		// User declarations
  AnsiString ReplayRules;
  __fastcall TfrmReplaceRule(TComponent* Owner,AnsiString BillName,AnsiString FieldName,AnsiString AReplayRules,bool ReadOnly);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmReplaceRule *frmReplaceRule;
//---------------------------------------------------------------------------
#endif
