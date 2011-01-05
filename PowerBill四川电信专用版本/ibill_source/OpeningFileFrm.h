//---------------------------------------------------------------------------

#ifndef OpeningFileFrmH
#define OpeningFileFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RzPrgres.hpp"
#include "RzButton.hpp"
#include <ExtCtrls.hpp>
#include "BillFile.h"
#include "OpenFileThread.h"
#include "RzPanel.hpp"
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TfrmOpeningFile : public TForm
{
__published:	// IDE-managed Components
  TRzPanel *RzPanel1;
  TRzProgressBar *RzProgressBar1;
  TImage *Image1;
  TLabel *labInformation;
  TLabel *Label1;
  TRzBitBtn *btnCancel;
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall FormShow(TObject *Sender);
private:	// User declarations
  bool CloseByUser;
public:		// User declarations
  TBillFile * BillFile;
  AnsiString ErrorMessage;
  AnsiString FileName;
  AnsiString BillName;
  HANDLE hOldBillOwnerWnd;
  TOpenFileThread * OpenFileThread;
  __fastcall TfrmOpeningFile(TComponent* Owner);
  //void __fastcall CloseWindow(TModalResult Result);
  void __fastcall OnFileThreadTerminate(TMessage Message);

  void __fastcall OnFTPWorkBegin(TMessage Message);
  void __fastcall OnFTPWork(TMessage Message);
  void __fastcall OnSetProcessMaxCount(TMessage Message);
  void __fastcall OnSetProcessPos(TMessage Message);
  //void __fastcall OnThreadCompleted(TMessage Message);
  BEGIN_MESSAGE_MAP
    MESSAGE_HANDLER(MSG_FTP_WORK_BEGIN,TMessage,OnFTPWorkBegin)
    MESSAGE_HANDLER(MSG_FTP_WORK,TMessage,OnFTPWork)
    MESSAGE_HANDLER(MSG_SET_PROCESS_MAX_COUNT,TMessage,OnSetProcessMaxCount)
    MESSAGE_HANDLER(MSG_SET_PROCESS_POS,TMessage,OnSetProcessPos)
    MESSAGE_HANDLER(MSG_THREAD_COMPLETED,TMessage,OnFileThreadTerminate)
  END_MESSAGE_MAP(TForm)    
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmOpeningFile *frmOpeningFile;
//---------------------------------------------------------------------------
#endif
