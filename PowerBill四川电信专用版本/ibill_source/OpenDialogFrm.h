//---------------------------------------------------------------------------

#ifndef OpenDialogFrmH
#define OpenDialogFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RzCmboBx.hpp"
#include "RzShellCtrls.hpp"
#include "RzGroupBar.hpp"
#include "RzPanel.hpp"
#include <ExtCtrls.hpp>
#include "RzListVw.hpp"
#include <ComCtrls.hpp>
#include <Graphics.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>
#include "RzButton.hpp"
#include <Menus.hpp>
#include "RzTreeVw.hpp"
#include "dcOutPanel.hpp"
#include "RzRadChk.hpp"
//---------------------------------------------------------------------------
class TfrmOpenDialog : public TForm
{
__published:	// IDE-managed Components
  TLabel *Label1;
  TRzShellCombo *cbxFolder;
  TImage *Image1;
  TToolBar *ToolBar1;
  TToolButton *ToolButton1;
  TToolButton *ToolButton4;
  TImageList *ImageList1;
  TRzBitBtn *btnCancel;
  TRzBitBtn *btnOpen;
  TPopupMenu *PopupMenu1;
  TMenuItem *menuIcon;
  TMenuItem *menuSmall;
  TMenuItem *menuList;
  TMenuItem *menuDetail;
  TToolButton *ToolButton2;
  TRzPanel *RzPanel1;
  TDCHeaderPanel *DCHeaderPanel1;
  TRzShellList *lvFiles;
  TSplitter *Splitter1;
  TRzShellTree *tvFolder;
  TLabel *Label2;
  TRzComboBox *cbxFileFormat;
  TRzCheckBox *cbxCheckFileFormat;
  TRzCheckBox *cbxScanRecordCount;
  void __fastcall btnCancelClick(TObject *Sender);
  void __fastcall btnOpenClick(TObject *Sender);
  void __fastcall cbxFolderChange(TObject *Sender);
  void __fastcall lvFilesFolderChanged(TObject *Sender);
  void __fastcall ToolButton1Click(TObject *Sender);
  void __fastcall menuIconClick(TObject *Sender);
  void __fastcall menuSmallClick(TObject *Sender);
  void __fastcall menuListClick(TObject *Sender);
  void __fastcall menuDetailClick(TObject *Sender);
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall lvFilesDblClickOpen(TObject *Sender, bool &Handled);
  void __fastcall tvFolderChange(TObject *Sender, TTreeNode *Node);
  bool __fastcall FormHelp(WORD Command, int Data, bool &CallHelp);
private:	// User declarations
public:		// User declarations
  //TStringList * Files;
  AnsiString PathName;
  bool __fastcall Execute();
  __fastcall TfrmOpenDialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmOpenDialog *frmOpenDialog;
//---------------------------------------------------------------------------
#endif
