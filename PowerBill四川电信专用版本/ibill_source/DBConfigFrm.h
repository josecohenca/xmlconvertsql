//---------------------------------------------------------------------------

#ifndef DBConfigFrmH
#define DBConfigFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "RzPanel.hpp"
#include "RzButton.hpp"
#include "RzListVw.hpp"
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include "DBConfig.h"
//---------------------------------------------------------------------------
class TfrmDBConfig : public TForm
{
__published:	// IDE-managed Components
  TRzPanel *RzPanel1;
  TRzListView *lvDBConnections;
  TRzPanel *RzPanel2;
  TImage *Image1;
  TLabel *Label1;
  TRzBitBtn *btnSave;
  TPopupMenu *PopupMenu1;
  TMenuItem *menuAddNew;
  TMenuItem *menuEdit;
  TMenuItem *menuDelete;
  TRzBitBtn *btnOk;
  void __fastcall lvDBConnectionsDblClick(TObject *Sender);
  void __fastcall btnSaveClick(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall PopupMenu1Popup(TObject *Sender);
  void __fastcall menuDeleteClick(TObject *Sender);
  void __fastcall menuAddNewClick(TObject *Sender);
  void __fastcall menuEditClick(TObject *Sender);
  void __fastcall btnOkClick(TObject *Sender);
  void __fastcall lvDBConnectionsDragDrop(TObject *Sender, TObject *Source,
          int X, int Y);
  void __fastcall lvDBConnectionsDragOver(TObject *Sender, TObject *Source,
          int X, int Y, TDragState State, bool &Accept);
private:	// User declarations
  bool ForSelect;
  TDBConfig * DBConfig;
  void __fastcall FillListItem(TListItem * Item);
  void __fastcall EditDBConfig(TListItem * Item);
public:		// User declarations
  __fastcall TfrmDBConfig(TComponent* Owner,bool AForSelect = false);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDBConfig *frmDBConfig;
//---------------------------------------------------------------------------
#endif
