//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SearchFileFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzButton"
#pragma link "RzEdit"
#pragma link "RzSpnEdt"
#pragma link "RzRadChk"
#pragma resource "*.dfm"
TfrmSearchFile *frmSearchFile;
//---------------------------------------------------------------------------
__fastcall TfrmSearchFile::TfrmSearchFile(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSearchFile::btnOkClick(TObject *Sender)
{
  //if(txtFileName->Text == "")
  //  txtFileName->Text = "*.*";
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
