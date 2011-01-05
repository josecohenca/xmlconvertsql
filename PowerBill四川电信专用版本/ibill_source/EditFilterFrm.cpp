//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EditFilterFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzButton"
#pragma link "RzCmboBx"
#pragma link "RzEdit"
#pragma link "RzSpnEdt"
#pragma resource "*.dfm"
TfrmEditFilter *frmEditFilter;
//---------------------------------------------------------------------------
__fastcall TfrmEditFilter::TfrmEditFilter(TComponent* Owner,RecordFilter * AFilter,int ABillRecordLength)
  : TForm(Owner)
{
  cbxAction->Items->AddObject("跳过该条记录",(TObject *)RECORD_FILTER_SKIP);
  BillRecordLength = ABillRecordLength;
  if(AFilter != NULL)
  {
    Filter = AFilter;
    txtPos->Text    = Filter->Pos;
    txtIs->Text     = Filter->Is;
    txtWidth->Text  = Filter->Width;
    txtLength->Text = Filter->Length;
    cbxAction->ItemIndex = cbxAction->Items->IndexOfObject((TObject *)Filter->Action);
    cbxCause->Text = Filter->Cause;
  }
  else
  {
    cbxAction->ItemIndex = 0;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmEditFilter::FormKeyPress(TObject *Sender, char &Key)
{
  if(Key == 27)
  {
    Key = 0;
    ModalResult = mrCancel;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditFilter::btnOkClick(TObject *Sender)
{
  int n;
  try
  {
    n = StrToInt(txtPos->Text);
    if(n < 0 || n > BillRecordLength - 1)
      throw new Exception("");
  }
  catch(...)
  {
    MessageBox(Handle,("位置必须是一个位于0与" + IntToStr(BillRecordLength) + "之间的整数!").c_str(),"提示",MB_OK | MB_ICONWARNING);
    txtPos->SetFocus();
    return;
  }
  try
  {
    n = StrToInt(txtWidth->Text);
    if(n < 1 || n > 4)
      throw new Exception(""); 
  }
  catch(...)
  {
    MessageBox(Handle,"宽度必须是一个位于1与4之间的整数!","提示",MB_OK | MB_ICONWARNING);
    txtWidth->SetFocus();
    return;
  }
  try
  {
    StrToInt(txtIs->Text);
  }
  catch(...)
  {
    MessageBox(Handle,"值必须是一个整数!","提示",MB_OK | MB_ICONWARNING);
    txtIs->SetFocus();
    return;
  }  
  try
  {
    n = StrToInt(txtLength->Text);
    if(n < 0)
      throw new Exception("");
  }
  catch(...)
  {
    MessageBox(Handle,"跳过长度必须是一个大于0的整数!","提示",MB_OK | MB_ICONWARNING);
    txtIs->SetFocus();
    return;
  }
  if(cbxCause->Text == "")
  {
    MessageBox(Handle,"请选择过滤原因!","提示",MB_OK | MB_ICONWARNING);
    cbxCause->SetFocus();
    return;
  }
  if(Filter != NULL)
  {
    Filter->Pos    = StrToInt(txtPos->Text);
    Filter->Width  = StrToInt(txtWidth->Text);
    Filter->Is     = StrToInt(txtIs->Text);
    Filter->Action = (int)cbxAction->Items->Objects[cbxAction->ItemIndex];
    Filter->Length = StrToInt(txtLength->Text);
    strncpy(&Filter->Cause[0],cbxCause->Text.c_str(),100);
  }
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
bool __fastcall TfrmEditFilter::FormHelp(WORD Command, int Data,
      bool &CallHelp)
{
  if(Command != HELP_CONTEXTPOPUP)
  {
    CallHelp = false;
    return true;
  }
  WinHelp(Handle,(HelpFilePath + "filter2.hlp").c_str(),HELP_CONTEXTPOPUP,Data);
  CallHelp = false;
  return true;    
}
//---------------------------------------------------------------------------

