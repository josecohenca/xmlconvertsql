//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ExportToDBFrm.h"
#include "MainFrm.h"
#include "ViewBillFrm.h"
#include "DBConfigFrm.h"
#include "public.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzCmboBx"
#pragma link "RzEdit"
#pragma link "RzListVw"
#pragma link "RzButton"
#pragma link "RzRadChk"
#pragma link "RzShellCtrls"
#pragma link "RzFilSys"
#pragma link "RzPrgres"
#pragma link "dcOutPanel"
#pragma resource "*.dfm"
TfrmExportToDB *frmExportToDB;
//---------------------------------------------------------------------------
__fastcall TfrmExportToDB::TfrmExportToDB(TComponent* Owner)
  : TForm(Owner)
{
  ThreadConvertForDB   = NULL;
  MessageTextList      = NULL;
  FileList             = new TStringList;
  BillNameList         = new TStringList;
  SQLCommandList       = new TStringList;
  cbxDBType->ItemIndex = 0;
  chDriver = 0;
  IsSearchResult = false;
  TermByUser     = false;
}
//---------------------------------------------------------------------------
//命令行模式下从话单文件列表加载文件
bool __fastcall TfrmExportToDB::LoadFileList(TStringList * FileList,AnsiString BillName)
{
  try
  {
    bool IsTry = EncryRegisteCode(frmMain->RegistUserName).SubString(1,25) != frmMain->SerialNumber;
    if(FileList->Count == 0)
      throw new Exception("话单列表文件是空的\n");
    int FileFormatType     = frmMain->BillConfig->GetFileFormatType(BillName);
    AnsiString FileFormatTypeName = frmMain->BillConfig->TransFormatTypeName(FileFormatType);
    AnsiString FileName;
    AnsiString FilePath;
    int pos;
    TListItem * Item;
    bool FileExistsInList;
    for(int n = 0;n < FileList->Count;n++)
    {
      FileName = FileList->Strings[n];
      if(FileName.SubString(1,6).LowerCase() == "ftp://")
      {
        pos = StrRScan(FileName.c_str(),"|",-1);
        if(pos < 0)
          throw new Exception(FileName + " 格式不正确");
        FilePath = FileName.SubString(1,pos);
        FileName = FileName.SubString(pos + 2,FileName.Length() - pos);
      }
      else
      {
        FilePath = ExtractFilePath(FileName);
        FileName = ExtractFileName(FileName);
      }
      Item = lvFileList->FindCaption(-1,FileName,false,false,false);
      FileExistsInList = false;
      while(Item != NULL)
      {
        if(Item != NULL && Item->SubItems->Strings[5] == FilePath)
        {
          FileExistsInList = true;
          break;
        }
        Item = lvFileList->FindCaption(Item->Index,FileName,false,false,false);
      }
      if(FileExistsInList)
      {
        SaveLog((GetFullFTPFileName(FileList->Strings[n]) + "该文件没有被添加到列表中,因为它已经在先前的操作中被加载到了列表中").c_str());
        continue;
      }
      Item = lvFileList->Items->Add();
      Item->ImageIndex = 95;
      Item->Caption = FileName;
      Item->SubItems->Add(FileFormatTypeName);
      Item->SubItems->Add(BillName);
      Item->SubItems->Add("0");
      Item->SubItems->Add("-1");
      Item->SubItems->Add("");
      Item->SubItems->Add(FilePath);
      SaveLog(("文件[" + GetFullFTPFileName(FileList->Strings[n]) + "]已加载.").c_str());
      if(IsTry && lvFileList->Items->Count >= TRY_VERSION_FILE_COUNT)
      {
        SaveLog(("试用版软件最多只能同时加载" + IntToStr(TRY_VERSION_FILE_COUNT) + "个文件.").c_str());
        break;
      }            
    }
  }
  catch(Exception * E)
  {
    ShowErrorMessage(Handle,(E->Message + ".\n").c_str(),false,99);
    return false;
  }
  SaveLog(("文件加载完成,共加载了" + IntToStr(lvFileList->Items->Count) + "个文件.").c_str());
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmExportToDB::menuViewBillClick(TObject *Sender)
{
  TListItem * Item = lvFileList->Selected;
  if(Item == NULL)
    return;
  TfrmViewBill * frmViewBill = new TfrmViewBill(this,frmMain->BillConfig);
  try
  {
    if(IsSearchResult)
      frmViewBill->OpenBillFile(SR_BILL_FORMAT_STR,"",Item->SubItems->Strings[5],Item->Caption,(unsigned int)Item->Data);    
    else
      frmViewBill->OpenBillFile(Item->SubItems->Strings[0],Item->SubItems->Strings[1],Item->SubItems->Strings[5],Item->Caption,(unsigned int)Item->Data);
    frmViewBill->ShowModal();
  }
  catch(Exception * E)
  {
    MessageBox(Handle,E->Message.c_str(),"错误",MB_OK | MB_ICONSTOP);
  }
  delete frmViewBill;
}
void __fastcall TfrmExportToDB::OnThreadCompleted(TMessage Message)
{
  if(Message.WParam == (int)ThreadConvertForDB)
  {
    OnConvertThreadTerminate(NULL);
  }
  else
  {
    OnSubmiteThreadTerminate(NULL);
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmExportToDB::menuDeleteClick(TObject *Sender)
{
  TListItem * Item;
  for(int n = lvFileList->Items->Count - 1;n > -1;n--)
  {
    Item = lvFileList->Items->Item[n];
    if(Item->Selected)
      lvFileList->Items->Delete(Item->Index);
  }   
}
//---------------------------------------------------------------------------
void __fastcall TfrmExportToDB::btnStartClick(TObject *Sender)
{
  if(ThreadConvertForDB != NULL||lvFileList->Items->Count == 0)
    return;
  if(txtTableName->Text == "")
  {
    MessageBox(Handle,"请输入导出数据的目标表名称!","提示",MB_OK | MB_ICONWARNING);
    txtTableName->SetFocus();
    return;
  }
  if(cbxDBType->ItemIndex == 2 && cbxDataBases->Text == "")
  {
    MessageBox(Handle,"请选择一个目标数据库!","提示",MB_OK | MB_ICONWARNING);
    cbxDataBases->SetFocus();
    return;
  }
  if(!InCmdMode)
    if(MessageBox(Handle,"您要开始导出选定的这些文件吗?","问题",MB_YESNO | MB_ICONQUESTION) == IDNO)
      return;
  if(!StartConvert(false))
    return;
  ThreadConvertForDB->Suspended  = false;
}
bool __fastcall TfrmExportToDB::StartConvert(bool ManualStart)
{
  if(!IsNumber(txtMaxError->Text.c_str()))
    txtMaxError->Text = "0";
  TempFileName = frmMain->TempDirectory + txtTableName->Text + ".tmp";
  DeleteFile(frmMain->TempDirectory +  txtTableName->Text + ".bad");
  DeleteFile(frmMain->TempDirectory +  txtTableName->Text + ".log");
  //确定目标话单格式类型
  FileList->Clear();
  BillNameList->Clear();
  TListItem * Item;
  DestFileFormat = 0;
  AnsiString DestBillTypeName = "";
  int BillFormatType;
  RzProgressBar1->PartsComplete = 0;
  RzProgressBar1->TotalParts = 0;
  Screen->Cursor = crHourGlass;
  int RecCount;
  int TotalRecordCount = 0;
  for(int n = 0;n < lvFileList->Items->Count;n++)
  {
    Item = lvFileList->Items->Item[n];
    RecCount = StrToInt(Item->SubItems->Strings[3]);
    if(Item->SubItems->Strings[5].SubString(1,6).LowerCase() == "ftp://")
      FileList->AddObject(Item->SubItems->Strings[5] + "|" + Item->Caption,(TObject *)RecCount);
    else
      FileList->AddObject(Item->SubItems->Strings[5] + Item->Caption,(TObject *)RecCount);
    if(IsSearchResult)
    {
      BillNameList->Add(SR_BILL_FORMAT_STR);
      DestFileFormat = SEARCH_RESULT_FORMAT;
    }
    else
    {
      BillNameList->Add(Item->SubItems->Strings[1]);
      BillFormatType = frmMain->BillConfig->TransFormatType(Item->SubItems->Strings[0]);
      if(DestFileFormat == 0)
        DestFileFormat = BillFormatType;
    }
    /*else if(DestFileFormat != STD_BILL_FORMAT)
    {
      if(DestFileFormat != BillFormatType)
        DestFileFormat = STD_BILL_FORMAT;
      else if(DestBillTypeName == "")
        DestBillTypeName = Item->SubItems->Strings[1];
      else if(DestBillTypeName != Item->SubItems->Strings[1])
      {
        DestFileFormat = STD_BILL_FORMAT;
      }
    }*/
    if(RecCount > -1)
      TotalRecordCount += RecCount;
    else
      TotalRecordCount = 0;
  }
  if(TotalRecordCount > 0)
  {
    RzProgressBar1->TotalParts = TotalRecordCount;
    RzProgressBar1->Tag = 0;
  }
  else
  {
    RzProgressBar1->Tag = 1;
    RzProgressBar1->TotalParts = lvFileList->Items->Count;
  }

  if(!OpenDBConnection())
  {
    Screen->Cursor = crDefault;
    if(!InCmdMode)
    {
#ifdef SCTELE_COM_VERSION
      ShellExecute(NULL,"open","hh.exe",(ExtractFilePath(Application->ExeName) + "Help\\ibillsc.chm::/exttools.htm").c_str(),"",SW_NORMAL);
#else
      ShellExecute(NULL,"open","hh.exe",(ExtractFilePath(Application->ExeName) + "Help\\ibill.chm::/exttools.htm").c_str(),"",SW_NORMAL);
#endif    
    }
    return false;
  }
  //检查目标表是否已存在,如果存在,则报错
  TStringList * Tables = new TStringList;
  try
  {
    ADOConnection1->GetTableNames(Tables);
    if(Tables->IndexOf(txtTableName->Text) > -1)
    {
      throw new Exception("目标表" + txtTableName->Text + "已经存在,您必须指定一个新表.");
    }
    ADOConnection1->Close();
  }
  catch(Exception * E)
  {
    delete Tables;
    if(ADOConnection1->Connected)
      ADOConnection1->Close();
    Screen->Cursor = crDefault;
    ShowErrorMessage(Handle,(E->Message + "  操作已中止.").c_str(),false,99);
    return false;
  }
  delete Tables;
  txtMessages->Lines->Clear();
  EnableCtrls(false);
  if(TotalRecordCount > 0)
    Label8->Caption = "共有" + IntToStr(lvFileList->Items->Count) + "个文件(" + IntToStr(RzProgressBar1->TotalParts) + "条记录)需要转换";
  else
    Label8->Caption = "共有" + IntToStr(lvFileList->Items->Count) + "个文件需要转换";
  //开始转换文件
  DCHeaderPanel1->Visible = true;
  labWarning->Tag = 0;
  labWarning->Caption = "正在转换话单文件,请稍候...";
  ThreadConvertForDB = new TThreadConvertForDB(true);
  ThreadConvertForDB->FreeOnTerminate = true;
  ThreadConvertForDB->BillFileList = FileList;
  ThreadConvertForDB->BillNameList = BillNameList;
  if(ManualStart)
  {
    ThreadConvertForDB->hWnd = NULL;
    MessageTextList = new TStringList;
    ThreadConvertForDB->MessageTextList = MessageTextList;
  }
  else
  {
    ThreadConvertForDB->hWnd = Handle;
    ThreadConvertForDB->MessageTextList = txtMessages->Lines;
  }
  ThreadConvertForDB->TempDirectory = frmMain->TempDirectory;
  ThreadConvertForDB->DestFileFormat = DestFileFormat;
  ThreadConvertForDB->DestDBType = DBType;
  ThreadConvertForDB->BillConfig = frmMain->BillConfig;
  ThreadConvertForDB->DestTableName = txtTableName->Text;
  ThreadConvertForDB->TransFieldValue = cbxTransFieldValue->Checked;
  ThreadConvertForDB->SQLCommandList = SQLCommandList;
  ThreadConvertForDB->BillName = lvFileList->Items->Item[0]->SubItems->Strings[1];
  ThreadConvertForDB->TempFileName = TempFileName;
  Screen->Cursor = crDefault;
  return true;
}
//---------------------------------------------------------------------------
bool __fastcall TfrmExportToDB::ManualStartJob()
{
  if(!StartConvert(true))
    return false;
  ThreadConvertForDB->FreeOnTerminate = false;
  ThreadConvertForDB->Suspended  = false;
  WaitForSingleObject((void *)ThreadConvertForDB->Handle,INFINITE);
  if(ThreadConvertForDB->ErrorMessage != "")
  {
    ShowErrorMessage(Handle,("转换文件时发生错误:" + ThreadConvertForDB->ErrorMessage + " 操作已中止.").c_str(),false,99);
    ThreadConvertForDB = NULL;
    return false;
  }
  StartImport(true);
  ThreadSubmitToDB->FreeOnTerminate = false;
  ThreadSubmitToDB->Suspended  = false;
  WaitForSingleObject((void *)ThreadSubmitToDB->Handle,INFINITE);
  bool Succeed = ThreadSubmitToDB->ErrorMessage == "";
  TThreadSubmitToDB * AThreadSubmitToDB = ThreadSubmitToDB;
  OnSubmiteThreadTerminate(NULL);
  delete AThreadSubmitToDB;
  return Succeed;
}
//---------------------------------------------------------------------------
bool __fastcall TfrmExportToDB::StartImport(bool ManualStart)
{
  RecordCount = ThreadConvertForDB->TotalRecordCount;
  if(InCmdMode)
    SaveLog((Label8->Caption + ",共转换完成了" + IntToStr(RecordCount) + "条记录").c_str());
  Label8->Caption = Label8->Caption + ",共转换完成了" + IntToStr(RecordCount) + "条记录";
  CreateTableSQL = ThreadConvertForDB->CreateTableSQL;
  RzProgressBar1->TotalParts = RecordCount;
  if(ManualStart)
    delete ThreadConvertForDB;
  ThreadConvertForDB = NULL;
  //建表
  try
  {
    if(!SlentMode)
    {
      //FILE * f = fopen("c:\jlj.txt","w");
      //fprintf(f,"%s",CreateTableSQL);
      //fclose(f);
      txtMessages->Lines->Add("正在建立目标表:" + CreateTableSQL);
    }
    ADOConnection1->Execute(CreateTableSQL);
  }
  catch(Exception * E)
  {
    if(ADOConnection1->Connected)
      ADOConnection1->Close();
    ShowErrorMessage(Handle,("尝试建立新表" + txtTableName->Text + "时发生错误:" + E->Message + "  操作已中止.").c_str(),false,99);
    if(!ManualStart)
      EnableCtrls(true);
    return false;
  }
  labWarning->Caption = "正在提交数据,请稍候...";
  Animate1->Active = true;
  //启动入库线程
  RzProgressBar1->PartsComplete = 0;
  RzProgressBar1->Tag = 0;
  ThreadSubmitToDB = new TThreadSubmitToDB(true);
  ThreadSubmitToDB->TempFileName = TempFileName;
  ThreadSubmitToDB->DBType = DBType;
  ThreadSubmitToDB->SQLCommandList = SQLCommandList;
  ThreadSubmitToDB->FreeOnTerminate = true;
  ThreadSubmitToDB->DBServer    = txtDBServer->Text;
  ThreadSubmitToDB->DBUserName  = txtDBUserName->Text;
  ThreadSubmitToDB->DBPassword  = txtDBPassword->Text;
  ThreadSubmitToDB->WorkDir     = ExtractFilePath(Application->ExeName);

  ThreadSubmitToDB->TotalRecordCount = RecordCount;
  ThreadSubmitToDB->ADOConnection = ADOConnection1;
  if(ManualStart)
  {
    ThreadSubmitToDB->hWnd = NULL;
    MessageTextList->Clear();
    ThreadSubmitToDB->MessageTextList = MessageTextList;
  }
  else
  {
    ThreadSubmitToDB->hWnd = Handle;
    txtMessages->Lines->Clear();
    ThreadSubmitToDB->MessageTextList = txtMessages->Lines;
  }
  ThreadSubmitToDB->ToolPath = ToolPath;
  ThreadSubmitToDB->DestTableName = txtTableName->Text;
  ThreadSubmitToDB->DataBaseName  = cbxDataBases->Text;
  ThreadSubmitToDB->ServerPort = txtDBPort->Text;
  ThreadSubmitToDB->MaxError   = txtMaxError->Text;
  ThreadSubmitToDB->MaxMessageLine = 1000;
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmExportToDB::OnConvertThreadTerminate(TObject * Sender)
{
  if(ThreadConvertForDB->ErrorMessage != "")
  {
    ShowErrorMessage(Handle,
      ("转换文件时发生错误:" + ThreadConvertForDB->ErrorMessage + " 操作已中止.").c_str(),
      false,99);
    ThreadConvertForDB = NULL;
    EnableCtrls(true);
    return;
  }
  if(StartImport(false))
    ThreadSubmitToDB->Suspended  = false;
}
void __fastcall TfrmExportToDB::EnableCtrls(bool Enabled)
{
  ExportCompleted         = Enabled;
  btnStart->Enabled       = Enabled;
  cbxDBType->Enabled      = Enabled;
  txtDBServer->Enabled    = Enabled;
  txtDBUserName->Enabled  = Enabled;
  txtDBPassword->Enabled  = Enabled;
  txtTableName->Enabled   = Enabled;
  cbxTransFieldValue->Enabled  = Enabled;
  lvFileList->Enabled  = Enabled;
  Label1->Enabled   = Enabled;
  Label5->Enabled   = Enabled;
  Label2->Enabled   = Enabled;
  Label3->Enabled   = Enabled;
  Label4->Enabled   = Enabled;
  Label11->Enabled  = Enabled;
  btnTerm->Enabled  = !Enabled;
  cbxDataBases->Enabled = Enabled && cbxDBType->ItemIndex > 0;
  Label10->Enabled      = Enabled && cbxDBType->ItemIndex == 2;
  txtDBPort->Enabled    = Label10->Enabled;
  txtMaxError->Enabled  = cbxDBType->ItemIndex != 2 && Enabled;
  Label12->Enabled      = txtMaxError->Enabled;
  Label9->Enabled = cbxDataBases->Enabled;
  Animate1->Active = false;
  if(Enabled)
    Label8->Caption = "";
  labWarning->Caption = "任务未启动";
  RzProgressBar1->Visible = true;
}
void __fastcall TfrmExportToDB::OnSubmiteThreadTerminate(TObject * Sender)
{
  EnableCtrls(true);
  ADOConnection1->Close();
  if(ThreadSubmitToDB->ErrorMessage != "")
  {
    ShowErrorMessage(Handle,("提交数据时发生错误:" + ThreadSubmitToDB->ErrorMessage + " 操作已终止.").c_str(),false,99);
    try
    {
      ADOConnection1->Execute("DROP TABLE " + txtTableName->Text);
    }
    catch(...)
    {
    }
    ThreadSubmitToDB = NULL;
  }
  else
  {
    if(!InCmdMode)//SlentMode)
      MessageBox(Handle,"数据提交完成!","信息",MB_OK | MB_ICONINFORMATION);
  }
  ThreadSubmitToDB = NULL;
  DeleteFile(TempFileName);
  DeleteFile(ExtractFilePath(TempFileName) + txtTableName->Text + ".fmt");
  if(MessageTextList == NULL)
    SaveLog(txtMessages->Text.c_str());
  else
    SaveLog(MessageTextList->Text.c_str());
  if(InCmdMode)
    Close();
}
void __fastcall TfrmExportToDB::FormDestroy(TObject *Sender)
{
  if(FileList != NULL)
  {
    delete FileList;
    FileList = NULL;
  }
  if(SQLCommandList != NULL)
  {
    delete SQLCommandList;
    SQLCommandList = NULL;
  }
  if(MessageTextList != NULL)
  {
    delete MessageTextList;
    MessageTextList = NULL;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmExportToDB::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  if(ThreadSubmitToDB != NULL || ThreadConvertForDB != NULL)
  {
    MessageBox(Handle,"请先终止导出数据操作!","提示",MB_OK | MB_ICONWARNING);
    Action = caNone;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmExportToDB::btnTermClick(TObject *Sender)
{
  if(Sender != NULL && MessageBox(Handle,"您要终止正在运行的任务吗?","警告",MB_YESNO|MB_ICONWARNING) == IDNO)
    return;
  if(ThreadConvertForDB != NULL)
  {
    ThreadConvertForDB->Suspended = true;
    if(ThreadConvertForDB->file != NULL)
    {
      fclose(ThreadConvertForDB->file);
    }
    if(ThreadConvertForDB->BillFile != NULL)
    {
      ThreadConvertForDB->BillFile->TermCurOperation();
      delete ThreadConvertForDB->BillFile;
    }
    TerminateThread((void *)ThreadConvertForDB->Handle,0);
    ThreadConvertForDB = NULL;
  }
  else if(ThreadSubmitToDB != NULL)
  {
    if(ThreadSubmitToDB->pi.hProcess != NULL)
    {
      TerminateProcess(ThreadSubmitToDB->pi.hProcess,0);
      CloseHandle(ThreadSubmitToDB->pi.hProcess);
      CloseHandle(ThreadSubmitToDB->pi.hThread);
      if(ThreadSubmitToDB->hWritePipe != NULL)
        CloseHandle(ThreadSubmitToDB->hWritePipe);
      if(ThreadSubmitToDB->hReadPipe != NULL)
        CloseHandle(ThreadSubmitToDB->hReadPipe);
    }
    TerminateThread((void *)ThreadSubmitToDB->Handle,0);
    ThreadSubmitToDB = NULL;
  }
  ShowErrorMessage(Handle,"操作被用户终止",false,98);
  DCHeaderPanel1->Visible = false;
  EnableCtrls(true);  
}
//---------------------------------------------------------------------------
void __fastcall TfrmExportToDB::OnSetProcessMaxCount(TMessage Message)
{
  if(ThreadSubmitToDB == NULL)
  {
    labWarning->Caption = "正在转换文件" + AnsiString((const char *)Message.LParam) + "  " + IntToStr(txtMessages->Lines->Count) + "/" + lvFileList->Items->Count;
  }
  RzProgressBar1->TotalParts = Message.WParam;
  RzProgressBar1->PartsComplete = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmExportToDB::OnSetProcessPos(TMessage Message)
{
  if(RzProgressBar1->PartsComplete <= Message.WParam)
  {
    if(RzProgressBar1->Tag == 0)
    {
      RzProgressBar1->PartsComplete = Message.WParam;
    }
    else
    {
      RzProgressBar1->PartsComplete += 1;
    }
  }
}

bool __fastcall TfrmExportToDB::OpenDBConnection()
{
  if(ADOConnection1->Connected)
    ADOConnection1->Close();
  switch(cbxDBType->ItemIndex)
  {
  case 0:
    ToolPath = frmMain->SQLLDRExePath;
    if(ToolPath == "" || !FileExists(ToolPath))
    {
      ShowErrorMessage(Handle,"您需要配置SQLLDR.EXE所在的路径后才能使将数据导入到Oracle!",false,99);
      return false;
    }
    DBType = DB_ORACLE;
    break;
  case 1:
    ToolPath = frmMain->BCPExePath;
    if(ToolPath == "" || !FileExists(ToolPath))
    {
      ShowErrorMessage(Handle,"您需要配置BCP.EXE所在的路径后才能使将数据导入到SQLServer!",false,99);
      return false;
    }
    DBType = DB_SQLSERVER;
    break;
  case 2:
    ToolPath = frmMain->MySQLImportExePath;
    if(ToolPath == "" || !FileExists(ToolPath))
    {
      ShowErrorMessage(Handle,"您需要配置MySQLImport.EXE所在的路径后才能使将数据导入到MySQL!",false,99);
      return false;
    }
    DBType = DB_MYSQL;
    if(!IsNumber(txtDBPort->Text.c_str()))
      txtDBPort->Text = "3306";
    break;
  default:
    return false;
  }
  ADOConnection1->ConnectionString =
    frmMain->DBConfig->GetDBConnectionString(DBType,txtDBServer->Text,txtDBUserName->Text,
      txtDBPassword->Text,cbxDataBases->Text,txtDBPort->Text);
  try
  {
    ADOConnection1->Open();
    return true;
  }
  catch(Exception * E)
  {
    ShowErrorMessage(Handle,
      (E->Message + "  请确认已安装了所选数据库的驱动程序并检查数据库服务器、用户名、密码是否设置正确.").c_str(),
      false,99);
    return false;
  }
}
//---------------------------------------------------------------------------


void __fastcall TfrmExportToDB::cbxDBTypeChange(TObject *Sender)
{
  cbxDataBases->Enabled = cbxDBType->ItemIndex > 0;
  if(cbxDBType->ItemIndex == 0)
  {
    Label5->Caption = "TNS名称";
  }
  else
  {
    Label5->Caption = "服务器地址";
  }
  Label9->Enabled       = cbxDataBases->Enabled;
  Label10->Enabled      = cbxDBType->ItemIndex == 2;
  txtDBPort->Enabled    = Label10->Enabled;
  txtMaxError->Enabled  = cbxDBType->ItemIndex !=2;
  Label12->Enabled      = txtMaxError->Enabled;
  cbxDataBases->Items->Clear();  
}
//---------------------------------------------------------------------------

void __fastcall TfrmExportToDB::cbxDataBasesDropDown(TObject *Sender)
{
  if(cbxDBType->ItemIndex == 0)
    return;
  Screen->Cursor = crHourGlass;
  AnsiString Text = cbxDataBases->Text;
  cbxDataBases->Items->Clear();
  if(!OpenDBConnection())
  {
    Screen->Cursor = crDefault;
    return;
  }
  if(ADODataSet1->Active)
    ADODataSet1->Close();
  switch(cbxDBType->ItemIndex)
  {
  case 1:
    ADODataSet1->CommandText = "SELECT name FROM master..sysdatabases ORDER BY name";
    break;
  case 2:
    ADODataSet1->CommandText = "show databases";
    break;
  }
  try
  {
    ADODataSet1->Open();
    while(!ADODataSet1->Eof)
    {
      cbxDataBases->Items->Add(ADODataSet1->Fields->Fields[0]->AsString);
      ADODataSet1->Next();  
    }
    ADODataSet1->Close();
    ADOConnection1->Close();
  }
  catch(Exception * E)
  {
    Screen->Cursor = crDefault;
    ADOConnection1->Close();
    MessageBox(Handle,E->Message.c_str(),"警告",MB_OK | MB_ICONSTOP);
  }
  cbxDataBases->ItemIndex = cbxDataBases->Items->IndexOf(Text);
  Screen->Cursor = crDefault;  
}


void __fastcall TfrmExportToDB::Label11Click(TObject *Sender)
{
  TfrmDBConfig * frmDBConfig = new TfrmDBConfig(this,true);
  if(frmDBConfig->ShowModal() == mrOk)
  {
    TListItem * Item = frmDBConfig->lvDBConnections->Selected;
    cbxDBType->ItemIndex =
      cbxDBType->Items->IndexOf(Item->SubItems->Strings[1]);
    txtDBServer->Text = Item->SubItems->Strings[2];
    txtDBUserName->Text = Item->SubItems->Strings[5];
    txtDBPassword->Text = frmMain->DBConfig->GetDBAttributeValue(Item->SubItems->Strings[0],"Password",true);
    cbxDBTypeChange(NULL);
    cbxDataBases->Items->Clear();
    cbxDataBases->Items->Add(Item->SubItems->Strings[3]);
    cbxDataBases->ItemIndex = 0;
    txtDBPort->Text = Item->SubItems->Strings[4];
  }
  delete frmDBConfig;  
}
//---------------------------------------------------------------------------

void __fastcall TfrmExportToDB::DCHeaderPanel1RightButtonClick(
      TObject *Sender)
{
  DCHeaderPanel1->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmExportToDB::Label7Click(TObject *Sender)
{
  DCHeaderPanel1->Visible = true;  
}
//---------------------------------------------------------------------------

bool __fastcall TfrmExportToDB::FormHelp(WORD Command, int Data,
      bool &CallHelp)
{
    if(Command != HELP_CONTEXTPOPUP)
    {
        CallHelp = false;
        return true;
    }
    WinHelp(Handle,(HelpFilePath + "exporttodb.hlp").c_str(),HELP_CONTEXTPOPUP,Data);
    CallHelp = false;
    return true;   
}
//---------------------------------------------------------------------------

