//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SearchFrm.h"
#include "MainFrm.h"
#include <Clipbrd.hpp>
#include "BillFileFrm.h"
#include <StrUtils.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzButton"
#pragma link "RzCmboBx"
#pragma link "RzEdit"
#pragma link "RzListVw"
#pragma link "RzPrgres"
#pragma link "RzRadChk"
#pragma link "SkyEdit"
#pragma link "SkyParser"
#pragma link "RzSplit"
#pragma resource "*.dfm"
TfrmSearch *frmSearch;
//---------------------------------------------------------------------------
__fastcall TfrmSearch::TfrmSearch(TComponent* Owner)
  : TForm(Owner)
{
  BillFile = NULL;
  frmViewBill = NULL;
  frmEditSearchParam = NULL;
  BillSearchThread   = NULL;
  BillName = "1";
  FileList     = new TStringList;
  BillNameList = new TStringList;
  MatchFieldList = NULL;           //要查找的字段列表
  MatchFieldOperatorList = NULL;   //查找时的操作符列表
  MatchFieldValueList    = NULL;   //查找的值列表
  MatchModeList = NULL;            //查找的匹配方式列表
  AndOrList     = NULL;
  frmEditSearchParam = new TfrmEditSearchParam(this);
  txtRetryCount->Text = RetryCount;
  SkyEdit2->Modified = false;
  labScriptFile->Caption = "新的查询脚本";
  Label4->Caption = "";
}
bool __fastcall TfrmSearch::SaveScript()
{
  if(CurScriptFileName == "")
  {
    if(!SaveDialog1->Execute())
      return false;
    CurScriptFileName = SaveDialog1->FileName;
    labScriptFile->Caption = CurScriptFileName;
  }
  try
  {
    SkyEdit2->Lines->SaveToFile(CurScriptFileName);
    SkyEdit2->Modified = false;
  }
  catch(Exception * E)
  {
    MessageBox(Handle,E->Message.c_str(),"错误",MB_OK | MB_ICONSTOP);
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------
bool __fastcall TfrmSearch::AddParamsToList(AnsiString ABillName)
{
  if(BillFile != NULL)
    delete BillFile;
  bool Result = false;
  frmEditSearchParam->cbxField->Items->Clear();
  lvParams->Items->Clear();
  VBParser->Words_8->Clear();
  if(ABillName == "")
    return true;
  try
  {
    switch(frmMain->BillConfig->GetFileFormatType(ABillName))
    {
    case BIN_BILL_FORMAT:
      BillFile = new TBinBillFile(ABillName,frmMain->BillConfig,Handle,frmMain->TempDirectory);
      break;
    case TXT_BILL_FORMAT:
      BillFile = new TTxtBillFile(ABillName,frmMain->BillConfig,Handle,frmMain->TempDirectory);
      break;
    }

    TBillField * BillField;
    for(int n = 0;n < BillFile->FieldCount;n++)
    {
      BillField = BillFile->GetBillField(n);
      frmEditSearchParam->cbxField->Items->AddObject(BillField->Label,(TObject *)BillField);
      VBParser->Words_8->Add(BillField->FieldName);
    }
    Result   = true;
    BillName = ABillName;
  }
  catch(Exception * E)
  {
    MessageBox(Handle,E->Message.c_str(),"错误",MB_OK | MB_ICONSTOP);
  }
  return Result;
}
void __fastcall TfrmSearch::FormDestroy(TObject *Sender)
{
  if(BillFile != NULL)
  {
    delete BillFile;
    BillFile = NULL;
  }
  if(frmEditSearchParam != NULL)
  {
    delete frmEditSearchParam;
    frmEditSearchParam = NULL;
  }
  if(FileList != NULL)
  {
    delete FileList;
    FileList = NULL;
  }
  if(BillNameList != NULL)
  {
    delete BillNameList;
    BillNameList = NULL;
  }
  if(MatchFieldList != NULL)
  {
    delete MatchFieldList;
    MatchFieldList = NULL;
  }
  if(MatchFieldOperatorList != NULL)
  {
    delete MatchFieldOperatorList;
    MatchFieldOperatorList = NULL;
  }
  if(MatchFieldValueList != NULL)
  {
    delete MatchFieldValueList;
    MatchFieldValueList = NULL;
  }
  if(MatchModeList != NULL)
  {
    delete MatchModeList;
    MatchModeList = NULL;
  }
  if(AndOrList != NULL)
  {
    delete AndOrList;
    AndOrList = NULL;
  }
  if(frmViewBill != NULL)
  {
    delete frmViewBill;
    frmViewBill = NULL;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmSearch::btnCloseClick(TObject *Sender)
{
  Close();  
}
//---------------------------------------------------------------------------
void __fastcall TfrmSearch::menuClearParamClick(TObject *Sender)
{
  TListItem * Item;
  for(int n = lvParams->Items->Count - 1;n > -1;n--)
  {
    Item = lvParams->Items->Item[n];
    if(Item->Selected)
      lvParams->Items->Delete(Item->Index);
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmSearch::PopupMenu1Popup(TObject *Sender)
{
  menuClearParam->Enabled    = lvParams->SelCount > 0;
  menuEditParameter->Enabled = menuClearParam->Enabled;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSearch::FormKeyPress(TObject *Sender, char &Key)
{
  switch(Key)
  {
  case VK_RETURN:
    if(lvParams->Visible)
    {
      menuEditParameterClick(NULL);
      Key = 0;
    }
    break;
  case VK_ESCAPE:
    Close();
    Key = 0;
    break;
  }
}
void __fastcall TfrmSearch::OnFileProcessed(TMessage Message)
{
  Label4->Caption = "共有" + IntToStr(FileList->Count) + "个文件需要查找,当前完成了" + IntToStr(Message.WParam) + "个";
  RzProgressBar2->PartsComplete += 1;
  Label2->Caption = "找到" + IntToStr(BillSearchThread->FoundCount) + "条记录";
}
void __fastcall TfrmSearch::OnSetProcessMaxCount(TMessage Message)
{
  if(Message.LParam == NULL)
  {
    RzProgressBar1->PartsComplete = RzProgressBar1->TotalParts;
  }
  else
  {
    RzProgressBar1->TotalParts = Message.WParam;
    RzProgressBar1->PartsComplete = 0;
    Label1->Caption = "正在查找文件" + AnsiString((const char *)Message.LParam);
  }
}
void __fastcall TfrmSearch::OnSetProcessPos(TMessage Message)
{
  RzProgressBar1->PartsComplete = Message.WParam;
  //if(BillFile->FileLoaded)
  //{
  Label2->Caption = "找到" + IntToStr(BillSearchThread->FoundCount) + "条记录";
  //}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSearch::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  if(BillSearchThread != NULL)
  {
    MessageBox(Handle,"请先停止查找!","提示",MB_OK | MB_ICONWARNING);
    Action = caNone;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::btnQueryClick(TObject *Sender)
{
  //
  if(btnQuery->Caption == "停止")
  {
    if(MessageBox(Handle,"您要停止查找吗?","问题",MB_YESNO | MB_ICONQUESTION) == IDNO)
      return;
    //BillSearchThread->Suspended = true;
    //FoundCount = BillSearchThread->FoundCount;
    if(BillSearchThread == NULL)
      return;
    BillSearchThread->Continue = false;
    BillFile->TermCurOperation();
    /*
    if(BillSearchThread->TempFile != NULL)
    {
      fclose(BillSearchThread->TempFile);
      BillSearchThread->TempFile = NULL;
    }
    TerminateThread((void *)BillSearchThread->Handle,0);
    BillSearchThread = NULL;
    ShowSearchResult();
    */
  }
  else
  {
    char chTempFileName[MAX_PATH];
    GetTempFileName(frmMain->TempDirectory.c_str(),"IBILLQUERY",0,&chTempFileName[0]);
    TempFileName = chTempFileName;

    Screen->Cursor = crHourGlass;
    TListItem * Item;
    for(int n = 0;n < lvFileList->Items->Count;n++)
    {
      Item =lvFileList->Items->Item[n];
      Item->ImageIndex = 128;
      Item->SubItems->Strings[3] = "";
    }
    if(MatchFieldList == NULL)
      MatchFieldList = new TStringList;
    else
      MatchFieldList->Clear();
    AnsiString Script;
    if(btnVBScript->Down) //用脚本查询
    {
      Script = AnalyseScript();  //分析脚本
      if(Script == "")
      {
        Screen->Cursor = crDefault;
        return;
      }
    }
    else
    {
      if(MatchFieldOperatorList == NULL)
        MatchFieldOperatorList = new TStringList;
      else
        MatchFieldOperatorList->Clear();
      if(MatchFieldValueList == NULL)
        MatchFieldValueList = new TStringList;
      else
        MatchFieldValueList->Clear();
      if(MatchModeList == NULL)
        MatchModeList = new TList;
      else
        MatchModeList->Clear();
      if(AndOrList == NULL)
        AndOrList = new TList;
      else
        AndOrList->Clear();
      for(int n = 0;n < lvParams->Items->Count;n++)
      {
        Item = lvParams->Items->Item[n];
        MatchFieldList->AddObject(Item->SubItems->Strings[0],(TObject *)Item->Data);
        MatchFieldOperatorList->Add(Item->SubItems->Strings[1]);
        MatchFieldValueList->Add(Item->SubItems->Strings[2]);
        if(rbAnd->Checked)
          AndOrList->Add((TObject *)1);  //AND
        else
          AndOrList->Add((TObject *)2);  //OR
        MatchModeList->Add((TObject *)(int)(Item->SubItems->Strings[3] == "√"));
      }
    }
    RzGroupBox2->Enabled = false;
    lvParams->Enabled = false;
    btnAddParameter->Enabled = false;
    btnDelParameter->Enabled = false;
    rbAnd->Enabled = false;
    rbOr->Enabled  = false;
    cbxBillType->Enabled = false;
    Label9->Enabled      = false;
    btnVBScript->Enabled = false;
    Label4->Caption = "共有" + IntToStr(FileList->Count) + "个文件需要查找,当前完成了0个";
    Label2->Caption = "";
    Label4->Tag = 0;
    btnQuery->Caption    = "停止";
    btnQuery->ImageIndex = 113;
    RzProgressBar1->Visible = true;
    RzProgressBar2->Visible = true;
    Label10->Visible = true;
    Label11->Visible = true;

    Label1->Caption = "正在查找";
    RzProgressBar1->PartsComplete = 0;
    RzProgressBar2->PartsComplete = 0;
    RzProgressBar2->TotalParts = FileList->Count;
    BillSearchThread = new TBillSearchThread(true);
    BillSearchThread->AutoScroll = menuAutoScroll->Checked;
    //测试脚本
    /*BillSearchThread->VBScript =
      "Function MatchBill(Str1,Str2)\n"
      " 'MsgBox(Str1 & \"  \" & Str2)\n"\
      " if len(Str1) = 7 AND mid(Str1,1,4) = \"8332\" and len(Str2) = 7 then\n"\
      "   MatchBill = 1\n"\
      " else \n"\
      "  MatchBill = 0\n"\
      " end if\n"\
      "End Function"
      ;
    //ShowMessage(BillSearchThread->VBScript);
    BillSearchThread->UseScript = true;
    MatchFieldList->Clear();
    MatchFieldList->AddObject("CallerNbr",(TObject *)BillFile->GetBillField("CallerNbr"));
    MatchFieldList->AddObject("CalledNbr",(TObject *)BillFile->GetBillField("CalledNbr"));
    //End of 测试脚本*/
    BillSearchThread->UseScript = RzPanel2->Visible;
    if(BillSearchThread->UseScript)
      BillSearchThread->VBScript = Script;
    BillSearchThread->FileList       = FileList;
    BillSearchThread->BillNameList   = BillNameList;
    BillSearchThread->BillFile       = BillFile;
    BillSearchThread->ListView       = lvFileList;
    BillSearchThread->MatchFieldList = MatchFieldList;
    BillSearchThread->MatchFieldOperatorList = MatchFieldOperatorList;
    BillSearchThread->MatchFieldValueList = MatchFieldValueList;
    BillSearchThread->MatchModeList   = MatchModeList;
    BillSearchThread->AndOrList       = AndOrList;
    BillSearchThread->TempFileName    = TempFileName;
    BillSearchThread->FreeOnTerminate = true;
    BillSearchThread->OnTerminate     = OnSearchComplete;
    BillSearchThread->hWnd            = Handle;
    StartTime = Now();
    BillSearchThread->Suspended       = false;
    Screen->Cursor = crDefault;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmSearch::OnSearchComplete(TObject * Sender)
{
  //if(BillSearchThread->ErrorMessage != "")
  //  MessageBox(Handle,BillSearchThread->ErrorMessage.c_str(),"警告",MB_OK | MB_ICONWARNING);
  //else
  if(BillSearchThread->ErrorCount > 0)
    MessageBox(Handle,("在查找过程中发生了" + IntToStr(BillSearchThread->ErrorCount) + "个文件错误,查找结果可能是不完整的,建议重新查找一次.").c_str(),"警告",MB_OK | MB_ICONWARNING);
  FoundCount = BillSearchThread->FoundCount;
  if(FoundCount > 0)
  {
    TfrmBillFile * frmBillFile = (TfrmBillFile * )Owner;
    frmBillFile->TempFileList->Insert(0,TempFileName);
    TMenuItem * MenuItem = new TMenuItem(Owner);
    MenuItem->Caption = "查询结果" + IntToStr(frmBillFile->TempFileList->Count) + "[在" + FormatDateTime("mm-dd hh:nn",Now()) + " 共" + IntToStr(FoundCount) + "条记录]";
    //MenuItem->Tag = frmBillFile->TempFileList->Count - 1;
    MenuItem->OnClick = frmBillFile->btnReviewClick;
    MenuItem->ImageIndex = 4;
    frmBillFile->PopupMenu4->Items->Insert(0,MenuItem);
    for(int n = 0;n < frmBillFile->PopupMenu4->Items->Count;n++)
    {
      frmBillFile->PopupMenu4->Items->Items[n]->Tag = n;
    }
  }
  else
  {
    DeleteFile(TempFileName.c_str());
  }
  BillSearchThread = NULL;
  Label1->Caption = "共找到" + IntToStr(FoundCount) + "条记录";
  Label2->Caption = "共找到" + IntToStr(FoundCount) + "条记录,耗时   " + AnsiString(Now() - StartTime); 
  RzGroupBox2->Enabled = true;
  lvParams->Enabled = true;
  btnQuery->Enabled = true;
  btnAddParameter->Enabled = lvParams->Visible;
  btnDelParameter->Enabled = lvParams->Visible;
  cbxBillType->Enabled = true;
  rbAnd->Enabled  = true;
  rbOr->Enabled   = true;
  Label9->Enabled      = cbxBillType->Items->Count > 0;
  cbxBillType->Enabled = cbxBillType->Items->Count > 0;
  btnQuery->Caption = "查找";
  btnQuery->Enabled = true;
  btnVBScript->Enabled = true;
  btnQuery->ImageIndex    = 114;
  RzProgressBar1->Visible = false;
  RzProgressBar2->Visible = false;
  Label10->Visible = false;
  Label11->Visible = false;
  Label1->Caption = "共找到" + IntToStr(FoundCount) + "条记录";
  if(FoundCount > 0)
    ShowSearchResult();
  else
    MessageBox(Handle,"没有找到符合条件的记录!","信息",MB_OK | MB_ICONINFORMATION);
}
void __fastcall TfrmSearch::ShowSearchResult()
{
  if(frmViewBill == NULL)
    frmViewBill = new TfrmViewBill(this,frmMain->BillConfig);
  try
  {
    frmViewBill->OpenBillFile(SR_BILL_FORMAT_STR,"",ExtractFilePath(TempFileName),ExtractFileName(TempFileName),GetLocalFileSize(TempFileName));
    frmViewBill->StatusBar1->Panels->Items[0]->Text = "话单查找结果 共找到" + IntToStr(FoundCount) + "条记录";
    frmViewBill->ShowModal();
    delete frmViewBill;
    frmViewBill = NULL;
  }
  catch(Exception * E)
  {
    delete frmViewBill;
    frmViewBill = NULL;
    MessageBox(Handle,E->Message.c_str(),"错误",MB_OK | MB_ICONSTOP);
  }
}
void __fastcall TfrmSearch::OnSaveTempFile(TMessage Message)
{
  Label1->Caption = "正在写入临时文件,共有" + IntToStr(Message.WParam) + "条记录要写入,请稍候...";
  btnQuery->Enabled = false;  
}
//---------------------------------------------------------------------------
/*
void __fastcall TfrmSearch::OnFTPWorkBegin(TMessage Message)
{
  Label1->Caption = "正在下载文件" + AnsiString((const char *)Message.WParam);
  RzProgressBar1->TotalParts = Message.LParam;
  RzProgressBar1->PartsComplete = 0;
}
void __fastcall TfrmSearch::OnFTPWork(TMessage Message)
{
  RzProgressBar1->PartsComplete = Message.LParam;
}
*/
void __fastcall TfrmSearch::rbRetryClick(TObject *Sender)
{
  Label5->Enabled        = rbRetry->Checked;
  txtRetryCount->Enabled = rbRetry->Checked;
  Label6->Enabled        = rbRetry->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::Action2Execute(TObject *Sender)
{
  if(btnQuery->Caption == "停止")
    return;
  btnQueryClick(NULL);    
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::PopupMenu2Popup(TObject *Sender)
{
  menuCopyFileList->Enabled = lvFileList->Items->Count > 0;  
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::menuCopyFileListClick(TObject *Sender)
{
  AnsiString Text = "";
  int n;
  for(n = 1;n < lvFileList->Columns->Count - 1;n++)
  {
    Text += lvFileList->Columns->Items[n]->Caption + "\t";
  }
  Text += lvFileList->Columns->Items[n]->Caption + "\r\n";
  TListItem * Item;
  for(n = 0;n < lvFileList->Items->Count;n++)
  {
    Item = lvFileList->Items->Item[n];
    Text += Item->SubItems->Strings[0] +  "\t" +
      Item->SubItems->Strings[1] +  "\t" +
      Item->SubItems->Strings[2] +  "\t" +
      Item->SubItems->Strings[3] +  "\t" +
      Item->SubItems->Strings[4] +  "\r\n";
  }
  TClipboard * Clipboard = new TClipboard();
  Clipboard->AsText = Text;
  delete Clipboard;  
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::lvFileListCustomDrawItem(
      TCustomListView *Sender, TListItem *Item, TCustomDrawState State,
      bool &DefaultDraw)
{
  switch(Item->ImageIndex)
  {
  case IMAGE_CUR_ITEM:
    Sender->Canvas->Font->Color = clBlue;
    Sender->Canvas->Font->Style = Sender->Canvas->Font->Style << fsBold;
    break;
  case IMAGE_RETRYING_ITEM:
  case IMAGE_FAILED_ITEM:
    Sender->Canvas->Font->Color = clRed;
    Sender->Canvas->Font->Style = Sender->Canvas->Font->Style >> fsBold;
    break;
  default:
    Sender->Canvas->Font->Color = clBlack;
    Sender->Canvas->Font->Style = Sender->Canvas->Font->Style >> fsBold;
    break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::menuAutoScrollClick(TObject *Sender)
{
  if(menuAutoScroll->Checked)
    menuAutoScroll->Checked = false;
  else
    menuAutoScroll->Checked = true;
  if(BillSearchThread != NULL)
  {
    BillSearchThread->AutoScroll = menuAutoScroll->Checked;  
  }
}
//---------------------------------------------------------------------------


void __fastcall TfrmSearch::menuAddParameterClick(TObject *Sender)
{
/*  frmEditSearchParam->cbxField->ItemIndex = -1;
  frmEditSearchParam->txtContext->Text = "";
  frmEditSearchParam->cbxOperator->ItemIndex = 0;
  frmEditSearchParam->cbxValues->Text = "";
  frmEditSearchParam->cbxAndOr->ItemIndex = 0;
  frmEditSearchParam->cbxFieldChange(NULL);*/
  if(frmEditSearchParam->ShowModal() == mrOk)
  {
    TListItem * Item = lvParams->Items->Add();
    Item->ImageIndex = 48;
    TBillField * Field = (TBillField * )frmEditSearchParam->cbxField->Items->Objects[frmEditSearchParam->cbxField->ItemIndex];
    Item->Data = (void *)Field;
    Item->SubItems->Add(Field->Label); 
    Item->SubItems->Add(frmEditSearchParam->cbxOperator->Text); 
    //Item->SubItems->Add(frmEditSearchParam->cbxAndOr->Text);
    if(frmEditSearchParam->txtContext->Visible)
      Item->SubItems->Add(frmEditSearchParam->txtContext->Text);
    else if(frmEditSearchParam->cbxValues->Visible)
      Item->SubItems->Add(frmEditSearchParam->cbxValues->Text);
    else
      Item->SubItems->Add(
        FormatDateTime("yyyy-mm-dd",frmEditSearchParam->dtDate->DateTime) + " " +
        FormatDateTime("hh:nn:ss",frmEditSearchParam->dtTime->DateTime));
    if(frmEditSearchParam->cbxMatch->Checked)
      Item->SubItems->Add("√");
    else
      Item->SubItems->Add("");     
  } 
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::menuEditParameterClick(TObject *Sender)
{
  TListItem * Item = lvParams->Selected;
  if(Item == NULL)
    return;
  TBillField * Field = (TBillField *)Item->Data;
  frmEditSearchParam->SetSelectedFieldIndex((TBillField * )Item->Data);
  frmEditSearchParam->cbxOperator->ItemIndex =
   frmEditSearchParam->cbxOperator->Items->IndexOf(Item->SubItems->Strings[1]);
  frmEditSearchParam->txtContext->Text = "";
  frmEditSearchParam->cbxValues->Text  = "";
  if(Field->SortType == SORT_AS_DATETIME)
  {
    if(Item->SubItems->Strings[2] == "")
      frmEditSearchParam->dtDate->DateTime = StrToDateTime(FormatDateTime("yyyy-mm-dd",Now()) + " 00:00:00");
    else
      frmEditSearchParam->dtDate->DateTime = StrToDateTime(Item->SubItems->Strings[2]);
    frmEditSearchParam->dtTime->DateTime = frmEditSearchParam->dtDate->DateTime;
  }
  //else if(frmEditSearchParam->cbxValues->Items->Count > 0)
  frmEditSearchParam->cbxValues->Text = Item->SubItems->Strings[2];
  //else
  frmEditSearchParam->txtContext->Text = Item->SubItems->Strings[2];
  frmEditSearchParam->cbxMatch->Checked = Item->SubItems->Strings[3] == "√";
  if(frmEditSearchParam->ShowModal() == mrOk)
  {
    TBillField * Field = (TBillField * )frmEditSearchParam->cbxField->Items->Objects[frmEditSearchParam->cbxField->ItemIndex];
    Item->Data = (void *)Field;
    Item->SubItems->Strings[0] = Field->Label;
    Item->SubItems->Strings[1] = frmEditSearchParam->cbxOperator->Text;
    //Item->SubItems->Strings[2] = frmEditSearchParam->cbxAndOr->Text;
    if(frmEditSearchParam->txtContext->Visible)
      Item->SubItems->Strings[2] = frmEditSearchParam->txtContext->Text;
    else if(frmEditSearchParam->cbxValues->Visible)
      Item->SubItems->Strings[2] = frmEditSearchParam->cbxValues->Text;
    else
      Item->SubItems->Strings[2] =
        FormatDateTime("yyyy-mm-dd",frmEditSearchParam->dtDate->DateTime) + " " +
        FormatDateTime("hh:nn:ss",frmEditSearchParam->dtTime->DateTime);
    if(frmEditSearchParam->cbxMatch->Checked)
      Item->SubItems->Strings[3] = "√";
    else
      Item->SubItems->Strings[3] = "";
  }
/*
  if(frmEditSearchParam->cbxOperator->ItemIndex < 0)
    frmEditSearchParam->cbxOperator->ItemIndex = 0;
  TBillField * BillField = (TBillField * )Item->Data;
  if(BillField->ValueTransList != NULL && BillField->ValueTransList->Count > 0)
  {
    frmEditSearchParam->cbxValues->Items->Clear();
    frmEditSearchParam->cbxValues->Items->Assign(BillField->ValueTransList);
    frmEditSearchParam->cbxValues->Text     = Item->SubItems->Strings[2];
    frmEditSearchParam->cbxValues->Visible  = true;
    frmEditSearchParam->txtContext->Visible = false;
  }
  else
  {
    frmEditSearchParam->txtContext->Text    = Item->SubItems->Strings[2];
    frmEditSearchParam->cbxValues->Visible  = false;
    frmEditSearchParam->txtContext->Visible = true;
  }
  if(BillField->SortType == SORT_AS_DATETIME)
  {
    frmEditSearchParam->cbxValues->Visible  = false;
    frmEditSearchParam->txtContext->Visible = false;    
    frmEditSearchParam->dtDate->Visible = true;
    frmEditSearchParam->dtTime->Visible = true;
    if(Item->SubItems->Strings[2] == "")
      frmEditSearchParam->dtDate->DateTime = StrToDateTime(FormatDateTime("yyyy-mm-dd",Now()) + " 00:00:00");
    else
      frmEditSearchParam->dtDate->DateTime = StrToDateTime(Item->SubItems->Strings[2]);
    frmEditSearchParam->dtTime->DateTime = frmEditSearchParam->dtDate->DateTime;
  }
  else
  {
    frmEditSearchParam->dtDate->Visible = false;
    frmEditSearchParam->dtTime->Visible = false;
  }
  frmEditSearchParam->cbxMatch->Checked = Item->SubItems->Strings[3] == "是";
  if(frmEditSearchParam->ShowModal() == mrOk)
  {
    Item->SubItems->Strings[1] = frmEditSearchParam->cbxOperator->Text;
    if(frmEditSearchParam->txtContext->Visible)
      Item->SubItems->Strings[2] = frmEditSearchParam->txtContext->Text;
    else if(frmEditSearchParam->cbxValues->Visible)
      Item->SubItems->Strings[2] = frmEditSearchParam->cbxValues->Text;
    else
      Item->SubItems->Strings[2] =
        FormatDateTime("yyyy-mm-dd",frmEditSearchParam->dtDate->DateTime) + " " +
        FormatDateTime("hh:nn:ss",frmEditSearchParam->dtTime->DateTime);
    if(frmEditSearchParam->cbxMatch->Checked)
      Item->SubItems->Strings[3] = "是";
    else
      Item->SubItems->Strings[3] = "否";
  }*/  
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::lvParamsDragOver(TObject *Sender,
      TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
  TListItem * SourItem = lvParams->Selected;
  TListItem * DestItem = lvParams->GetItemAt(X,Y);
  Accept = (DestItem != NULL && DestItem != SourItem);  
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::lvParamsDragDrop(TObject *Sender,
      TObject *Source, int X, int Y)
{
  TListItem * SourItem = lvParams->Selected;
  TListItem * DestItem = lvParams->GetItemAt(X,Y);
  TListItem * TempItem = lvParams->Items->Add();
  TempItem->Assign(SourItem);
  SourItem->Assign(DestItem);
  DestItem->Assign(TempItem);
  lvParams->Items->Delete(TempItem->Index);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::cbxBillTypeChange(TObject *Sender)
{
  if(cbxBillType->ItemIndex > -1)
    AddParamsToList(cbxBillType->Text);
  else
    AddParamsToList("");
}
//---------------------------------------------------------------------------

bool __fastcall TfrmSearch::FormHelp(WORD Command, int Data,
      bool &CallHelp)
{
    if(Command != HELP_CONTEXTPOPUP)
    {
        CallHelp = false;
        return true;
    }
    WinHelp(Handle,(HelpFilePath + "search.hlp").c_str(),HELP_CONTEXTPOPUP,Data);
    CallHelp = false;
    return true;    
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::btnVBScriptClick(TObject *Sender)
{
  if(RzPanel2->Visible)
  {
    btnVBScript->Down = false;
  }
  else
  {
    btnVBScript->Down = true;
  }
  RzPanel2->Visible = btnVBScript->Down;
  labScriptFile->Visible = btnVBScript->Down;
  lvParams->Visible = !btnVBScript->Down;
  rbAnd->Visible = lvParams->Visible;
  rbOr->Visible  = lvParams->Visible;
  btnAddParameter->Enabled = lvParams->Visible;
  btnDelParameter->Enabled = lvParams->Visible;
  if(RzPanel2->Visible)
  {
    SkyEdit2->SetFocus();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::menuCopyClick(TObject *Sender)
{
  if(ActiveControl == SkyEdit2)
    SkyEdit2->CopyToClipboard();
  else if(ActiveControl == SkyEdit1)
    SkyEdit1->CopyToClipboard();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::menuCutClick(TObject *Sender)
{
  if(ActiveControl == SkyEdit2)
    SkyEdit2->CutToClipboard();
  else if(ActiveControl == SkyEdit1)
    SkyEdit1->CutToClipboard();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::menuPasteClick(TObject *Sender)
{
  if(ActiveControl == SkyEdit2)
    SkyEdit2->PasteFromClipboard();
  else if(ActiveControl == SkyEdit1)
    SkyEdit1->PasteFromClipboard();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::menuRedoClick(TObject *Sender)
{
  if(ActiveControl == SkyEdit2)
    SkyEdit2->Undo();
  else if(ActiveControl == SkyEdit1)
    SkyEdit1->Undo();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::menuNewScriptClick(TObject *Sender)
{
  if(SkyEdit2->Modified)
  {
    switch(MessageBox(Handle,"是否保存所做的修改?","问题",MB_ICONWARNING | MB_YESNOCANCEL))
    {
    case IDYES:
      if(!SaveScript())
        return;
      break;
    case IDNO:
      break;
    default:
      return;
    }
  }
  SkyEdit2->Text    = "";
  CurScriptFileName = "";
  labScriptFile->Caption = "新的查询脚本";
  SkyEdit2->Modified = false;  
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::menuSaveClick(TObject *Sender)
{
  if(CurScriptFileName == "" && SkyEdit2->Text == "")
    return;
  if(!SaveScript())
    return;
  MessageBox(Handle,"查询脚本已保存!","信息",MB_OK | MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::menuOpenScriptClick(TObject *Sender)
{
  if(SkyEdit2->Modified)
  {
    switch(MessageBox(Handle,"是否保存所做的修改?","问题",MB_ICONWARNING | MB_YESNOCANCEL))
    {
    case IDYES:
      if(!SaveScript())
        return;
      break;
    case IDNO:
      break;
    default:
      return;
    }
  }
  if(!OpenDialog1->Execute())
    return;
  try
  {
    SkyEdit2->Lines->LoadFromFile(OpenDialog1->FileName);
    SkyEdit2->SelLength = 0;
    CurScriptFileName = OpenDialog1->FileName;
  }
  catch(Exception * E)
  {
    MessageBox(Handle,E->Message.c_str(),"错误",MB_OK | MB_ICONSTOP);
    return;
  }
  SkyEdit2->Modified = false;
  labScriptFile->Caption = CurScriptFileName;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TfrmSearch::AnalyseScript()
{
  AnsiString Script;
  AnsiString s;
  for(int n = 0;n < SkyEdit2->Lines->Count;n++)
  {
    s = SkyEdit2->Lines->Strings[n];
    s = AnsiReplaceStr(AnsiReplaceStr(AnsiReplaceStr(AnsiReplaceStr(s,"\n"," "),"\r"," ")," "," "),"\t"," ");
    if(s.SubString(1,1) == "'" || s.SubString(1,4).LowerCase() == "rem "||s.SubString(1,4).LowerCase() == "rem\t") //注释行
      continue;
    Script = Script + " " + SkyEdit2->Lines->Strings[n];
  }
  Script = AnsiReplaceStr(AnsiReplaceStr(AnsiReplaceStr(Script,"\n"," "),"\r"," "),"\t"," ");
  AnsiString LowerCaseScript = Script.LowerCase();

  AnsiString ParamString;
  TBillField * BillField;
  AnsiString LowerCaseFieldName;
  AnsiString ParamName;
  int Pos;
  for(int n = 0;n < BillFile->FieldCount;n++)
  {
    BillField = BillFile->GetBillField(n);
    LowerCaseFieldName = BillField->FieldName.LowerCase();
    Pos = LowerCaseScript.Pos(LowerCaseFieldName);
    if(Pos < 1)
      continue;
    //ParamName = BillField->FieldName.LowerCase();
    if(ParamString == "")
      //ParamString = ParamString + ParamName;
      ParamString = ParamString + BillField->FieldName;
    else
      //ParamString = ParamString + "," + ParamName;
      ParamString = ParamString + "," + BillField->FieldName;
    //将原字符串中的字段名替换为ParamName
    int k = 1;
    while(k <= LowerCaseScript.Length())
    {
      if(LowerCaseScript.SubString(k,LowerCaseFieldName.Length()) == LowerCaseFieldName)
      {
        Script = StuffString(Script,k,LowerCaseFieldName.Length(),BillField->FieldName);
        LowerCaseScript = StuffString(LowerCaseScript,k,LowerCaseFieldName.Length(),LowerCaseFieldName);
      }
      ++k;
    }
    MatchFieldList->AddObject(BillField->FieldName,(TObject *)BillField);
  }
  ParamString = "(" + ParamString + ")";
  Script = "'执行过程\nFunction MatchBill" + ParamString + "\n"
    +  "if " + Script + " then \n"\
    " MatchBill = 1\n"\
    "else\n MatchBill = 0\n"\
    "end if\n"\
    "End Function";
  SkyEdit1->Text = Script;
  //脚本语法检查
  TScriptControl * ScriptControl = new TScriptControl(this);
  ScriptControl->Language = "VBScript";
  ScriptControl->AllowUI  = false;
  LPSAFEARRAY Params; //传递给脚本的参数
  Params = SafeArrayCreateVector(VT_VARIANT, 0, MatchFieldList->Count);
  VARIANT * pData = NULL;
  SafeArrayAccessData(Params, (void **)&pData);     //锁定数据
  TList * ParamList = new TList;                    //参数列表
  VARIANT * pDataTmp;
  for(int n = 0;n < MatchFieldList->Count;n++)
  {
    pDataTmp = new VARIANT;
    pDataTmp->vt = VT_BSTR;
    ParamList->Add((TObject *)pDataTmp);
  }
  ScriptControl->AddCode(WideString(Script).c_bstr());
  bool ScriptSucceed;
  ScriptControl->Error->get_Number();
  ScriptSucceed = ScriptControl->Error->Number == 0;
  if(!ScriptSucceed)//ScriptControl->Error->Number > 0)
  {
    MessageBox(Handle,("查询脚本错误:第 " + AnsiString(ScriptControl->Error->Line) \
                       + " 行，第 " + AnsiString(ScriptControl->Error->Column) \
                       + "列出现错误 " + AnsiString(ScriptControl->Error->Number) \
                       + " (" + AnsiString(ScriptControl->Error->Description) + ")").c_str(),"错误",MB_OK | MB_ICONSTOP);
    //ScriptSucceed = false;
  }
  else
  {
  //生成参数列表
  for(int p = 0;p < MatchFieldList->Count;p++)
  {
    pDataTmp = (VARIANT *)ParamList->Items[p];
    switch(((TBillField *)MatchFieldList->Objects[p])->SortType)
    {
    case SORT_AS_STR:
      pDataTmp->bstrVal = SysAllocString(L"000");
      break;
    case SORT_AS_DATETIME:
      pDataTmp->bstrVal = SysAllocString(L"2000-01-01 00:00:00");
      break;
    case SORT_AS_NUMBER:
      pDataTmp->bstrVal = SysAllocString(L"1000");
      break;
    case SORT_AS_CURRENCY:
      pDataTmp->bstrVal = SysAllocString(L"2.18");
      break;
    }
    * (pData + p) = * pDataTmp;
  }
  //执行脚本
  try
  {
    int Found = ScriptControl->Run(WideString("MatchBill").c_bstr(),&Params);
    if(Found != 0 && Found != 1)
    {
      MessageBox(Handle,"脚本没有返回预期的0或1","脚本编译错误",MB_OK | MB_ICONSTOP);
      ScriptSucceed = false;
    }
    else
      ScriptSucceed = true;
  }
  catch(Exception & E)
  {
    ScriptSucceed = false;
  }
  if(!ScriptSucceed && ScriptControl->Error->Number != 0)
    MessageBox(Handle,("查询脚本错误:\n"\
      "第 " + AnsiString(ScriptControl->Error->Line) \
       + " 行，第 " + AnsiString(ScriptControl->Error->Column) \
       + "列出现错误\n " + AnsiString(ScriptControl->Error->Number) \
       + " (" + AnsiString(ScriptControl->Error->Description) + ")").c_str(),"脚本编译错误",MB_OK | MB_ICONSTOP);
  //释放字符串占用的空间
  for(int p = 0;p < MatchFieldList->Count;p++)
  {
    pDataTmp = (VARIANT *)ParamList->Items[p];
    SysFreeString(pDataTmp->bstrVal);
  }
  SafeArrayUnaccessData(Params);
  for(int n = 0;n < ParamList->Count;n++)
  {
    delete (VARIANT *)ParamList->Items[n];
  }
  delete ParamList;          
  delete ScriptControl;
  }
  if(ScriptSucceed)
    return Script;
  else
    return "";
}


void __fastcall TfrmSearch::btnScriptHelpClick(TObject *Sender)
{
#ifdef SCTELE_COM_VERSION
    ShellExecute(NULL,"open","hh.exe",(ExtractFilePath(Application->ExeName) + "Help\\ibillsc.chm::/vbscript.htm").c_str(),"",SW_NORMAL);
#else
    ShellExecute(NULL,"open","hh.exe",(ExtractFilePath(Application->ExeName) + "Help\\ibill.chm::/vbscript.htm").c_str(),"",SW_NORMAL);
#endif  
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::btnVBHelpClick(TObject *Sender)
{
  ShellExecute(NULL,"open","hh.exe",(ExtractFilePath(Application->ExeName) + "Help\\VBScript语言参考.chm").c_str(),"",SW_NORMAL);
}
//---------------------------------------------------------------------------

