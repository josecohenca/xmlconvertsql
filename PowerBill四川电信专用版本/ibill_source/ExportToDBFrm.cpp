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
//������ģʽ�´ӻ����ļ��б�����ļ�
bool __fastcall TfrmExportToDB::LoadFileList(TStringList * FileList,AnsiString BillName)
{
  try
  {
    bool IsTry = EncryRegisteCode(frmMain->RegistUserName).SubString(1,25) != frmMain->SerialNumber;
    if(FileList->Count == 0)
      throw new Exception("�����б��ļ��ǿյ�\n");
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
          throw new Exception(FileName + " ��ʽ����ȷ");
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
        SaveLog((GetFullFTPFileName(FileList->Strings[n]) + "���ļ�û�б���ӵ��б���,��Ϊ���Ѿ�����ǰ�Ĳ����б����ص����б���").c_str());
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
      SaveLog(("�ļ�[" + GetFullFTPFileName(FileList->Strings[n]) + "]�Ѽ���.").c_str());
      if(IsTry && lvFileList->Items->Count >= TRY_VERSION_FILE_COUNT)
      {
        SaveLog(("���ð�������ֻ��ͬʱ����" + IntToStr(TRY_VERSION_FILE_COUNT) + "���ļ�.").c_str());
        break;
      }            
    }
  }
  catch(Exception * E)
  {
    ShowErrorMessage(Handle,(E->Message + ".\n").c_str(),false,99);
    return false;
  }
  SaveLog(("�ļ��������,��������" + IntToStr(lvFileList->Items->Count) + "���ļ�.").c_str());
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
    MessageBox(Handle,E->Message.c_str(),"����",MB_OK | MB_ICONSTOP);
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
    MessageBox(Handle,"�����뵼�����ݵ�Ŀ�������!","��ʾ",MB_OK | MB_ICONWARNING);
    txtTableName->SetFocus();
    return;
  }
  if(cbxDBType->ItemIndex == 2 && cbxDataBases->Text == "")
  {
    MessageBox(Handle,"��ѡ��һ��Ŀ�����ݿ�!","��ʾ",MB_OK | MB_ICONWARNING);
    cbxDataBases->SetFocus();
    return;
  }
  if(!InCmdMode)
    if(MessageBox(Handle,"��Ҫ��ʼ����ѡ������Щ�ļ���?","����",MB_YESNO | MB_ICONQUESTION) == IDNO)
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
  //ȷ��Ŀ�껰����ʽ����
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
  //���Ŀ����Ƿ��Ѵ���,�������,�򱨴�
  TStringList * Tables = new TStringList;
  try
  {
    ADOConnection1->GetTableNames(Tables);
    if(Tables->IndexOf(txtTableName->Text) > -1)
    {
      throw new Exception("Ŀ���" + txtTableName->Text + "�Ѿ�����,������ָ��һ���±�.");
    }
    ADOConnection1->Close();
  }
  catch(Exception * E)
  {
    delete Tables;
    if(ADOConnection1->Connected)
      ADOConnection1->Close();
    Screen->Cursor = crDefault;
    ShowErrorMessage(Handle,(E->Message + "  ��������ֹ.").c_str(),false,99);
    return false;
  }
  delete Tables;
  txtMessages->Lines->Clear();
  EnableCtrls(false);
  if(TotalRecordCount > 0)
    Label8->Caption = "����" + IntToStr(lvFileList->Items->Count) + "���ļ�(" + IntToStr(RzProgressBar1->TotalParts) + "����¼)��Ҫת��";
  else
    Label8->Caption = "����" + IntToStr(lvFileList->Items->Count) + "���ļ���Ҫת��";
  //��ʼת���ļ�
  DCHeaderPanel1->Visible = true;
  labWarning->Tag = 0;
  labWarning->Caption = "����ת�������ļ�,���Ժ�...";
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
    ShowErrorMessage(Handle,("ת���ļ�ʱ��������:" + ThreadConvertForDB->ErrorMessage + " ��������ֹ.").c_str(),false,99);
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
    SaveLog((Label8->Caption + ",��ת�������" + IntToStr(RecordCount) + "����¼").c_str());
  Label8->Caption = Label8->Caption + ",��ת�������" + IntToStr(RecordCount) + "����¼";
  CreateTableSQL = ThreadConvertForDB->CreateTableSQL;
  RzProgressBar1->TotalParts = RecordCount;
  if(ManualStart)
    delete ThreadConvertForDB;
  ThreadConvertForDB = NULL;
  //����
  try
  {
    if(!SlentMode)
    {
      //FILE * f = fopen("c:\jlj.txt","w");
      //fprintf(f,"%s",CreateTableSQL);
      //fclose(f);
      txtMessages->Lines->Add("���ڽ���Ŀ���:" + CreateTableSQL);
    }
    ADOConnection1->Execute(CreateTableSQL);
  }
  catch(Exception * E)
  {
    if(ADOConnection1->Connected)
      ADOConnection1->Close();
    ShowErrorMessage(Handle,("���Խ����±�" + txtTableName->Text + "ʱ��������:" + E->Message + "  ��������ֹ.").c_str(),false,99);
    if(!ManualStart)
      EnableCtrls(true);
    return false;
  }
  labWarning->Caption = "�����ύ����,���Ժ�...";
  Animate1->Active = true;
  //��������߳�
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
      ("ת���ļ�ʱ��������:" + ThreadConvertForDB->ErrorMessage + " ��������ֹ.").c_str(),
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
  labWarning->Caption = "����δ����";
  RzProgressBar1->Visible = true;
}
void __fastcall TfrmExportToDB::OnSubmiteThreadTerminate(TObject * Sender)
{
  EnableCtrls(true);
  ADOConnection1->Close();
  if(ThreadSubmitToDB->ErrorMessage != "")
  {
    ShowErrorMessage(Handle,("�ύ����ʱ��������:" + ThreadSubmitToDB->ErrorMessage + " ��������ֹ.").c_str(),false,99);
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
      MessageBox(Handle,"�����ύ���!","��Ϣ",MB_OK | MB_ICONINFORMATION);
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
    MessageBox(Handle,"������ֹ�������ݲ���!","��ʾ",MB_OK | MB_ICONWARNING);
    Action = caNone;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmExportToDB::btnTermClick(TObject *Sender)
{
  if(Sender != NULL && MessageBox(Handle,"��Ҫ��ֹ�������е�������?","����",MB_YESNO|MB_ICONWARNING) == IDNO)
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
  ShowErrorMessage(Handle,"�������û���ֹ",false,98);
  DCHeaderPanel1->Visible = false;
  EnableCtrls(true);  
}
//---------------------------------------------------------------------------
void __fastcall TfrmExportToDB::OnSetProcessMaxCount(TMessage Message)
{
  if(ThreadSubmitToDB == NULL)
  {
    labWarning->Caption = "����ת���ļ�" + AnsiString((const char *)Message.LParam) + "  " + IntToStr(txtMessages->Lines->Count) + "/" + lvFileList->Items->Count;
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
      ShowErrorMessage(Handle,"����Ҫ����SQLLDR.EXE���ڵ�·�������ʹ�����ݵ��뵽Oracle!",false,99);
      return false;
    }
    DBType = DB_ORACLE;
    break;
  case 1:
    ToolPath = frmMain->BCPExePath;
    if(ToolPath == "" || !FileExists(ToolPath))
    {
      ShowErrorMessage(Handle,"����Ҫ����BCP.EXE���ڵ�·�������ʹ�����ݵ��뵽SQLServer!",false,99);
      return false;
    }
    DBType = DB_SQLSERVER;
    break;
  case 2:
    ToolPath = frmMain->MySQLImportExePath;
    if(ToolPath == "" || !FileExists(ToolPath))
    {
      ShowErrorMessage(Handle,"����Ҫ����MySQLImport.EXE���ڵ�·�������ʹ�����ݵ��뵽MySQL!",false,99);
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
      (E->Message + "  ��ȷ���Ѱ�װ����ѡ���ݿ���������򲢼�����ݿ���������û����������Ƿ�������ȷ.").c_str(),
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
    Label5->Caption = "TNS����";
  }
  else
  {
    Label5->Caption = "��������ַ";
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
    MessageBox(Handle,E->Message.c_str(),"����",MB_OK | MB_ICONSTOP);
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

