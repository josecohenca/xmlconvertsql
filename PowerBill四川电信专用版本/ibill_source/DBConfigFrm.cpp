//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DBConfigFrm.h"
#include "MainFrm.h"
#include "EditDBFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzButton"
#pragma link "RzListVw"
#pragma resource "*.dfm"
TfrmDBConfig *frmDBConfig;
//---------------------------------------------------------------------------
__fastcall TfrmDBConfig::TfrmDBConfig(TComponent* Owner,bool AForSelect)
  : TForm(Owner)
{
  ForSelect = AForSelect;
  if(ForSelect)
    DBConfig = frmMain->DBConfig;
  else
    DBConfig = new TDBConfig(frmMain->DBConfig->ConfigFileName);
  btnOk->Visible = ForSelect;
  TListItem * Item;
  for(int n = 0;n < DBConfig->GetDBCount();n++)
  {
    Item = lvDBConnections->Items->Add();
    Item->ImageIndex = 115;
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
void __fastcall TfrmDBConfig::FillListItem(TListItem * Item)
{
  AnsiString DBName = DBConfig->GetDBName(Item->Index);
  Item->SubItems->Strings[0] = DBName;
  Item->SubItems->Strings[1] = DBConfig->GetDBAttributeValue(DBName,"DBType");
  Item->SubItems->Strings[2] = DBConfig->GetDBAttributeValue(DBName,"Server");
  Item->SubItems->Strings[3] = DBConfig->GetDBAttributeValue(DBName,"DataBase");
  Item->SubItems->Strings[4] = DBConfig->GetDBAttributeValue(DBName,"Port");
  Item->SubItems->Strings[5] = DBConfig->GetDBAttributeValue(DBName,"UserName");
}
//---------------------------------------------------------------------------

void __fastcall TfrmDBConfig::EditDBConfig(TListItem * Item)
{
  TfrmEditDB * frmEditDB = new TfrmEditDB(this,true,DBConfig);
  frmEditDB->OldDBName = Item->SubItems->Strings[0];
  frmEditDB->txtDBName->Text = Item->SubItems->Strings[0];
  frmEditDB->cbxDBType->ItemIndex =
    frmEditDB->cbxDBType->Items->IndexOf(Item->SubItems->Strings[1]);
  frmEditDB->txtDBServer->Text = Item->SubItems->Strings[2];
  frmEditDB->txtDBUserName->Text = Item->SubItems->Strings[5];
  frmEditDB->txtDBPassword->Text = DBConfig->GetDBAttributeValue(Item->SubItems->Strings[0],"Password",true);
  frmEditDB->txtDBPort->Text = Item->SubItems->Strings[4];
  frmEditDB->cbxDBTypeChange(NULL);
  if(Item->SubItems->Strings[3] != "")
  {
    frmEditDB->cbxDataBases->Items->Add(Item->SubItems->Strings[3]);
    frmEditDB->cbxDataBases->ItemIndex = 0;
  }
  if(frmEditDB->ShowModal() == mrOk)
  {
    FillListItem(Item);
  }
  delete frmEditDB;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDBConfig::lvDBConnectionsDblClick(TObject *Sender)
{
  TListItem * Item = lvDBConnections->Selected;
  if(Item == NULL)
    return;
  if(ForSelect)
  {
    ModalResult = mrOk;
    return;
  }
  else
  {
    EditDBConfig(Item);
  }
}

void __fastcall TfrmDBConfig::btnSaveClick(TObject *Sender)
{
  if(!DBConfig->IsModified())
    return;
  if(MessageBox(Handle,"您要将所做的修改保存到配置文件吗?","问题",MB_YESNO | MB_ICONQUESTION) == IDNO)
    return;
  if(!DBConfig->SaveChanges())
  {
    MessageBox(Handle,DBConfig->LastErrorMessage.c_str(),"错误",MB_OK | MB_ICONSTOP);
    return;
  }
  if(!ForSelect)
  {
    frmMain->DBConfig->LoadConfig(frmMain->DBConfig->ConfigFileName);
  }
  MessageBox(Handle,"数据库配置已保存!","信息",MB_OK | MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------
void __fastcall TfrmDBConfig::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  if(DBConfig->IsModified())
  {
    switch(MessageBox(Handle,"您已经修改了配置文件.是否保存这些修改?","问题",MB_YESNOCANCEL | MB_ICONQUESTION))
    {
    case IDYES:
      if(!DBConfig->SaveChanges())
      {
        MessageBox(Handle,DBConfig->LastErrorMessage.c_str(),"错误",MB_OK | MB_ICONSTOP);
        Action = caNone;
        return;
      }
      break;
    case IDNO:
      DBConfig->Rollback();
      break;
    default:
      Action = caNone;
      throw new Exception("");
    }
  }
  if(!ForSelect)
    delete DBConfig;  
}
//---------------------------------------------------------------------------
void __fastcall TfrmDBConfig::PopupMenu1Popup(TObject *Sender)
{
  menuDelete->Enabled = lvDBConnections->Selected != NULL;
  menuEdit->Enabled = menuDelete->Enabled;   
}
//---------------------------------------------------------------------------
void __fastcall TfrmDBConfig::menuDeleteClick(TObject *Sender)
{
  TListItem * Item = lvDBConnections->Selected;
  if(Item == NULL)
    return;
  if(MessageBox(Handle,("您要删除数据库连接  " + Item->SubItems->Strings[0] + "  吗?").c_str(),
    "警告",MB_YESNO | MB_ICONWARNING) == IDNO)
    return;
  if(DBConfig->DeleteDBNode(Item->SubItems->Strings[0]))
  {
    lvDBConnections->Items->Delete(Item->Index);  
    if(!ForSelect)
    {
      frmMain->DBConfig->LoadConfig(frmMain->DBConfig->ConfigFileName);
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmDBConfig::menuAddNewClick(TObject *Sender)
{
  TfrmEditDB * frmEditDB = new TfrmEditDB(this,false,DBConfig);
  if(frmEditDB->ShowModal() == mrOk)
  {
    TListItem * Item = lvDBConnections->Items->Add();
    Item->ImageIndex = 115;
    Item->SubItems->Add("");
    Item->SubItems->Add("");
    Item->SubItems->Add("");
    Item->SubItems->Add("");
    Item->SubItems->Add("");
    Item->SubItems->Add("");
    FillListItem(Item);
  }
  delete frmEditDB;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDBConfig::menuEditClick(TObject *Sender)
{
  TListItem * Item = lvDBConnections->Selected;
  if(Item == NULL)
    return;
  EditDBConfig(Item);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDBConfig::btnOkClick(TObject *Sender)
{
  TListItem * Item = lvDBConnections->Selected;
  if(Item == NULL)
    return;
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDBConfig::lvDBConnectionsDragDrop(TObject *Sender,
      TObject *Source, int X, int Y)
{
  TListItem * DestItem = lvDBConnections->GetItemAt(X,Y);
  TListItem * Item     = lvDBConnections->Selected;
  int SourIndex,DestIndex;
  SourIndex = Item->Index;
  DestIndex = DestItem->Index;
  TListItem * NewItem;
  if(SourIndex < DestIndex)
    NewItem  = lvDBConnections->Items->Insert(DestIndex + 1);
  else
    NewItem  = lvDBConnections->Items->Insert(DestIndex);
  NewItem->Assign(Item);
  _di_IXMLNode SourNode  = DBConfig->GetDBNode(Item->SubItems->Strings[0]);
  _di_IXMLNode DestNode  = DBConfig->GetDBNode(DestItem->SubItems->Strings[0]);
  _di_IXMLNode TmpNode   = SourNode->CloneNode(true);
  //DBConfig->RootNode->ChildNodes->Remove(DestNode);
  //DBConfig->RootNode->ChildNodes->Insert(DestIndex,TmpNode);

  DBConfig->RootNode->ChildNodes->Remove(SourNode);
  DBConfig->RootNode->ChildNodes->Insert(DestIndex,TmpNode);
  lvDBConnections->Items->Delete(Item->Index);

  /*TListItem * DestItem = lvDBConnections->GetItemAt(X,Y);
  TListItem * Item     = lvDBConnections->Selected;
  int OldIndex = Item->Index;
  TListItem * NewItem  = lvDBConnections->Items->Insert(DestItem->Index);
  int NewIndex = NewItem->Index;
  if(OldIndex < NewIndex)
    NewIndex -= 1;
  _di_IXMLNode OldNode  = DBConfig->GetDBNode(Item->SubItems->Strings[0]);
  _di_IXMLNode NewNode = OldNode->CloneNode(true);
  DBConfig->RootNode->ChildNodes->Delete(OldIndex);
  DBConfig->RootNode->ChildNodes->Insert(NewIndex,NewNode);
  NewItem->Assign(Item);
  lvDBConnections->Items->Delete(Item->Index);*/  
}
//---------------------------------------------------------------------------

void __fastcall TfrmDBConfig::lvDBConnectionsDragOver(TObject *Sender,
      TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
  TListItem * DestItem = lvDBConnections->GetItemAt(X,Y);
  Accept = DestItem != NULL && DestItem != lvDBConnections->Selected;  
}
//---------------------------------------------------------------------------

