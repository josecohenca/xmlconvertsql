//---------------------------------------------------------------------------

#ifndef ExportToFileFrmH
#define ExportToFileFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RzPanel.hpp"
#include <ExtCtrls.hpp>
#include "RzButton.hpp"
#include "RzPrgres.hpp"
#include <ComCtrls.hpp>
#include "ExportToFileThread.h"
#include <ActnList.hpp>
//---------------------------------------------------------------------------
class TfrmExportToFile : public TForm
{
__published:	// IDE-managed Components
  TActionList *ActionList1;
  TAction *Action1;
  TRzPanel *RzPanel1;
  TLabel *Label1;
  TRzProgressBar *RzProgressBar1;
  TLabel *Label3;
  TLabel *Label2;
  TLabel *Label4;
  TAnimate *Animate1;
  TRzBitBtn *btnCancel;
  void __fastcall btnCancelClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmExportToFile(TComponent* Owner);
  TExportToFileThead * ExportToFileThead;
  TBillFile   * BillFile;
  TStringList * FileList;
  TStringList * BillNameList;
  AnsiString DestFileName;
  AnsiString ErrorMessage;
  int DestFileType;
  bool AutoClose;
  bool IsSearchResult;
  bool __fastcall CreateBillFileClass(AnsiString ABillName);
  void __fastcall OnExportComplete(TObject * Sender);
  //void __fastcall OnFTPWorkBegin(TMessage Message);
  //void __fastcall OnFTPWork(TMessage Message);
  void __fastcall OnSetProcessMaxCount(TMessage Message);
  void __fastcall OnSetProcessPos(TMessage Message);
  void __fastcall OnThreadCompleted(TMessage Message);
  BEGIN_MESSAGE_MAP
    //MESSAGE_HANDLER(MSG_FTP_WORK_BEGIN,TMessage,OnFTPWorkBegin)
    //MESSAGE_HANDLER(MSG_FTP_WORK,TMessage,OnFTPWork)
    MESSAGE_HANDLER(MSG_SET_PROCESS_MAX_COUNT,TMessage,OnSetProcessMaxCount)
    MESSAGE_HANDLER(MSG_SET_PROCESS_POS,TMessage,OnSetProcessPos)
    MESSAGE_HANDLER(MSG_THREAD_COMPLETED,TMessage,OnThreadCompleted)
  END_MESSAGE_MAP(TForm)  
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmExportToFile *frmExportToFile;
//---------------------------------------------------------------------------
#endif
