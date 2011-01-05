//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RecordFilterFrm.h"
#include "EditFilterFrm.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzButton"
#pragma link "RzEdit"
#pragma link "RzListVw"
#pragma resource "*.dfm"
TfrmRecordFilter *frmRecordFilter;
//---------------------------------------------------------------------------
__fastcall TfrmRecordFilter::TfrmRecordFilter(TComponent* Owner,TList * AFilterList,int ABillRecordLength)
  : TForm(Owner)
{
  FilterList = AFilterList;
  for(int n = 0;n < FilterList->Count;n++)
  {
    FillFilterList(NULL,(RecordFilter *)FilterList->Items[n]);
  }
  BillRecordLength = ABillRecordLength;
  Changed = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecordFilter::FormKeyPress(TObject *Sender, char &Key)
{
  if(Key == 27)
  {
    Key = 0;
    ModalResult = mrCancel;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecordFilter::lvFiltersDblClick(TObject *Sender)
{
  TListItem * Item = lvFilters->Selected;
  if(Item == NULL)
    return;
  RecordFilter rf = {0};
  rf.Pos    = StrToInt(Item->SubItems->Strings[0]);
  rf.Width  = StrToInt(Item->SubItems->Strings[1]);
  rf.Is     = StrToInt(Item->SubItems->Strings[2]);
  if(Item->SubItems->Strings[3] == "跳过该记录")
    rf.Action = RECORD_FILTER_SKIP;
  rf.Length = StrToInt(Item->SubItems->Strings[4]);
  strncpy(&rf.Cause[0],Item->SubItems->Strings[5].c_str(),100);
  TfrmEditFilter * frmEditFilter = new TfrmEditFilter(this,&rf,BillRecordLength);
  if(frmEditFilter->ShowModal() == mrOk)
  {
    FillFilterList(Item,&rf);
    Changed = true;
  }
  delete frmEditFilter;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecordFilter::FillFilterList(TListItem * Item,RecordFilter * Filter)
{
  if(Item == NULL)
  {
    Item = lvFilters->Items->Add();
    Item->SubItems->Add(Filter->Pos);
    Item->SubItems->Add(Filter->Width);
    Item->SubItems->Add(Filter->Is);
    switch(Filter->Action)
    {
    case RECORD_FILTER_SKIP:
      Item->SubItems->Add("跳过该记录");
      break;
/*    case RECORD_FILTER_READ:
      Item->SubItems->Add("读取该记录");
      break;*/
    }
    Item->SubItems->Add(Filter->Length);
    Item->SubItems->Add(Filter->Cause);
    Item->ImageIndex = 0; 
  }
  else
  {
    Item->SubItems->Strings[0] = Filter->Pos;
    Item->SubItems->Strings[1] = Filter->Width;
    Item->SubItems->Strings[2] = Filter->Is;
    switch(Filter->Action)
    {
    case RECORD_FILTER_SKIP:
      Item->SubItems->Strings[3] = "跳过该记录";
      break;
/*    case RECORD_FILTER_READ:
      Item->SubItems->Strings[3] = "读取该记录";
      break;*/
    }
    Item->SubItems->Strings[4] = Filter->Length;
    Item->SubItems->Strings[5] = Filter->Cause;
  }
}

void __fastcall TfrmRecordFilter::menuAddNewClick(TObject *Sender)
{
  TfrmEditFilter * frmEditFilter = new TfrmEditFilter(this,NULL,BillRecordLength);
  if(frmEditFilter->ShowModal() == mrOk)
  {
    RecordFilter rf = {0};
    rf.Pos    = StrToInt(frmEditFilter->txtPos->Text);
    rf.Width  = StrToInt(frmEditFilter->txtWidth->Text);
    rf.Is     = StrToInt(frmEditFilter->txtIs->Text);
    rf.Action = (int)(frmEditFilter->cbxAction->Items->Objects[frmEditFilter->cbxAction->ItemIndex]);
    rf.Length = StrToInt(frmEditFilter->txtLength->Text);
    strncpy(&rf.Cause[0],frmEditFilter->cbxCause->Text.c_str(),100);
    FillFilterList(NULL,&rf);
    Changed = true;
  }
  delete frmEditFilter;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecordFilter::btnOkClick(TObject *Sender)
{
  if(Changed)
  {
    for(int n = 0;n < FilterList->Count;n++)
    {
      delete (RecordFilter *)FilterList->Items[n];
    }
    FilterList->Clear();
    RecordFilter * prf;
    TListItem * Item;
    for(int n = 0;n < lvFilters->Items->Count;n++)
    {
      Item = lvFilters->Items->Item[n];
      prf = new RecordFilter;
      memset(prf,0,sizeof(RecordFilter));
      prf->Pos    = StrToInt(Item->SubItems->Strings[0]);
      prf->Width  = StrToInt(Item->SubItems->Strings[1]);
      prf->Is     = StrToInt(Item->SubItems->Strings[2]);
      if(Item->SubItems->Strings[3] == "跳过该记录")
        prf->Action = RECORD_FILTER_SKIP;
      //else if(Item->SubItems->Strings[3] == "读取该记录")
      //  prf->Action = RECORD_FILTER_READ;
      prf->Length = StrToInt(Item->SubItems->Strings[4]);
      strncpy(&prf->Cause[0],Item->SubItems->Strings[5].c_str(),100);
      FilterList->Add((TObject *)prf);
    }
    ModalResult = mrOk;
  }
  else
  {
    ModalResult = mrCancel;
  }
}
//---------------------------------------------------------------------------



void __fastcall TfrmRecordFilter::menuDeleteClick(TObject *Sender)
{
  TListItem * Item = lvFilters->Selected;
  if(Item != NULL)
  {
    lvFilters->Items->Delete(Item->Index);
  }
}
//---------------------------------------------------------------------------

bool __fastcall TfrmRecordFilter::FormHelp(WORD Command, int Data,
      bool &CallHelp)
{
  if(Command != HELP_CONTEXTPOPUP)
  {
    CallHelp = false;
    return true;
  }
  WinHelp(Handle,(HelpFilePath + "filter1.hlp").c_str(),HELP_CONTEXTPOPUP,Data);
  CallHelp = false;
  return true;   
}
//---------------------------------------------------------------------------

