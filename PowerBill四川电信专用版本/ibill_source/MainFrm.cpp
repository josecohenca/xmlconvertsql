//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainFrm.h"
#include <stdio.h>
#include <Math.hpp>
#include "StrUtils.hpp"
#include "OptionFrm.h"
#include "RegisterFrm.h"

#ifdef SCTELE_COM_VERSION

#include "cpuinfo.h"

#endif

#include "IniFiles.hpp"
#include "DBConfigFrm.h"
#include "BillConfigFrm.h"
#include "BillFileFrm.h"
#include "FTPConfigFrm.h"
#include "AboutFrm.h"
#include "ViewBillFrm.h"
#include "SelBillNameFrm.h"
//#include "Registry.hpp"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzListVw"
#pragma link "RzPanel"
#pragma link "RzTabs"
#pragma link "RzGroupBar"
#pragma link "CWGIFImage"
#pragma link "RzPrgres"
#pragma link "RzStatus"
#pragma link "CWGIFImage"
#pragma resource "*.dfm"
TfrmMain *frmMain;
AnsiString HelpFilePath;
//---------------------------------------------------------------------------
#ifdef SCTELE_COM_VERSION
void GetWmiInfo(TStrings *lpList, WideString wsClass)
{
  IWbemLocator *pWbemLocator = NULL;
  if(CoCreateInstance(CLSID_WbemAdministrativeLocator, NULL, CLSCTX_INPROC_SERVER|CLSCTX_LOCAL_SERVER, IID_IUnknown, (void**)&pWbemLocator) == S_OK)
   {
     IWbemServices *pWbemServices = NULL;
     WideString wsNamespace = (L"root\\cimv2");
     if(pWbemLocator->ConnectServer(wsNamespace, NULL, NULL, NULL, 0, NULL, NULL, &pWbemServices) == S_OK)
      {
        IEnumWbemClassObject *pEnumClassObject = NULL;
        WideString wsWQL=L"WQL", wsQuery=WideString(L"Select * from ")+wsClass;
        if(pWbemServices->ExecQuery(wsWQL, wsQuery, WBEM_FLAG_RETURN_IMMEDIATELY,NULL, &pEnumClassObject) == S_OK)
         {
           IWbemClassObject *pClassObject = NULL;
           ULONG uCount = 1, uReturned;
           if(pEnumClassObject->Reset() == S_OK)
            {
              int iEnumIdx = 0;
              while(pEnumClassObject->Next(WBEM_INFINITE, uCount, &pClassObject, &uReturned) == S_OK)
               {
                 lpList->Add("---------------- ["+IntToStr(iEnumIdx)+"] -----------------");

                 SAFEARRAY *pvNames = NULL;
                 if(pClassObject->GetNames(NULL, WBEM_FLAG_ALWAYS | WBEM_MASK_CONDITION_ORIGIN, NULL, &pvNames) == S_OK)
                  {
                    long vbl, vbu;
                    SafeArrayGetLBound(pvNames, 1, &vbl);
                    SafeArrayGetUBound(pvNames, 1, &vbu);
                    for(long idx=vbl; idx<=vbu; idx++)
                     {
                       long aidx = idx;
                       wchar_t *wsName = 0;
                       VARIANT vValue;
                       VariantInit(&vValue);
                       SafeArrayGetElement(pvNames, &aidx, &wsName);

                       BSTR bs = SysAllocString(wsName);
                       HRESULT hRes = pClassObject->Get(bs, 0, &vValue, NULL, 0);
                       SysFreeString(bs);

                       if(hRes == S_OK)
                        {
                          AnsiString s;
                          Variant v = *(Variant*)&vValue;
                          if(v.IsArray())
                           {
                             for(int i=v.ArrayLowBound(); i<=v.ArrayHighBound(); i++)
                              {
                                Variant a = v.GetElement(i);
                                if(!s.IsEmpty())
                                  s+=", ";
                                s+=VarToStr(a);
                              }
                           }
                          else
                           {
                             s = VarToStr(v);
                           }
                          lpList->Add(AnsiString(wsName)+"="+s);
                        }

                       VariantClear(&vValue);
                       SysFreeString(wsName);
                     }
                  }
                 if(pvNames)SafeArrayDestroy(pvNames);
                 iEnumIdx++;
               }
            }
           if(pClassObject)pClassObject->Release();
         }
        if(pEnumClassObject)pEnumClassObject->Release();
      }
     if(pWbemServices)pWbemServices->Release();
   }
  if(pWbemLocator)pWbemLocator->Release();
}
#endif
bool __fastcall TfrmMain::IsRegisted()
{
  return EncryRegisteCode(RegistUserName).SubString(1,25) == SerialNumber;
}
void __fastcall TfrmMain::WriteFirstRunDate()
{
  TIniFile * IniFile;
  try
  {
    IniFile = new TIniFile(ExtractFilePath(Application->ExeName) + "ibill.ini");
    IniFile->WriteString("System","Key",FirstRunDate);
    delete IniFile;
  }
  catch(...)
  {
    delete IniFile;
    ShowErrorMessage(Handle,"д�����ļ�ʧ��",true,4);
    ExitProcess(99);
    return;
  }
}
void __fastcall TfrmMain::ShowInitializeStatus(AnsiString StatusText)
{
  if(!SlentMode && frmAbout != NULL)
  {
    frmAbout->Label4->Caption = StatusText;
    frmAbout->Update(); 
  }
}
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
  : TForm(Owner)
{
  frmAbout = NULL;
  if(!SlentMode)
  {
    frmAbout = new TfrmAbout(this);
    frmAbout->Show();
    ShowInitializeStatus("���ڳ�ʼ��Ӧ�ó������Ժ�...");
    //frmAbout->Update();
  }
  if(ExtractFilePath(Application->ExeName).Pos(" ") > 0 && !SlentMode)
  {
    //ShowErrorMessage(Handle,"Ϊ���������ܱ��ɹ�����Oracle���ݿ�,���齫����PowerBill���ڲ������ո��·����!",false,0);
    MessageBox(Handle,"Ϊ���������ܱ��ɹ�����Oracle���ݿ�,���齫����PowerBill���ڲ������ո��·����!","��ʾ",MB_OK | MB_ICONINFORMATION);
  }
/*
#ifdef SCTELE_COM_VERSION //�Ĵ�����ר�ð�,��BIOS���кź�CPU��Ϣ��Ϊע���
  TStringList * BIOSInfoList = new TStringList;
  CoInitialize(NULL);
  AnsiString BIOSSerialNumber;
  if(CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, 0) != S_OK)
  {
    ShowErrorMessage(Handle,"�޷���ȡȨ��",true,4);
    ExitProcess(99);
    return;
  }
  GetWmiInfo(BIOSInfoList,DecrySysInfo(WIN32_BIOS_STRING));
  for(int n = 0;n < BIOSInfoList->Count;n++)
  {
    if(BIOSInfoList->Strings[n].UpperCase().SubString(1,13) == DecrySysInfo(SERIALNUMBER))
    {
      BIOSSerialNumber = BIOSInfoList->Strings[n].UpperCase().SubString(14,BIOSInfoList->Strings[n].Length() - 13);
      break;  
    }
  }
  delete BIOSInfoList;
  char cpuname[128],temp[128];
  CCPUInfo *MyCpu=new CCPUInfo();
  MyCpu->GetName(cpuname);
  BIOSSerialNumber += ";" + String(cpuname) + ";";
  BIOSSerialNumber += IntToStr(MyCpu->GetSpeed()) + ";";
  MyCpu->GetTypeName(temp);
  BIOSSerialNumber += String(temp) = ";";
  if(MyCpu->hasFPU())
    BIOSSerialNumber += "YES;";
  else
    BIOSSerialNumber += "NO;";
  if(MyCpu->withMMX())
    BIOSSerialNumber += "YES;";
  else
    BIOSSerialNumber += "NO;";
  delete MyCpu;
#endif
*/
  HelpFilePath = ExtractFilePath(Application->ExeName) + "help\\";
  frmOpenDialog = new TfrmOpenDialog(this);
  frmOpenDialog->lvFiles->Folder->CSIDL = csidlDesktop;
  frmOpenDialog->cbxFolder->SelectedFolder->CSIDL = csidlDesktop;
  frmOpenFTPDialog = new TfrmOpenFTPDialog(this);
  TIniFile * IniFile = NULL;
  ShowInitializeStatus("���ڶ�ȡ��ʼ�������ļ�...");
  DBConfig  = new TDBConfig();
  FTPConfig = new TFTPConfig();
  try
  {
    IniFile = new TIniFile(ExtractFilePath(Application->ExeName) + "ibill.ini");
    //SerialNumber = IniFile->ReadString("System","SerialNumber","");/*Ҫ��ע��İ汾_20071229ɾȥ*/
#ifndef SCTELE_COM_VERSION  //ͨ�ð�,���û�����Ϊ���к�
    RegistUserName = IniFile->ReadString("System","RegisterUserName","");
#else
    //RegistUserName = EncrySysInfo(BIOSSerialNumber).SubString(1,25);
    RegistUserName = EncrySysInfo("�й�����").SubString(1,25);
#endif
    FirstRunDate = IniFile->ReadString("System","Key","");
    SerialNumber = EncryRegisteCode(RegistUserName).SubString(1,25);/*��ע��汾_20071229*/
    int Days;
    if(FirstRunDate == "")
    {
      FirstRunDate = EncrySysInfo(FormatDateTime("yyyy-mm-dd hh:nn:ss",Now()));
      WriteFirstRunDate();
      Days = 40;
    }
    else
    {
      try
      {
        Days = 40 - abs(DaysBetween(StrToDateTime(DecrySysInfo(FirstRunDate)),Now()));
        if(Days < 0)
          Days = 0;
      }
      catch(...)
      {
        delete IniFile;
        ShowErrorMessage(Handle,"�ⲿ�ļ�����",true,4);
        ExitProcess(99);
        return;
      }
    }
    if(EncryRegisteCode(RegistUserName).SubString(1,25) != SerialNumber)
    {
      if(!InCmdMode)
      {
        AnsiString Message;
        if(Days == 0)
        {
          Message = "��ʹ�õ��Ǳ������δע�ḱ��.�������ѹ�,���뾭��ע����ܼ���ʹ��.�Ƿ�����ע��?";
        }
        else
        {
          Message = "��ʹ�õ��Ǳ������δע�ḱ��.������" + IntToStr(Days) + "���������.�Ƿ�����ע��?";
        }
        if(MessageBox(Handle,Message.c_str(),"��ʾ",MB_YESNO | MB_ICONQUESTION) == IDYES)
        {
          TfrmRegister * frmRegister = new TfrmRegister(this);
          frmRegister->ShowModal();
          delete frmRegister;
        }
      }
      if(!IsRegisted() && Days <= 0)
      {
        ShowErrorMessage(Handle,"����ѹ���,��ע��",true,4);
        ExitProcess(99);
        return;
      }
    }
    else
    {
      #ifndef SCTELE_COM_VERSION
        Caption = "Power Bill ͨ�û�����ѯת��ϵͳ V2.0 ��Ȩ�� " + RegistUserName + " ʹ��";
      #else
        Caption = "Power Bill ͨ�û�����ѯת��ϵͳ V2.0";
      #endif
    }
    AnsiString s = IniFile->ReadString("Global","LastDirectory","");
    ShowInitializeStatus("���ڴ����һ�β������ļ���...");
    if(s == "")
    {
      frmOpenDialog->lvFiles->Folder->CSIDL = csidlDesktop;
      frmOpenDialog->cbxFolder->SelectedFolder->CSIDL = csidlDesktop;
    }
    else
    {
      frmOpenDialog->lvFiles->Folder->PathName = s;
      frmOpenDialog->cbxFolder->SelectedFolder->PathName = s;
    }
    if(IniFile->ReadString("Global","FirstRun","1") != "0" && !InCmdMode
      &&MessageBox(Handle,"��һ�����б�����,����Ҫ�趨һЩ����ѡ��,�Ƿ����ھ�����?","��ӭ",MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
      FTPTimeout = 0;
      TfrmOption * frmOption = new TfrmOption(this);
      frmOption->ShowModal();
      delete frmOption;
      IniFile->WriteString("Global","FirstRun","0");
    }
    s = IniFile->ReadString("Global","RecursionListDir","0");
    frmOpenFTPDialog->Recursion = s != "0";
    s = IniFile->ReadString("Global","SearchFileDir","0");
    OpenDialog1->InitialDir = s;
    SortConfirm = IniFile->ReadInteger("Global","SortConfirm",10000);
    if(FTPTimeout < 0)
      FTPTimeout  = IniFile->ReadInteger("FTP","Timeout",60);

    AnsiString LastBillName    = IniFile->ReadString("Global","LastBillName","");
    AnsiString LastFTPBillName = IniFile->ReadString("Global","LastFTPBillName","");
    RetryCount = IniFile->ReadInteger("Global","AutoRetry",0);
    TempDirectory =  IniFile->ReadString("Global","TempDir","");
    if(TempDirectory == "" || TempDirectory.Pos(" " ) > 0)
      TempDirectory = ExtractFilePath(Application->ExeName) + "Temp";
    if(!DirectoryExists(TempDirectory))
    {
      if(!DirectoryExists(TempDirectory))
      {
        if(!CreateDir(TempDirectory))
        {
          TempDirectory = ExtractFilePath(Application->ExeName) + "Temp";
          if(!DirectoryExists(TempDirectory))
            if(!CreateDir(TempDirectory))
              throw new Exception("�޷���������ʱ�ļ��� " + TempDirectory);
        }
      }
    }
    if(TempDirectory.SubString(TempDirectory.Length(),1) != "\\")
      TempDirectory = TempDirectory + "\\";
    BillConfigFile = IniFile->ReadString("Global","BillConfig","");
    AnsiString DBConfigFile   = IniFile->ReadString("Global","DBConfig","");
    AnsiString FTPConfigFile  = IniFile->ReadString("Global","FTPConfig","");
    BCPExePath    = IniFile->ReadString("ExternalTools","bcppath","");
    SQLLDRExePath = IniFile->ReadString("ExternalTools","sqlldrpath","");
    MySQLImportExePath = IniFile->ReadString("ExternalTools","mysqlimportpath","");
    delete IniFile;
    IniFile = NULL;
    if(BillConfigFile == "")
    {
      TOpenDialog * OpenDialog = new TOpenDialog(this);
      OpenDialog->Filter = "������ʽ�����ļ�|*.xml";
      if(!OpenDialog->Execute())
      {
        delete OpenDialog;
        throw new Exception("û��ָ��������ʽ�����ļ�,�����޷���������.");
      }
      BillConfigFile = OpenDialog->FileName;
      delete OpenDialog;
    }
    else if(BillConfigFile.AnsiPos("\\") < 1)
    {
      BillConfigFile = ExtractFilePath(Application->ExeName) +  BillConfigFile;
    }
    ShowInitializeStatus("�������뻰����ʽ������Ϣ...");
    BillConfig = new TBillConfig(BillConfigFile);
    ShowInitializeStatus("�����������ݿ�������Ϣ...");
    if(DBConfigFile != "")
    {
      if(!DBConfig->LoadConfig(DBConfigFile))
      {
        if(SlentMode)
          throw new Exception(DBConfig->LastErrorMessage + "  ���ܴ����ݿ������ļ�.");
        else
          MessageBox(Handle,(DBConfig->LastErrorMessage + "  ���ܴ����ݿ������ļ�.").c_str(),"����",
            MB_OK | MB_ICONWARNING);
      }
    }
    frmOpenDialog->cbxFileFormat->Items->Assign(BillConfig->BillNameList);
    frmOpenFTPDialog->cbxFileFormat->Items->Assign(frmOpenDialog->cbxFileFormat->Items);
    ShowInitializeStatus("��������FTP������Ϣ...");
    if(FTPConfigFile != "")
    {
      if(!FTPConfig->LoadConfig(FTPConfigFile))
      {
        if(SlentMode)
          throw new Exception(FTPConfig->LastErrorMessage + "  ���ܴ�FTP�����������ļ�.");
        else
          MessageBox(Handle,(FTPConfig->LastErrorMessage + "  ���ܴ�FTP�����������ļ�.").c_str(),"����",
            MB_OK | MB_ICONWARNING);
      }
    }
    if(LastBillName != "")
    {
      frmOpenDialog->cbxFileFormat->ItemIndex =
        frmOpenDialog->cbxFileFormat->Items->IndexOf(LastBillName);
    }
    else
    {
      frmOpenDialog->cbxFileFormat->ItemIndex    = 0;
    }
    if(LastFTPBillName != "")
    {
      frmOpenFTPDialog->cbxFileFormat->ItemIndex =
        frmOpenFTPDialog->cbxFileFormat->Items->IndexOf(LastFTPBillName);
    }
    else
    {
      frmOpenFTPDialog->cbxFileFormat->ItemIndex = 0;
    }
    RzStatusPane1->Caption = "��ǰ�����ļ�:" + BillConfigFile;
  }
  catch(Exception * E)
  {
    if(BillConfig != NULL)
    {
      delete BillConfig;
      BillConfig = NULL;
    }
    if(IniFile != NULL)
    {
      delete IniFile;
      IniFile = NULL;
    }
    ShowErrorMessage(Handle,E->Message.c_str(),true,4);
  }
  if(!SlentMode)
  {
    try
    {
      delete frmAbout;
    }
    catch(...)
    {
    }
  }
}
__fastcall TfrmMain::~TfrmMain()
{
#ifdef SCTELE_COM_VERSION
  CoUninitialize();
#endif
}
void __fastcall TfrmMain::AddFormToPageControl(TForm * AForm)
{
  if(!RzPageControl1->Visible)
    RzPageControl1->Visible = true;
  TRzTabSheet * TabSheet = new TRzTabSheet(this);
  TabSheet->PageControl = RzPageControl1;
  TabSheet->Caption = AForm->Caption;
  AForm->Parent = TabSheet;
  AForm->Align=alClient;
  AForm->BorderStyle = Forms::bsNone;
  TabSheet->Tag = (int)AForm;
  AForm->Tag = (int)TabSheet;
  RzPageControl1->ActivePage = TabSheet;
  RzPageControl1->ShowCloseButton = RzPageControl1->PageCount > 0;
  /*
  TMenuItem * MenuItem = new TMenuItem(this);
  MenuItem->Caption    = AForm->Caption;
  MenuItem->RadioItem  = true;
  MenuItem->GroupIndex = 1000;
  MenuItem->Tag        = (int)TabSheet;
  menuWindows->Add(MenuItem);
  if(!menuWindows->Enabled)
    menuWindows->Enabled = true;
  MenuItem->Checked    = true;
  */
  AForm->Show();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormDestroy(TObject *Sender)
{
  delete frmOpenDialog;
  delete frmOpenFTPDialog;  
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::ToolButton10Click(TObject *Sender)
{
  Close();  
}
TForm * __fastcall TfrmMain::IsFormCreated(AnsiString FormClassName)
{
  TForm * Form;
  for(int n = 0;n < RzPageControl1->PageCount;n++)
  {
    Form = (TForm *)RzPageControl1->Pages[n]->Tag;
    if(Form != NULL&&Form->ClassNameIs(FormClassName))
    {
      return Form;
    }
  }
  return NULL;
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::RzPageControl1Close(TObject *Sender,
      bool &AllowClose)
{
  TRzTabSheet * TabSheet = RzPageControl1->ActivePage;
  if(TabSheet == NULL)
    return;
  int Index = TabSheet->TabIndex;
  AllowClose = CloseAChildForm(TabSheet);
  if(!AllowClose)
    return;
  if(RzPageControl1->PageCount > 0)
  {
    if(RzPageControl1->PageCount > Index)
      RzPageControl1->ActivePageIndex = Index;
    else
      RzPageControl1->ActivePageIndex = Index - 1;
  }
  if(RzPageControl1->PageCount == 1)
  {
    RzPageControl1->Visible = false;
  }
}
//---------------------------------------------------------------------------

bool __fastcall TfrmMain::CloseAChildForm(TRzTabSheet * TabSheet)
{
  if(TabSheet->Tag == 0)
    return true;
  TForm * Form = (TForm *)TabSheet->Tag;
  try
  {
    Form->Close();
  }
  catch(Exception * E)
  {
    if(!SlentMode && E->Message != "")
      MessageBox(Handle,E->Message.c_str(),"��ʾ",MB_OK | MB_ICONWARNING);
    return false;
  }
  delete Form;
  TabSheet->Tag = 0;
  return true;
}
void __fastcall TfrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
  for(int n =RzPageControl1->PageCount - 1;n > -1 ;n--)
  {
    if(!CloseAChildForm(RzPageControl1->Pages[n]))
    {
      Action = caNone;
      return;
    }
    else
    {
      delete RzPageControl1->Pages[n];
    }
  }
  TIniFile * IniFile = NULL;
  try
  {
    IniFile = new TIniFile(ExtractFilePath(Application->ExeName) + "ibill.ini");
    IniFile->WriteString("Global","LastDirectory",frmOpenDialog->lvFiles->Folder->PathName);
    IniFile->WriteString("Global","LastBillName",frmOpenDialog->cbxFileFormat->Text);
    IniFile->WriteString("Global","LastFTPBillName",frmOpenFTPDialog->cbxFileFormat->Text);
    if(OpenDialog1->FileName != "")
      IniFile->WriteString("Global","SearchFileDir",ExtractFilePath(OpenDialog1->FileName));
    if(frmOpenFTPDialog->Recursion)
      IniFile->WriteString("Global","RecursionListDir","1");
    else
      IniFile->WriteString("Global","RecursionListDir","0");
    /*if(btnShowLeftBar->Down)
      IniFile->WriteString("Global","ShowLeftBar","1");
    else
      IniFile->WriteString("Global","ShowLeftBar","0");*/
    delete IniFile;
    IniFile = NULL;
  }
  catch(...)
  {
    if(IniFile != NULL)
    {
      delete IniFile;
      IniFile = NULL;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Action1Execute(TObject *Sender)
{
  TRzTabSheet * TabSheet = RzPageControl1->ActivePage; 
  if(TabSheet == NULL || TabSheet->Tag == 0)
    return;
  TForm * Form = (TForm *)TabSheet->Tag;
  SendMessage(Form->Handle,MSG_CUSTOMER_KEYDOWN,1,'A');
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Action2Execute(TObject *Sender)
{
  TRzTabSheet * TabSheet = RzPageControl1->ActivePage;
  if(TabSheet == NULL || TabSheet->Tag == 0)
    return;
  TForm * Form = (TForm *)TabSheet->Tag;
  SendMessage(Form->Handle,MSG_CUSTOMER_KEYDOWN,0,127);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Action3Execute(TObject *Sender)
{
  TRzTabSheet * TabSheet = RzPageControl1->ActivePage;
  if(TabSheet == NULL || TabSheet->Tag == 0)
    return;
  TForm * Form = (TForm *)TabSheet->Tag;
  SendMessage(Form->Handle,MSG_CUSTOMER_KEYDOWN,1,'C');
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Action4Execute(TObject *Sender)
{
  TRzTabSheet * TabSheet = RzPageControl1->ActivePage;
  if(TabSheet == NULL || TabSheet->Tag == 0)
    return;
  TForm * Form = (TForm *)TabSheet->Tag;
  SendMessage(Form->Handle,MSG_CUSTOMER_KEYDOWN,1,'E');
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnOpenBillConfigInIEClick(TObject *Sender)
{
  ShellExecute(NULL,"open","iexplore.exe",BillConfigFile.c_str(),"",SW_SHOW);
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::btnCloseClick(TObject *Sender)
{
  Close();  
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnBillFileClick(TObject *Sender)
{
  AddFormToPageControl(new TfrmBillFile(this)); 
}
//---------------------------------------------------------------------------



void __fastcall TfrmMain::btnDBLinkConfigClick(TObject *Sender)
{
  TfrmDBConfig * frmDBConfig = (TfrmDBConfig *)IsFormCreated("TfrmDBConfig");
  if(frmDBConfig == NULL)
  {
    frmDBConfig = new TfrmDBConfig(this);
    AddFormToPageControl(frmDBConfig);
  }
  else
  {
    TRzTabSheet * TabSheet = (TRzTabSheet *)frmDBConfig->Tag;
    RzPageControl1->ActivePage = TabSheet;
  }  
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnBillConfigClick(TObject *Sender)
{
  TfrmBillConfig * frmBillConfig = (TfrmBillConfig *)IsFormCreated("TfrmBillConfig");
  if(frmBillConfig == NULL)
  {
    frmBillConfig = new TfrmBillConfig(this);
    AddFormToPageControl(frmBillConfig);
  }
  else
  {
    TRzTabSheet * TabSheet = (TRzTabSheet *)frmBillConfig->Tag;
    RzPageControl1->ActivePage = TabSheet;
  }  
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnFTPConfigClick(TObject *Sender)
{
  TfrmFTPConfig * frmFTPConfig = (TfrmFTPConfig *)IsFormCreated("TfrmFTPConfig");
  if(frmFTPConfig == NULL)
  {
    frmFTPConfig = new TfrmFTPConfig(this);
    AddFormToPageControl(frmFTPConfig);
  }
  else
  {
    TRzTabSheet * TabSheet = (TRzTabSheet *)frmFTPConfig->Tag;
    RzPageControl1->ActivePage = TabSheet;
  }    
}
//---------------------------------------------------------------------------



void __fastcall TfrmMain::SaveDialog1TypeChange(TObject *Sender)
{
  switch(SaveDialog1->FilterIndex)
  {
  case 1:
    SaveDialog1->DefaultExt = "dbf";
    break;
  case 2:
    SaveDialog1->DefaultExt = "csv";
    break;
  case 3:
    SaveDialog1->DefaultExt = "txt";
    break;
  case 4:
    SaveDialog1->DefaultExt = "bil";
    break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::menuExitClick(TObject *Sender)
{
  Close();  
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::menuOptionClick(TObject *Sender)
{
  TfrmOption * frmOption = new TfrmOption(this);
  frmOption->ShowModal();
  delete frmOption;  
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::menuAboutClick(TObject *Sender)
{
  TfrmAbout * frmAbout = new TfrmAbout(this);
  frmAbout->ShowModal();
  delete frmAbout;  
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnOpenSearchResultClick(TObject *Sender)
{
  if(!OpenDialog1->Execute())
    return;
  AnsiString FileName = OpenDialog1->FileName;
  //��ȡ�ļ�ͷ����Ϣ
  FILE * file = fopen(FileName.c_str(),"r+b");
  if(file == NULL)
  {
    MessageBox(Handle,"�޷����ļ�!","����",MB_OK | MB_ICONSTOP);
    return;
  }
  SearchResultFileHead srfh = {0};
  if(fread(&srfh,sizeof(srfh),1,file) != 1)
  {
    fclose(file);
    MessageBox(Handle,"�ļ���ʽ����ȷ!","����",MB_OK | MB_ICONSTOP);
    return;
  }
  AnsiString BillName = srfh.BillName;
  if(BillConfig->GetBillNode(BillName) == NULL)
  {
    TfrmSelBillName * frmSelBillName = new TfrmSelBillName(this,ExtractFileName(FileName));
    frmSelBillName->Label1->Caption =
      "���ļ��еĻ�����ʽ�� " + BillName + " .���ڵ�ǰ�Ļ����������Ҳ��������������."\
      "����Ҫѡ��һ�ֻ����������򿪴��ļ�."; 
    if(frmSelBillName->ShowModal() == mrOk)
    {
      memset(&srfh.BillName[0],0,256);
      strcpy(&srfh.BillName[0],frmSelBillName->lvBillTypes->Selected->SubItems->Strings[0].c_str());
      fseek(file,SEEK_SET,0);
      fwrite(&srfh,sizeof(srfh),1,file);
      delete frmSelBillName;
    }
    else
    {
      fclose(file);
      delete frmSelBillName;
      return;
    }
  }
  fclose(file);
  TfrmViewBill * frmViewBill = new TfrmViewBill(this,BillConfig);
  if(frmViewBill->OpenBillFile(SR_BILL_FORMAT_STR,"",ExtractFilePath(FileName),ExtractFileName(FileName),GetLocalFileSize(FileName)))
  {
    frmViewBill->Caption = "������ѯ��� " + ExtractFileName(FileName);
    AddFormToPageControl(frmViewBill);
    TRzTabSheet * TabSheet = (TRzTabSheet *)frmViewBill->Tag;
    frmMain->RzPageControl1->ActivePage = TabSheet;
  }
  else
  {
    delete frmViewBill;
  }    
}
//---------------------------------------------------------------------------
bool __fastcall TfrmMain::ShowOpenFileDialog(AnsiString BillName)
{
  AnsiString s = frmOpenDialog->cbxFileFormat->Text;
  frmOpenDialog->cbxFileFormat->Items->Assign(BillConfig->BillNameList);
  if(BillName != "")
  {
    frmOpenDialog->cbxFileFormat->ItemIndex =
      frmOpenDialog->cbxFileFormat->Items->IndexOf(BillName);
    if(frmOpenDialog->cbxFileFormat->ItemIndex > -1)
      frmOpenDialog->cbxFileFormat->Enabled = false;
    else
      frmOpenDialog->cbxFileFormat->Enabled = true;
  }
  else
  {
    frmOpenDialog->cbxFileFormat->Enabled = true;
    frmOpenDialog->cbxFileFormat->ItemIndex =
      frmOpenDialog->cbxFileFormat->Items->IndexOf(s);
  }
  return frmOpenDialog->Execute();
}

bool __fastcall TfrmMain::ShowOpenFTPDialog(AnsiString BillName)
{
  AnsiString s = frmOpenFTPDialog->cbxFileFormat->Text;
  frmOpenFTPDialog->cbxFileFormat->Items->Assign(BillConfig->BillNameList);
  frmOpenFTPDialog->cbxServer->Items->Clear();
  for(int n = 0;n < FTPConfig->GetFTPCount();n++)
  {
    frmOpenFTPDialog->cbxServer->Items->Add(FTPConfig->GetFTPName(n));
  }
  if(BillName != "")
  {
    frmOpenFTPDialog->cbxFileFormat->ItemIndex =
      frmOpenFTPDialog->cbxFileFormat->Items->IndexOf(BillName);//lvFileList->Items->Item[0]->SubItems->Strings[1]);
    if(frmOpenFTPDialog->cbxFileFormat->ItemIndex > -1)
      frmOpenFTPDialog->cbxFileFormat->Enabled = false;
    else
      frmOpenFTPDialog->cbxFileFormat->Enabled = true;
  }
  else
  {
    frmOpenFTPDialog->cbxFileFormat->Enabled = true;
    frmOpenFTPDialog->cbxFileFormat->ItemIndex =
      frmOpenFTPDialog->cbxFileFormat->Items->IndexOf(s);
  }
  return frmOpenFTPDialog->Execute();
}
void __fastcall TfrmMain::btnOpenFileClick(TObject *Sender)
{
  if(!ShowOpenFileDialog())
    return;
  TfrmBillFile * frmBillFile = new TfrmBillFile(this);
  frmBillFile->menuAddFileClick(NULL);
  AddFormToPageControl(frmBillFile);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnOpenFTPFileClick(TObject *Sender)
{
  if(!ShowOpenFTPDialog())
    return;
  TfrmBillFile * frmBillFile = new TfrmBillFile(this);
  frmBillFile->btnAddFTPFileClick(NULL);
  AddFormToPageControl(frmBillFile);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::RzPageControl1Change(TObject *Sender)
{
  if(RzPageControl1->ActivePage->Tag != 0)
  {
    TForm * Form = (TForm *)RzPageControl1->ActivePage->Tag;
    if(Form->ClassNameIs("TfrmViewBill"))
    {
      frmViewBill = (TfrmViewBill *)Form;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::Action5Execute(TObject *Sender)
{
  TRzTabSheet * TabSheet = RzPageControl1->ActivePage;
  if(TabSheet == NULL || TabSheet->Tag == 0)
    return;
  TForm * Form = (TForm *)TabSheet->Tag;
  SendMessage(Form->Handle,MSG_CUSTOMER_KEYDOWN,1,'F');  
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::OnFTPWorkBegin(TMessage Message)
{
  RzStatusPane2->Caption = "���������ļ�" + AnsiString((const char *)Message.WParam);
  RzProgressBar1->TotalParts = Message.LParam;
  RzProgressBar1->PartsComplete = 0;
}
void __fastcall TfrmMain::OnFTPWork(TMessage Message)
{
  RzProgressBar1->PartsComplete = Message.LParam;
}
void __fastcall TfrmMain::OnSetProcessMaxCount(TMessage Message)
{
  RzStatusPane2->Font->Color = clBlack;
  RzGlyphStatus1->ImageIndex = -1;
  RzStatusPane2->Caption = "���ڴ��ļ�" + AnsiString((const char *)Message.LParam);
  RzProgressBar1->TotalParts = Message.WParam;
  RzProgressBar1->PartsComplete = 0;
}
void __fastcall TfrmMain::OnSetProcessPos(TMessage Message)
{
  RzProgressBar1->PartsComplete = Message.WParam;
}
void __fastcall TfrmMain::OnThreadTerminate(TMessage Message)
{
  RzProgressBar1->TotalParts = 0;
  RzProgressBar1->PartsComplete = 0;
  RzStatusPane2->Caption = ""; 
}
void __fastcall TfrmMain::OnOpenFileError(TMessage Message)
{
  RzStatusPane2->Font->Color = clRed;
  RzGlyphStatus1->ImageIndex = 74;
  RzStatusPane2->Caption =
    "���ļ� " + AnsiString((const char *)Message.WParam) + " ����:" + AnsiString((const char *)Message.LParam);
}

void __fastcall TfrmMain::menuHelpClick(TObject *Sender)
{
#ifdef SCTELE_COM_VERSION
  if((int)ShellExecute(NULL,"open","hh.exe",
    (ExtractFilePath(Application->ExeName)+"Help\\ibillsc.chm").c_str(),"",SW_SHOWNORMAL)<32)
    MessageBox(Handle,"�򿪰����ĵ�����!�����ļ��Ƿ����.","ϵͳ����",MB_OK | MB_ICONWARNING);
#else
  if((int)ShellExecute(NULL,"open","hh.exe",
    (ExtractFilePath(Application->ExeName)+"Help\\ibill.chm").c_str(),"",SW_SHOWNORMAL)<32)
    MessageBox(Handle,"�򿪰����ĵ�����!�����ļ��Ƿ����.","ϵͳ����",MB_OK | MB_ICONWARNING);
#endif
}
//---------------------------------------------------------------------------

