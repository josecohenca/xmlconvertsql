//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ThreadSubmitToDB.h"
#include <StrUtils.hpp>
#include "public.h"
#include "DBConfig.h"

#pragma package(smart_init)
//�������ύ�����ݿ�
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TThreadSubmitToDB::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TThreadSubmitToDB::TThreadSubmitToDB(bool CreateSuspended)
  : TThread(CreateSuspended)
{
  ErrorMessage   = "";
  SQLCommandList = NULL;
  MaxMessageLine = 20;
  MaxError = "0";
}
//---------------------------------------------------------------------------
void __fastcall TThreadSubmitToDB::Execute()
{
  //---- Place thread code here ----
  switch(DBType)
  {
  case DB_ORACLE:
    SubmitToOracle();
    break;
  case DB_SQLSERVER:
    SubmitToSQLServer();
    break;
  case DB_MYSQL:
    SubmitToMySQL();
    break;
  default:
    break;
  }
  if(hWnd != NULL)
    SendMessage(hWnd,MSG_THREAD_COMPLETED,(WPARAM)this,0);
}
bool __fastcall TThreadSubmitToDB::SubmitToMySQL()
{
  SECURITY_ATTRIBUTES se = {0};
  se.nLength = sizeof(se);

  se.bInheritHandle       = true;
  se.lpSecurityDescriptor = NULL;
  CreatePipe(&hReadPipe,&hWritePipe,&se,0);
  STARTUPINFO si = {0};
  si.cb = sizeof(si);
  si.dwFlags     = STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
  si.wShowWindow = SW_HIDE;
  si.hStdOutput  = hWritePipe;
  si.hStdError   = hWritePipe;
  memset(&pi,sizeof(pi),0);
  //AnsiString Text;
  //mysqlimport -prl --fields-enclosed-by= --fields-terminated-by=,
  //  mysql --local c:\hdhd_test_bills1.tmp --user=root --port=3306 --password=root --host=127.0.0.1
  AnsiString Command = ToolPath + " -prl --fields-enclosed-by= --fields-terminated-by=, "
    + DataBaseName + " --local " + TempFileName +
    " --user=" + DBUserName + " --password=" + DBPassword + " --host=" + DBServer +
    " --port=" + ServerPort;
  MessageTextList->Add("ִ������:" + ToolPath + " -prl --fields-enclosed-by= --fields-terminated-by=, "
    + DataBaseName + " --local " + TempFileName +
    " --user=" + DBUserName + " --password=******* --host=" + DBServer +
    " --port=" + ServerPort);
  if(SlentMode)
  {
    printf("%s\n","ִ������:" + ToolPath + " -prl --fields-enclosed-by= --fields-terminated-by=, "
    + DataBaseName + " " + TempFileName +
    " --user=" + DBUserName + " --password=******* --host=" + DBServer +
    " --port=" + ServerPort);
  }
  if(CreateProcess(NULL,Command.c_str(),&se,&se,true,NORMAL_PRIORITY_CLASS,NULL,WorkDir.c_str(),&si,&pi))
  {
      bool bContinue = CloseHandle(hWritePipe);
      hWritePipe = NULL;
      char chBuffer[1025];
      unsigned long Bytesread;
      AnsiString Text;
      MessageTextList->Add("MySQL�������ƽ���������,��ȴ�...");
      while(bContinue)
      {
          bContinue = ReadFile(hReadPipe,&chBuffer[0],1024,&Bytesread,NULL);
          try
          {
            chBuffer[Bytesread]   = 0;
            if(strlen(&chBuffer[0]) > 0)
            {
                Text = chBuffer;
            }
            if(Text != ""&&UpperCase(MessageTextList->Text).Pos("RECORDS") < 1)
            {
              MessageTextList->Add(Text);
            }
          }
          catch(...)
          {
          }
          chBuffer[0] = 0;
      }
      WaitForSingleObject(pi.hProcess,INFINITE);
      CloseHandle(pi.hProcess);
      CloseHandle(pi.hThread);
      CloseHandle(hReadPipe);
      pi.hProcess = NULL;
      hReadPipe = NULL;
      if(UpperCase(MessageTextList->Text).Pos("RECORDS") < 1)
      {
        ErrorMessage = "�ύ����ʧ��,��鿴״̬�����еĳ�����Ϣ.";
        return false;
      }
      else
      {
        SendMessage(hWnd,MSG_SET_PROCESS_POS,TotalRecordCount,0);
        int k = 0;
        int n;
        for(int n = MessageTextList->Count - 1;n > -1;n--)
        {
          Text = MessageTextList->Strings[n];
          int pos  = Text.Pos("Records: ");
          if(pos > 0)
          {
            k = StrToIntEx(Text.SubString(pos + strlen("Records: "),Text.Length() - strlen("Records: ")).c_str());
            if(TotalRecordCount - k > StrToInt(MaxError))
            {
              ErrorMessage = "�ύ������������ת����������һ��,�Ҳ��쳬��������Ĵ���Χ(���" + MaxError + ").ת������������Ϊ:" +
                IntToStr(TotalRecordCount) +
                ",ʵ���ύ������Ϊ:" + IntToStr(k) + ".";
            }
            break;
          }
        }
        if(n == -1)
        {
          ErrorMessage = "�Ҳ���MySQLImport�ؼ���(Records).����ʧ��.";
        }
        //�������ɹ�,�����ִ��SQL���
        if(ErrorMessage == "" && SQLCommandList != NULL)
        {
          for(int n = 0;n < SQLCommandList->Count;n++)
          {
            try
            {
              if(SlentMode)
              {
                printf("����ִ�����:%s\n",SQLCommandList->Strings[n]);
              }
              MessageTextList->Add("����ִ�����:" + SQLCommandList->Strings[n]);
              ADOConnection->Execute(SQLCommandList->Strings[n]);
            }
            catch(...){}
          }
        }
        try
        {
          MessageTextList->Add("*********************************�����ύ����*********************************");
          MessageTextList->Add("���ʱ��:" + FormatDateTime("yyyy-mm-dd hh:nn:ss",Now()) + ".");
          MessageTextList->Add("Ŀ���:" + DestTableName);
          MessageTextList->Add("��ʱ�ļ�:" + TempFileName);
          MessageTextList->Add("��ת����" + IntToStr(TotalRecordCount) + "����¼,������" + IntToStr(k) + "����¼.");
        }
        catch(...)
        {
        }
        return true;
      }
  }
  else
  {
    CloseHandle(hWritePipe);
    CloseHandle(hReadPipe);
    ErrorMessage = "����MySQLImport����ʧ��.�����Ƿ�װ��MySQL�ͻ��������ibill.ini��[ExternalTools]��MySQLImport�Ƿ�������ȷ.";
    return false;
  }
}
//---------------------------------------------------------------------------
#define BCP_KEY_WORD1 "�ϼ��ͳ�: "
#define BCP_KEY_WORD2 "�Ѹ����� "
#define BCP_KEY_WORD3 " �С�"
int  __fastcall TThreadSubmitToDB::GetRowsForBCP(const AnsiString  & Text)
{
  int Result = -1;
  int pos = StrRScan(Text.c_str(),BCP_KEY_WORD1);
  if(pos > -1)
  {
    Result = StrToIntEx(Text.SubString(pos + strlen(BCP_KEY_WORD1) + 1,
      Text.Length() - pos - strlen(BCP_KEY_WORD1) - 1).c_str());
  }
  return Result;
}
//---------------------------------------------------------------------------
bool __fastcall TThreadSubmitToDB::SubmitToSQLServer()
{
  SECURITY_ATTRIBUTES se = {0};
  se.nLength = sizeof(se);

  se.bInheritHandle       = true;
  se.lpSecurityDescriptor = NULL;
  CreatePipe(&hReadPipe,&hWritePipe,&se,0);
  STARTUPINFO si = {0};
  si.cb = sizeof(si);
  si.dwFlags     = STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
  si.wShowWindow = SW_HIDE;
  si.hStdOutput  = hWritePipe;
  si.hStdError   = hWritePipe;
  memset(&pi,sizeof(pi),0);
  //AnsiString Text;
  //bcp pubs..hdhd_test_bills1 in c:\convert.tmp -Usa -Pwbzx9801 -S133.57.9.21 -fc:\convert.fmt
  AnsiString Command =
    ToolPath + "  " + DataBaseName + ".." + DestTableName + " in " + TempFileName +
    " -U" + DBUserName + " -P" + DBPassword + " -S" + DBServer +
    " -f" + ExtractFilePath(TempFileName) + DestTableName + ".fmt" +
    " -m" + MaxError;
  MessageTextList->Add("ִ������:" + ToolPath + "  " + DataBaseName + ".." + DestTableName + " in " + TempFileName +
    " -U" + DBUserName + " -P******** -S" + DBServer +
    " -f" + ExtractFilePath(TempFileName) + DestTableName + ".fmt" +
    " -m" + MaxError);
  if(SlentMode)
  {
    printf("%s\n","ִ������:" + ToolPath + "  " + DataBaseName + ".." + DestTableName + " in " + TempFileName +
    " -U" + DBUserName + " -P******** -S" + DBServer +
    " -f" + ExtractFilePath(TempFileName) + DestTableName + ".fmt" +
    " -m" + MaxError);
  }
  if(CreateProcess(NULL,Command.c_str(),&se,&se,true,NORMAL_PRIORITY_CLASS,NULL,WorkDir.c_str(),&si,&pi))
  {
      bool bContinue = CloseHandle(hWritePipe);
      hWritePipe = NULL;
      char chBuffer[2049];
      unsigned long Bytesread;
      AnsiString Text;
      int pos = 0;
      while(bContinue)
      {
          bContinue = ReadFile(hReadPipe,&chBuffer[0],2048,&Bytesread,NULL);
          try
          {
            chBuffer[Bytesread]   = 0;
            if(strlen(&chBuffer[0]) > 0)
            {
                Text = chBuffer;
            }
            //MessageBox(0,Text.c_str(),"",MB_OK);
            if(Text != "")
            {
              //if(MessageTextList->Count > MaxMessageLine)
              //  MessageTextList->Clear();
              if(SlentMode)
                printf("%s\n",Text);
              MessageTextList->Add(Text);
            }
            pos = GetRowsForBCP(Text);
            if(pos > -1)
            {
              SendMessage(hWnd,MSG_SET_PROCESS_POS,pos,0);
            }
            if(Text.Pos("ʱ��ʱ��") > 0)
              break;
          }
          catch(...)
          {
          }
          chBuffer[0] = 0;
      }
      WaitForSingleObject(pi.hProcess,INFINITE);
      CloseHandle(pi.hProcess);
      CloseHandle(pi.hThread);
      CloseHandle(hReadPipe);
      pi.hProcess = NULL;
      hReadPipe = NULL;
      if(UpperCase(MessageTextList->Text).Pos("ERROR") > 0)
      {
        ErrorMessage = "�ύ����ʧ��,��鿴״̬�����еĳ�����Ϣ.";
        return false;
      }
      else
      {
        SendMessage(hWnd,MSG_SET_PROCESS_POS,TotalRecordCount,0);
        int n;
        for(n = MessageTextList->Count - 1;n > -1;n--)
        {
          Text = MessageTextList->Strings[n];
          pos = Text.Pos(BCP_KEY_WORD2);
          if(pos > 0)
          {
            pos = StrToIntEx(Text.SubString(pos + strlen(BCP_KEY_WORD2),Text.Length() - strlen(BCP_KEY_WORD2)).c_str());
            if(TotalRecordCount - pos > StrToInt(MaxError))
            {
              ErrorMessage = "�ύ������������ת����������һ��,�Ҳ��쳬��������Ĵ���Χ(���" + MaxError + ").ת������������Ϊ:" +
                IntToStr(TotalRecordCount) +
                ",ʵ���ύ������Ϊ:" + IntToStr(pos) + ".";
            }
            break;
          }
        }
        if(n == -1)
        {
          ErrorMessage = "�Ҳ���BCP�ؼ���,����ʧ��.";
        }
        //�������ɹ�,�����ִ��SQL���
        if(ErrorMessage == "" && SQLCommandList != NULL)
        {
          for(int n = 0;n < SQLCommandList->Count;n++)
          {
            try
            {
              if(SlentMode)
              {
                printf("����ִ�����:%s\n",SQLCommandList->Strings[n]);
              }
              MessageTextList->Add("����ִ�����:" + SQLCommandList->Strings[n]);
              ADOConnection->Execute(SQLCommandList->Strings[n]);
            }
            catch(...){}
          }
        }
        try
        {
          MessageTextList->Add("*********************************�����ύ����*********************************");
          MessageTextList->Add("���ʱ��:" + FormatDateTime("yyyy-mm-dd hh:nn:ss",Now()) + ".");
          MessageTextList->Add("Ŀ���:" + DestTableName);
          MessageTextList->Add("��ʱ�ļ�:" + TempFileName);
          MessageTextList->Add("��ת����" + IntToStr(TotalRecordCount) + "����¼,������" + IntToStr(pos) + "����¼.");
        }
        catch(...)
        {
        }
        return true;
      }
  }
  else
  {
    CloseHandle(hWritePipe);
    CloseHandle(hReadPipe);
    ErrorMessage = "����BCP����ʧ��.�����Ƿ�װ��SQLServer�ͻ��������ibill.ini��[ExternalTools]��bcppath�Ƿ�������ȷ.";
    return false;
  }
}
//---------------------------------------------------------------------------
#define SQLLDR_KEY_WORD1 "�ﵽ�ύ�㣬�߼���¼����"
int  __fastcall TThreadSubmitToDB::GetRowsForSQLLDR(const AnsiString  & Text)
{
  int Result = -1;
  if(Text.Pos(SQLLDR_KEY_WORD1) > 0)
  {
    Result = StrToIntEx(Text.SubString(strlen(SQLLDR_KEY_WORD1) + 1,Text.Length() - strlen(SQLLDR_KEY_WORD1) + 1).c_str());
  }
  return Result;

}
//---------------------------------------------------------------------------
bool __fastcall TThreadSubmitToDB::SubmitToOracle()
{
  SECURITY_ATTRIBUTES se = {0};
  se.nLength = sizeof(se);

  se.bInheritHandle       = true;
  se.lpSecurityDescriptor = NULL;
  CreatePipe(&hReadPipe,&hWritePipe,&se,0);
  STARTUPINFO si = {0};
  si.cb = sizeof(si);
  si.dwFlags     = STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
  si.wShowWindow = SW_HIDE;
  si.hStdOutput  = hWritePipe;
  si.hStdError   = hWritePipe;
  memset(&pi,sizeof(pi),0);
  if(CreateProcess(NULL,(ToolPath + " " + DBUserName + "/" + DBPassword + "@" + DBServer +
    " control=\"" + TempFileName + "\" errors=" + MaxError).c_str(),&se,&se,true,NORMAL_PRIORITY_CLASS,NULL,WorkDir.c_str(),&si,&pi))
  {
      bool bContinue = CloseHandle(hWritePipe);
      hWritePipe = NULL;
      char chBuffer[2049];
      unsigned long Bytesread;
      AnsiString Text;
      int pos = 0;
      while(bContinue)
      {
          bContinue = ReadFile(hReadPipe,&chBuffer[0],2048,&Bytesread,NULL);
          try
          {
            chBuffer[Bytesread]   = 0;
            if(strlen(&chBuffer[0]) > 0)
            {
                Text = chBuffer;
            }
            //Text = AnsiReplaceStr(AnsiReplaceStr(Text,"\r",""),"\n","");
            if(Text != "")
            {
              if(MessageTextList->Count > MaxMessageLine)
                MessageTextList->Clear();
              if(SlentMode)
              {
                printf("%s\n",Text);
              }
              MessageTextList->Add(Text);
            }
            pos = GetRowsForSQLLDR(Text);
            if(pos > -1)
            {
              SendMessage(hWnd,MSG_SET_PROCESS_POS,pos,0);
            }
          }
          catch(...)
          {
          }
      }
      WaitForSingleObject(pi.hProcess,INFINITE);
      CloseHandle(pi.hProcess);
      pi.hProcess = NULL;
      CloseHandle(pi.hThread);
      CloseHandle(hReadPipe);
      hReadPipe = NULL;
      if(MessageTextList->Text.Pos("SQL*Loader-") > 0)
      {
        ErrorMessage = "�ύ����ʧ��,��鿴״̬�����еĳ�����Ϣ.";
        return false;
      }
      else
      {
        int n;
        AnsiString str;// = "�߼���¼����";
        /*for(n = MessageTextList->Count - 1;n > -1;n--)
        {
          Text = MessageTextList->Strings[n];
          pos = Text.Pos(str);
          if(pos > 0)
          {
            pos = StrToIntEx(Text.SubString(pos + str.Length(),Text.Length() - str.Length() + 1).c_str());
            SendMessage(hWnd,MSG_SET_PROCESS_POS,pos,0);
            if(pos != TotalRecordCount)
            {
              ErrorMessage = "�ύ������������ת����������һ��.ת������������Ϊ:" +
                IntToStr(TotalRecordCount) + ",ʵ���ύ������Ϊ:" + IntToStr(pos) + ".";
            }
            break;
          }
        }
        if(n == -1)
        {
          ErrorMessage = "�Ҳ���SQLLDR�ؼ���.����ʧ��.";
        }*/
        //������־�ļ�
        TStringList * LogFile = new TStringList;
        LogFile->LoadFromFile(ExtractFilePath(Application->ExeName) + DestTableName + ".log");
        //if(LogFile->Pos("
        str = "�ɹ�";
        for(n = LogFile->Count - 1;n > -1;n--)
        {
          Text = LogFile->Strings[n];
          int pos1 = Text.Pos(str);
          if(pos1 > 0)
          {
            pos1 = StrToIntEx(Text.c_str());//Text.SubString(pos1 + str.Length(),Text.Length() - str.Length() + 1).c_str());
            if(TotalRecordCount - pos1 > StrToInt(MaxError))
            {
              ErrorMessage = "����" + IntToStr(TotalRecordCount) + "����Ҫ����,��ʵ����ֻ������" + IntToStr(pos1) + "��.";
            }
            break;
          }
        }
        if(n == -1)
        {
          ErrorMessage = "�Ҳ���SQLLDR�ؼ���,�޷�������־.";
        }
        delete LogFile;
        //�������ɹ�,�����ִ��SQL���
        if(ErrorMessage == "" && SQLCommandList != NULL)
        {
          for(int n = 0;n < SQLCommandList->Count;n++)
          {
            try
            {
              if(SlentMode)
              {
                printf("����ִ�����:%s\n",SQLCommandList->Strings[n]);
              }
              MessageTextList->Add("����ִ�����:" + SQLCommandList->Strings[n]);
              ADOConnection->Execute(SQLCommandList->Strings[n]);
            }
            catch(...){}
          }
        }
        //MessageTextList->Clear();
        try
        {
          for(int n = 0;n < 10;n++)
          {
            try
            {
              MessageTextList->LoadFromFile(ExtractFilePath(Application->ExeName) + DestTableName + ".log");
              break;
            }
            catch(...)
            {
            }
          }
          MessageTextList->Insert(0,"*********************************�����ύ����*********************************");
          MessageTextList->Insert(1,"���ʱ��:" + FormatDateTime("yyyy-mm-dd hh:nn:ss",Now()) + ".");
          MessageTextList->Insert(2,"Ŀ���:" + DestTableName);
          MessageTextList->Insert(3,"��ʱ�ļ�:" + TempFileName);
          MessageTextList->Insert(4,"��ת����" + IntToStr(TotalRecordCount) + "����¼,������" + IntToStr(pos) + "����¼.");
          MessageTextList->Insert(5,"������Oracle �� SQLLDR ���ɵı���:");
        }
        catch(...)
        {
        }
        return true;
      }
  }
  else
  {
    CloseHandle(hWritePipe);
    CloseHandle(hReadPipe);
    ErrorMessage = "����SQLLDR����ʧ��.�����Ƿ�װ��Oracle�ͻ��������ibill.ini��[ExternalTools]��sqlldrpath�Ƿ�������ȷ.";
    return false;
  }
}
