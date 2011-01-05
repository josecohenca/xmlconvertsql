//---------------------------------------------------------------------------

#ifndef BillConfigFrmH
#define BillConfigFrmH
//---------------------------------------------------------------------------
//#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RzListVw.hpp"
#include "RzPanel.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "RzTreeVw.hpp"
#include <ToolWin.hpp>
#include <Menus.hpp>
#include "BillConfig.h"
//---------------------------------------------------------------------------
class TfrmBillConfig : public TForm
{
__published:	// IDE-managed Components
  TToolBar *ToolBar1;
  TToolButton *ToolButton1;
  TRzListView *lvBills;
  TPopupMenu *PopupMenu1;
  TMenuItem *menuNewBill;
  TMenuItem *menuEditBill;
  TMenuItem *menuDeleteBill;
  TToolButton *ToolButton2;
  TMenuItem *N1;
  TMenuItem *menuCopyBillConfig;
  TMenuItem *menuPaste;
  TToolButton *btnSave;
  TToolButton *ToolButton3;
  TToolButton *btnRollback;
  void __fastcall lvBillsDblClick(TObject *Sender);
  void __fastcall PopupMenu1Popup(TObject *Sender);
  void __fastcall menuNewBillClick(TObject *Sender);
  void __fastcall menuDeleteBillClick(TObject *Sender);
  void __fastcall menuCopyBillConfigClick(TObject *Sender);
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall menuPasteClick(TObject *Sender);
  void __fastcall btnSaveClick(TObject *Sender);
  void __fastcall btnRollbackClick(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall lvBillsDragOver(TObject *Sender, TObject *Source, int X,
          int Y, TDragState State, bool &Accept);
  void __fastcall lvBillsDragDrop(TObject *Sender, TObject *Source, int X,
          int Y);
private:	// User declarations
  //_di_IXMLNode CloneBillNode;
  void __fastcall FillBillList(TListItem * Item,AnsiString BillName);
  bool __fastcall SaveChanges(bool ShowHint);
  bool __fastcall CheckClipboard(TBillConfig * TmpBillConfig);
public:		// User declarations
  TBillConfig * BillConfig;
  __fastcall TfrmBillConfig(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmBillConfig *frmBillConfig;
//---------------------------------------------------------------------------
#endif
