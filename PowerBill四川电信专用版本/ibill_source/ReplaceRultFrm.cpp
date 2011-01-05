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
//替换规则的内部表示方法:
//  FROM|TO|LEFT
//用|线作分隔符.字符串中的|线将被写成XML的ASCII码表示法,即&#124;
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
    Item->SubItems->Add(AnsiReplaceText(AnsiReplaceText(AnsiReplaceText(List->Strings[n],"&#124;","|"),"&#32"," "),"&#13","(回车)"));
    Item->SubItems->Add(AnsiReplaceText(AnsiReplaceText(AnsiReplaceText(List->Strings[++n],"&#124;","|"),"&#32"," "),"&#13","(回车)"));
    s = List->Strings[++n];
    if(s == "RIGHT")
      Item->SubItems->Add("替换源字符串右边");
    else if(s == "ANY")
      Item->SubItems->Add("替换源字符中所有符合条件的字符");
    else
      Item->SubItems->Add("替换源字符串左边");
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
