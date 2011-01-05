//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ReplaceRultFrm.h"
#include "MainFrm.h"
#include "public.h"
#include <StrUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzButton"
#pragma link "RzEdit"
#pragma link "RzListVw"
#pragma resource "*.dfm"
TfrmReplaceRule *frmReplaceRule;
//�滻������ڲ���ʾ����:
//  FROM|TO|LEFT
//��|�����ָ���.�ַ����е�|�߽���д��XML��ASCII���ʾ��,��&#124;
//---------------------------------------------------------------------------
__fastcall TfrmReplaceRule::TfrmReplaceRule(TComponent* Owner,AnsiString BillName,AnsiString FieldName,AnsiString AReplayRules)
  : TForm(Owner)
{
  ReplayRules = AReplayRules;
  txtBillName->Text  = BillName;
  txtFieldName->Text = FieldName;
  TStringList * List = new TStringList();
  Split(ReplayRules,"|",List);
  TListItem * Item;
  AnsiString s;
  for(int n = 0;n < List->Count;n++)
  {
    Item = lvReplaceRules->Items->Add();
    Item->SubItems->Add(AnsiReplaceText(AnsiReplaceText(AnsiReplaceText(List->Strings[n],"&#124;","|"),"&#32"," "),"&#13","(�س�)"));
    Item->SubItems->Add(AnsiReplaceText(AnsiReplaceText(AnsiReplaceText(List->Strings[++n],"&#124;","|"),"&#32"," "),"&#13","(�س�)"));
    s = List->Strings[++n];
    if(s == "RIGHT")
      Item->SubItems->Add("�滻Դ�ַ����ұ�");
    else if(s == "ANY")
      Item->SubItems->Add("�滻Դ�ַ������з����������ַ�");
    else
      Item->SubItems->Add("�滻Դ�ַ������");
  }
  delete List;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReplaceRule::PopupMenu1Popup(TObject *Sender)
{
  menuEditRule->Enabled   = lvReplaceRules->Selected != NULL;
  menuDeleteRule->Enabled = menuEditRule->Enabled;
}
//---------------------------------------------------------------------------
