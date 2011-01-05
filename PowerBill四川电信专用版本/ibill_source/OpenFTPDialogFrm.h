//---------------------------------------------------------------------------

#ifndef OpenFTPDialogFrmH
#define OpenFTPDialogFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RzCmboBx.hpp"
#include <ImgList.hpp>
#include "dcOutPanel.hpp"
#include "RzButton.hpp"
#include "RzListVw.hpp"
#include "RzPanel.hpp"
#include "RzShellCtrls.hpp"
#include "RzTreeVw.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <ActnList.hpp>
#include "RzEdit.hpp"
#include <Mask.hpp>
#include "RzRadChk.hpp"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdFTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include "SearchFileFrm.h"
//---------------------------------------------------------------------------
class TfrmOpenFTPDialog : public TForm
{
__published:	// IDE-managed Components
  TLabel *Label1;
  TImage *Image1;
  TLabel *Label3;
  TToolBar *ToolBar1;
  TToolButton *ToolButton4;
  TRzBitBtn *btnCancel;
  TRzBitBtn *btnOpen;
  TRzPanel *RzPanel1;
  TRzComboBox *cbxFileFormat;
  TImageList *ImageList2;
  TPopupMenu *PopupMenu1;
  TMenuItem *menuIcon;
  TMenuItem *menuSmall;
  TMenuItem *menuList;
  TMenuItem *menuDetail;
  TRzListView *lvFiles;
  TImageList *ImageList3;
  TImageList *ImageList4;
  TActionList *ActionList1;
  TAction *Action1;
  TDCHeaderPanel *DCHeaderPanel1;
  TSplitter *Splitter1;
  TRzTreeView *tvFolder;
  TLabel *Label2;
  TRzEdit *txtUserName;
  TLabel *Label4;
  TRzEdit *txtPassword;
  TToolButton *ToolButton2;
  TLabel *Label5;
  TRzEdit *txtPort;
  TRzCheckBox *cbPasv;
  TIdFTP *IdFTP1;
  TRzComboBox *cbxServer;
  TPopupMenu *PopupMenu2;
  TMenuItem *menuSearchFile;
  void __fastcall menuIconClick(TObject *Sender);
  void __fastcall menuSmallClick(TObject *Sender);
  void __fastcall menuListClick(TObject *Sender);
  void __fastcall menuDetailClick(TObject *Sender);
  void __fastcall Action1Execute(TObject *Sender);
  void __fastcall btnCancelClick(TObject *Sender);
  void __fastcall btnOpenClick(TObject *Sender);
  void __fastcall tvFolderChange(TObject *Sender, TTreeNode *Node);
  void __fastcall ToolButton2Click(TObject *Sender);
  void __fastcall lvFilesColumnClick(TObject *Sender, TListColumn *Column);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall lvFilesCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall cbxServerChange(TObject *Sender);
  void __fastcall menuSearchFileClick(TObject *Sender);
  void __fastcall PopupMenu2Popup(TObject *Sender);
  void __fastcall tvFolderMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  bool __fastcall FormHelp(WORD Command, int Data, bool &CallHelp);
private:	// User declarations
  TfrmSearchFile * frmSearchFile;
  void __fastcall SearchFile(AnsiString APath,AnsiString SearchFileName,int Level);
  void __fastcall ListDir(TTreeNode * ParentNode);
  void __fastcall GetNodePath(TTreeNode * Node,AnsiString & Path);
  void __fastcall CreateListPaths(TTreeNode * ParentNode,TStringList * PathList,bool Recursion);
  void __fastcall GetSelectedNodes(TTreeNode * ParentNode,TList * NodeList);
public:		// User declarations
  bool Recursion;
  bool __fastcall Execute();
  __fastcall TfrmOpenFTPDialog(TComponent* Owner);
  //AnsiString Path;
  //TStringList * FileList;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmOpenFTPDialog *frmOpenFTPDialog;
//---------------------------------------------------------------------------
#endif
