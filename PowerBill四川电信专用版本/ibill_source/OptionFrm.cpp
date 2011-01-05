//---------------------------------------------------------------------------
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif

#include "FolderBrowseDialog.h"
#include "OptionFrm.h"
#include "MainFrm.h"
#include "IniFiles.hpp"

#define NO_WIN32_LEAN_AND_MEAN
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzButton"
#pragma link "RzPanel"
#pragma link "RzBtnEdt"
#pragma link "RzEdit"
#pragma resource "*.dfm"
TfrmOption *frmOption;
//---------------------------------------------------------------------------
__fastcall TfrmOption::TfrmOption(TComponent* Owner)
  : TForm(Owner)
{
  txtBCP->Text    = frmMain->BCPExePath;    //BCP�ĳ���·��
  txtSQLLDR->Text = frmMain->SQLLDRExePath; //SQLLDR�ĳ���·��
  txtMySQLImport->Text    = frmMain->MySQLImportExePath;
  txtTempDir->Text        = frmMain->TempDirectory;
  txtSortConfirm->Text    = frmMain->SortConfirm;
  txtFTPTimeout->Text     = FTPTimeout;
  txtRetryOpenCount->Text = RetryCount;
}
//---------------------------------------------------------------------------
void __fastcall TfrmOption::txtBCPButtonClick(TObject *Sender)
{
  OpenDialog1->InitialDir = txtBCP->Text;
  OpenDialog1->FileName   = txtBCP->Text;
  OpenDialog1->Filter     = "SQLServer�������Ƴ���(BCP.EXE)|bcp.exe";
  OpenDialog1->Title = "BCP����λ��";
  while(true)
  {
    if(OpenDialog1->Execute())
    {
      TSearchRec sr;
      if(FindFirst(ExtractFilePath(OpenDialog1->FileName) + "bcp.exe",faAnyFile,sr)==0)
      {
        if((sr.Attr&faDirectory) != faDirectory)
        {
          txtBCP->Text = OpenDialog1->FileName;
          FindClose(sr);
          break;
        }
        FindClose(sr);
      }
      else
      {
        MessageBox(Handle,"ָ����λ���Ҳ���bcp.exe,������ѡ��!","��ʾ",MB_OK | MB_ICONWARNING);
      }
    }
    else
      return;
  }  
}
//---------------------------------------------------------------------------
void __fastcall TfrmOption::txtSQLLDRButtonClick(TObject *Sender)
{
  OpenDialog1->InitialDir = txtSQLLDR->Text;
  OpenDialog1->FileName   = txtSQLLDR->Text;
  OpenDialog1->Filter     = "Oracle�������Ƴ���(SQLLDR.EXE)|SQLLDR.exe";
  OpenDialog1->Title = "SQLLDR����λ��";
  while(true)
  {
    if(OpenDialog1->Execute())
    {
      TSearchRec sr;
      if(FindFirst(ExtractFilePath(OpenDialog1->FileName) + "sqlldr.exe",faAnyFile,sr)==0)
      {
        if((sr.Attr&faDirectory) != faDirectory)
        {
          txtSQLLDR->Text = OpenDialog1->FileName;
          FindClose(sr);
          break;
        }
        FindClose(sr);
      }
      else
      {
        MessageBox(Handle,"ָ����λ���Ҳ���sqlldr.exe,������ѡ��!","��ʾ",MB_OK | MB_ICONWARNING);
      }
    }
    else
      return;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmOption::txtMySQLImportButtonClick(TObject *Sender)
{
  OpenDialog1->InitialDir = txtMySQLImport->Text;
  OpenDialog1->FileName   = txtMySQLImport->Text;
  OpenDialog1->Filter     = "MySQL�������Ƴ���(MySQLImport.EXE)|mysqlimport.exe";
  OpenDialog1->Title = "MySQLImport����λ��";
  while(true)
  {
    if(OpenDialog1->Execute())
    {
      TSearchRec sr;
      if(FindFirst(ExtractFilePath(OpenDialog1->FileName) + "mysqlimport.exe",faAnyFile,sr)==0)
      {
        if((sr.Attr&faDirectory) != faDirectory)
        {
          txtMySQLImport->Text = OpenDialog1->FileName;
          FindClose(sr);
          break;
        }
        FindClose(sr);
      }
      else
      {
        MessageBox(Handle,"ָ����λ���Ҳ���mysqlimport.exe,������ѡ��!","��ʾ",MB_OK | MB_ICONWARNING);
      }
    }
    else
      return;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmOption::txtTempDirButtonClick(TObject *Sender)
{
  FolderBrowseDialog * fbd = new FolderBrowseDialog(Handle);
  fbd->Title      = "ѡ����ʱĿ¼";
  fbd->FolderPath = txtTempDir->Text;
  while(true)
  {
    if(fbd->Execute())
    {
      if(fbd->FolderPath.Pos(" ") > 0)
      {
        MessageBox(Handle,
          ("����ǰѡ���Ŀ¼\"" + fbd->FolderPath + "\"�а����пո�."\
          "Ϊ�����ⲿ��������������,��ѡ��һ���������ո����ʱĿ¼λ��!").c_str(),"��ʾ",MB_OK | MB_ICONWARNING);
      }
      else
      {
        if(fbd->FolderPath.SubString(fbd->FolderPath.Length(),1) != "\\")
          txtTempDir->Text = fbd->FolderPath + "\\";
        else
          txtTempDir->Text = fbd->FolderPath;
        break;
      }
    }
    else
      break;
  }
  delete fbd;
}
//---------------------------------------------------------------------------
void __fastcall TfrmOption::btnOkClick(TObject *Sender)
{
  if(txtSortConfirm->Text == "")
    txtSortConfirm->Text = frmMain->SortConfirm;
  else
  {
    try
    {
      int n = StrToInt(txtSortConfirm->Text);
      if(n < 0)
        throw new Exception("");
    }
    catch(...)
    {
      MessageBox(Handle,"����ȷ������������һ������-1������!","��ʾ",MB_OK | MB_ICONWARNING);
      txtSortConfirm->SetFocus();
      txtSortConfirm->SelectAll();
      return;
    }
  }
  if(txtFTPTimeout->Text == "")
    txtFTPTimeout->Text = FTPTimeout;
  try
  {
    int n = StrToInt(txtFTPTimeout->Text);
    if(n < -1)
      throw new Exception("");
  }
  catch(...)
  {
    MessageBox(Handle,"FTP��������ʱʱ�������һ������-1������!","��ʾ",MB_OK | MB_ICONWARNING);
    txtFTPTimeout->SetFocus();
    txtFTPTimeout->SelectAll();
    return;
  }
  try
  {
    int n = StrToInt(txtRetryOpenCount->Text);
    if(n < 0 || n > 999)
      throw new Exception("");
  }
  catch(...)
  {
    MessageBox(Handle,"ʧ�����Դ���������һ��0��999֮�������!","��ʾ",MB_OK | MB_ICONWARNING);
    txtRetryOpenCount->SetFocus();
    txtRetryOpenCount->SelectAll();
    return;
  }
  if(MessageBox(Handle,"�޸ĺ��ѡ�������Ч,�Ƿ�Ҫ����?","����",MB_YESNO | MB_ICONQUESTION) == IDNO)
    return;
  TIniFile * IniFile = NULL;
  try
  {
    IniFile = new TIniFile(ExtractFilePath(Application->ExeName) + "ibill.ini");
    IniFile->WriteString("Global","TempDir",txtTempDir->Text);
    IniFile->WriteString("ExternalTools","bcppath",txtBCP->Text);
    IniFile->WriteString("ExternalTools","sqlldrpath",txtSQLLDR->Text);
    IniFile->WriteString("ExternalTools","mysqlimportpath",txtMySQLImport->Text);
    IniFile->WriteString("Global","SortConfirm",txtSortConfirm->Text);
    IniFile->WriteString("FTP","Timeout",txtFTPTimeout->Text);
    IniFile->WriteString("Global","AutoRetry",txtRetryOpenCount->Text);
    delete IniFile;
    IniFile = NULL;
    frmMain->BCPExePath    = txtBCP->Text;    //BCP�ĳ���·��
    frmMain->SQLLDRExePath = txtSQLLDR->Text; //SQLLDR�ĳ���·��
    frmMain->MySQLImportExePath = txtMySQLImport->Text;
    frmMain->TempDirectory = txtTempDir->Text;
    frmMain->SortConfirm = StrToInt(txtSortConfirm->Text);
    RetryCount   = StrToInt(txtRetryOpenCount->Text);
    FTPTimeout   = StrToInt(txtFTPTimeout->Text);
    MessageBox(Handle,"�µ�ѡ���ѱ���!","��Ϣ",MB_OK | MB_ICONINFORMATION);
    ModalResult = mrOk;
  }
  catch(Exception * E)
  {
    if(IniFile != NULL)
      delete IniFile;
    MessageBox(Handle,E->Message.c_str(),"����",MB_OK | MB_ICONSTOP);
    return;
  }
}
//---------------------------------------------------------------------------
bool __fastcall TfrmOption::FormHelp(WORD Command, int Data,
      bool &CallHelp)
{
    if(Command != HELP_CONTEXTPOPUP)
    {
        CallHelp = false;
        return true;
    }
    WinHelp(Handle,(HelpFilePath + "option.hlp").c_str(),HELP_CONTEXTPOPUP,Data);
    CallHelp = false;
    return true;   
}
//---------------------------------------------------------------------------

