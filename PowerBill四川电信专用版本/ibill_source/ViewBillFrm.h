//---------------------------------------------------------------------------

#ifndef ViewBillFrmH
#define ViewBillFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RzListVw.hpp"
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include "public.h"
#include "ALStatusBar.hpp"
#include "RzPrgres.hpp"
#include <Menus.hpp>
#include "RzPanel.hpp"
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "RecordDetailFrm.h"
#include "RzButton.hpp"
#include "RzRadChk.hpp"
//---------------------------------------------------------------------------
class TfrmViewBill : public TForm
{
__published:	// IDE-managed Components
  TRzListView *lvBills;
  TImageList *ImageList1;
  TToolBar *ToolBar1;
  TToolButton *btnSaveAs;
  TALStatusBar *StatusBar1;
  TToolButton *btnExportToDB;
  TToolButton *btnFilterReport;
  TPopupMenu *PopupMenu1;
  TMenuItem *menuCopyRecord;
  TRzPanel *RzPanel1;
  TImage *Image1;
  TLabel *Label3;
  TMenuItem *N1;
  TMenuItem *menuRecordDetail;
  TRzCheckBox *cbxTrans;
  void __fastcall lvBillsData(TObject *Sender, TListItem *Item);
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall lvBillsColumnClick(TObject *Sender, TListColumn *Column);
  void __fastcall btnSaveAsClick(TObject *Sender);
  void __fastcall btnExportToDBClick(TObject *Sender);
  void __fastcall btnFilterReportClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall lvBillsDataStateChange(TObject *Sender, int StartIndex,
          int EndIndex, TItemStates OldState, TItemStates NewState);
  void __fastcall PopupMenu1Popup(TObject *Sender);
  void __fastcall menuCopyRecordClick(TObject *Sender);
  void __fastcall menuRecordDetailClick(TObject *Sender);
  void __fastcall cbxTransClick(TObject *Sender);
private:	// User declarations
  AnsiString FileName,FilePath,BillName,FormatType;
  TBillFile    * BillFile;
  TBillConfig  * BillConfig;
  TList * FieldList;
  TfrmRecordDetail * frmRecordDetail;
public:		// User declarations
  TList * SelectedItems;
  __fastcall TfrmViewBill(TComponent* Owner,TBillConfig  * ABillConfig);
  bool __fastcall OpenBillFile(AnsiString AFormatType,AnsiString ABillName,AnsiString AFilePath,AnsiString AFileName,unsigned int FileSize);
  void __fastcall OnFTPWorkBegin(TMessage Message);
  void __fastcall OnFTPWork(TMessage Message);
  BEGIN_MESSAGE_MAP
    MESSAGE_HANDLER(MSG_FTP_WORK_BEGIN,TMessage,OnFTPWorkBegin)
    MESSAGE_HANDLER(MSG_FTP_WORK,TMessage,OnFTPWork)
  END_MESSAGE_MAP(TForm)  
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmViewBill *frmViewBill;
//---------------------------------------------------------------------------
#endif
