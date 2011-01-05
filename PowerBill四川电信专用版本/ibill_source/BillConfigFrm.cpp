//---------------------------------------------------------------------------

//#include <vcl.h>
#pragma hdrstop

#include "BillConfigFrm.h"
#include "MainFrm.h"
#include "BillFormatFrm.h"
#include <Clipbrd.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzListVw"
#pragma link "RzPanel"
#pragma link "RzTreeVw"
#pragma resource "*.dfm"
TfrmBillConfig *frmBillConfig;
//---------------------------------------------------------------------------
__fastcall TfrmBillConfig::TfrmBillConfig(TComponent* Owner)
  : TForm(Owner)
{
  TListItem * Item;
  BillConfig = new TBillConfig("");
  BillConfig->ReadBillConfig(frmMain->BillConfigFile);
  for(int n = 0;n < BillConfig->BillRootNode->ChildNodes->Count;n++)
  {
    FillBillList(NULL,BillConfig->GetAttributeNodeValue(BillConfig->BillRootNode->ChildNodes->Nodes[n],"BillName"));
  }
  //CloneBillNode = NULL;
}
void __fastcall TfrmBillConfig::FillBillList(TListItem * Item,AnsiString BillName)
{
  _di_IXMLNode BillNode;
  if(Item == NULL)
  {
    BillNode = BillConfig->GetBillNode(BillName);
    Item = lvBills->Items->Add();
    Item->SubItems->Add(BillName);
    Item->SubItems->Add(
      BillConfig->TransFormatTypeName(BillConfig->GetFileFormatType(BillName)));
    Item->SubItems->Add(BillConfig->GetAttributeNodeValue(BillNode,"Description"));
    Item->SubItems->Add(BillConfig->GetRecordNode(BillName)->ChildNodes->Count);
    if(BillConfig->IsDisabled(BillName))
      Item->SubItems->Add("");
    else
      Item->SubItems->Add("√");    
    Item->ImageIndex = 95;
  }
  else
  {
    _di_IXMLNode BillNode = BillConfig->GetBillNode(BillName);
    Item->SubItems->Strings[0] = BillName;
    Item->SubItems->Strings[1] = BillConfig->TransFormatTypeName(BillConfig->GetFileFormatType(BillName));
    Item->SubItems->Strings[2] = BillConfig->GetAttributeNodeValue(BillNode,"Description");
    Item->SubItems->Strings[3] = BillConfig->GetRecordNode(BillName)->ChildNodes->Count;
    if(BillConfig->IsDisabled(BillName))
      Item->SubItems->Strings[4] = "";
    else
      Item->SubItems->Strings[4] = "√";
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillConfig::lvBillsDblClick(TObject *Sender)
{
  TListItem * Item = lvBills->Selected;
  if(Item == NULL)
    return;
  //生成临时文件
  AnsiString TempXMLFileName = frmMain->TempDirectory + Item->SubItems->Strings[0] + ".tmp";
  FILE * file = fopen(TempXMLFileName.c_str(),"w");
  if(file == NULL)
  {
    MessageBox(Handle,"无法创建临时文件!","错误",MB_OK | MB_ICONSTOP);
    return;
  }
  fprintf(file,"<?xml version=\"1.0\" encoding=\"GBK\"?>\n<Bill>\n%s</Bill>",AnsiString(BillConfig->GetBillNode(Item->SubItems->Strings[0])->XML));
  fclose(file);
  TBillConfig * TempBillConfig = new TBillConfig(TempXMLFileName);
  TfrmBillFormat * frmBillFormat = new TfrmBillFormat(this,Item->SubItems->Strings[0],TempBillConfig);
  frmBillFormat->ShowModal();
  if(frmBillFormat->BillChanged)
  {
    _di_IXMLNode BillNode = TempBillConfig->GetBillNode(frmBillFormat->BillName);
    BillConfig->BillRootNode->ChildNodes->Remove(BillConfig->GetBillNode(Item->SubItems->Strings[0]));
    BillConfig->BillRootNode->ChildNodes->Insert(Item->Index,BillNode);
    FillBillList(Item,frmBillFormat->BillName);
  }
  delete frmBillFormat;
  delete TempBillConfig;
  DeleteFile(TempXMLFileName);
}
//---------------------------------------------------------------------------


void __fastcall TfrmBillConfig::PopupMenu1Popup(TObject *Sender)
{
  menuEditBill->Enabled       = lvBills->Selected != NULL;
  menuDeleteBill->Enabled     =  menuEditBill->Enabled;
  menuCopyBillConfig->Enabled =  menuEditBill->Enabled;
  TBillConfig * BillConfig = new TBillConfig("");
  menuPaste->Enabled = CheckClipboard(BillConfig);
  delete BillConfig;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillConfig::menuNewBillClick(TObject *Sender)
{
  AnsiString TempXMLFileName = frmMain->TempDirectory + "newbill.tmp";
  FILE * file = fopen(TempXMLFileName.c_str(),"w");
  if(file == NULL)
  {
    MessageBox(Handle,"无法创建临时文件!","错误",MB_OK | MB_ICONSTOP);
    return;
  }
  fprintf(file,"<?xml version=\"1.0\" encoding=\"GBK\"?>\n<Bill>\n</Bill>");
  fclose(file);
  TBillConfig * TempBillConfig = new TBillConfig(TempXMLFileName);
  TfrmBillFormat * frmBillFormat = new TfrmBillFormat(this,"",TempBillConfig);
  frmBillFormat->ShowModal();
  if(frmBillFormat->BillChanged)
  {
    BillConfig->BillRootNode->ChildNodes->Insert(lvBills->Items->Count,TempBillConfig->GetBillNode(frmBillFormat->BillName));
    FillBillList(NULL,frmBillFormat->BillName);
  }
  delete frmBillFormat;
  delete TempBillConfig;
  DeleteFile(TempXMLFileName);   
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillConfig::menuDeleteBillClick(TObject *Sender)
{
  TListItem * Item = lvBills->Selected;
  if(Item == NULL)
    return;
  if(MessageBox(Handle,("您确信要删除话单格式定义:" + Item->SubItems->Strings[0] + "吗?").c_str(),
    "警告",MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2) != IDYES)
    return;
  BillConfig->DeleteBill(Item->SubItems->Strings[0]);
  lvBills->Items->Delete(Item->Index);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillConfig::menuCopyBillConfigClick(TObject *Sender)
{
  TListItem * Item = lvBills->Selected;
  if(Item == NULL)
    return;
  _di_IXMLNode BillNode = BillConfig->GetBillNode(Item->SubItems->Strings[0]);
  TClipboard * Clipboard = new TClipboard();
  Clipboard->AsText = BillNode->XML;
  delete Clipboard;
  BillNode = NULL;
}
bool __fastcall TfrmBillConfig::CheckClipboard(TBillConfig * TmpBillConfig)
{
  TClipboard * Clipboard = NULL;
  FILE * file;
  TBillFile * BillFile = NULL;
  try
  {
    bool Result = false;
    Clipboard = new TClipboard();
    if(Clipboard->AsText != "")
    {
      file = fopen((frmMain->TempDirectory + "Clipboard.fil").c_str(),"w");
      fprintf(file,"%s","<?xml version=\"1.0\" encoding=\"GBK\"?><Bill>" + Clipboard->AsText + "</Bill>");
      delete Clipboard;
      Clipboard = NULL;
      fclose(file);
      file = NULL;
      TmpBillConfig->ReadBillConfig(frmMain->TempDirectory + "Clipboard.fil",true);
      if(TmpBillConfig->BillNameList->Count == 0)
      {
        Result = false;
      }
      else
      {
        BillFile = new TBillFile(TmpBillConfig->BillNameList->Strings[0],
          TmpBillConfig,
          Handle,
          frmMain->TempDirectory);
        delete BillFile;
        BillFile = NULL;
        Result = true;
      }
    }
    return Result;
  }
  catch(Exception * E)
  {
    if(Clipboard != NULL)
      delete Clipboard;
    if(file != NULL)
      fclose(file);
    if(BillFile != NULL)
      delete BillFile;
  }
  return false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBillConfig::menuPasteClick(TObject *Sender)
{
  TBillConfig * TmpBillConfig = new TBillConfig("");
  if(!CheckClipboard(TmpBillConfig))
  {
    delete TmpBillConfig;
    return;
  }
  AnsiString NewBillName = TmpBillConfig->BillNameList->Strings[0];
  _di_IXMLNode BillNode  = TmpBillConfig->GetBillNode(TmpBillConfig->BillNameList->Strings[0]);
  _di_IXMLNode BillNameNode = BillNode->ChildNodes->FindNode("BillName");
  if(BillConfig->GetBillNode(NewBillName) != NULL)
  {
    int n = 1;
    do
    {
      if(BillConfig->GetBillNode(NewBillName + "的副本(" + IntToStr(n) + ")") == NULL)
      {
        NewBillName = NewBillName + "的副本(" + IntToStr(n) + ")";
        break;
      }
      ++n;
    }while(true);
  }
  BillNameNode->Text = NewBillName;
  BillConfig->BillRootNode->ChildNodes->Insert(BillConfig->BillRootNode->ChildNodes->Count,BillNode);
  FillBillList(NULL,NewBillName);
  delete TmpBillConfig;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillConfig::FormDestroy(TObject *Sender)
{
  //if(CloneBillNode != NULL)
  //  CloneBillNode = NULL;
  if(BillConfig != NULL)
  {
    delete BillConfig;
  }     
}

//---------------------------------------------------------------------------
bool __fastcall TfrmBillConfig::SaveChanges(bool ShowHint)
{
  if(ShowHint && MessageBox(Handle,"所做的更改在保存后将立即生效,您要继续保存吗?","问题",MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDNO)
    return false;
  if(!BillConfig->SaveChanges(true))
  {
    MessageBox(Handle,BillConfig->LastErrorMessage.c_str(),"错误",MB_OK | MB_ICONSTOP);
    return false;
  }
  frmMain->BillConfig->ReadBillConfig(frmMain->BillConfigFile);
  BillConfig->ReadBillConfig(frmMain->BillConfigFile,true);
  MessageBox(Handle,"话单格式配置已保存!","信息",MB_OK | MB_ICONINFORMATION);
  return true;
}

void __fastcall TfrmBillConfig::btnSaveClick(TObject *Sender)
{
  if(!BillConfig->IsModified())
    return;
  SaveChanges(true);  
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillConfig::btnRollbackClick(TObject *Sender)
{
  if(!BillConfig->IsModified())
    return;
  if(MessageBox(Handle,"撤消更改后,所有未保存的更改将会丢失,您要撤消更改吗?","警告",
    MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2) == IDNO)
    return;
  BillConfig->Rollback();
  lvBills->Items->Clear();
  for(int n = 0;n < BillConfig->BillRootNode->ChildNodes->Count;n++)
  {
    FillBillList(NULL,BillConfig->GetAttributeNodeValue(BillConfig->BillRootNode->ChildNodes->Nodes[n],"BillName"));
  }  
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillConfig::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  if(BillConfig->IsModified())
  {
    switch(MessageBox(Handle,"是否要保存所做的更改?","问题",MB_YESNOCANCEL | MB_ICONQUESTION))
    {
    case IDYES:
      if(!SaveChanges(false))
        throw new Exception("");
      break;
    case IDCANCEL:
      throw new Exception("");
    default:
      break;
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TfrmBillConfig::lvBillsDragOver(TObject *Sender,
      TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
  TListItem * Item     = lvBills->Selected;
  TListItem * DestItem = lvBills->GetItemAt(X,Y);
  Accept = DestItem != NULL && DestItem != Item;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillConfig::lvBillsDragDrop(TObject *Sender,
      TObject *Source, int X, int Y)
{
  TListItem * DestItem = lvBills->GetItemAt(X,Y);
  TListItem * Item     = lvBills->Selected;
  int SourIndex,DestIndex;
  SourIndex = Item->Index;
  DestIndex = DestItem->Index;
  TListItem * NewItem;
  if(SourIndex < DestIndex)
    NewItem  = lvBills->Items->Insert(DestIndex + 1);
  else
    NewItem  = lvBills->Items->Insert(DestIndex);
  NewItem->Assign(Item);
  _di_IXMLNode SourNode  = BillConfig->GetBillNode(Item->SubItems->Strings[0]);
  _di_IXMLNode TmpNode   = SourNode->CloneNode(true);

  BillConfig->BillRootNode->ChildNodes->Remove(SourNode);
  BillConfig->BillRootNode->ChildNodes->Insert(DestIndex,TmpNode);
  lvBills->Items->Delete(Item->Index);
}
//---------------------------------------------------------------------------

