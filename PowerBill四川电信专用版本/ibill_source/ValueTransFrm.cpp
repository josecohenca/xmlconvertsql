//---------------------------------------------------------------------------

//#include <vcl.h>
#pragma hdrstop

#include "ValueTransFrm.h"
#include "MainFrm.h"
#include "EditTransFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzButton"
#pragma link "RzEdit"
#pragma link "RzListVw"
#pragma link "RzPanel"
#pragma resource "*.dfm"
TfrmValueTrans *frmValueTrans;
//---------------------------------------------------------------------------
__fastcall TfrmValueTrans::TfrmValueTrans(TComponent* Owner,AnsiString BillName,AnsiString FieldName,TStringList * ATransList,bool ReadOnly)
  : TForm(Owner)
{
  TransList = ATransList;
  txtBillName->Text  = BillName;
  txtFieldName->Text = FieldName;
  TransList = ATransList;
  TListItem * Item;
  for(int n = 0;n < TransList->Count;n++)
  {
    Item = lvTrans->Items->Add();
    Item->SubItems->Add((int)(TransList->Objects[n]));
    Item->SubItems->Add(TransList->Strings[n]);
  }
  ConfigChanged = false;
  if(ReadOnly)
  {
    lvTrans->PopupMenu  = NULL;
    lvTrans->OnDblClick = NULL;
    btnOk->Visible = false;
    btnCancel->Caption = "关闭";
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmValueTrans::btnOkClick(TObject *Sender)
{
  if(ConfigChanged)
  {
    TransList->Clear();
    TListItem * Item;
    for(int n = 0;n < lvTrans->Items->Count;n++)
    {
      Item = lvTrans->Items->Item[n];
      TransList->AddObject(Item->SubItems->Strings[1],(TObject *)StrToInt(Item->SubItems->Strings[0]));
    }
  }
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmValueTrans::PopupMenu1Popup(TObject *Sender)
{
  menuEdit->Enabled   = lvTrans->Selected != NULL;
  menuDelete->Enabled = menuEdit->Enabled;
}
//---------------------------------------------------------------------------
void __fastcall TfrmValueTrans::menuAddClick(TObject *Sender)
{
  TfrmEditTrans * frmEditTrans = new TfrmEditTrans(this,"","");
  if(frmEditTrans->ShowModal() == mrOk)
  {
    TListItem * Item = lvTrans->Items->Add();
    Item->SubItems->Add(frmEditTrans->txtValue->Text);
    Item->SubItems->Add(frmEditTrans->txtContext->Text);
    ConfigChanged = true;
  }
  delete frmEditTrans;
}
//---------------------------------------------------------------------------
void __fastcall TfrmValueTrans::menuEditClick(TObject *Sender)
{
  TListItem * Item = lvTrans->Selected;
  if(Item == NULL)
    return;
  TfrmEditTrans * frmEditTrans = new TfrmEditTrans(this,Item->SubItems->Strings[0],Item->SubItems->Strings[1]);
  if(frmEditTrans->ShowModal() == mrOk)
  {
    Item->SubItems->Strings[0] = frmEditTrans->txtValue->Text;
    Item->SubItems->Strings[1] = frmEditTrans->txtContext->Text;
    ConfigChanged = true;
  }
  delete frmEditTrans;
}
//---------------------------------------------------------------------------
void __fastcall TfrmValueTrans::menuDeleteClick(TObject *Sender)
{
  TListItem * Item = lvTrans->Selected;
  if(Item == NULL)
    return;
  if(MessageBox(Handle,"您要删除选定的值翻译定义吗?","警告",MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2) == IDNO)
    return;
  lvTrans->Items->Delete(Item->Index);
  ConfigChanged = true;
}
//---------------------------------------------------------------------------
