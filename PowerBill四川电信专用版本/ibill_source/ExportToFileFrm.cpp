//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ExportToFileFrm.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzButton"
#pragma link "RzPrgres"
#pragma resource "*.dfm"
TfrmExportToFile *frmExportToFile;
//---------------------------------------------------------------------------
__fastcall TfrmExportToFile::TfrmExportToFile(TComponent* Owner)
  : TForm(Owner)
{
  ExportToFileThead = NULL;
  BillFile = NULL;
  FileList = new TStringList;
  BillNameList = new TStringList;
  //Label4->Caption = "�ѵ�����0����¼";
  Label4->Tag = 0;
  AutoClose   = false;
  IsSearchResult = false;
  ErrorMessage   = "";
}
void __fastcall TfrmExportToFile::OnThreadCompleted(TMessage Message)
{
  OnExportComplete(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmExportToFile::btnCancelClick(TObject *Sender)
{
  Close();  
}
//---------------------------------------------------------------------------
bool __fastcall TfrmExportToFile::CreateBillFileClass(AnsiString ABillName)
{
  bool Result = false;
  try
  {
    if(IsSearchResult)  //���ҽ��
    {
      BillFile = new TSearchResultBillFile(FileList->Strings[0],frmMain->BillConfig,Handle,frmMain->TempDirectory);
    }
    else
    {
      switch(frmMain->BillConfig->GetFileFormatType(ABillName))
      {
      case BIN_BILL_FORMAT:
        BillFile = new TBinBillFile(ABillName,frmMain->BillConfig,Handle,frmMain->TempDirectory);
        break;
      case TXT_BILL_FORMAT:
        BillFile = new TTxtBillFile(ABillName,frmMain->BillConfig,Handle,frmMain->TempDirectory);
        break;
      }
    }
    Result = true;
  }
  catch(Exception * E)
  {
    MessageBox(Handle,E->Message.c_str(),"����",MB_OK | MB_ICONSTOP);
  }
  return Result;
}
void __fastcall TfrmExportToFile::OnExportComplete(TObject * Sender)
{
  Animate1->Active = false;
  btnCancel->Caption = "�ر�";
  btnCancel->ImageIndex = 16;
  if(ExportToFileThead->ErrorMessage != "")
  {
    MessageBox(Handle,ExportToFileThead->ErrorMessage.c_str(),"����",MB_OK | MB_ICONWARNING);
    ErrorMessage = ExportToFileThead->ErrorMessage;
  }
  else
  {
    Label1->Caption = "����ת�����!";
    MessageBox(Handle,"����ת�����","��Ϣ",MB_OK | MB_ICONINFORMATION);
  }
  ExportToFileThead = NULL;
  if(AutoClose)
    Close();
}
void __fastcall TfrmExportToFile::OnSetProcessMaxCount(TMessage Message)
{
  if(Message.LParam == NULL)
  {
    RzProgressBar1->PartsComplete = RzProgressBar1->TotalParts;
    ++Label3->Tag;
    Label3->Caption = "����" + IntToStr(FileList->Count) + "���ļ���Ҫ����,��ǰ�����" + IntToStr(Label3->Tag) + "��";
    Label4->Caption = "��ת����" + IntToStr(Label4->Tag) + "����¼";
  }
  else
  {
    RzProgressBar1->TotalParts = Message.WParam;
    RzProgressBar1->PartsComplete = 0;
    Label1->Caption = "����ת���ļ�" + AnsiString((const char *)Message.LParam);
    Label4->Tag += Message.WParam;
  }
}
void __fastcall TfrmExportToFile::OnSetProcessPos(TMessage Message)
{
  RzProgressBar1->PartsComplete = Message.WParam;
}
/*
//---------------------------------------------------------------------------
void __fastcall TfrmExportToFile::OnFTPWorkBegin(TMessage Message)
{
  Label1->Caption = "���������ļ�" + AnsiString((const char *)Message.WParam);
  RzProgressBar1->TotalParts = Message.LParam;
  RzProgressBar1->PartsComplete = 0;
}
void __fastcall TfrmExportToFile::OnFTPWork(TMessage Message)
{
  RzProgressBar1->PartsComplete = Message.LParam;
}
*/
void __fastcall TfrmExportToFile::FormShow(TObject *Sender)
{
  if(ExportToFileThead != NULL)
    return;
  if(DestFileType == EXPORT_ORI)
  {
    DestFileName = ExtractFilePath(DestFileName) + BillNameList->Strings[0] + "_" + ExtractFileName(DestFileName);
  }
  //Label3->Caption = "";
  //Label4->Caption = "";
  Label3->Caption = "����" + IntToStr(FileList->Count) + "���ļ���Ҫ����,��ǰ�����0��";
  Label2->Caption = "ת��������ʽ��" + DestFileName;
  ExportToFileThead = new TExportToFileThead(true);
  ExportToFileThead->FileList = FileList;
  ExportToFileThead->DestFileName = DestFileName;
  ExportToFileThead->DestFileType = DestFileType;
  ExportToFileThead->BillNameList = BillNameList;
  ExportToFileThead->BillFile     = BillFile;
  ExportToFileThead->FreeOnTerminate = true;
  //ExportToFileThead->RetryCount = RetryCount;
  //ExportToFileThead->OnTerminate     = OnExportComplete;
  ExportToFileThead->Suspended = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmExportToFile::FormDestroy(TObject *Sender)
{
  if(BillFile != NULL)
  {
    delete BillFile;
    BillFile = NULL;
  }
  delete FileList;
  delete BillNameList;  
}
//---------------------------------------------------------------------------
void __fastcall TfrmExportToFile::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  if(ExportToFileThead != NULL)
  {
    if(MessageBox(Handle,"��Ҫ��ֹת�������Ĳ�����?","����",MB_YESNO | MB_ICONWARNING) == IDNO)
    {
      Action = caNone;
      return;
    }
    ErrorMessage = "ת�����̱��û���ֹ";
    BillFile->TermCurOperation();
    while(ExportToFileThead != NULL)
    {
      Application->ProcessMessages();
    }
  }
}
//---------------------------------------------------------------------------
