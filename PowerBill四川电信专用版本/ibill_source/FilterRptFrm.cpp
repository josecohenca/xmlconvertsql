//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FilterRptFrm.h"
#include "MainFrm.h"
#include <Clipbrd.hpp>
#include <stdlib.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzButton"
#pragma link "RzEdit"
#pragma link "RzListVw"
#pragma resource "*.dfm"
TfrmFilterRpt *frmFilterRpt;
//---------------------------------------------------------------------------
__fastcall TfrmFilterRpt::TfrmFilterRpt(TComponent* Owner,TBillFile * ABillFile,AnsiString AFileName)
  : TForm(Owner)
{
  RecordFilter * prf;
  TListItem * Item;
  txtBillName->Text = ABillFile->GetBillName();
  txtFileName->Text = AFileName;
  char ch[50] = {0};
  for(int n = 0;n < ABillFile->FilterList->Count;n++)
  {
    prf = (RecordFilter *)ABillFile->FilterList->Items[n];
    Item = lvReport->Items->Add();
    Item->ImageIndex = 0;
    itoa(prf->Is,&ch[0],16);
    if(strlen(&ch[0]) % 2 == 0)
      Item->SubItems->Add(IntToStr(prf->Is) + "(" + AnsiString(ch).UpperCase() + ")");
    else
      Item->SubItems->Add(IntToStr(prf->Is) + "(0" + AnsiString(ch).UpperCase() + ")");
    Item->SubItems->Add(prf->SkipCount);
    Item->SubItems->Add(prf->Cause);
  }
  Label3->Caption = "��������" + IntToStr(ABillFile->SkipRecordCount) + "����¼";
}
//---------------------------------------------------------------------------
void __fastcall TfrmFilterRpt::btnCapyClick(TObject *Sender)
{
  AnsiString Text = "�������˱���\r\n"\
    "��������:" + txtBillName->Text + "\r\n"\
    "�����ļ�:" + txtFileName->Text + "\r\n"\
    "�����ǹ������\r\n";
  TListItem * Item;
  for(int n = 0;n < lvReport->Items->Count;n++)
  {
    Item = lvReport->Items->Item[n];
    Text += "��־λֵ:" + Item->SubItems->Strings[0] + "\t"\
      "���˼�¼��:" + Item->SubItems->Strings[1] + "\t"\
      "����ԭ��:" + Item->SubItems->Strings[2] + "\r\n";
  }
  Text += Label3->Caption;
  TClipboard * Clipboard = new TClipboard();
  Clipboard->AsText = Text;
  delete Clipboard;
  MessageBox(Handle,"�������˱����Ѹ��Ƶ�������!","��Ϣ",MB_OK | MB_ICONINFORMATION);   
}
//---------------------------------------------------------------------------
