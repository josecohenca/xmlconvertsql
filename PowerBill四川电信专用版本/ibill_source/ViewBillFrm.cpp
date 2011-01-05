//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ViewBillFrm.h"
#include "MainFrm.h"
#include "ExportToFileFrm.h"
#include "ExportToDBFrm.h"
#include "FilterRptFrm.h"
#include <Clipbrd.hpp>
#include "OpeningFileFrm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzListVw"
#pragma link "ALStatusBar"
#pragma link "RzPrgres"
#pragma link "RzPanel"
#pragma link "RzButton"
#pragma link "RzRadChk"
#pragma resource "*.dfm"
TfrmViewBill *frmViewBill;

FARPROC OldProc = NULL; //用于保存旧的ListView控件的WindowProc
/**
* 新的ListView的WindowProc
*/
LRESULT CALLBACK ListViewProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
  //处理Window消息
  if(uMsg == WM_LBUTTONDOWN)
  {
    if((wParam & MK_CONTROL) == 0)
    {
      frmViewBill->SelectedItems->Clear();
    }
  }
  //调用原ListView的WindowProc
  return (::CallWindowProc(OldProc,hWnd,uMsg,wParam,lParam));
}
//---------------------------------------------------------------------------
__fastcall TfrmViewBill::TfrmViewBill(TComponent* Owner,TBillConfig  * ABillConfig)
  : TForm(Owner)
{
  frmViewBill = this;
  BillFile    = NULL;
  BillConfig  = ABillConfig;
  FieldList   = new TList();
  SelectedItems = new TList();
  frmRecordDetail = new TfrmRecordDetail(this);
}
//---------------------------------------------------------------------------
void __fastcall TfrmViewBill::lvBillsData(TObject *Sender, TListItem *Item)
{
  if(!BillFile->ReadSingleRecord(Item->Index))
    return;
  Item->Caption = AnsiString(Item->Index + 1);
  for(int n = 0;n < FieldList->Count;n++)
  {
    Item->SubItems->Add(((TBillField *)FieldList->Items[n])->GetFieldValue(cbxTrans->Checked));
  }
  int Index = SelectedItems->IndexOf((void *)Item->Index);
  if(Item->Selected&&Index < 0)
    SelectedItems->Add((void *)Item->Index);
  else if(!Item->Selected&&Index > -1)
    SelectedItems->Delete(Index);
  StatusBar1->Panels->Items[1]->Text = "共选择了" + IntToStr(SelectedItems->Count) + "条记录";      
}
//---------------------------------------------------------------------------
void __fastcall TfrmViewBill::OnFTPWorkBegin(TMessage Message)
{
  /*RzProgressBar1->TotalParts = Message.LParam;
  RzProgressBar1->PartsComplete = 0;
  RzProgressBar1->Visible = true;
  Label1->Visible = true;*/
}
void __fastcall TfrmViewBill::OnFTPWork(TMessage Message)
{
  //RzProgressBar1->PartsComplete = Message.LParam;
}

//---------------------------------------------------------------------------
bool __fastcall TfrmViewBill::OpenBillFile(AnsiString AFormatType,AnsiString ABillName,AnsiString AFilePath,AnsiString AFileName,unsigned int FileSize)
{
  Screen->Cursor = crHourGlass;
  if(BillFile != NULL)
  {
    delete BillFile;
    BillFile = NULL;
  }
  FilePath = AFilePath;
  FileName = AFileName;
  BillName = ABillName;
  FormatType = AFormatType;
  TBillRecord * BillRecord;
  AnsiString s;
  if(FilePath.SubString(1,6) == "ftp://")
    s = FilePath + "|" + FileName;
  else
    s = FilePath + FileName;
  int FileFormat = BillConfig->TransFormatType(FormatType);
  switch(FileFormat)
  {
  case BIN_BILL_FORMAT:
    BillFile = new TBinBillFile(BillName,BillConfig,Handle,frmMain->TempDirectory);
    break;
  case TXT_BILL_FORMAT:
    BillFile = new TTxtBillFile(BillName,BillConfig,Handle,frmMain->TempDirectory);
    break;
  case SEARCH_RESULT_FORMAT:
    /*TSearchResultBillFile构造函数的第一个参数为文件名*/
    BillName = SR_BILL_FORMAT_STR;
    BillFile = new TSearchResultBillFile(s,BillConfig,Handle,frmMain->TempDirectory);
    break;
  default:
    MessageBox(Handle,"定义的话单文件格式不正确!","错误",MB_OK | MB_ICONSTOP);
    return false;
  }
  if((FileSize > 102400 &&
     ((FileFormat == BIN_BILL_FORMAT && BillFile->IsBlockBinFile) || FileFormat == TXT_BILL_FORMAT || FileFormat == SEARCH_RESULT_FORMAT))
    || FilePath.SubString(1,6) == "ftp://")
  {
    TfrmOpeningFile * frmOpeningFile = new TfrmOpeningFile(this);
    frmOpeningFile->BillFile = BillFile;
    frmOpeningFile->FileName = s;
    frmOpeningFile->BillName = ABillName;
    if(frmOpeningFile->ShowModal() != mrOk)
    {
      Screen->Cursor = crDefault;
      delete frmOpeningFile;
      delete BillFile;
      BillFile = NULL;
      return false;
    }
    delete frmOpeningFile;
  }
  else
  {
    try
    {
      BillFile->OpenBillFile(s,ABillName);
    }
    catch(Exception * E)
    {
      Screen->Cursor = crDefault;
      MessageBox(Handle,("不能打开文件:" + E->Message).c_str(),"错误",MB_OK | MB_ICONSTOP);
      delete BillFile;
      BillFile = NULL;
      return false;
    }  
  }
/*
  int ErrCount = 0;
OPENBILLFILE:
  try
  {
    BillFile->OpenBillFile(s,ABillName);
  }
  catch(Exception * E)
  {
    ++ErrCount;
    if(ErrCount < frmMain->AutoRetry || frmMain->AutoRetry == 0)
    {
      goto OPENBILLFILE;
    }
    else
    {
      Screen->Cursor = crDefault;
      MessageBox(Handle,("不能打开文件:" + E->Message).c_str(),"错误",MB_OK | MB_ICONSTOP);
      return false;
    }
  }
*/
  btnFilterReport->Enabled = BillFile->SkipRecordCount > 0;
  //RzPanel1->Visible = btnFilterReport->Enabled;
  Image1->Visible = btnFilterReport->Enabled;
  Label3->Visible = btnFilterReport->Enabled;
  if(RzPanel1->Visible)
    Label3->Caption = "提示:由于启用了话单过滤规则,该话单文件中有" + IntToStr(BillFile->SkipRecordCount) + "条记录被过滤";
  FieldList->Clear();
  TBillField * BillField;
  for(int n = 0;n < BillFile->FieldCount;n++)
  {
    BillField = BillFile->GetBillField(n);
    if(!BillField->DisplayInList)
      continue;
    FieldList->Add((void *)BillField);
  }
  for(int n = lvBills->Columns->Count - 1;n > 0;n--)
  {
    lvBills->Columns->Delete(n);
  }
  TListColumn * Column;
  for(int n = 0;n < FieldList->Count;n++)
  {
    BillField = (TBillField *)FieldList->Items[n];
    Column = lvBills->Columns->Add();
    Column->Caption = BillField->Label;
    if(BillField->DisplayWidth > 0)
      Column->Width = BillField->DisplayWidth;
    switch(BillField->DisplayAlign)
    {
    case ALIGN_CENTER:
      Column->Alignment = taCenter;
      break;
    case ALIGN_RIGHT:
      Column->Alignment = taRightJustify;
      break;
    default:
      Column->Alignment = taLeftJustify;
      break;
    }
  }
  Column = lvBills->Columns->Add();
  Column->Width = 0;
  Column->MaxWidth = 1;
  Column->MinWidth = 0;
  lvBills->Items->Count = BillFile->GetRecordCount();
  if(BillName == SR_BILL_FORMAT_STR)
    StatusBar1->Panels->Items[0]->Text = "文件:" + FileName + " 查找结果文件 话单类型:" + BillFile->GetBillName() + " 记录数:" + IntToStr(lvBills->Items->Count);
  else
    StatusBar1->Panels->Items[0]->Text = "文件:" + FileName + " 话单类型:" + BillName + "  记录数:" + IntToStr(lvBills->Items->Count);
  Screen->Cursor = crDefault;
  return true;
}
void __fastcall TfrmViewBill::FormDestroy(TObject *Sender)
{
  if(frmRecordDetail != NULL)
  {
    delete frmRecordDetail;
    frmRecordDetail = NULL;
  }
  if(FieldList != NULL)
  {
    delete FieldList;
    FieldList = NULL;
  }
  if(BillFile != NULL)
  {
    delete BillFile;
    BillFile = NULL;
  }
  if(SelectedItems != NULL)
  {
    delete SelectedItems;
    SelectedItems = NULL;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmViewBill::lvBillsColumnClick(TObject *Sender,
      TListColumn *Column)
{
  if(Column->Index == 0 || Column->Index == lvBills->Columns->Count - 1)
    return;
  if(frmMain->SortConfirm != 0 && BillFile->GetRecordCount() > frmMain->SortConfirm)
  {
    if(MessageBox(Handle,("当前文件中共有" + IntToStr(BillFile->GetRecordCount()) + "条记录需要排序,"\
      "这可能需要一些时间,是否继续?").c_str(),"问题",MB_YESNO | MB_ICONQUESTION) == IDNO)
      return;
  }
  Screen->Cursor = crHourGlass;
  lvBills->HeaderSortColumn = Column->Index;
  Column->Tag = Column->Tag * 1;
  if(Column->Tag > 0)
  {
    Column->Tag = -1;
    lvBills->HeaderSortDirection = sdAscending;
  }
  else
  {
    Column->Tag = 1;
    lvBills->HeaderSortDirection = sdDescending;
  }
  BillFile->SortFile((TBillField * )FieldList->Items[lvBills->HeaderSortColumn - 1],Column->Tag);
  lvBills->UpdateItems(0,BillFile->GetRecordCount() - 1);
  Screen->Cursor = crDefault;   
}
//---------------------------------------------------------------------------

void __fastcall TfrmViewBill::btnSaveAsClick(TObject *Sender)
{
  if(BillConfig->TransFormatType(FormatType) == SEARCH_RESULT_FORMAT)
  {
    AnsiString OldFilter = frmMain->SaveDialog1->Filter;
    frmMain->SaveDialog1->Filter += "|查询结果文件(*.RST)|*.rst";
    bool Continue = frmMain->SaveDialog1->Execute();
    frmMain->SaveDialog1->Filter = OldFilter;
    if(!Continue)
      return;
  }
  else if(!frmMain->SaveDialog1->Execute())
    return;
  int DestFileType;
  switch(frmMain->SaveDialog1->FilterIndex)
  {
  case 1:
    DestFileType = EXPORT_DBF;
    break;
  case 2:
    DestFileType = EXPORT_CSV;
    break;
  case 3:
    DestFileType = EXPORT_TXT;
    break;
  case 4:
    DestFileType = EXPORT_ORI;
    break;
  case 5:
    {
      Screen->Cursor = crHourGlass;
      if(CopyFile((FilePath + FileName).c_str(),frmMain->SaveDialog1->FileName.c_str(),false))
      {
        MessageBox(Handle,"查询结果已保存!","信息",MB_OK | MB_ICONINFORMATION);
      }
      else
      {
        MessageBox(Handle,("保存文件错误:" + GetErrorString(GetLastError())).c_str(),"错误",MB_OK | MB_ICONSTOP); 
      }
      Screen->Cursor = crDefault;
      return;
    }
  }
  TfrmExportToFile * frmExportToFile = new TfrmExportToFile(this);
  if(FilePath.SubString(1,6) == "ftp://")
    frmExportToFile->FileList->AddObject(FilePath + "|" + FileName,NULL);
  else
    frmExportToFile->FileList->AddObject(FilePath + FileName,(TObject *)GetLocalFileSize(FilePath + FileName));
  frmExportToFile->BillNameList->Add(BillName);
  frmExportToFile->DestFileName = frmMain->SaveDialog1->FileName;
  frmExportToFile->DestFileType = DestFileType;
  frmExportToFile->IsSearchResult = FormatType == SR_BILL_FORMAT_STR;
  if(frmExportToFile->CreateBillFileClass(BillName))
    frmExportToFile->ShowModal();
  delete frmExportToFile;
}
//---------------------------------------------------------------------------

void __fastcall TfrmViewBill::btnExportToDBClick(TObject *Sender)
{
  TfrmExportToDB * frmExportToDB = new TfrmExportToDB(this);
  TListItem * Item;
  Screen->Cursor = crHourGlass;
  Item = frmExportToDB->lvFileList->Items->Add();
  Item->Caption = FileName;
  if(FormatType == SR_BILL_FORMAT_STR)
  {
    Item->SubItems->Add("话单查找结果");
    Item->SubItems->Add("");
    frmExportToDB->IsSearchResult = true;
  }
  else
  {
    Item->SubItems->Add(FormatType);
    Item->SubItems->Add(BillName);
  }
  Item->SubItems->Add("0"); //文件大小
  Item->SubItems->Add(BillFile->GetRecordCount());
  Item->SubItems->Add("");  //修改日期
  Item->SubItems->Add(FilePath);
  Item->ImageIndex = 95;
  Screen->Cursor = crDefault;
  frmExportToDB->ShowModal();
  delete frmExportToDB;   
}
//---------------------------------------------------------------------------


void __fastcall TfrmViewBill::btnFilterReportClick(TObject *Sender)
{
  TfrmFilterRpt * frmFilterRpt = new TfrmFilterRpt(this,BillFile,FileName);
  frmFilterRpt->ShowModal();
  delete frmFilterRpt;  
}
//---------------------------------------------------------------------------

void __fastcall TfrmViewBill::FormShow(TObject *Sender)
{
  frmViewBill = this;
  OldProc = (FARPROC)::SetWindowLong(lvBills->Handle,GWL_WNDPROC,(long)ListViewProc);
}
//---------------------------------------------------------------------------

void __fastcall TfrmViewBill::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  ::SetWindowLong(lvBills->Handle,GWL_WNDPROC,(long)OldProc);
  frmViewBill = NULL;  
}
//---------------------------------------------------------------------------

void __fastcall TfrmViewBill::lvBillsDataStateChange(TObject *Sender,
      int StartIndex, int EndIndex, TItemStates OldState,
      TItemStates NewState)
{
  bool Selected = OldState.Contains(isSelected);
  int Index;
  for(int n = StartIndex;n <= EndIndex;n++)
  {
    Index = SelectedItems->IndexOf((void *)n);
    if(Selected&&Index < 0)
      SelectedItems->Add((void *)n);
    else if(!Selected&&Index > -1)
      SelectedItems->Delete(Index);
  }
  //btnSaveSelected->Enabled = SelectedItems->Count > 0;
  StatusBar1->Panels->Items[1]->Text = "共选择了" + IntToStr(SelectedItems->Count) + "条记录";
}
//---------------------------------------------------------------------------

void __fastcall TfrmViewBill::PopupMenu1Popup(TObject *Sender)
{
  menuCopyRecord->Enabled   = SelectedItems->Count > 0;
  menuRecordDetail->Enabled = SelectedItems->Count > 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmViewBill::menuCopyRecordClick(TObject *Sender)
{
  if(SelectedItems->Count == 0)
    return;
  Screen->Cursor = crHourGlass;
  AnsiString Text = "序号\t";
  int n,j;
  for(n = 0;n < FieldList->Count - 1;n++)
  {
    Text += ((TBillField *)FieldList->Items[n])->Label + "\t";
  }
  Text += ((TBillField *)FieldList->Items[n])->Label + "\r\n";
  for(n = 0;n < SelectedItems->Count;n++)
  {
    BillFile->ReadSingleRecord((int)SelectedItems->Items[n]);
    Text += IntToStr(((int)SelectedItems->Items[n]) + 1) + "\t";
    for(j = 0;j < FieldList->Count - 1;j++)
    {
      Text += ((TBillField *)FieldList->Items[j])->GetFieldValue(cbxTrans->Checked) + "\t";
    }
    Text += ((TBillField *)FieldList->Items[j])->GetFieldValue(cbxTrans->Checked) + "\r\n";
  }
  TClipboard * Clipboard = new TClipboard();
  Clipboard->AsText = Text;
  delete Clipboard;
  Screen->Cursor = crDefault;  
}
//---------------------------------------------------------------------------


void __fastcall TfrmViewBill::menuRecordDetailClick(TObject *Sender)
{
  if(SelectedItems->Count == 0)
    return;
  if(frmRecordDetail == NULL)
  {
    frmRecordDetail = new TfrmRecordDetail(this);
  }
  frmRecordDetail->SetBillName(BillFile,BillFile->GetBillName(),BillFile->GetShortFileName());
  if(!BillFile->ReadSingleRecord((int)SelectedItems->Items[0]))
    return;
  if(BillFile->FileType == TXT_BILL_FORMAT || frmRecordDetail->cbxViewAsText->Checked) 
  {
    char * Buffer = new char[BillFile->GetCurRecordLength()];
    strncpy(Buffer,(char *)(BillFile->GetRecordBuffer()),BillFile->GetCurRecordLength());
    char * pch = strchr(Buffer,'\n');
    if(pch != NULL)
      * pch = 0;
    pch = strchr(Buffer,'\r');
    if(pch != NULL)
      * pch = 0;
    frmRecordDetail->SetBuffer(Buffer,strlen(Buffer));
    frmRecordDetail->txtRecordLength->Text = strlen(Buffer);
    frmRecordDetail->txtRecordData->Lines->Clear();
    frmRecordDetail->txtRecordData->Lines->Add(Buffer);
    delete[] Buffer;
    for(int n = 1;n <= frmRecordDetail->txtRecordData->Text.Length();n++)
    {
      if(frmRecordDetail->txtRecordData->Text.SubString(n,1) == BillFile->Separator)
      {
        frmRecordDetail->txtRecordData->SelStart  = n - 1;
        frmRecordDetail->txtRecordData->SelLength = 1;
        frmRecordDetail->txtRecordData->SelAttributes->Color = clRed;
        //frmViewOriData->txtRecordData->SelAttributes->Style = frmViewOriData->txtRecordData->SelAttributes->Style << fsBold;
      }
      frmRecordDetail->txtRecordData->SelStart  = 0;
      frmRecordDetail->txtRecordData->SelLength = 0;
    }
    frmRecordDetail->txtRecordData->SelectAll();
    frmRecordDetail->txtRecordData->SelAttributes->Name = Font->Name;
    frmRecordDetail->txtRecordData->SelStart  = 0;
    frmRecordDetail->txtRecordData->SelLength = 0;
  }
  else if(BillFile->FileType == BIN_BILL_FORMAT)
  {
    frmRecordDetail->txtRecordData->Lines->Clear();
    int Len = BillFile->GetCurRecordLength();
    frmRecordDetail->txtRecordLength->Text = Len;
    unsigned char * Buffer = (unsigned char *)BillFile->GetRecordBuffer();
    frmRecordDetail->SetBuffer(Buffer,Len);
    int Position = -1;
    AnsiString str;
    do
    {
      //str = "";
      //for(int n = 0;n < 16;n++)
      //{
        ++Position;
        if(Position >= Len)
          break;
        str = str + IntToHex( * (Buffer + Position),2) + " ";
      //}
      //frmRecordDetail->txtRecordData->Lines->Add(str);
      //frmRecordDetail->txtRecordData->Lines->Text = frmRecordDetail->txtRecordData->Lines->Text + " " + IntToHex( * (Buffer + Position),2);
    }while(Position < Len);
    frmRecordDetail->txtRecordData->Lines->Text = str;
  }
  else
  {
    delete frmRecordDetail;
    frmRecordDetail = NULL;
    return;
  }
  frmRecordDetail->txtRecordPosition->Text = BillFile->GetCurRecordPosition() + 1;
  frmRecordDetail->ShowFieldValues((int)SelectedItems->Items[0]);
  frmRecordDetail->Show();
}
//---------------------------------------------------------------------------



void __fastcall TfrmViewBill::cbxTransClick(TObject *Sender)
{
  Screen->Cursor = crHourGlass;
  lvBills->UpdateItems(0,BillFile->GetRecordCount() - 1);
  Screen->Cursor = crDefault;    
}
//---------------------------------------------------------------------------

