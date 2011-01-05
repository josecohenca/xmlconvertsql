//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BillFormatFrm.h"
#include "MainFrm.h"
#include "FieldFrm.h"
#include "RecordFilterFrm.h"
#include "BillConfigFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzButton"
#pragma link "RzEdit"
#pragma link "RzCmboBx"
#pragma link "RzListVw"
#pragma link "RzRadChk"
#pragma resource "*.dfm"
TfrmBillFormat *frmBillFormat;
//---------------------------------------------------------------------------
__fastcall TfrmBillFormat::TfrmBillFormat(TComponent* Owner,AnsiString ABillName,TBillConfig * ABillConfig)
  : TForm(Owner)
{
  BillConfig = ABillConfig;
  BillName   = ABillName;
  FilterList = new TList;
  if(BillName != "")
  {
    _di_IXMLNode BillNode = BillConfig->GetBillNode(BillName);
    txtBillName->Text    = BillName;
    txtDescription->Text = BillConfig->GetAttributeNodeValue(BillNode,"Description");
    cbxFileFormatType->ItemIndex = BillConfig->GetFileFormatType(BillName) - 1;
    chkReadByBlock->Checked = BillConfig->IsBlockFile(BillName);
    cbxFileFormatTypeChange(NULL);
    txtFileHeaderLength->Text  = IntToStr(BillConfig->GetFileHeadLength(BillName));
    txtFileTrailLength->Text   = IntToStr(BillConfig->GetFileTrailLength(BillName));
    txtRecordLength->Text      = IntToStr(BillConfig->GetFileRecordLength(BillName));
    txtSeparator->Text         = BillConfig->GetFieldSeparator(BillName);
    txtBlockLength->Text       = BillConfig->GetSummaryAttributeValue(BillName,"BlockLength");
    txtBlockHeaderLength->Text = BillConfig->GetSummaryAttributeValue(BillName,"BlockHeadHength");
    txtBlockTrailLength->Text  = BillConfig->GetSummaryAttributeValue(BillName,"BlockTrailLength");
    cbxDisabled->Checked       = !BillConfig->IsDisabled(BillName);
    _di_IXMLNode FieldsNode    = BillConfig->GetRecordNode(BillName);
    //cbxDisabledClick(NULL);
    BillConfig->GetRecordFilter(BillName,FilterList);
    for(int n = 0;n < FieldsNode->ChildNodes->Count;n++)
    {
      ShowFieldInList(FieldsNode->ChildNodes->Nodes[n],NULL);
    }
  }
  else
  {
    RzGroupBox2->Enabled = false;
    lvFields->Enabled    = false;
    btnAction->Enabled   = false;
    cbxDisabled->Checked = true;
    //btnSaveFieldConfig->Enabled = false;
  }
  BillChanged = false;
  Modified    = false;
}
void __fastcall TfrmBillFormat::ShowFieldInList(_di_IXMLNode FieldNode,TListItem * Item)
{
   if(Item == NULL)
   {
      Item = lvFields->Items->Add();
      Item->ImageIndex = 47;
      Item->SubItems->Add(BillConfig->GetFieldAttributeValue(FieldNode,"FieldName"));
      Item->SubItems->Add(BillConfig->GetFieldAttributeValue(FieldNode,"Label"));
      if(BillConfig->GetFieldAttributeValue(FieldNode,"Disabled","0") == "0")
        Item->SubItems->Add("启用");
      else
        Item->SubItems->Add("禁用");
      Item->SubItems->Add(BillConfig->GetFieldAttributeValue(FieldNode,"Offset"));
      Item->SubItems->Add(BillConfig->GetFieldAttributeValue(FieldNode,"FieldLength"));
      Item->SubItems->Add(BillConfig->GetFieldDataTypeNameText(BillConfig->TransFieldDataType(BillConfig->GetFieldAttributeValue(FieldNode,"DataType"))));
      Item->SubItems->Add(BillConfig->GetFieldAttributeValue(FieldNode,"BitPosition"));
      Item->SubItems->Add(BillConfig->GetFieldAttributeValue(FieldNode,"BitWidth"));
      if(BillConfig->GetFieldAttributeValue(FieldNode,"DisplayFormat") == "BITS")
        Item->SubItems->Add("二进制代码");
      else
        Item->SubItems->Add("默认");
      Item->SubItems->Add(BillConfig->GetFieldAttributeValue(FieldNode,"SortType","STR"));
      if(BillConfig->GetFieldAttributeValue(FieldNode,"DisplayInList","1") != "0")
        Item->SubItems->Add("√");
      else
        Item->SubItems->Add("");
   }
   else
   {
      Item->SubItems->Strings[0] = BillConfig->GetFieldAttributeValue(FieldNode,"FieldName");
      Item->SubItems->Strings[1] = BillConfig->GetFieldAttributeValue(FieldNode,"Label");
      if(BillConfig->GetFieldAttributeValue(FieldNode,"Disabled","0") == "0")
        Item->SubItems->Strings[2] = "启用";
      else
        Item->SubItems->Strings[2] = "禁用";
      Item->SubItems->Strings[3] = BillConfig->GetFieldAttributeValue(FieldNode,"Offset");
      Item->SubItems->Strings[4] = BillConfig->GetFieldAttributeValue(FieldNode,"FieldLength");
      Item->SubItems->Strings[5] = BillConfig->GetFieldDataTypeNameText(BillConfig->TransFieldDataType(BillConfig->GetFieldAttributeValue(FieldNode,"DataType")));
      Item->SubItems->Strings[6] = BillConfig->GetFieldAttributeValue(FieldNode,"BitPosition");
      Item->SubItems->Strings[7] = BillConfig->GetFieldAttributeValue(FieldNode,"BitWidth");
      if(BillConfig->GetFieldAttributeValue(FieldNode,"DisplayFormat") == "BITS")
        Item->SubItems->Strings[8] = "二进制代码";
      else
        Item->SubItems->Strings[8] = "默认";
      Item->SubItems->Strings[9] = BillConfig->GetFieldAttributeValue(FieldNode,"SortType","STR");
      if(BillConfig->GetFieldAttributeValue(FieldNode,"DisplayInList","1") != "0")
        Item->SubItems->Strings[10] = "√";
      else
        Item->SubItems->Strings[10] = "";      
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmBillFormat::cbxFileFormatTypeChange(TObject *Sender)
{
  switch(cbxFileFormatType->ItemIndex)
  {
  case 0:   //二进制格式
    Label5->Caption = "Byte";
    Label7->Caption = "Byte";
    Label8->Enabled = true;
    txtRecordLength->Enabled = true;
    Label9->Enabled = true;

    Label10->Enabled = false;
    txtSeparator->Enabled = false;

    /*Label12->Enabled = true;
    txtBlockLength->Enabled = true;
    Label13->Enabled = true;
    Label11->Enabled = true;
    txtBlockTrailLength->Enabled = true;
    Label14->Enabled = true;
    Label15->Enabled = true;
    txtBlockHeaderLength->Enabled = true;
    txtBlockTrailLength->Enabled  = true;
    Label16->Enabled = true;*/
    chkReadByBlock->Enabled = true;
    chkReadByBlockClick(NULL);
    break;
  case 1:   //文本文件
    Label5->Caption = "行";
    Label7->Caption = "行";
    Label8->Enabled = false;
    txtRecordLength->Enabled = false;
    Label9->Enabled = false;

    Label10->Enabled = true;
    txtSeparator->Enabled = true;

    Label12->Enabled = false;
    txtBlockLength->Enabled = false;
    Label13->Enabled = false;
    Label11->Enabled = false;
    txtBlockTrailLength->Enabled = false;
    Label14->Enabled = false;
    Label15->Enabled = false;
    txtBlockHeaderLength->Enabled = false;
    txtBlockTrailLength->Enabled  = false;
    Label16->Enabled = false;
    chkReadByBlock->Enabled = false;
    chkReadByBlockClick(NULL); 
    break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmBillFormat::lvFieldsDblClick(TObject *Sender)
{
  TListItem * Item = lvFields->Selected;
  if(Item == NULL)
    return;
  _di_IXMLNode FieldNode = BillConfig->GetFieldNode(BillName,Item->SubItems->Strings[0]);
  if(FieldNode == NULL)
  {
    MessageBox(Handle,("找不到字段节点[" + Item->SubItems->Strings[0] + "]!").c_str(),"错误",MB_OK | MB_ICONSTOP);
    return;
  }
  TfrmField * frmField = new TfrmField(this,txtBillName->Text,BillConfig,FieldNode,false);
  if(frmField->ShowModal() == mrOk)
  {
    ShowFieldInList(BillConfig->GetFieldNode(BillName,frmField->txtFieldName->Text),Item);
  }
  delete frmField;
}
//---------------------------------------------------------------------------
bool __fastcall TfrmBillFormat::CheckBillParameter()
{
  if(txtBillName->Text == "")
  {
    MessageBox(Handle,"必须输入话单名称!","提示",MB_OK | MB_ICONWARNING);
    txtBillName->SetFocus();
    txtBillName->SelectAll();
    return false;
  }
  if(txtBillName->Text.Pos("\\") > 0 ||
     txtBillName->Text.Pos("/") > 0 ||
     txtBillName->Text.Pos(":") > 0 ||
     txtBillName->Text.Pos("*") > 0 ||
     txtBillName->Text.Pos("?") > 0 ||
     txtBillName->Text.Pos("\"") > 0 ||
     txtBillName->Text.Pos("<") > 0 ||
     txtBillName->Text.Pos(">") > 0 ||
     txtBillName->Text.Pos("|") > 0 ||
     txtBillName->Text.Pos(".") > 0 ||
     txtBillName->Text.Pos(" ") > 0
     )
  {
    MessageBox(Handle,"话单名称不能包含 \\  /  :  *  ?  \"  <  >  |  .  字符和空格.","提示",MB_OK | MB_ICONWARNING);
    txtBillName->SetFocus();
    txtBillName->SelectAll();
    return false;
  }
  if(txtBillName->Text != BillName && ((TfrmBillConfig * )Owner)->BillConfig->GetBillNode(txtBillName->Text) != NULL)
  {
    MessageBox(Handle,"输入的话单名称已经存在!","提示",MB_OK | MB_ICONWARNING);
    txtBillName->SetFocus();
    txtBillName->SelectAll();
    return false;
  }
  if(cbxFileFormatType->ItemIndex < 0)
  {
    MessageBox(Handle,"必须选择文件格式类型!","提示",MB_OK | MB_ICONWARNING);
    cbxFileFormatType->SetFocus();
    return false;
  }
  int n;
  try
  {
    n = StrToInt(txtFileHeaderLength->Text);
    if(n < 0)
      throw new Exception("");
  }
  catch(...)
  {
    MessageBox(Handle,"文件头长度必须是一个大于-1的整数!","提示",MB_OK | MB_ICONWARNING);
    txtFileHeaderLength->SetFocus();
    txtFileHeaderLength->SelectAll();
    return false;
  }
  try
  {
    n = StrToInt(txtFileTrailLength->Text);
    if(n < 0)
      throw new Exception("");
  }
  catch(...)
  {
    MessageBox(Handle,"文件尾长度必须是一个大于-1的整数!","提示",MB_OK | MB_ICONWARNING);
    txtFileTrailLength->SetFocus();
    txtFileTrailLength->SelectAll();
    return false;
  }
  if(txtFileHeaderLength->Text == "")
    txtFileHeaderLength->Text = "0";
  if(txtFileTrailLength->Text == "")
    txtFileTrailLength->Text = "0";
  if(txtRecordLength->Text == "")
    txtRecordLength->Text = "0";
  if(txtBlockLength->Text == "")
     txtBlockLength->Text = "0";
  if(txtBlockHeaderLength->Text == "")
     txtBlockHeaderLength->Text = "0";
  if(txtBlockTrailLength->Text == "")
     txtBlockTrailLength->Text = "0";
  try
  {
    n = StrToInt(txtRecordLength->Text);
    if(cbxFileFormatType->ItemIndex == 0 && n < 1)
      throw new Exception("");
  }
  catch(...)
  {
    MessageBox(Handle,"记录长度必须是一个大于0的整数!","提示",MB_OK | MB_ICONWARNING);
    txtRecordLength->SetFocus();
    txtRecordLength->SelectAll();
    return false;
  }
  if(cbxFileFormatType->ItemIndex == 0 && !chkReadByBlock->Checked)
    txtBlockLength->Text = txtRecordLength->Text;
  try
  {
    n = StrToInt(txtBlockLength->Text);
    if(cbxFileFormatType->ItemIndex == 0 && chkReadByBlock->Checked && n < 1)
      throw new Exception("");
  }
  catch(...)
  {
    MessageBox(Handle,"块长度必须是一个大于0的整数!","提示",MB_OK | MB_ICONWARNING);
    txtBlockLength->SetFocus();
    txtBlockLength->SelectAll();
    return false;
  }
  try
  {
    n = StrToInt(txtBlockHeaderLength->Text);
    if(n < 0)
      throw new Exception("");
  }
  catch(...)
  {
    MessageBox(Handle,"块头长度必须是一个大于-1的整数!","提示",MB_OK | MB_ICONWARNING);
    txtBlockHeaderLength->SetFocus();
    txtBlockHeaderLength->SelectAll();
    return false;
  }
  if(txtBlockTrailLength->Text == "")
    txtBlockTrailLength->Text = "0";
  try
  {
    n = StrToInt(txtBlockTrailLength->Text);
    if(n < 0)
      throw new Exception("");
  }
  catch(...)
  {
    MessageBox(Handle,"块尾长度必须是一个大于-1的整数!","提示",MB_OK | MB_ICONWARNING);
    txtBlockTrailLength->SetFocus();
    txtBlockTrailLength->SelectAll();
    return false;
  }
  if(cbxFileFormatType->ItemIndex == 1 && txtSeparator->Text == "")
  {
    MessageBox(Handle,"必须指定文本文件的字段分隔符!","提示",MB_OK | MB_ICONWARNING);
    txtSeparator->SetFocus();
    return false;
  }
  return true;
}
void __fastcall TfrmBillFormat::PopupMenu1Popup(TObject *Sender)
{
  menuEditField->Enabled   = lvFields->Selected != NULL;
  menuDeleteField->Enabled = menuEditField->Enabled;
}
//---------------------------------------------------------------------------
bool __fastcall TfrmBillFormat::SaveChangeToFile(bool ShowHint)
{
  if(ShowHint && MessageBox(Handle,"您要提交所做的修改吗?","问题",MB_YESNO | MB_ICONQUESTION) == IDNO)
    return false;
  if(!CommitBillFormat())
    return false;
  if(!BillConfig->SaveChanges(false))
  {
    MessageBox(Handle,BillConfig->LastErrorMessage.c_str(),"错误",MB_OK | MB_ICONSTOP);
    return false;
  }
  MessageBox(Handle,"所做的修改已提交,但还需要在\"话单格式配置\"窗口中保存后才能生效!","信息",MB_OK | MB_ICONINFORMATION);
  BillChanged = true;
  Modified    = false;
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBillFormat::btnSaveFieldConfigClick(TObject *Sender)
{
  //if(BillConfig->IsModified() || Modified)
  SaveChangeToFile(true);
}
//---------------------------------------------------------------------------
bool __fastcall TfrmBillFormat::CommitBillFormat()
{
  if(!CheckBillParameter())
    return false;
  if(BillName == "")
  {
    BillConfig->AddNewBill(txtBillName->Text,
      txtDescription->Text,
      cbxFileFormatType->ItemIndex + 1,
      StrToInt(txtFileHeaderLength->Text),
      StrToInt(txtRecordLength->Text),
      StrToInt(txtFileTrailLength->Text),
      txtSeparator->Text,
      StrToInt(txtBlockLength->Text),
      StrToInt(txtBlockHeaderLength->Text),
      StrToInt(txtBlockTrailLength->Text),
      FilterList,chkReadByBlock->Checked,!cbxDisabled->Checked);
    RzGroupBox2->Enabled        = true;
    lvFields->Enabled           = true;
    btnSaveFieldConfig->Enabled = true;
    btnAction->Enabled          = cbxFileFormatType->ItemIndex == 0;
  }
  else
  {
    BillConfig->ModifyBill(BillName,txtBillName->Text,
      txtDescription->Text,
      cbxFileFormatType->ItemIndex + 1,
      StrToInt(txtFileHeaderLength->Text),
      StrToInt(txtRecordLength->Text),
      StrToInt(txtFileTrailLength->Text),
      txtSeparator->Text,
      StrToInt(txtBlockLength->Text),
      StrToInt(txtBlockHeaderLength->Text),
      StrToInt(txtBlockTrailLength->Text),
      FilterList,chkReadByBlock->Checked,!cbxDisabled->Checked);
  }
  BillName = txtBillName->Text;
  return true;
}

void __fastcall TfrmBillFormat::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  if(BillConfig->IsModified() || Modified)
  {
    switch(MessageBox(Handle,"话单配置已被修改,是否要将所做的提交给配置文件?","问题",
      MB_YESNOCANCEL | MB_ICONQUESTION))
    {
    case IDYES:
      if(!SaveChangeToFile(false))
        Action = caNone;
      break;
    case IDCANCEL:
      Action = caNone;
      break;
    default:
      break;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmBillFormat::menuNewFieldClick(TObject *Sender)
{
  TfrmField * frmField = new TfrmField(this,BillName,BillConfig,NULL,false);
  if(frmField->ShowModal() == mrOk)
  {
    ShowFieldInList(BillConfig->GetFieldNode(BillName,frmField->txtFieldName->Text),NULL);
  }
  delete frmField;  
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillFormat::menuDeleteFieldClick(TObject *Sender)
{
  TListItem * Item = lvFields->Selected;
  if(Item == NULL)
    return;
  if(MessageBox(Handle,("您要从该配置中删除字段" + Item->SubItems->Strings[0] + "吗?").c_str(),
    "警告",MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2) == IDNO)
    return;
  BillConfig->DeleteField(BillName,Item->SubItems->Strings[0]);
  lvFields->Items->Delete(Item->Index);
  BillChanged = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillFormat::lvFieldsDragOver(TObject *Sender,
      TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
  TListItem * DestItem = lvFields->GetItemAt(X,Y);
  Accept = DestItem != NULL && DestItem != lvFields->Selected;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillFormat::lvFieldsDragDrop(TObject *Sender,
      TObject *Source, int X, int Y)
{
  TListItem * DestItem = lvFields->GetItemAt(X,Y);
  TListItem * Item     = lvFields->Selected;
  int SourIndex,DestIndex;
  SourIndex = Item->Index;
  DestIndex = DestItem->Index;
  TListItem * NewItem;
  if(SourIndex < DestIndex)
    NewItem  = lvFields->Items->Insert(DestIndex + 1);
  else
    NewItem  = lvFields->Items->Insert(DestIndex);
  NewItem->Assign(Item);
  _di_IXMLNode RecordNode = BillConfig->GetRecordNode(BillName);
  _di_IXMLNode SourNode   = BillConfig->GetFieldNode(BillName,Item->SubItems->Strings[0]);
  _di_IXMLNode DestNode   = BillConfig->GetFieldNode(BillName,DestItem->SubItems->Strings[0]);
  _di_IXMLNode TmpNode    = SourNode->CloneNode(true);
  //RecordNode->ChildNodes->Remove(DestNode);
  //RecordNode->ChildNodes->Insert(DestIndex,TmpNode);

  RecordNode->ChildNodes->Remove(SourNode);
  RecordNode->ChildNodes->Insert(DestIndex,TmpNode);
  lvFields->Items->Delete(Item->Index);
/*
  TListItem * DestItem = lvFields->GetItemAt(X,Y);
  TListItem * Item     = lvFields->Selected;
  int OldIndex = Item->Index;
  TListItem * NewItem  = lvFields->Items->Insert(DestItem->Index);
  int NewIndex = NewItem->Index;
  if(OldIndex < NewIndex)
    NewIndex -= 1;
  _di_IXMLNode RecordNode   = BillConfig->GetRecordNode(BillName);
  _di_IXMLNode OldFieldNode = BillConfig->GetFieldNode(BillName,Item->SubItems->Strings[0]);
  _di_IXMLNode NewFieldNode = OldFieldNode->CloneNode(true);
  RecordNode->ChildNodes->Delete(OldIndex);
  RecordNode->ChildNodes->Insert(OldIndex,NewFieldNode);

  NewItem->Assign(Item);
  lvFields->Items->Delete(Item->Index);
*/
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillFormat::FormKeyPress(TObject *Sender, char &Key)
{
  if(Key == 27)
  {
    Key = 0;
    Close();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillFormat::btnActionClick(TObject *Sender)
{
  int RecordLength = BillConfig->GetFileRecordLength(BillName);
  TfrmRecordFilter * frmRecordFilter = new TfrmRecordFilter(this,FilterList,RecordLength);
  frmRecordFilter->txtBillName->Text = BillName;
  if(frmRecordFilter->ShowModal() == mrOk)
    Modified = true;
  delete frmRecordFilter;  
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillFormat::chkReadByBlockClick(TObject *Sender)
{
  Label12->Enabled = chkReadByBlock->Checked;
  txtBlockLength->Enabled = chkReadByBlock->Checked;
  Label13->Enabled = chkReadByBlock->Checked;
  Label11->Enabled = chkReadByBlock->Checked;
  txtBlockHeaderLength->Enabled = chkReadByBlock->Checked;
  Label14->Enabled = chkReadByBlock->Checked;
  Label15->Enabled = chkReadByBlock->Checked;
  txtBlockTrailLength->Enabled = chkReadByBlock->Checked;
  Label16->Enabled = chkReadByBlock->Checked;
  btnAction->Enabled = chkReadByBlock->Checked && BillName != "";
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillFormat::FormDestroy(TObject *Sender)
{
  for(int n = 0;n < FilterList->Count;n++)
  {
    delete ((RecordFilter *)FilterList->Items[n]);
  }
  delete FilterList;
  FilterList = NULL;
}
//---------------------------------------------------------------------------

bool __fastcall TfrmBillFormat::FormHelp(WORD Command, int Data,
      bool &CallHelp)
{
    if(Command != HELP_CONTEXTPOPUP)
    {
        CallHelp = false;
        return true;
    }
    WinHelp(Handle,(HelpFilePath + "billformat.hlp").c_str(),HELP_CONTEXTPOPUP,Data);
    CallHelp = false;
    return true;  
}
//---------------------------------------------------------------------------



