//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "OpeningFileFrm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPrgres"
#pragma link "RzButton"
#pragma link "RzPanel"
#pragma resource "*.dfm"
TfrmOpeningFile *frmOpeningFile;
//---------------------------------------------------------------------------
__fastcall TfrmOpeningFile::TfrmOpeningFile(TComponent* Owner)
  : TForm(Owner)
{
  CloseByUser      = true;
  BillFile         = NULL;
  OpenFileThread   = NULL;
  hOldBillOwnerWnd = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfrmOpeningFile::OnFTPWorkBegin(TMessage Message)
{
  labInformation->Caption = "���������ļ�" + AnsiString((const char *)Message.WParam);
  RzProgressBar1->TotalParts = Message.LParam;
  RzProgressBar1->PartsComplete = 0;
}
void __fastcall TfrmOpeningFile::OnFTPWork(TMessage Message)
{
  RzProgressBar1->PartsComplete = Message.LParam;
}
void __fastcall TfrmOpeningFile::OnSetProcessMaxCount(TMessage Message)
{
  labInformation->Caption = "����ɨ���ļ�" + AnsiString((const char *)Message.LParam);
  RzProgressBar1->TotalParts = Message.WParam;
  RzProgressBar1->PartsComplete = 0;
}
void __fastcall TfrmOpeningFile::OnSetProcessPos(TMessage Message)
{
  RzProgressBar1->PartsComplete = Message.WParam;
  Label1->Caption = "���ҵ�" + IntToStr(BillFile->GetRecordCount()) + "����¼,������" + IntToStr(BillFile->SkipRecordCount) + "����¼";
}
//---------------------------------------------------------------------------
void __fastcall TfrmOpeningFile::FormKeyPress(TObject *Sender, char &Key)
{
  if(Key == 27)
  {
    Key = 0;
    ModalResult = mrCancel;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmOpeningFile::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  if(OpenFileThread != NULL)
  {
    if(!OpenFileThread->Continue)
      return;
    if(MessageBox(Handle,"��Ҫ������ǰ�Ĳ�����?","����",MB_YESNO | MB_ICONQUESTION) == IDNO)
    {
      if(OpenFileThread != NULL)
      {
        Action = caNone;
        return;
      }
    }
    if(OpenFileThread != NULL)
    {
      BillFile->TermCurOperation();
      OpenFileThread->Continue = false;
      btnCancel->Enabled = false;
      while(OpenFileThread != NULL)
      {
        Application->ProcessMessages();
      }
    }
  }
  BillFile->SetOwnerWnd(hOldBillOwnerWnd);
}
//---------------------------------------------------------------------------
void __fastcall TfrmOpeningFile::OnFileThreadTerminate(TMessage Message)
{
  Label1->Caption = "���ҵ�" + IntToStr(BillFile->GetRecordCount()) + "����¼,������" + IntToStr(BillFile->SkipRecordCount) + "����¼";
  ErrorMessage = OpenFileThread->ErrorMessage;
  bool Continue = OpenFileThread->Continue;
  OpenFileThread = NULL;
  if(ErrorMessage != "")
  {
    MessageBox(Handle,ErrorMessage.c_str(),"����",MB_OK | MB_ICONSTOP);
    ModalResult = mrCancel;
    return;
  }
  else if(!Continue)
  {
    ErrorMessage = "�������û�ȡ��";
    ModalResult = mrCancel;
    return;
  }
  else
  {
    ModalResult = mrOk;
    return;
  }
}



void __fastcall TfrmOpeningFile::FormShow(TObject *Sender)
{
  if(OpenFileThread != NULL)
    return;
  ModalResult = mrNone;
  Label1->Caption = "";
  RzProgressBar1->TotalParts    = 0;
  RzProgressBar1->PartsComplete = 0;
  OpenFileThread = new TOpenFileThread(true);
  OpenFileThread->BillFile = BillFile;
  OpenFileThread->FileName = FileName;
  OpenFileThread->BillName = BillName;
  OpenFileThread->FreeOnTerminate = true;
  OpenFileThread->hWnd = Handle;
  hOldBillOwnerWnd = BillFile->hWnd;
  BillFile->SetOwnerWnd(this->Handle);
  labInformation->Caption = "���������ļ�" + BillFile->GetShortFileName() + ",���Ժ�...";
  OpenFileThread->Suspended = false;  
}
//---------------------------------------------------------------------------

