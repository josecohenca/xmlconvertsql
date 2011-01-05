
#ifndef FTPFILE_H
#define FTPFILE_H


#include <windows.h>
#include <Classes.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdFTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include "FTPWatchThread.h"

class TFTPFile
{
private:
  TFTPWatchThread * FTPWatchThread;
  AnsiString LastServerString;
  AnsiString CurFileName;
  AnsiString TempDirectory;
  int CurFileSize;
  AnsiString __fastcall GetUserName(AnsiString ServerString);
  AnsiString __fastcall GetPassword(AnsiString ServerString);
  bool       __fastcall GetPasv(AnsiString ServerString);
  void       __fastcall KillFTPTimer();
public:
  AnsiString CurTempFileName;
  HANDLE hWnd;
  AnsiString LastErrorMessage;
  TIdFTP * IdFTP;
  int Timeout;
  AnsiString __fastcall GetServerName(AnsiString ServerString);
  AnsiString __fastcall GetPort(AnsiString ServerString);
  AnsiString __fastcall GetFilePath(AnsiString ServerString);
  TFTPFile(HANDLE hParentWnd,AnsiString ATempDirectory,int ATimeout);
  ~TFTPFile();
  AnsiString __fastcall GetFile(AnsiString AFileName,AnsiString ServerString,AnsiString ATempDirectory = "");
  void __fastcall IdFTPWorkBegin(TObject *Sender,TWorkMode AWorkMode, const int AWorkCountMax);
  void __fastcall IdFTPWork(TObject *Sender,TWorkMode AWorkMode, const int AWorkCount);
};
#endif
 