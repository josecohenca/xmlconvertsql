//---------------------------------------------------------------------------

//#include <vcl.h>
#pragma hdrstop

#include "ThreadExportToOra.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TThreadExportToOra::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TThreadExportToOra::TThreadExportToOra(bool CreateSuspended)
  : TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void __fastcall TThreadExportToOra::Execute()
{
  //---- Place thread code here ----
}
//---------------------------------------------------------------------------
