//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FTPWatchThread.h"
#include "FTPFile.h"
#include <DateUtils.hpp>

#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TFTPWatchThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TFTPWatchThread::TFTPWatchThread(bool CreateSuspended)
  : TThread(CreateSuspended)
{
  FTPFile  = NULL;
  Continue = true;
}
//---------------------------------------------------------------------------
void __fastcall TFTPWatchThread::Execute()
{
  //LastStartTime = Now();
  Continue = true;
  while(Continue)
  {
    Sleep((unsigned int)1000);
    if(Timeout > 0)
    {
      //fprintf(file,"监视线程工作:%d\n",abs(SecondsBetween(LastStartTime,Now())));
      if(abs(SecondsBetween(LastStartTime,Now())) > Timeout)
      {
        //fprintf(file,"监视线程工作_超时:%d\n",abs(SecondsBetween(LastStartTime,Now())));
        ((TFTPFile * )FTPFile)->LastErrorMessage = "操作超时";
        try
        {
          ((TFTPFile * )FTPFile)->IdFTP->Abort();
          ((TFTPFile * )FTPFile)->IdFTP->Quit();
        }
        catch(...)
        {
        }
      }
      //fflush(file);
    }
  }
}
//---------------------------------------------------------------------------
