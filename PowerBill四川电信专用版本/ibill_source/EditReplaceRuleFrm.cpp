//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EditReplaceRuleFrm.h"
#include "public.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzButton"
#pragma link "RzEdit"
#pragma link "RzCmboBx"
#pragma link "RzSpnEdt"
#pragma resource "*.dfm"
TfrmEditReplaceRule *frmEditReplaceRule;

//---------------------------------------------------------------------------
__fastcall TfrmEditReplaceRule::TfrmEditReplaceRule(TComponent* Owner,AnsiString AFrom,AnsiString ATo,AnsiString APosition,AnsiString ACount)
  : TForm(Owner)
{
  From = XMLDecode(AFrom);
  To   = XMLDecode(ATo);
  Position = APosition;
  Count = ACount;
  txtFrom->Text = From;
  txtTo->Text   = To;
  txtReplaceCount->Text = Count;
  cbxPosition->ItemIndex = cbxPosition->Items->IndexOf(Position);
  if(cbxPosition->ItemIndex < 0)
    cbxPosition->ItemIndex = 0;
  btnAscii1->Tag = (int)txtFrom;
  btnAscii2->Tag = (int)txtTo;
  cbxPositionChange(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditReplaceRule::btnOkClick(TObject *Sender)
{
  try
  {
    int n = StrToInt(txtReplaceCount->Text);
    if(n < 0)
      throw new Exception("");
  }
  catch(...)
  {
    MessageBox(Handle,"替换次数必须是一个大于-1的整数!","提示",MB_OK | MB_ICONWARNING);
    txtReplaceCount->SetFocus();
    txtReplaceCount->SelectAll();
    return;
  }
  if(txtFrom->Text != From || txtTo->Text != To || cbxPosition->Text != Position || Count != txtReplaceCount->Text)
  {
    From     = XMLEncode(txtFrom->Text);
    To       = XMLEncode(txtTo->Text);
    Position = cbxPosition->Text;
    Count    = txtReplaceCount->Text;
    ModalResult = mrOk;
  }
  else
    ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditReplaceRule::btnAscii1Click(TObject *Sender)
{
  PopupMenu1->Tag = ((TSpeedButton *)Sender)->Tag;
  TPoint p(((TSpeedButton *)Sender)->Left,((TSpeedButton *)Sender)->Top + btnAscii1->Height);
  p = ClientToScreen(p);  
  PopupMenu1->Popup(p.x,p.y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditReplaceRule::N1Click(TObject *Sender)
{
  TRzEdit * Edit = (TRzEdit *)PopupMenu1->Tag;
  Edit->SelText  = "[回车符]";   
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditReplaceRule::N2Click(TObject *Sender)
{
  TRzEdit * Edit = (TRzEdit *)PopupMenu1->Tag;
  Edit->SelText  = "[换行符]";   
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditReplaceRule::ASCII91Click(TObject *Sender)
{
  TRzEdit * Edit = (TRzEdit *)PopupMenu1->Tag;
  Edit->SelText  = "[制表符]";   
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditReplaceRule::cbxPositionChange(TObject *Sender)
{
  txtReplaceCount->Enabled = cbxPosition->ItemIndex != 2;  
}
//---------------------------------------------------------------------------
bool __fastcall TfrmEditReplaceRule::FormHelp(WORD Command, int Data,
      bool &CallHelp)
{
  if(Command != HELP_CONTEXTPOPUP)
  {
    CallHelp = false;
    return true;
  }
  WinHelp(Handle,(HelpFilePath + "ReplaceRule2.hlp").c_str(),HELP_CONTEXTPOPUP,Data);
  CallHelp = false;
  return true;   
}
//---------------------------------------------------------------------------

