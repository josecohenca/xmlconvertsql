//---------------------------------------------------------------------------

#ifndef MainFrmH
#define MainFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "OpenDialogFrm.h"
#include <ImgList.hpp>
#include "RzListVw.hpp"
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include "RzPanel.hpp"
#include <ExtCtrls.hpp>
#include "RzTabs.hpp"
#include <Menus.hpp>
#include <ActnList.hpp>
#include "public.h"
#include "BillConfig.h"
#include "BillFile.h"
#include "BillRecord.h"
#include "BillField.h"
#include "DBConfig.h"
#include "OpenFTPDialogFrm.h"
#include <Dialogs.hpp>
#include "RzGroupBar.hpp"
#include "FTPConfig.h"
//#include "CWGIFImage.hpp"
#include <Graphics.hpp>
#include "RzPrgres.hpp"
#include "RzStatus.hpp"
#include "CWGIFImage.hpp"


//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
  TImageList *ImageList1;
  TImageList *ImageList2;
  TToolBar *ToolBar1;
  TRzPageControl *RzPageControl1;
  TToolButton *btnBillFile;
  TActionList *ActionList1;
  TAction *Action1;
  TAction *Action2;
  TAction *Action3;
  TAction *Action4;
  TToolButton *btnOpenBillConfigInIE;
  TToolButton *btnHelp;
  TToolButton *btnClose;
  TToolButton *ToolButton2;
  TToolButton *ToolButton4;
  TToolButton *btnDBLinkConfig;
  TToolButton *btnBillConfig;
  TToolButton *btnFTPConfig;
  TSaveDialog *SaveDialog1;
  TMainMenu *MainMenu1;
  TMenuItem *N1;
  TMenuItem *menuExit;
  TMenuItem *N3;
  TMenuItem *menuOption;
  TMenuItem *N5;
  TMenuItem *menuBillConfig;
  TMenuItem *menuDBConfig;
  TMenuItem *menuFTPConfig;
  TMenuItem *N2;
  TMenuItem *menuOpenBill;
  TMenuItem *N4;
  TMenuItem *menuHelp;
  TMenuItem *menuAbout;
  TImage *Image1;
  TOpenDialog *OpenDialog1;
  TToolButton *btnOpenSearchResult;
  TMenuItem *N6;
  TMenuItem *menuOpenSearchResult;
  TToolButton *btnOpenFTPFile;
  TToolButton *btnOpenFile;
  TMenuItem *menuWindows;
  TMenuItem *menuOpenLocalBill;
  TMenuItem *menuOpenFTPBill;
  TToolButton *btnOptions;
  TAction *Action5;
  TRzStatusBar *RzStatusBar1;
  TRzStatusPane *RzStatusPane1;
  TRzStatusPane *RzStatusPane2;
  TRzProgressBar *RzProgressBar1;
  TRzGlyphStatus *RzGlyphStatus1;
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall ToolButton10Click(TObject *Sender);
  void __fastcall RzPageControl1Close(TObject *Sender, bool &AllowClose);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall Action1Execute(TObject *Sender);
  void __fastcall Action2Execute(TObject *Sender);
  void __fastcall Action3Execute(TObject *Sender);
  void __fastcall Action4Execute(TObject *Sender);
  void __fastcall btnOpenBillConfigInIEClick(TObject *Sender);
  void __fastcall btnCloseClick(TObject *Sender);
  void __fastcall btnBillFileClick(TObject *Sender);
  void __fastcall btnDBLinkConfigClick(TObject *Sender);
  void __fastcall btnBillConfigClick(TObject *Sender);
  void __fastcall btnFTPConfigClick(TObject *Sender);
  void __fastcall SaveDialog1TypeChange(TObject *Sender);
  void __fastcall menuExitClick(TObject *Sender);
  void __fastcall menuOptionClick(TObject *Sender);
  void __fastcall menuAboutClick(TObject *Sender);
  void __fastcall btnOpenSearchResultClick(TObject *Sender);
  void __fastcall btnOpenFileClick(TObject *Sender);
  void __fastcall btnOpenFTPFileClick(TObject *Sender);
  void __fastcall RzPageControl1Change(TObject *Sender);
  void __fastcall Action5Execute(TObject *Sender);
  void __fastcall menuHelpClick(TObject *Sender);
private:	// User declarations
  bool __fastcall CloseAChildForm(TRzTabSheet * TabSheet);
  void __fastcall ShowInitializeStatus(AnsiString StatusText);
public:		// User declarations
  bool __fastcall IsRegisted();
  void __fastcall WriteFirstRunDate();
  void __fastcall AddFormToPageControl(TForm * AForm);
  TForm * __fastcall IsFormCreated(AnsiString FormClassName);
  TfrmOpenDialog    * frmOpenDialog;
  TfrmOpenFTPDialog * frmOpenFTPDialog;
  AnsiString BillConfigFile;  //话单配置文件名
  AnsiString BCPExePath;      //BCP的程序路径
  AnsiString SQLLDRExePath;   //SQLLDR的程序路径
  AnsiString MySQLImportExePath;
  AnsiString TempDirectory;
  TBillConfig * BillConfig;
  TDBConfig   * DBConfig;
  TFTPConfig  * FTPConfig;
  AnsiString RegistUserName;
  AnsiString SerialNumber;
  AnsiString FirstRunDate;
  int SortConfirm;
  bool __fastcall ShowOpenFTPDialog(AnsiString BillName = "");
  bool __fastcall ShowOpenFileDialog(AnsiString BillName = "");
  __fastcall TfrmMain(TComponent* Owner);
  __fastcall ~TfrmMain();
  void __fastcall AddItemToWarning(AnsiString Type,AnsiString FileName,AnsiString Comments);

  void __fastcall OnOpenFileError(TMessage Message);
  void __fastcall OnFTPWorkBegin(TMessage Message);
  void __fastcall OnFTPWork(TMessage Message);
  void __fastcall OnSetProcessMaxCount(TMessage Message);
  void __fastcall OnSetProcessPos(TMessage Message);
  void __fastcall OnThreadTerminate(TMessage Message);
  BEGIN_MESSAGE_MAP
    MESSAGE_HANDLER(MSG_OPEN_FILE_ERROR,TMessage,OnOpenFileError)
    MESSAGE_HANDLER(MSG_FTP_WORK_BEGIN,TMessage,OnFTPWorkBegin)
    MESSAGE_HANDLER(MSG_FTP_WORK,TMessage,OnFTPWork)
    MESSAGE_HANDLER(MSG_SET_PROCESS_MAX_COUNT,TMessage,OnSetProcessMaxCount)
    MESSAGE_HANDLER(MSG_SET_PROCESS_POS,TMessage,OnSetProcessPos)
    MESSAGE_HANDLER(MSG_THREAD_COMPLETED,TMessage,OnThreadTerminate)
  END_MESSAGE_MAP(TForm)     
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
extern void GetWmiInfo(TStrings *lpList, WideString wsClass);
//---------------------------------------------------------------------------
#endif
 