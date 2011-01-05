//---------------------------------------------------------------------------

#ifndef EditReplaceRuleFrmH
#define EditReplaceRuleFrmH
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
#include "RzCmboBx.hpp"
#include <Buttons.hpp>
#include <Menus.hpp>
#include "RzSpnEdt.hpp"
//---------------------------------------------------------------------------
class TfrmEditReplaceRule : public TForm
{
__published:	// IDE-managed Components
  TRzGroupBox *RzGroupBox1;
  TRzBitBtn *btnCancel;
  TRzBitBtn *btnOk;
  TLabel *Label4;
  TRzEdit *txtFrom;
  TLabel *Label1;
  TRzEdit *txtTo;
  TLabel *Label2;
  TRzComboBox *cbxPosition;
  TSpeedButton *btnAscii1;
  TSpeedButton *btnAscii2;
  TPopupMenu *PopupMenu1;
  TMenuItem *N1;
  TMenuItem *N2;
  TMenuItem *ASCII91;
  TLabel *Label3;
  TRzSpinEdit *txtReplaceCount;
  void __fastcall btnOkClick(TObject *Sender);
  void __fastcall btnAscii1Click(TObject *Sender);
  void __fastcall N1Click(TObject *Sender);
  void __fastcall N2Click(TObject *Sender);
  void __fastcall ASCII91Click(TObject *Sender);
  void __fastcall cbxPositionChange(TObject *Sender);
  bool __fastcall FormHelp(WORD Command, int Data, bool &CallHelp);
private:	// User declarations
public:		// User declarations
  AnsiString From;
  AnsiString To;
  AnsiString Position;
  AnsiString Count;
  __fastcall TfrmEditReplaceRule(TComponent* Owner,AnsiString AFrom,AnsiString ATo,AnsiString APosition,AnsiString ACount);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmEditReplaceRule *frmEditReplaceRule;
//---------------------------------------------------------------------------
#endif
