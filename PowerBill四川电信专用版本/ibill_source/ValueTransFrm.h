//---------------------------------------------------------------------------

#ifndef ValueTransFrmH
#define ValueTransFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RzButton.hpp"
#include "RzEdit.hpp"
#include "RzListVw.hpp"
#include "RzPanel.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TfrmValueTrans : public TForm
{
__published:	// IDE-managed Components
  TRzGroupBox *RzGroupBox1;
  TLabel *Label4;
  TLabel *Label1;
  TLabel *Label2;
  TRzEdit *txtBillName;
  TRzEdit *txtFieldName;
  TRzListView *lvTrans;
  TRzBitBtn *btnOk;
  TRzBitBtn *btnCancel;
  TPopupMenu *PopupMenu1;
  TMenuItem *menuAdd;
  TMenuItem *menuEdit;
  TMenuItem *menuDelete;
  void __fastcall btnOkClick(TObject *Sender);
  void __fastcall PopupMenu1Popup(TObject *Sender);
  void __fastcall menuAddClick(TObject *Sender);
  void __fastcall menuEditClick(TObject *Sender);
  void __fastcall menuDeleteClick(TObject *Sender);
private:	// User declarations
  TStringList * TransList;
public:		// User declarations
  bool ConfigChanged;
  __fastcall TfrmValueTrans(TComponent* Owner,AnsiString BillName,AnsiString FieldName,TStringList * ATransList,bool ReadOnly);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmValueTrans *frmValueTrans;
//---------------------------------------------------------------------------
#endif
