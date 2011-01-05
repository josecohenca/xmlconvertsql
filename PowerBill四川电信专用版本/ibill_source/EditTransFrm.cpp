//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EditTransFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzButton"
#pragma link "RzCmboBx"
#pragma link "RzEdit"
#pragma link "RzPanel"
#pragma link "RzSpnEdt"
#pragma resource "*.dfm"
TfrmEditTrans *frmEditTrans;
//---------------------------------------------------------------------------
__fastcall TfrmEditTrans::TfrmEditTrans(TComponent* Owner,AnsiString AValue,AnsiString AContext)
  : TForm(Owner)
{
  Value   = AValue;
  Context = AContext;
  txtValue->Text   = Value;
  txtContext->Text = Context;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditTrans::btnOkClick(TObject *Sender)
{
  try
  {
    StrToInt(txtValue->Text);
  }
  catch(...)
  {
    MessageBox(Handle,"值必须是一个整数!","提示",MB_OK | MB_ICONWARNING);
    txtValue->SetFocus();
    txtValue->SelectAll();
    return;
  }
  if(txtValue->Text == Value && txtContext->Text == Context)
    ModalResult = mrCancel;
  else
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------
