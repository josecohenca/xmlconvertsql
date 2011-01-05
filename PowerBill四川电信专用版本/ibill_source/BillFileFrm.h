//---------------------------------------------------------------------------

#ifndef BillFileFrmH
#define BillFileFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RzListVw.hpp"
#include "RzPanel.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include "SearchFrm.h"
#include "ViewBillFrm.h"
#include <Dialogs.hpp>
#include "dcOutPanel.hpp"
#include "RzTabs.hpp"
#include "RzSplit.hpp"
//---------------------------------------------------------------------------
class TfrmBillFile : public TForm
{
__published:	// IDE-managed Components
  TRzPanel *RzPanel1;
  TPopupMenu *PopupMenu1;
  TMenuItem *menuAddFile;
  TMenuItem *N8;
  TMenuItem *menuSelectAll;
  TMenuItem *N3;
  TMenuItem *N1;
  TMenuItem *menuDelete;
  TMenuItem *menuClearList;
  TMenuItem *N5;
  TMenuItem *menuSearch;
  TMenuItem *N7;
  TMenuItem *menuExportToFile;
  TMenuItem *menuExportToDB;
  TMenuItem *menuAddFromFTP;
  TToolBar *ToolBar1;
  TToolButton *btnAddFile;
  TToolButton *btnAddFTPFile;
  TPopupMenu *PopupMenu2;
  TMenuItem *menuDeleteWarning;
  TToolButton *btnReview;
  TToolButton *btnSearch;
  TMenuItem *menuCopyFileLink;
  TMenuItem *N2;
  TMenuItem *menuStat;
  TToolButton *btnExportToDB;
  TMenuItem *menuCopyFileList;
  TToolButton *btnExportToFile;
  TMenuItem *menuDeleteAWarning;
  TMenuItem *N4;
  TMenuItem *menuCopyWarning;
  TRzSplitter *RzSplitter1;
  TDCHeaderPanel *DCHeaderPanel1;
  TRzListView *lvWarning;
  TRzListView *lvFileList;
  TToolButton *ToolButton2;
  TMenuItem *N6;
  TMenuItem *menuSelectAllWarning;
  TPopupMenu *PopupMenu4;
  void __fastcall PopupMenu1Popup(TObject *Sender);
  void __fastcall menuAddFileClick(TObject *Sender);
  void __fastcall menuSelectAllClick(TObject *Sender);
  void __fastcall menuDeleteClick(TObject *Sender);
  void __fastcall menuClearListClick(TObject *Sender);
  void __fastcall menuExportToDBClick(TObject *Sender);
  void __fastcall lvFileListColumnClick(TObject *Sender,
          TListColumn *Column);
  void __fastcall lvFileListDblClick(TObject *Sender);
  void __fastcall btnAddFTPFileClick(TObject *Sender);
  void __fastcall menuDeleteWarningClick(TObject *Sender);
  void __fastcall PopupMenu2Popup(TObject *Sender);
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall menuSearchClick(TObject *Sender);
  void __fastcall btnReviewClick(TObject *Sender);
  void __fastcall menuExportToFileClick(TObject *Sender);
  void __fastcall menuCopyFileLinkClick(TObject *Sender);
  void __fastcall menuStatClick(TObject *Sender);
  void __fastcall menuCopyFileListClick(TObject *Sender);
  void __fastcall menuDeleteAWarningClick(TObject *Sender);
  void __fastcall menuCopyWarningClick(TObject *Sender);
  void __fastcall N3Click(TObject *Sender);
  void __fastcall menuSelectAllWarningClick(TObject *Sender);
private:	// User declarations
  void __fastcall AddItemToWarning(AnsiString Type,AnsiString FileName,AnsiString Comments);
  TDateTime __fastcall CovFileDate(FILETIME Fd);
  TDateTime __fastcall GetModifiedTime(AnsiString FileName);
  TfrmSearch  * frmSearch;
  TStringList * LoadDirList;
  //TfrmViewBill * frmViewBill;
public:		// User declarations
  TStringList * TempFileList;
  bool __fastcall AddFileToList(//AnsiString BillName,
    //TStringList * FileList,
    //AnsiString ListFileName = "",
    bool CheckFileFormat,
    bool ScanRecordCount
    );
  bool __fastcall AddFTPFileToList(AnsiString BillName,/*TStringList * FileList*/TRzListView * FileListView,
    AnsiString Server,AnsiString UserName,AnsiString Password/*,AnsiString Path*/,
    AnsiString Port,AnsiString Pasv);
  __fastcall TfrmBillFile(TComponent* Owner);
  void __fastcall OnCustomerKeyDown(TMessage Message);
  BEGIN_MESSAGE_MAP
    MESSAGE_HANDLER(MSG_CUSTOMER_KEYDOWN,TMessage,OnCustomerKeyDown)
  END_MESSAGE_MAP(TForm)      
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmBillFile *frmBillFile;
//---------------------------------------------------------------------------
#endif
