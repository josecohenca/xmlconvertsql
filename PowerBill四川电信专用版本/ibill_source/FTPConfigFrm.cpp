//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FTPConfigFrm.h"
#include "MainFrm.h"
#include "EditFTPFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzButton"
#pragma link "RzListVw"
#pragma resource "*.dfm"
TfrmFTPConfig *frmFTPConfig;
//---------------------------------------------------------------------------
__fastcall TfrmFTPConfig::TfrmFTPConfig(TComponent* Owner,bool AForSelect)
  : TForm(Owner)
{
  ForSelect = AForSelect;
  btnOk->Visible = ForSelect;
  TListItem * Item;
  for(int n = 0;n < frmMain->FTPConfig->GetFTPCount();n++)
  {
    Item = lvFTPServers->Items->Add();
    Item->ImageIndex = 116;
    Item->SubItems->Add("");
    Item->SubItems->Add("");
    Item->SubItems->Add("");
    Item->SubItems->Add("");
    Item->SubItems->Add("");
    Item->SubItems->Add("");
    FillListItem(Item);
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmFTPConfig::FillListItem(TListItem * Item)
{
  AnsiString FTPName = frmMain->FTPConfig->GetFTPName(Item->Index);
  Item->SubItems->Strings[0] = FTPName;
  Item->SubItems->Strings[1] = frmMain->FTPConfig->GetFTPAttributeValue(FTPName,"Server");
  Item->SubItems->Strings[2] = frmMain->FTPConfig->GetFTPAttributeValue(FTPName,"Port");
  Item->SubItems->Strings[3] = frmMain->FTPConfig->GetFTPAttributeValue(FTPName,"UserName");
  Item->SubItems->Strings[4] = frmMain->FTPConfig->GetFTPAttributeValue(FTPName,"Mode");
  //Item->SubItems->Strings[5] = frmMain->FTPConfig->GetFTPAttributeValue(FTPName,"TimeOut");
}
//---------------------------------------------------------------------------

void __fastcall TfrmFTPConfig::EditFTPConfig(TListItem * Item)
{
  TfrmEditFTP * frmEditFTP = new TfrmEditFTP(this,true);
  frmEditFTP->OldFTPName = Item->SubItems->Strings[0];
  frmEditFTP->txtServerName->Text = Item->SubItems->Strings[0];
  frmEditFTP->txtServer->Text     = Item->SubItems->Strings[1];
  frmEditFTP->txtPort->Text       = Item->SubItems->Strings[2];

  frmEditFTP->txtUserName->Text   = Item->SubItems->Strings[3];
  frmEditFTP->txtPassword->Text   = frmMain->FTPConfig->GetFTPAttributeValue(
    Item->SubItems->Strings[0],"Password",true);
  frmEditFTP->cbxMode->ItemIndex = frmEditFTP->cbxMode->Items->IndexOf(Item->SubItems->Strings[4]);
  //frmEditFTP->txtTimeOut->Text   =  Item->SubItems->Strings[5];
  if(frmEditFTP->ShowModal() == mrOk)
  {
    FillListItem(Item);
  }
  delete frmEditFTP;
}
//---------------------------------------------------------------------------
void __fastcall TfrmFTPConfig::lvFTPServersDblClick(TObject *Sender)
{
  TListItem * Item = lvFTPServers->Selected;
  if(Item == NULL)
    return;
  if(ForSelect)
  {
    ModalResult = mrOk;
    return;
  }
  else
  {
    EditFTPConfig(Item);
  }
}

void __fastcall TfrmFTPConfig::btnSaveClick(TObject *Sender)
{
    SaveChanges(true);
}
bool __fastcall TfrmFTPConfig::SaveChanges(bool ShowHint)
{
  if(!frmMain->FTPConfig->IsModified())
    return true;
  if(ShowHint && MessageBox(Handle,"您要将所做的修改保存到配置文件吗?","问题",MB_YESNO | MB_ICONQUESTION) == IDNO)
    return false;
  if(!frmMain->FTPConfig->SaveChanges())
  {
    MessageBox(Handle,frmMain->FTPConfig->LastErrorMessage.c_str(),"错误",MB_OK | MB_ICONSTOP);
    return false;
  }
  frmMain->FTPConfig->LoadConfig(frmMain->FTPConfig->ConfigFileName);
  MessageBox(Handle,"FTP服务器配置已保存!","信息",MB_OK | MB_ICONINFORMATION);
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmFTPConfig::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  if(frmMain->FTPConfig->IsModified())
  {
    switch(MessageBox(Handle,"您已经修改了配置文件.是否保存这些修改?","问题",MB_YESNOCANCEL | MB_ICONQUESTION))
    {
    case IDYES:
      if(!SaveChanges(false))
      {
        Action = caNone;
        return;
      }
      break;
    case IDNO:
      frmMain->FTPConfig->Rollback();
      break;
    default:
      Action = caNone;
      throw new Exception("");
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmFTPConfig::PopupMenu1Popup(TObject *Sender)
{
  menuDelete->Enabled = lvFTPServers->Selected != NULL;
  menuEdit->Enabled = menuDelete->Enabled;   
}
//---------------------------------------------------------------------------
void __fastcall TfrmFTPConfig::menuDeleteClick(TObject *Sender)
{
  TListItem * Item = lvFTPServers->Selected;
  if(Item == NULL)
    return;
  if(MessageBox(Handle,("您要删除FTP服务器  " + Item->SubItems->Strings[0] + "  吗?").c_str(),
    "警告",MB_YESNO | MB_ICONWARNING) == IDNO)
    return;
  if(frmMain->FTPConfig->DeleteFTPNode(Item->SubItems->Strings[0]))
  {
    lvFTPServers->Items->Delete(Item->Index);  
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmFTPConfig::menuAddNewClick(TObject *Sender)
{
  TfrmEditFTP * frmEditFTP = new TfrmEditFTP(this,false);
  if(frmEditFTP->ShowModal() == mrOk)
  {
    TListItem * Item = lvFTPServers->Items->Add();
    Item->ImageIndex = 116;
    Item->SubItems->Add("");
    Item->SubItems->Add("");
    Item->SubItems->Add("");
    Item->SubItems->Add("");
    Item->SubItems->Add("");
    Item->SubItems->Add("");
    FillListItem(Item);
  }
  delete frmEditFTP;
}
//---------------------------------------------------------------------------
void __fastcall TfrmFTPConfig::menuEditClick(TObject *Sender)
{
  TListItem * Item = lvFTPServers->Selected;
  if(Item == NULL)
    return;
  EditFTPConfig(Item);
}
//---------------------------------------------------------------------------

void __fastcall TfrmFTPConfig::btnOkClick(TObject *Sender)
{
  TListItem * Item = lvFTPServers->Selected;
  if(Item == NULL)
    return;
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmFTPConfig::lvFTPServersDragOver(TObject *Sender,
      TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
  TListItem * DestItem = lvFTPServers->GetItemAt(X,Y);
  Accept = DestItem != NULL && DestItem != lvFTPServers->Selected;
}
//---------------------------------------------------------------------------

void __fastcall TfrmFTPConfig::lvFTPServersDragDrop(TObject *Sender,
      TObject *Source, int X, int Y)
{
  TListItem * DestItem = lvFTPServers->GetItemAt(X,Y);
  TListItem * Item     = lvFTPServers->Selected;
  int SourIndex,DestIndex;
  SourIndex = Item->Index;
  DestIndex = DestItem->Index;
  TListItem * NewItem;
  if(SourIndex < DestIndex)
    NewItem  = lvFTPServers->Items->Insert(DestIndex + 1);
  else
    NewItem  = lvFTPServers->Items->Insert(DestIndex);
  NewItem->Assign(Item);
  _di_IXMLNode SourNode  = frmMain->FTPConfig->GetFTPNode(Item->SubItems->Strings[0]);
  _di_IXMLNode DestNode  = frmMain->FTPConfig->GetFTPNode(DestItem->SubItems->Strings[0]);
  _di_IXMLNode TmpNode   = SourNode->CloneNode(true);
  //frmMain->FTPConfig->RootNode->ChildNodes->Remove(DestNode);
  //frmMain->FTPConfig->RootNode->ChildNodes->Insert(DestIndex,TmpNode);

  frmMain->FTPConfig->RootNode->ChildNodes->Remove(SourNode);
  frmMain->FTPConfig->RootNode->ChildNodes->Insert(DestIndex,TmpNode);
  lvFTPServers->Items->Delete(Item->Index);

  /*TListItem * DestItem = lvFTPServers->GetItemAt(X,Y);
  TListItem * Item     = lvFTPServers->Selected;
  int OldIndex = Item->Index;
  TListItem * NewItem  = lvFTPServers->Items->Insert(DestItem->Index);
  int NewIndex = NewItem->Index;
  if(OldIndex < NewIndex)
    NewIndex -= 1;
  _di_IXMLNode OldNode  = frmMain->FTPConfig->GetFTPNode(Item->SubItems->Strings[0]);
  _di_IXMLNode NewNode = OldNode->CloneNode(true);
  frmMain->FTPConfig->RootNode->ChildNodes->Delete(OldIndex);
  frmMain->FTPConfig->RootNode->ChildNodes->Insert(NewIndex,NewNode);
  NewItem->Assign(Item);
  lvFTPServers->Items->Delete(Item->Index);*/

}
//---------------------------------------------------------------------------

