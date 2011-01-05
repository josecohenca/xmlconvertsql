//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ReplaceRuleFrm.h"
#include "MainFrm.h"
#include "public.h"
#include "EditReplaceRuleFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzButton"
#pragma link "RzEdit"
#pragma link "RzListVw"
#pragma resource "*.dfm"
TfrmReplaceRule * frmReplaceRule;
//替换规则的内部表示方法:
//  FROM|TO|LEFT|COUNT
//用|线作分隔符.字符串中的|线将被写成XML的ASCII码表示法,即&#124;
//---------------------------------------------------------------------------
__fastcall TfrmReplaceRule::TfrmReplaceRule(TComponent* Owner,AnsiString BillName,AnsiString FieldName,AnsiString AReplayRules,bool ReadOnly)
  : TForm(Owner)
{
  ReplayRules = AReplayRules;
  //ShowMessage(ReplayRules);
  txtBillName->Text  = BillName;
  txtFieldName->Text = FieldName;
  TStringList * List = new TStringList();
  Split(ReplayRules,"|",List);
  TListItem * Item;
  AnsiString s;
  for(int n = 0;n < List->Count;n++)
  {
    Item = lvReplaceRules->Items->Add();
    Item->SubItems->Add(List->Strings[n]);
    Item->SubItems->Add(List->Strings[++n]);
    s = List->Strings[++n];
    if(s == "RIGHT")
      Item->SubItems->Add("替换源字符串右边");
    else if(s == "ANY")
      Item->SubItems->Add("替换源字符中所有符合条件的字符");
    else
      Item->SubItems->Add("替换源字符串左边");
    Item->SubItems->Add(List->Strings[++n]);
  }
  delete List;
  if(ReadOnly)
  {
    lvReplaceRules->PopupMenu  = NULL;
    lvReplaceRules->OnDblClick = NULL;
    btnOk->Visible = false;
    btnCancel->Caption = "关闭";    
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmReplaceRule::PopupMenu1Popup(TObject *Sender)
{
  menuEditRule->Enabled   = lvReplaceRules->Selected != NULL;
  menuDeleteRule->Enabled = menuEditRule->Enabled;
}
//---------------------------------------------------------------------------
void __fastcall TfrmReplaceRule::menuEditRuleClick(TObject *Sender)
{
  TListItem * Item = lvReplaceRules->Selected;
  if(Item == NULL)
    return;
  TfrmEditReplaceRule * frmEditReplaceRule = new TfrmEditReplaceRule(this,Item->SubItems->Strings[0],Item->SubItems->Strings[1],
    Item->SubItems->Strings[2],Item->SubItems->Strings[3]);
  if(frmEditReplaceRule->ShowModal() == mrOk)
  {
    Item->SubItems->Strings[0] = frmEditReplaceRule->From; 
    Item->SubItems->Strings[1] = frmEditReplaceRule->To;
    Item->SubItems->Strings[2] = frmEditReplaceRule->Position;
    Item->SubItems->Strings[3] = frmEditReplaceRule->Count; 
  }
  delete frmEditReplaceRule;
}
//---------------------------------------------------------------------------


void __fastcall TfrmReplaceRule::btnOkClick(TObject *Sender)
{
  AnsiString s;
  AnsiString t;
  TListItem * Item;
  for(int n = 0;n < lvReplaceRules->Items->Count;n++)
  {
    Item = lvReplaceRules->Items->Item[n];
    s += Item->SubItems->Strings[0] + "|" + Item->SubItems->Strings[1] + "|";
    t = Item->SubItems->Strings[2];
    if(t == "替换源字符串右边")
      s += "RIGHT|";
    else if(t == "替换源字符中所有符合条件的字符")
      s += "ANY|";
    else
      s += "LEFT|";
    s += Item->SubItems->Strings[3] + "|";
  }
  if(s != ReplayRules)
  {
    ReplayRules = s;
    ModalResult = mrOk;
  }
  else
    ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TfrmReplaceRule::menuDeleteRuleClick(TObject *Sender)
{
  TListItem * Item = lvReplaceRules->Selected;
  if(Item == NULL)
    return;
  if(MessageBox(Handle,"您要删除选定的替换规则吗?","警告",MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2) == IDNO)
    return;
  lvReplaceRules->Items->Delete(Item->Index);
}
//---------------------------------------------------------------------------

void __fastcall TfrmReplaceRule::menuAddRuleClick(TObject *Sender)
{
  TfrmEditReplaceRule * frmEditReplaceRule = new TfrmEditReplaceRule(this,"","","","1");
  if(frmEditReplaceRule->ShowModal() == mrOk)
  {
    TListItem * Item = lvReplaceRules->Items->Add();
    Item->SubItems->Add(frmEditReplaceRule->From);
    Item->SubItems->Add(frmEditReplaceRule->To);
    Item->SubItems->Add(frmEditReplaceRule->Position);
    Item->SubItems->Add(frmEditReplaceRule->Count);
  }
  delete frmEditReplaceRule;  
}
//---------------------------------------------------------------------------

void __fastcall TfrmReplaceRule::lvReplaceRulesDragOver(TObject *Sender,
      TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
  TListItem * DestItem = lvReplaceRules->GetItemAt(X,Y);
  Accept = DestItem != NULL && DestItem != lvReplaceRules->Selected;
}
//---------------------------------------------------------------------------

void __fastcall TfrmReplaceRule::lvReplaceRulesDragDrop(TObject *Sender,
      TObject *Source, int X, int Y)
{
  TListItem * DestItem = lvReplaceRules->GetItemAt(X,Y);
  TListItem * Item     = lvReplaceRules->Selected;
  TListItem * NewItem  = lvReplaceRules->Items->Insert(DestItem->Index);
  NewItem->Assign(Item);
  lvReplaceRules->Items->Delete(Item->Index);
}
//---------------------------------------------------------------------------

bool __fastcall TfrmReplaceRule::FormHelp(WORD Command, int Data,
      bool &CallHelp)
{
  if(Command != HELP_CONTEXTPOPUP)
  {
    CallHelp = false;
    return true;
  }
  WinHelp(Handle,(HelpFilePath + "ReplaceRule1.hlp").c_str(),HELP_CONTEXTPOPUP,Data);
  CallHelp = false;
  return true;    
}
//---------------------------------------------------------------------------

