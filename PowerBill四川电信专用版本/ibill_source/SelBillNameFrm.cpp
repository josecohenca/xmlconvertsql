//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SelBillNameFrm.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzButton"
#pragma link "RzListVw"
#pragma link "RzRadChk"
#pragma resource "*.dfm"
TfrmSelBillName *frmSelBillName;
//---------------------------------------------------------------------------
__fastcall TfrmSelBillName::TfrmSelBillName(TComponent* Owner,AnsiString AFileName)
  : TForm(Owner)
{
  FileName = AFileName;
  TBillConfig * BillConfig = frmMain->BillConfig;
  TListItem * Item;
  AnsiString BillName;
  for(int n = 0;n < BillConfig->BillRootNode->ChildNodes->Count;n++)
  {
    BillName = BillConfig->GetAttributeNodeValue(BillConfig->BillRootNode->ChildNodes->Nodes[n],"BillName");
    Item = lvBillTypes->Items->Add();
    Item->SubItems->Add(BillName);
    Item->SubItems->Add(BillConfig->TransFormatTypeName(BillConfig->GetFileFormatType(BillName)));
    Item->SubItems->Add(BillConfig->GetAttributeNodeValue(BillConfig->BillRootNode->ChildNodes->Nodes[n],"Description"));
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelBillName::lvBillTypesChange(TObject *Sender,
      TListItem *Item, TItemChange Change)
{
  btnOk->Enabled = lvBillTypes->Selected != NULL;  
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelBillName::btnOkClick(TObject *Sender)
{
  if(lvBillTypes->Selected == NULL)
    return;
  if(MessageBox(Handle,("该操作将会修改" + FileName + "的打开方式.如果选择不正确,可能会导到此文件以后都不能被正确读取.\n"\
    "您确定要用 " + lvBillTypes->Selected->SubItems->Strings[0] + " 来打开此文件吗?").c_str(),
    "提示",MB_YESNO | MB_ICONWARNING) == IDNO)
    return;
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
