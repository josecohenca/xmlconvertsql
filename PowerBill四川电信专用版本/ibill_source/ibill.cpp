//---------------------------------------------------------------------------

#include <vcl.h>
#include <windows.h>
#include <Classes.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdFTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include "MainFrm.h"
#include "ExportToDBFrm.h"
#include "BillFileFrm.h"
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("MainFrm.cpp", frmMain);
USEFORM("OpenDialogFrm.cpp", frmOpenDialog);
USEFORM("ViewBillFrm.cpp", frmViewBill);
USEFORM("ExportToDBFrm.cpp", frmExportToDB);
USEFORM("FTPConfigFrm.cpp", frmFTPConfig);
USEFORM("EditFTPFrm.cpp", frmEditFTP);
USEFORM("BillConfigFrm.cpp", frmBillConfig);
USEFORM("BillFileFrm.cpp", frmBillFile);
USEFORM("OpenFTPDialogFrm.cpp", frmOpenFTPDialog);
USEFORM("SearchFrm.cpp", frmSearch);
USEFORM("EditSearchParamFrm.cpp", frmEditSearchParam);
USEFORM("ExportToFileFrm.cpp", frmExportToFile);
USEFORM("DBConfigFrm.cpp", frmDBConfig);
USEFORM("EditDBFrm.cpp", frmEditDB);
USEFORM("BillFormatFrm.cpp", frmBillFormat);
USEFORM("FieldFrm.cpp", frmField);
USEFORM("ReplaceRuleFrm.cpp", frmReplaceRule);
USEFORM("EditReplaceRuleFrm.cpp", frmEditReplaceRule);
USEFORM("ValueTransFrm.cpp", frmValueTrans);
USEFORM("EditTransFrm.cpp", frmEditTrans);
USEFORM("RecordFilterFrm.cpp", frmRecordFilter);
USEFORM("EditFilterFrm.cpp", frmEditFilter);
USEFORM("OptionFrm.cpp", frmOption);
USEFORM("AboutFrm.cpp", frmAbout);
USEFORM("SearchFileFrm.cpp", frmSearchFile);
USEFORM("FilterRptFrm.cpp", frmFilterRpt);
USEFORM("SelBillNameFrm.cpp", frmSelBillName);
USEFORM("RecordDetailFrm.cpp", frmRecordDetail);
USEFORM("RegisterFrm.cpp", frmRegister);
USEFORM("OpeningFileFrm.cpp", frmOpeningFile);
//---------------------------------------------------------------------------
bool SlentMode;
FILE * LogFile;
bool InCmdMode;
int  FTPTimeout;
int  ProcessExitCode;

AnsiString GetWindowsVersion()
{
	OSVERSIONINFO osinfo;   
  osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&osinfo);
  switch(osinfo.dwPlatformId)   
  {   
  case VER_PLATFORM_WIN32s://Win32s   on   Windows   3.1.
  case VER_PLATFORM_WIN32_WINDOWS://Windows 95,Windows 98,or Windows Me.
    throw new Exception("��֧���ڴ˲���ϵͳ������");
    //break;
/*
    {
      switch(osinfo.dwMinorVersion)
      {
      case   0: //Win95
        return "Win95";
      case   10://Win98
        return "Win98";
      case   90://WinMe
        return "WinMe";
      }
    }
*/
  case VER_PLATFORM_WIN32_NT://Windows   NT   3.51,   Windows   NT   4.0,   Windows   2000,   or   Whistler
    //throw new Exception("��֧���ڴ˲���ϵͳ������");
    {
      //ShowMessage("VER_PLATFORM_WIN32_NT");
      switch(osinfo.dwMinorVersion)
      {
      case 51://WinNT3.51
        return "WinNT3.51";
      case 0:
        {
          switch(osinfo.dwMajorVersion)
          {
          case   4://WinNT4
            return "WinNT4.0";
          case   5://Win2000
            return "Win2000";
          }
        }
      case 1://Whistler
        return "Whistler";
      case 2://Windows2003
        return "Windows2003";
      default:
        return "WinXP,2003";
      }
    }
  }
  return "";
}

BOOL WINAPI HandlerRoutine(
  DWORD dwCtrlType   //  control signal type
)
{
  switch(dwCtrlType)
  {
  case CTRL_C_EVENT:
    if(frmExportToDB != NULL)
    {
      //printf("\n��Ҫ��ֹ��ǰ��������������?(Y/N):");
      //char c = getchar();
      //if(c == 'y' || c == 'Y')
      //{
      //  if(frmExportToDB->ThreadConvertForDB != NULL)
      //  {
      //    frmExportToDB->ThreadConvertForDB->BillFile->TermCurOperation();
      //    frmExportToDB->TermByUser = true;
      //  }
      //}
      if(frmExportToDB->ThreadSubmitToDB != NULL)
      {
        frmExportToDB->TermByUser = true;
      }
    }
    return true;
  default:
    break;
  }
  return true;
}

int __fastcall GetNextCharPos(const char * chStr)
{
  int n;
  for(n = 0;n < (int)strlen(chStr);n++)
  {
    if(*(chStr + n) > 32)
      return n;
  }
  return -1;
}
//ִ������
bool __fastcall BeginJob(
    int JobMode,              //����ģʽ.1:�����ļ�ģʽ;2:FTP�ļ�ģʽ
    AnsiString ListFileName,  //�б��ļ���
    AnsiString BillName,      //������������
    AnsiString DBName,        //���ݿ�������
    AnsiString TableName,     //����
    AnsiString LogFileName,   //��־�ļ���
    AnsiString TempPath,      //��ʱĿ¼
    bool TransFieldValue,     //�Ƿ���ֶ�ֵ���з���
    int  MaxError,            //����ʱ�������������
    bool VerifyFile,          //�Ƿ���ļ����и�ʽ���
    AnsiString FTPServer,     //�����úõ�FTP����������,FTP�ļ�ģʽ
    AnsiString DirList,       //ȡ�ļ���Ŀ¼�б�
    int FTPRetryCount         //��FTP�ļ�ʱ�����������Դ���
    )
{
  SaveLog(("��ʼ��������."\
          "\n��������:" + BillName +
          "\n���ݿ�����:" + DBName +
          "\n���뵽��:" + TableName +
          "\n��־�ļ�:" + LogFileName +
          "\n�������ݿ��������������:" + IntToStr(MaxError) + "\n"
          "\n��ʱ�ļ�������:" + TempPath + "������\n"
          ).c_str());
  if(JobMode == 1)
  {
    SaveLog(("�����ļ��б���" + ListFileName + ".\n").c_str());
  }
  else if(JobMode == 2)
  {
    SaveLog(("����Ŀ¼�б���" + DirList + "\n" +
      "FTP������:" + FTPServer).c_str());
  }  
  bool Result = false;
  frmMain     = NULL;
  TStringList * FileList = NULL;
  TIdFTP      * IdFTP    = NULL;
  try
  {
    frmMain = new TfrmMain(Application);
    if(TempPath != "")
    {
      if(!DirectoryExists(TempPath) && !CreateDir(TempPath))
        SaveLog(("ָ������ʱ�ļ�����Ч,�Ѹ�Ϊʹ��Ĭ�ϵ���ʱ�ļ���:" + frmMain->TempDirectory + "\n").c_str());
      else
      {
        if(TempPath.SubString(TempPath.Length(),1) != "\\")
          TempPath = TempPath + "\\";
        frmMain->TempDirectory = TempPath;
      }
    }
    if(!frmMain->DBConfig->IsDBNameExists(DBName))
      throw new Exception("�������ļ����Ҳ���" + DBName + "��������Ϣ");
    frmExportToDB = new TfrmExportToDB(NULL);
    frmExportToDB->txtDBServer->Text = frmMain->DBConfig->GetDBAttributeValue(DBName,"Server");
    if(frmExportToDB->txtDBServer->Text == "")
      throw new Exception("û��Ϊ" + DBName + "���÷�������ַ");
    frmExportToDB->cbxDBType->ItemIndex =
      frmExportToDB->cbxDBType->Items->IndexOf(
        frmMain->DBConfig->GetDBAttributeValue(DBName,"DBType"));
    if(frmExportToDB->cbxDBType->ItemIndex < 0)
    {
      throw new Exception("Ϊ" + DBName + "���õ����ݿ����Ͳ���ȷ");
    }
    FileList = new TStringList;
    if(JobMode == 1)
    {
      FileList->LoadFromFile(ListFileName);
      for(int n = FileList->Count - 1;n > -1;n--)
      {
        if(FileList->Strings[n] == "")
          FileList->Delete(n);
      }
    }
    else  //FTPģʽ
    {
      if(!frmMain->FTPConfig->IsFTPNameExists(FTPServer))
      {
        throw new Exception("��FTP�����ļ����Ҳ���ָ����FTP������");
      }
      IdFTP = new TIdFTP(NULL);
      IdFTP->Host = frmMain->FTPConfig->GetFTPAttributeValue(FTPServer,"Server");
      try
      {
        IdFTP->Port = StrToInt(frmMain->FTPConfig->GetFTPAttributeValue(FTPServer,"Port"));
      }
      catch(...)
      {
        IdFTP->Port = 21;
      }
      IdFTP->Username = frmMain->FTPConfig->GetFTPAttributeValue(FTPServer,"UserName");
      IdFTP->Password = frmMain->FTPConfig->GetFTPAttributeValue(FTPServer,"Password",true);
      IdFTP->Passive  = frmMain->FTPConfig->GetFTPAttributeValue(FTPServer,"Mode").UpperCase() == "PASV";
      AnsiString FTPString  = "ftp://" +
        frmMain->FTPConfig->EncryFTPString(IdFTP->Username) + ":" +
        frmMain->FTPConfig->EncryFTPString(IdFTP->Password) + "@" + IdFTP->Host + ":" + IntToStr(IdFTP->Port);
      AnsiString PasvString;
      if(IdFTP->Passive)
        PasvString = "|PASV|";
      else
        PasvString = "||";
      int ErrCount = 0;
      while(true)
      {
        AnsiString Status;
        TStringList * FTPDirList  = new TStringList;
        AnsiString CurFTPPath     = "";
        AnsiString SearchFileName = "";
        try
        {
          if(IdFTP->Connected())
            IdFTP->Disconnect();
          Status = "����FTP������" + IdFTP->Host;
          SaveLog((Status + "\n").c_str());
          IdFTP->Connect(true,FTPTimeout);
          Status = "��ȡFTP�ļ��б�";
          SaveLog((Status + "\n").c_str());
          FTPDirList->Clear();
          FTPDirList->LoadFromFile(DirList);
          AnsiString FileName;
          for(int n = FTPDirList->Count - 1;n > -1;n--)
          {
            if(FTPDirList->Strings[n] == "")
              FTPDirList->Delete(n);
          }
          FileList->Clear();
          if(FTPDirList->Count == 0)
            FTPDirList->Add("");
          for(int n = 0;n < FTPDirList->Count;n++)
          {
            CurFTPPath = FTPDirList->Strings[n];            
            if(CurFTPPath == "")
            {
              CurFTPPath     = "/";
              SearchFileName = "";
            }
            else
            {
              int pos = StrRScan(CurFTPPath.c_str(),"/",-1);
              if(pos > -1)
              {
                SearchFileName = CurFTPPath.SubString(pos + 2,CurFTPPath.Length() - pos - 1);
                CurFTPPath     = CurFTPPath.SubString(1,pos + 1);
              }
              else
              {
                SaveLog(("����·��[" + CurFTPPath + "]����ȷ,����������\n").c_str());
                continue;
              }
            }
            Status = "��ȡ�ļ��б�" + CurFTPPath + SearchFileName;
            try
            {
              IdFTP->ChangeDir(CurFTPPath);
            }
            catch(Exception * E)
            {
              if(E->Message.Pos("No such file or directory.") > 0)
              {
                SaveLog(("����·��[" + CurFTPPath + "]����ȷ,��������û���ҵ���Ŀ¼,�ѷ����Ը�Ŀ¼������\n").c_str());
                continue; 
              }
              else
              {
                throw new Exception(E->Message);
              }
            }
            if(SearchFileName != "" && SearchFileName.Pos("*") < 1 && SearchFileName.Pos("?") < 1)
            {
              //���SearchFileName��FTP���Ƿ���Ŀ¼
              IdFTP->List(NULL,"",true);
              for(int k = 0;k < IdFTP->DirectoryListing->Count;k++)
              {
                FileName = IdFTP->DirectoryListing->Items[k]->FileName;
                if(FileName == "." || FileName == "..")
                  continue;
                if(FileName.LowerCase() == SearchFileName.LowerCase() && IdFTP->DirectoryListing->Items[k]->ItemType == ditDirectory) //SearchFileName��һ��Ŀ¼
                {
                  CurFTPPath = CurFTPPath + FileName + "/";
                  SearchFileName = "";
                  IdFTP->ChangeDir(CurFTPPath);
                  break;
                }
              }
            }
            IdFTP->List(NULL,SearchFileName,true);
            int FileCount = 0;
            for(int k = 0;k < IdFTP->DirectoryListing->Count;k++)
            {
              FileName = IdFTP->DirectoryListing->Items[k]->FileName;
              if(FileName == "." || FileName == ".." || IdFTP->DirectoryListing->Items[k]->ItemType == ditDirectory) //Ŀ¼
                continue;
              FileList->Add(FTPString + CurFTPPath + PasvString + FileName);
              ++FileCount;
            }
            SaveLog((CurFTPPath + SearchFileName + "  �ҵ�" + IntToStr(FileCount) + "���ļ�").c_str());
          }
          delete IdFTP;
          IdFTP = NULL;
          delete FTPDirList;
          FTPDirList = NULL;
          SaveLog(("��������" + IntToStr(FileList->Count) + "���ļ�.\n").c_str());
          for(int n = 0;n < FileList->Count;n++)
          {
            SaveLog(("�ҵ��ļ�:" + GetFullFTPFileName(FileList->Strings[n])).c_str());
          }
          break;
        }
        catch(Exception * E)
        {
          ++ErrCount;
          if((FTPRetryCount > 0 && ErrCount > FTPRetryCount) || (FTPRetryCount == 0 && ErrCount > 1000))
          {
            if(FTPDirList != NULL)
            {
              delete FTPDirList;
              FTPDirList = NULL;
            }
            ProcessExitCode = 5;
            throw new Exception(Status + "��������:" + E->Message);
          }
          else
          {
            SaveLog((Status + "��������:" + E->Message + "  ����" + IntToStr(ErrCount) + "\n").c_str());
          }
        }
      }
    }
    frmExportToDB->txtDBUserName->Text = frmMain->DBConfig->GetDBAttributeValue(DBName,"UserName");
    frmExportToDB->txtDBPassword->Text = frmMain->DBConfig->GetDBAttributeValue(DBName,"Password",true);
    frmExportToDB->cbxDataBases->Items->Add(frmMain->DBConfig->GetDBAttributeValue(DBName,"DataBase"));
    frmExportToDB->cbxDataBases->ItemIndex = 0;
    frmExportToDB->txtDBPort->Text = frmMain->DBConfig->GetDBAttributeValue(DBName,"Port");
    frmExportToDB->txtTableName->Text = TableName;
    frmExportToDB->txtMaxError->Text = IntToStr(MaxError);
    frmExportToDB->cbxTransFieldValue->Checked = TransFieldValue;
    if(frmExportToDB->LoadFileList(FileList,BillName))
    {
      if(SlentMode)
      {
        if(!frmExportToDB->ManualStartJob())
        {
          if(frmExportToDB->TermByUser)
          {
            ProcessExitCode = 98;
            throw new Exception("�����û���ֹ");
          }
          else
          {
            throw new Exception("��������");
          }
        }
      }
      else
      {
        frmExportToDB->btnStartClick(NULL);
        frmExportToDB->ShowModal();
      }
      Result = true;
    }
    else
    {
      ProcessExitCode = 5;
      throw new Exception("�����ļ��б�ʧ��");
    }
  }
  catch(Exception * E)
  {
    ShowErrorMessage(Application->Handle,(E->Message).c_str(),false,99);
    if(ProcessExitCode == 0)
      ProcessExitCode = 99;
    Result = false;
  }
  SaveLog("*****************�������.*******************");
  if(IdFTP != NULL)
  {
    delete IdFTP;
    IdFTP = NULL;
  }
  if(FileList != NULL)
  {
    delete FileList;
    FileList = NULL;
  }
  if(frmExportToDB != NULL)
  {
    delete frmExportToDB;
    frmExportToDB = NULL;
  }
  if(frmMain != NULL)
  {
    delete frmMain;
    frmMain = NULL;
  }
  return Result;
}
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR lpCmdLine, int)
{
  try
  {
    GetWindowsVersion();
  }
  catch(Exception * E)
  {
    MessageBox(NULL,E->Message.c_str(),"����",MB_OK | MB_ICONSTOP);
    return 99;
  }
  DateSeparator   = '-';
  ShortDateFormat = "yyyy-m-d";
  try
  {
    ProcessExitCode = 0;
    frmExportToDB = NULL;
    SlentMode = false;
    LogFile = NULL;
    Application->Initialize();
    Application->Title = "PowerBillͨ�û�����ѯת��ϵͳ";
     InCmdMode = strlen(lpCmdLine) > 0;
    FTPTimeout = -1;
    //��������
    if(InCmdMode)
    {
      AnsiString Parameters(lpCmdLine);
      AnsiString ListFileName  = "";
      AnsiString LogFileName   = "";
      AnsiString TableName = "";
      AnsiString DBName    = "";
      AnsiString BillName  = "";
      AnsiString TempPath  = "";
      bool TransFieldValue = false;
      int MaxError = 0;
      int pos1 = 1;
      int pos2 = Parameters.Pos("=");
      AnsiString ParamName,ParamValue;
      AnsiString Message;
      bool VerifyFile = true;
      int JobMode = 1;
      AnsiString DirList;
      AnsiString FTPServer;
      while(pos2 > 0)
      {
        ParamName  = LowerCase(Parameters.SubString(pos1,pos2 - 1).Trim());
        Parameters = Parameters.SubString(pos2 + 1,Parameters.Length() - pos2);
        if(Parameters.SubString(1,1) == "\"")
        {
          Parameters = Parameters.SubString(2,Parameters.Length() - 1);
          pos2 = Parameters.Pos("\"");
          if(pos2 < 1)
            break;
          ParamValue = Parameters.SubString(1,pos2 - 1);
          Parameters = Parameters.SubString(pos2 + 1,Parameters.Length() - pos2);
          pos2 = GetNextCharPos(Parameters.c_str());
          Parameters = Parameters.SubString(pos2,Parameters.Length() - pos2 + 1);
        }
        else
        {
          pos2 = Parameters.Pos(" ");
          if(pos2 < 1)
          {
            ParamValue = Parameters.SubString(1,Parameters.Length());
          }
          else
          {
            ParamValue = Parameters.SubString(1,pos2 - 1);
            Parameters = Parameters.SubString(pos2 + 1,Parameters.Length() - ParamValue.Length());
            pos2 = GetNextCharPos(Parameters.c_str());
            Parameters = Parameters.SubString(pos2 + 1,Parameters.Length() - pos2);
          }
        }

        if(ParamName == "files")
          ListFileName = ParamValue;
        else if(ParamName == "db")
          DBName = ParamValue;
        else if(ParamName == "table")
          TableName = ParamValue;
        else if(ParamName == "silent")
          SlentMode = ParamValue == "1" || ParamValue.LowerCase() == "true" || ParamValue.LowerCase() == "yes";
        else if(ParamName == "log")
          LogFileName = ParamValue;
        else if(ParamName == "maxdberrors")
        {
          if(IsNumber(ParamValue.c_str()))
            MaxError = StrToInt(ParamValue);
          else
            Message += "����������Ч[" + ParamValue + "];";
        }
        else if(ParamName == "tempdir")
        {
          TempPath = ParamValue;
        }
        else if(ParamName == "billname")
          BillName = ParamValue;
        else if(ParamName == "trans")
          TransFieldValue = ParamValue == "1" || ParamValue.LowerCase() == "true" || ParamValue.LowerCase() == "yes";
        //else if(ParamName == "verifyfile")
        //  VerifyFile = ParamValue == "1" || ParamValue.LowerCase() == "true";
        else if(ParamName == "mode")
        {
          if(ParamValue == "local")
            JobMode = 1;
          else if(ParamValue == "ftp")
            JobMode = 2;
          else
            Message += "ָ���Ĺ���ģʽ��Ч[" + ParamValue + "]";
        }
        else if(ParamName == "ftpserver")
        {
          FTPServer = ParamValue;
        }
        else if(ParamName == "dirlist")
        {
          DirList = ParamValue;
        }
        else if(ParamName == "timeout")
        {
          if(IsNumber(ParamValue.c_str()))
            FTPTimeout = StrToInt(ParamValue);
        }
        else
        {
          Message += "��Ч����[" + ParamName + "];";
        }
        pos2 = Parameters.Pos("=");
      }
      if(Message == "")
      {
        if(TableName == "")
          Message += "û��ָ���������ݵ�Ŀ���;";
        if(DBName == "")
          Message += "û��ָ��Ҫ�����Ŀ�����ݿ����������;";
        if(BillName == "")
          Message += "û��ָ��������ʽ����";
        if(JobMode == 1)
        {
          if(ListFileName == "")
            Message += "û��ָ�������ļ��б��ı��ļ�;";
          if(!FileExists(ListFileName))
            Message += "ָ���ĵ����ļ��б��ı��ļ�������;";
        }
        else if(JobMode == 2)
        {
          if(DirList == "")
            Message += "û��ָ��FTP�б��ļ�;";
          if(!FileExists(DirList))
            Message += "ָ����FTPĿ¼�б��ı��ļ�������;";
          if(FTPServer == "")
            Message += "û��ָ��FTP����������;";
        }
      }
      LogFile = fopen(LogFileName.c_str(),"w");
      if(LogFile == NULL)
      {
        LogFileName = ExtractFilePath(Application->ExeName) + "ibill.log";
        LogFile = fopen(LogFileName.c_str(),"w");
        if(LogFile == NULL)
         ExitProcess(3);
      }
      if(SlentMode)
      {
        AllocConsole();
        SetConsoleCtrlHandler(HandlerRoutine,true);
      }
      if(Message != "")
      {
        ShowErrorMessage(Application->Handle,Message.c_str(),true,2);
      }
      BeginJob(JobMode,ListFileName,
          BillName,
          DBName,
          TableName,
          LogFileName,
          TempPath,
          TransFieldValue,
          MaxError,VerifyFile,FTPServer,DirList,0);
      fclose(LogFile);
      if(SlentMode)
        FreeConsole();
      ExitProcess(ProcessExitCode);
     }
     else
     {
       Application->CreateForm(__classid(TfrmMain), &frmMain);
       Application->Run();
     }
  }
  catch (Exception &exception)
  {
     //Application->ShowException(&exception);
  }
  catch (...)
  {
     try
     {
       throw Exception("");
     }
     catch (Exception &exception)
     {
       //Application->ShowException(&exception);
     }
  }
  return 0;
}
//---------------------------------------------------------------------------
