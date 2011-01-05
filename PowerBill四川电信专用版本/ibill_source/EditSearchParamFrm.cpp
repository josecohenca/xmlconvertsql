//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EditSearchParamFrm.h"
#include "MainFrm.h"
#include "public.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzButton"
#pragma link "RzEdit"
#pragma link "RzCmboBx"
#pragma link "RzRadChk"
#pragma link "RzDTP"
#pragma resource "*.dfm"
TfrmEditSearchParam *frmEditSearchParam;
//---------------------------------------------------------------------------
__fastcall TfrmEditSearchParam::TfrmEditSearchParam(TComponent* Owner)
  : TForm(Owner)
{
  dtDate->DateTime = Now();
  dtTime->DateTime = Now();
  cbxOperator->ItemIndex = 0;
  cbxFieldChange(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditSearchParam::FormKeyPress(TObject *Sender,
      char &Key)
{
  switch(Key)
  {
  case VK_RETURN:
    //btnQueryClick(NULL);
    SendMessage(Handle,WM_NEXTDLGCTL,0,0);
    Key = 0;
    break;
  case 27:
    Key = 0;
    ModalResult = mrCancel;
    break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmEditSearchParam::SetSelectedFieldIndex(TBillField * Field)
{
  for(int n = 0;n < cbxField->Items->Count;n++)
  {
    if(Field->FieldName == ((TBillField * )cbxField->Items->Objects[n])->FieldName)
    {
      cbxField->ItemIndex = n;
      cbxFieldChange(NULL);
      return;
    }
  }
  cbxField->ItemIndex = -1;
  cbxFieldChange(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TfrmEditSearchParam::btnQueryClick(TObject *Sender)
{
  if(cbxField->ItemIndex < 0)
  {
    MessageBox(Handle,"请选择要查找的字段!","提示",MB_OK | MB_ICONWARNING);
    cbxField->SetFocus();
    return;
  }
  if(cbxOperator->ItemIndex < 0)
    cbxOperator->ItemIndex = 0;
  TBillField * Field = (TBillField * )cbxField->Items->Objects[cbxField->ItemIndex];
  switch(Field->SortType)
  {
  case SORT_AS_NUMBER:
    if(Field->GetValueTransCount() == 0)
    {
      try
      {
        StrToInt(txtContext->Text);
      }
      catch(...)
      {
        MessageBox(Handle,"请输入一个有效的数字!","提示",MB_OK | MB_ICONWARNING);
        txtContext->SetFocus();
        return;
      }
    }
    break;
  }
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditSearchParam::cbxFieldChange(TObject *Sender)
{
  if(cbxField->ItemIndex < 0)
  {
    dtDate->Visible = false;
    dtTime->Visible = false;
    cbxValues->Visible  = false;
    txtContext->Visible = true;
    txtContext->Enabled = false;
    return;
  }
  TBillField * Field = ((TBillField * )cbxField->Items->Objects[cbxField->ItemIndex]);
  if(Field->GetValueTransCount() > 0)
  {
    dtDate->Visible = false;
    dtTime->Visible = false;
    cbxValues->Visible  = true;
    txtContext->Visible = false;
    txtContext->Enabled = false;
    cbxValues->Items->Clear();
    cbxValues->Items->Assign(Field->ValueTransList);
  }
  else
  {
    switch(Field->SortType)
    {
    case SORT_AS_DATETIME:
      dtDate->Visible = true;
      dtTime->Visible = true;
      cbxValues->Visible  = false;
      txtContext->Visible = false;
      break;
    case SORT_AS_NUMBER:
    case SORT_AS_STR:
      dtDate->Visible = false;
      dtTime->Visible = false;
      cbxValues->Visible  = false;
      txtContext->Visible = true;
      txtContext->Enabled = true;
      break;
    }
  }
}
//---------------------------------------------------------------------------

bool __fastcall TfrmEditSearchParam::FormHelp(WORD Command, int Data,
      bool &CallHelp)
{
    if(Command != HELP_CONTEXTPOPUP)
    {
        CallHelp = false;
        return true;
    }
    WinHelp(Handle,(HelpFilePath + "searchparam.hlp").c_str(),HELP_CONTEXTPOPUP,Data);
    CallHelp = false;
    return true;    
}
//---------------------------------------------------------------------------

