//---------------------------------------------------------------------------

#ifndef ExportToDBFrmH
#define ExportToDBFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RzPanel.hpp"
#include <ExtCtrls.hpp>
#include "RzCmboBx.hpp"
#include "RzEdit.hpp"
#include <Mask.hpp>
#include "RzListVw.hpp"
#include <ComCtrls.hpp>
#include "RzButton.hpp"
#include "RzRadChk.hpp"
#include <Graphics.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <Menus.hpp>
#include "ThreadConvertForDB.h"
#include "ThreadSubmitToDB.h"
//#include "MonitorFrm.h"
#include <ActnList.hpp>
#include "RzShellCtrls.hpp"
#include "RzFilSys.hpp"
#include <FileCtrl.hpp>
#include "RzPrgres.hpp"
#include "dcOutPanel.hpp"
//---------------------------------------------------------------------------
class TfrmExportToDB : public TForm
{
__published:	// IDE-managed Components
  TRzPanel *RzPanel1;
  TRzComboBox *cbxDBType;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TRzEdit *txtDBUserName;
  TRzEdit *txtDBPassword;
  TRzListView *lvFileList;
  TLabel *Label4;
  TRzBitBtn *btnStart;
  TADOConnection *ADOConnection1;
  TLabel *Label5;
  TRzEdit *txtDBServer;
  TPopupMenu *PopupMenu1;
  TMenuItem *menuDelete;
  TMenuItem *menuViewBill;
  TRzCheckBox *cbxTransFieldValue;
  TRzEdit *txtTableName;
  TRzBitBtn *btnTerm;
  TLabel *Label9;
  TADODataSet *ADODataSet1;
  TLabel *Label10;
  TRzEdit *txtDBPort;
  TRzComboBox *cbxDataBases;
  TLabel *Label11;
  TActionList *ActionList1;
  TAction *Action1;
  TLabel *Label12;
  TRzEdit *txtMaxError;
  TDCHeaderPanel *DCHeaderPanel1;
  TRzMemo *txtMessages;
  TRzPanel *RzPanel2;
  TLabel *labWarning;
  TImage *Image1;
  TRzProgressBar *RzProgressBar1;
  TLabel *Label8;
  TLabel *Label7;
  TAnimate *Animate1;
  void __fastcall menuViewBillClick(TObject *Sender);
  void __fastcall menuDeleteClick(TObject *Sender);
  void __fastcall btnStartClick(TObject *Sender);
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall btnTermClick(TObject *Sender);
  void __fastcall cbxDBTypeChange(TObject *Sender);
  void __fastcall cbxDataBasesDropDown(TObject *Sender);
  void __fastcall Label11Click(TObject *Sender);
  void __fastcall DCHeaderPanel1RightButtonClick(TObject *Sender);
  void __fastcall Label7Click(TObject *Sender);
  bool __fastcall FormHelp(WORD Command, int Data, bool &CallHelp);
private:	// User declarations
  TStringList * FileList;
  TStringList * BillNameList;
  int DestFileFormat;
  TStringList * SQLCommandList;
  AnsiString ToolPath;
  int DBType;
  int RecordCount;
  AnsiString CreateTableSQL;
//  TfrmMonitor * frmMonitor;
  AnsiString TempFileName;
  TStringList * MessageTextList;
  void __fastcall EnableCtrls(bool Enabled);
  bool __fastcall OpenDBConnection();
  bool __fastcall StartConvert(bool ManualStart);
  bool __fastcall StartImport(bool ManualStart);
public:		// User declarations
  TThreadConvertForDB * ThreadConvertForDB;
  TThreadSubmitToDB   * ThreadSubmitToDB;
  bool TermByUser;
  bool ExportCompleted;
  bool IsSearchResult;
  char chDriver;
  bool __fastcall ManualStartJob();
  __fastcall TfrmExportToDB(TComponent* Owner);
  void __fastcall OnConvertThreadTerminate(TObject * Sender);
  void __fastcall OnSubmiteThreadTerminate(TObject * Sender);
  void __fastcall OnSetProcessPos(TMessage Message);
  void __fastcall OnThreadCompleted(TMessage Message);
  void __fastcall OnSetProcessMaxCount(TMessage Message);
  bool __fastcall LoadFileList(TStringList * FileList,AnsiString BillName);
  BEGIN_MESSAGE_MAP
    MESSAGE_HANDLER(MSG_SET_PROCESS_MAX_COUNT,TMessage,OnSetProcessMaxCount)
    MESSAGE_HANDLER(MSG_SET_PROCESS_POS,TMessage,OnSetProcessPos)
    MESSAGE_HANDLER(MSG_THREAD_COMPLETED,TMessage,OnThreadCompleted)
  END_MESSAGE_MAP(TForm)    
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmExportToDB *frmExportToDB;
//---------------------------------------------------------------------------
#endif
