//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "OpenFileThread.h"
#include "MainFrm.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TOpenFileThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TOpenFileThread::TOpenFileThread(bool CreateSuspended)
  : TThread(CreateSuspended)
{
  BillFile     = NULL;
  ErrorMessage = "";
}
//---------------------------------------------------------------------------
void __fastcall TOpenFileThread::Execute()
{
  Continue = true;
  try
  {
    BillFile->OpenBillFile(FileName,BillName);
  }
  catch(Exception * E)
  {
    ErrorMessage = "不能打开文件:" + E->Message;
  }
  SendMessage(hWnd,MSG_THREAD_COMPLETED,0,0);
}
//---------------------------------------------------------------------------
