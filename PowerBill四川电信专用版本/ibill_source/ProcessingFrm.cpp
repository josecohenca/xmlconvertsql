//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ProcessingFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPrgres"
#pragma link "RzButton"
#pragma resource "*.dfm"
TfrmProcessing *frmProcessing;
//---------------------------------------------------------------------------
__fastcall TfrmProcessing::TfrmProcessing(TComponent* Owner)
  : TForm(Owner)
{
  CloseByUser = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmProcessing::CloseWindow(TModalResult Result)
{
  ModalResult = Result;
  CloseByUser = false;
  Close();
}

void __fastcall TfrmProcessing::FormKeyPress(TObject *Sender, char &Key)
{
  if(Key == 27)
  {
    Key = 0;
    ModalResult = mrCancel;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmProcessing::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  if(CloseByUser)
  {
    if(MessageBox(Handle,"您要放弃当前的操作吗?","问题",MB_YESNO | MB_ICONQUESTION) == IDNO)
    {
      ModalResult = mrNone;
      Action = caNone;
    }
  }
}
//---------------------------------------------------------------------------
