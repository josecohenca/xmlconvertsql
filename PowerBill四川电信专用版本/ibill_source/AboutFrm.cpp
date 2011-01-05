//---------------------------------------------------------------------------

//#include <vcl.h>
#pragma hdrstop

#include "AboutFrm.h"
#include "RegisterFrm.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CWGIFImage"
#pragma link "RzPanel"
#pragma resource "*.dfm"
TfrmAbout *frmAbout;
//---------------------------------------------------------------------------
__fastcall TfrmAbout::TfrmAbout(TComponent* Owner)
  : TForm(Owner)
{
  /*if(frmMain != NULL && EncryRegisteCode(frmMain->RegistUserName).SubString(1,25) != frmMain->SerialNumber)
  {
    Label2->Caption = "未注册的副本";
  }
  else
  */
  //{
  #ifndef SCTELE_COM_VERSION
    Label2->Caption = "本软件授权给" + frmMain->RegistUserName + "使用";
  #else
    Label2->Caption = "本软件授权给 中国电信 使用";
  #endif
  Label1->Visible = false;
  //}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAbout::Label1Click(TObject *Sender)
{
  TfrmRegister * frmRegister = new TfrmRegister(this);
  frmRegister->ShowModal();
  delete frmRegister;
  if(EncryRegisteCode(frmMain->RegistUserName).SubString(1,25) == frmMain->SerialNumber)
  {
    #ifndef SCTELE_COM_VERSION
      Label2->Caption = "本软件授权给" + frmMain->RegistUserName + "使用";
    #else
      Label2->Caption = "四川电信专用版本 已注册";  
    #endif
    Label1->Visible = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmAbout::FormClick(TObject *Sender)
{
  Close();  
}
//---------------------------------------------------------------------------

void __fastcall TfrmAbout::FormKeyPress(TObject *Sender, char &Key)
{
  if(Key == 27)
  {
    Key = 0;
    Close();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmAbout::Label5Click(TObject *Sender)
{
  ShellExecute(NULL,"open","mailto:lovejlj@yeah.net","","",SW_NORMAL);  
}
//---------------------------------------------------------------------------

