//---------------------------------------------------------------------------

#ifndef FTPConfigFrmH
#define FTPConfigFrmH
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
//---------------------------------------------------------------------------
class TfrmFTPConfig : public TForm
{
__published:	// IDE-managed Components
  TRzPanel *RzPanel1;
  TRzListView *lvFTPServers;
  TRzPanel *RzPanel2;
  TImage *Image1;
  TLabel *Label1;
  TRzBitBtn *btnSave;
  TPopupMenu *PopupMenu1;
  TMenuItem *menuAddNew;
  TMenuItem *menuEdit;
  TMenuItem *menuDelete;
  TRzBitBtn *btnOk;
  void __fastcall lvFTPServersDblClick(TObject *Sender);
  void __fastcall btnSaveClick(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall PopupMenu1Popup(TObject *Sender);
  void __fastcall menuDeleteClick(TObject *Sender);
  void __fastcall menuAddNewClick(TObject *Sender);
  void __fastcall menuEditClick(TObject *Sender);
  void __fastcall btnOkClick(TObject *Sender);
  void __fastcall lvFTPServersDragOver(TObject *Sender, TObject *Source,
          int X, int Y, TDragState State, bool &Accept);
  void __fastcall lvFTPServersDragDrop(TObject *Sender, TObject *Source,
          int X, int Y);
private:	// User declarations
  bool ForSelect;
  void __fastcall FillListItem(TListItem * Item);
  void __fastcall EditFTPConfig(TListItem * Item);
  bool __fastcall SaveChanges(bool ShowHint);
public:		// User declarations
  __fastcall TfrmFTPConfig(TComponent* Owner,bool AForSelect = false);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmFTPConfig *frmFTPConfig;
//---------------------------------------------------------------------------
#endif
