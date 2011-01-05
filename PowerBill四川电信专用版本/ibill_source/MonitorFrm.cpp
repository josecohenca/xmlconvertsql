//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MonitorFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzEdit"
#pragma link "RzButton"
#pragma link "RzPanel"
#pragma resource "*.dfm"
TfrmMonitor *frmMonitor;
//---------------------------------------------------------------------------
__fastcall TfrmMonitor::TfrmMonitor(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmMonitor::btnCloseClick(TObject *Sender)
{
  Close();  
}
//---------------------------------------------------------------------------
void __fastcall TfrmMonitor::btnSaveASClick(TObject *Sender)
{
  if(txtMessages->Lines->Count == 0 || !SaveDialog1->Execute())
  {
    return;
  }
  try
  {
    txtMessages->Lines->SaveToFile(SaveDialog1->FileName);
  }
  catch(Exception * E)
  {
    MessageBox(Handle,E->Message.c_str(),"´íÎó",MB_OK | MB_ICONSTOP);
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMonitor::txtMessagesChange(TObject *Sender)
{
  if(txtMessages->Lines->Count > 26)
  {
    txtMessages->ScrollBars = ssBoth;
  }
  else
  {
    txtMessages->ScrollBars = ssNone;
  }
}
//---------------------------------------------------------------------------

