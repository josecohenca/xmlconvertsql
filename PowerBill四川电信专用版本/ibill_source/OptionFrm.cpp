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
  txtBCP->Text    = frmMain->BCPExePath;    //BCP的程序路径
  txtSQLLDR->Text = frmMain->SQLLDRExePath; //SQLLDR的程序路径
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
  OpenDialog1->Filter     = "SQLServer批量复制程序(BCP.EXE)|bcp.exe";
  OpenDialog1->Title = "BCP工具位置";
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
        MessageBox(Handle,"指定的位置找不到bcp.exe,请重新选择!","提示",MB_OK | MB_ICONWARNING);
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
  OpenDialog1->Filter     = "Oracle批量复制程序(SQLLDR.EXE)|SQLLDR.exe";
  OpenDialog1->Title = "SQLLDR工具位置";
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
        MessageBox(Handle,"指定的位置找不到sqlldr.exe,请重新选择!","提示",MB_OK | MB_ICONWARNING);
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
  OpenDialog1->Filter     = "MySQL批量复制程序(MySQLImport.EXE)|mysqlimport.exe";
  OpenDialog1->Title = "MySQLImport工具位置";
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
        MessageBox(Handle,"指定的位置找不到mysqlimport.exe,请重新选择!","提示",MB_OK | MB_ICONWARNING);
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
  fbd->Title      = "选择临时目录";
  fbd->FolderPath = txtTempDir->Text;
  while(true)
  {
    if(fbd->Execute())
    {
      if(fbd->FolderPath.Pos(" ") > 0)
      {
        MessageBox(Handle,
          ("您当前选择的目录\"" + fbd->FolderPath + "\"中包含有空格."\
          "为了让外部工具能正常工作,请选择一个不包含空格的临时目录位置!").c_str(),"提示",MB_OK | MB_ICONWARNING);
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
      MessageBox(Handle,"排序确认行数必须是一个大于-1的整数!","提示",MB_OK | MB_ICONWARNING);
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
    MessageBox(Handle,"FTP服务器超时时间必须是一个大于-1的整数!","提示",MB_OK | MB_ICONWARNING);
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
    MessageBox(Handle,"失败重试次数必须是一个0到999之间的整数!","提示",MB_OK | MB_ICONWARNING);
    txtRetryOpenCount->SetFocus();
    txtRetryOpenCount->SelectAll();
    return;
  }
  if(MessageBox(Handle,"修改后的选项将立即生效,是否要保存?","问题",MB_YESNO | MB_ICONQUESTION) == IDNO)
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
    frmMain->BCPExePath    = txtBCP->Text;    //BCP的程序路径
    frmMain->SQLLDRExePath = txtSQLLDR->Text; //SQLLDR的程序路径
    frmMain->MySQLImportExePath = txtMySQLImport->Text;
    frmMain->TempDirectory = txtTempDir->Text;
    frmMain->SortConfirm = StrToInt(txtSortConfirm->Text);
    RetryCount   = StrToInt(txtRetryOpenCount->Text);
    FTPTimeout   = StrToInt(txtFTPTimeout->Text);
    MessageBox(Handle,"新的选项已保存!","信息",MB_OK | MB_ICONINFORMATION);
    ModalResult = mrOk;
  }
  catch(Exception * E)
  {
    if(IniFile != NULL)
      delete IniFile;
    MessageBox(Handle,E->Message.c_str(),"错误",MB_OK | MB_ICONSTOP);
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

