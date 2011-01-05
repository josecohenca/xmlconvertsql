
#include "public.h"
#include "FTPFile.h"
#include <Forms.hpp>

//FILE * afile;

TFTPFile::TFTPFile(HANDLE hParentWnd,AnsiString ATempDirectory,int ATimeout/*��ʱ,��λΪ��*/)
{
  Timeout = ATimeout;
  FTPWatchThread = new TFTPWatchThread(true);
  FTPWatchThread->FTPFile = this;
  FTPWatchThread->Timeout = Timeout;
  hWnd    = hParentWnd;
  IdFTP = new TIdFTP(NULL);
  IdFTP->Port    = 21;
  IdFTP->Passive = true;
  IdFTP->OnWorkBegin = IdFTPWorkBegin;
  IdFTP->OnWork      = IdFTPWork;
  TempDirectory      = ATempDirectory;
  IdFTP->ReadTimeout = Timeout * 1000;
  if(TempDirectory != "" && TempDirectory.Pos("\\") < 1)
  {
    TempDirectory = TempDirectory + "\\";
  }
  //afile = fopen("c:\\jlj.txt","w");
  //FTPWatchThread->file = afile;
}
TFTPFile::~TFTPFile()
{
  if(FTPWatchThread != NULL)
  {
    if(FTPWatchThread->Continue)
    {
      FTPWatchThread->Continue = false;
    }
    if(FTPWatchThread->Suspended)
    {
      FTPWatchThread->Suspended = false;
    }
    WaitForSingleObject((void *)FTPWatchThread->Handle,INFINITE);
    delete FTPWatchThread;
    FTPWatchThread = NULL;
  }
  if(IdFTP != NULL)
  {
    delete IdFTP;
    IdFTP = NULL;
  }
  //fclose(afile);
}
AnsiString __fastcall TFTPFile::GetServerName(AnsiString ServerString)
{
  int pos = ServerString.LowerCase().Pos("ftp://");
  if(pos < 1)
    return "";
  ServerString = ServerString.SubString(pos + 6,ServerString.Length() - pos);
  pos = StrLScan(ServerString.c_str(),"@");
  if(pos < 0)
    return "";
  ServerString = ServerString.SubString(pos + 2,ServerString.Length() - pos - 1);
  pos = StrLScan(ServerString.c_str(),":");
  if(pos < 0)
    return "";
  return ServerString.SubString(1,pos);
}

AnsiString __fastcall TFTPFile::GetUserName(AnsiString ServerString)
{
  int pos = ServerString.LowerCase().Pos("ftp://");
  if(pos < 1)
    return "";
  ServerString = ServerString.SubString(pos + 6,ServerString.Length() - pos);
  pos = StrLScan(ServerString.c_str(),":");
  if(pos < 0)
    return "";
  return DecryFTPPassword(ServerString.SubString(1,pos));
}
AnsiString __fastcall TFTPFile::GetPassword(AnsiString ServerString)
{
  int pos = ServerString.LowerCase().Pos("ftp://");
  if(pos < 1)
    return "";
  ServerString = ServerString.SubString(pos + 6,ServerString.Length() - pos);
  pos = StrLScan(ServerString.c_str(),":");
  if(pos < 0)
    return "";
  ServerString = ServerString.SubString(pos + 2,ServerString.Length() - pos - 1);
  pos = StrLScan(ServerString.c_str(),"@");
  if(pos < 0)
    return "";
  return DecryFTPPassword(ServerString.SubString(1,pos));
}
AnsiString __fastcall TFTPFile::GetPort(AnsiString ServerString)
{
  int pos = ServerString.LowerCase().Pos("ftp://");
  if(pos < 1)
    return "";
  ServerString = ServerString.SubString(pos + 6,ServerString.Length() - pos);
  pos = StrLScan(ServerString.c_str(),"@");
  if(pos < 0)
    return "";
  ServerString = ServerString.SubString(pos + 2,ServerString.Length() - pos - 1);
  pos = StrLScan(ServerString.c_str(),":");
  if(pos < 0)
    return "";
  ServerString = ServerString.SubString(pos + 2,ServerString.Length() - pos - 1);
  pos = StrLScan(ServerString.c_str(),"/");
  if(pos < 0)
    return "";
  return ServerString.SubString(1,pos);
}
AnsiString __fastcall TFTPFile::GetFilePath(AnsiString ServerString)
{
  int pos = ServerString.LowerCase().Pos("ftp://");
  if(pos < 1)
    return "";
  ServerString = ServerString.SubString(pos + 6,ServerString.Length() - pos);
  pos = StrLScan(ServerString.c_str(),"/");
  if(pos < 0)
    return "";
  ServerString = ServerString.SubString(pos + 2,ServerString.Length() - pos - 1);
  pos = StrRScan(ServerString.c_str(),"/");
  if(pos < 0)
    return "/";
  ServerString = ServerString.SubString(1,pos);
  if(ServerString.SubString(1,1) != "/")
    ServerString = "/" + ServerString;
  if(ServerString.SubString(ServerString.Length(),1) != "/")
    ServerString = ServerString + "/";
  return ServerString;
}

bool __fastcall TFTPFile::GetPasv(AnsiString ServerString)
{
  int pos = ServerString.LowerCase().Pos("ftp://");
  if(pos < 1)
    return false;
  pos = StrLScan(ServerString.c_str(),"|");
  if(pos < 0)
    return false;
  return ServerString.SubString(pos + 2,ServerString.Length() - pos - 1).UpperCase() == "PASV";
}

void __fastcall TFTPFile::IdFTPWorkBegin(TObject *Sender,TWorkMode AWorkMode, const int AWorkCountMax)
{
  SendMessage(hWnd,MSG_FTP_WORK_BEGIN,(WPARAM)CurFileName.c_str(),(LPARAM)CurFileSize);
}

void __fastcall TFTPFile::IdFTPWork(TObject *Sender,TWorkMode AWorkMode, const int AWorkCount)
{
  SendMessage(hWnd,MSG_FTP_WORK,(WPARAM)CurFileName.c_str(),(LPARAM)AWorkCount);
  FTPWatchThread->LastStartTime = Now();
}

AnsiString __fastcall TFTPFile::GetFile(AnsiString AFileName,AnsiString ServerString,AnsiString ATempDirectory)
{
  if(ATempDirectory == "" || !DirectoryExists(ATempDirectory))
  {
    ATempDirectory = TempDirectory;
  }
  else if(ATempDirectory != "" && ATempDirectory.Pos("\\") < 1)
  {
    ATempDirectory = ATempDirectory + "\\";
  }
  AnsiString Result;
  FTPWatchThread->LastStartTime = Now();
  FTPWatchThread->Suspended = false;
  LastErrorMessage = "";
  if(LastServerString != ServerString || !IdFTP->Connected())
  {
    if(IdFTP->Connected())
      IdFTP->Disconnect();
    Result = GetServerName(ServerString);
    if(Result == "")
    {
      FTPWatchThread->Suspended = true;
      LastErrorMessage = "��Ч�������ַ���!";
      return "";
    }
    IdFTP->Host = Result;
    Result = GetUserName(ServerString);
    if(Result == "")
      IdFTP->Username = "anonymous";
    else
      IdFTP->Username = Result;
    IdFTP->Password = GetPassword(ServerString);
    Result = GetPort(ServerString);
    if(!IsNumber(Result.c_str()))
      IdFTP->Port = 21;
    else
      IdFTP->Port = StrToInt(Result);
    IdFTP->Passive = GetPasv(ServerString);
    try
    {
      IdFTP->Connect(true,Timeout);
      IdFTP->ChangeDir(GetFilePath(ServerString));
      LastServerString = ServerString;
    }
    catch(Exception * E)
    {
      FTPWatchThread->Suspended = true;
      LastErrorMessage = "���ӷ�����" + IdFTP->Host + "ʧ��:" + E->Message;
      return "";
    }
  }
  try
  {
    //Result = ATempDirectory + AFileName;
    //char chTempFileName[MAX_PATH];
    //GetTempFileName(ATempDirectory.c_str(),"IBILL",0,&chTempFileName[0]);
    //Result = chTempFileName;
    //DeleteFile(Result);
    Result = GetATempFileName(ATempDirectory,"IBILL_FTP");
    CurFileName = AFileName;
    //fprintf(afile,"����ļ�%s�Ƿ����\n",CurFileName.c_str());
    IdFTP->List(NULL,AFileName,true);
    if(IdFTP->DirectoryListing->Count == 0)
      throw new Exception("�ļ�" + AFileName + "�ڷ������ϲ�����");
    CurFileSize = IdFTP->DirectoryListing->Items[0]->Size;
    //fprintf(afile,"�����ļ�%s\n",CurFileName.c_str());
    CurTempFileName = AFileName;
    IdFTP->Get(AFileName,Result,true,false);
    FTPWatchThread->Suspended = true;
    if(LastErrorMessage != "")
      throw new Exception(LastErrorMessage);
    //fprintf(afile,"�����ļ�%s\���\n",CurFileName.c_str());
    return Result; //********���ﷵ�����ص���ʱ�ļ���*********
  }
  catch(Exception * E)
  {
    FTPWatchThread->Suspended = true;
    //fprintf(afile,"�����ļ�%s\ʧ��:%s\n",CurFileName.c_str(),E->Message.c_str());
    //if(LastErrorMessage == "������ʱ")
    //  MessageBox(0,"������ʱ","",MB_OK | MB_ICONWARNING);
    if(Result != "" && FileExists(Result))
      DeleteFile(Result);
    try
    {
      IdFTP->Disconnect();
    }
    catch(...){}
    LastErrorMessage = "�ӷ�����" + IdFTP->Host + "�����ļ�" + CurFileName + "ʧ��:" + E->Message;
  }
  //MessageBox(0,LastErrorMessage.c_str(),"",MB_OK);
  return "";
}



