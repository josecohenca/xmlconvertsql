//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BillFileFrm.h"
#include "MainFrm.h"
#include "ExportToDBFrm.h"
#include "FTPFile.h"
#include "ExportToFileFrm.h"
#include <Clipbrd.hpp>
#include "OpeningFileFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzListVw"
#pragma link "RzPanel"
#pragma link "IdFTPServer"
#pragma link "dcOutPanel"
#pragma link "RzTabs"
#pragma link "RzSplit"
#pragma resource "*.dfm"
TfrmBillFile *frmBillFile;
//---------------------------------------------------------------------------
__fastcall TfrmBillFile::TfrmBillFile(TComponent* Owner)
  : TForm(Owner)
{
  frmSearch    = NULL;
  //TempFileList = NULL;
  TempFileList = new TStringList;
  LoadDirList  = new TStringList;
  if(EncryRegisteCode(frmMain->RegistUserName).SubString(1,25) != frmMain->SerialNumber && !InCmdMode)
  {
    MessageBox(Handle,("您使用的是本软件的未注册副本,最多只能同时加载" + IntToStr(TRY_VERSION_FILE_COUNT) + "个话单文件!").c_str(),"提示",MB_OK | MB_ICONINFORMATION);
  }
}
void __fastcall TfrmBillFile::OnCustomerKeyDown(TMessage Message)
{
  if(Message.WParam == 1)
  {
    switch(Message.LParam)
    {
    case 'A':
      lvFileList->SelectAll();
      break;
    case 'C':
      menuCopyFileLinkClick(NULL);
      break;
    case 'E':
      menuExportToDBClick(NULL);
      break;
    case 'F':
      menuSearchClick(NULL);
      break;
    default:
      return;
    }
  }
  else if(Message.LParam == 127)//Delete
      menuDeleteClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBillFile::PopupMenu1Popup(TObject *Sender)
{
  menuDelete->Enabled       = lvFileList->Selected != NULL;
  menuExportToFile->Enabled = menuDelete->Enabled; 
  menuExportToDB->Enabled   = menuDelete->Enabled;
  menuSearch->Enabled       = menuDelete->Enabled;
  menuCopyFileLink->Enabled = menuDelete->Enabled;
  menuCopyFileList->Enabled = menuDelete->Enabled;
  menuClearList->Enabled    = lvFileList->Items->Count > 0;
  menuStat->Enabled         = menuDelete->Enabled;
  menuSelectAll->Enabled    = menuClearList->Enabled;
  N3->Enabled               = menuClearList->Enabled;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBillFile::menuAddFileClick(TObject *Sender)
{
  if(Sender != NULL)
  {
    if(lvFileList->Items->Count > 0)
    {
      if(!frmMain->ShowOpenFileDialog(lvFileList->Items->Item[0]->SubItems->Strings[1]))
        return;
    }
    else
    {
      if(!frmMain->ShowOpenFileDialog())
        return;
    }
  }
  Screen->Cursor = crHourGlass;
  if(AddFileToList(frmMain->frmOpenDialog->cbxCheckFileFormat->Checked,frmMain->frmOpenDialog->cbxScanRecordCount->Checked))
  {
    AddItemToWarning("信息","文件加载","当前共加载了" + IntToStr(lvFileList->Items->Count) + "个文件");
  }
  Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBillFile::menuSelectAllClick(TObject *Sender)
{
  for(int n = 0;n < lvFileList->Items->Count;n++)
  {
    lvFileList->Items->Item[n]->Selected = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmBillFile::menuDeleteClick(TObject *Sender)
{
  TListItem * Item;
  for(int n = lvFileList->Items->Count - 1;n > -1;n--)
  {
    Item = lvFileList->Items->Item[n];
    if(Item->Selected)
      lvFileList->Items->Delete(Item->Index);
  }
  if(lvFileList->Items->Count == 0)
    LoadDirList->Clear();
  AddItemToWarning("信息","文件加载","当前共加载了" + IntToStr(lvFileList->Items->Count) + "个文件");   
}
//---------------------------------------------------------------------------
void __fastcall TfrmBillFile::menuClearListClick(TObject *Sender)
{
  lvFileList->Items->Clear();    
  //if(lvFileList->Items->Count == 0)
  LoadDirList->Clear();
  AddItemToWarning("信息","文件加载","当前共加载了0个文件");   
}
//---------------------------------------------------------------------------
void __fastcall TfrmBillFile::menuExportToDBClick(TObject *Sender)
{
  if(lvFileList->Selected == NULL)
    return;
  TfrmExportToDB * frmExportToDB = new TfrmExportToDB(Application);
  TListItem * Item1;
  TListItem * Item2;
  Screen->Cursor = crHourGlass;
  for(int n = 0;n < lvFileList->Items->Count;n++)
  {
    Item1 = lvFileList->Items->Item[n];
    if(!Item1->Selected)
      continue;
    Item2 = frmExportToDB->lvFileList->Items->Add();
    Item2->Assign(Item1);
  }
  Screen->Cursor = crDefault;
  frmExportToDB->ShowModal();
  delete frmExportToDB;  
}

//将FILETIME转换为TDateTime
TDateTime __fastcall TfrmBillFile::CovFileDate(FILETIME Fd)
{
  SYSTEMTIME Tct;
  FILETIME Temp;
  FileTimeToLocalFileTime(&Fd,&Temp);
  FileTimeToSystemTime(&Temp,&Tct);
  return SystemTimeToDateTime(Tct);
}
//获取文件的修改时间，Tf表示目标文件路径和名称
TDateTime __fastcall TfrmBillFile::GetModifiedTime(AnsiString FileName)
{
  HANDLE   hFind;
  WIN32_FIND_DATA fd;
  hFind = FindFirstFile(FileName.c_str(),&fd);
  if(hFind!=INVALID_HANDLE_VALUE)
    FindClose(hFind);
  return CovFileDate(fd.ftLastWriteTime);
}

//---------------------------------------------------------------------------
bool __fastcall TfrmBillFile::AddFileToList(
  //AnsiString    BillName,
  //TStringList * FileList,
  //AnsiString ListFileName,
  bool CheckFileFormat,
  bool ScanRecordCount
  )
{
  bool IsTry = EncryRegisteCode(frmMain->RegistUserName).SubString(1,25) != frmMain->SerialNumber;
  if(IsTry && lvFileList->Items->Count >= TRY_VERSION_FILE_COUNT)
    return false;
  TfrmOpenDialog * frmOpenDialog = frmMain->frmOpenDialog;
  AnsiString BillName = frmOpenDialog->cbxFileFormat->Text;
  
  if(frmOpenDialog->lvFiles->SelCount == 0)
    return false;
  AnsiString FilePath = frmOpenDialog->PathName;
  AnsiString FileName;
  int RecordCount;
  AnsiString FileDateTime;
  int FileFormatType;
  AnsiString FileFormatTypeName;
  int FileHeadLength;
  int FileTrailLength;
  int RecordLength;
  try
  {
    FileFormatType     = frmMain->BillConfig->GetFileFormatType(BillName);
    FileFormatTypeName = frmMain->BillConfig->TransFormatTypeName(FileFormatType);
  }
  catch(Exception * E)
  {
    ShowErrorMessage(Handle,E->Message.c_str(),false,6);
    return false;
  }
  TBillFile * BillFile = NULL;
  TBillField * AnswerTimeField;
  bool IsNonBlockFile = true;
  try
  {
    switch(FileFormatType)
    {
    case BIN_BILL_FORMAT:
      FileHeadLength  = frmMain->BillConfig->GetFileHeadLength(BillName);
      FileTrailLength = frmMain->BillConfig->GetFileTrailLength(BillName);
      RecordLength    = frmMain->BillConfig->GetFileRecordLength(BillName);
      IsNonBlockFile  = !frmMain->BillConfig->IsBlockFile(BillName);
      if(CheckFileFormat || (!IsNonBlockFile && ScanRecordCount))
      {
        BillFile = new TBinBillFile(BillName,frmMain->BillConfig,Handle,frmMain->TempDirectory);
        AnswerTimeField = BillFile->AnswerTimeField;
      }
      break;
    case TXT_BILL_FORMAT:
      IsNonBlockFile = false;
      if(CheckFileFormat || ScanRecordCount)
      {
        BillFile = new TTxtBillFile(BillName,frmMain->BillConfig,Handle,frmMain->TempDirectory);
        AnswerTimeField = BillFile->AnswerTimeField;
      }
      break;
    default:
      return false;
    }
  }
  catch(Exception * E)
  {
    ShowErrorMessage(Handle,(E->Message + ".\n").c_str(),false,99);
    return false;
  }
  int j;
  int FileSize = 0;
  if(frmOpenDialog->lvFiles->SelCount > 100)
    lvFileList->Hide();
  RecordFilter * prf;
  bool SearchExistsFile = lvFileList->Items->Count > 0 && LoadDirList->IndexOf(FilePath) > -1;
  //TfrmOpeningFile * frmOpeningFile = NULL;
  //if(SearchExistsFile)
  //  ShowMessage("OK");
  LoadDirList->Add(FilePath);
  //TItemStates States = TItemStates() << isSelected;
  TListItem * FileItem;// = frmOpenDialog->lvFiles->GetNextItem(NULL,sdAll,States);
  TListItem * Item;
  //for(int n = 0;n < FileList->Count;n++)
  HANDLE hFile;
  //BY_HANDLE_FILE_INFORMATION FileInfo;
  FILETIME FileTime;
  bool FileExistsInList;
  TDateTime LastModifyTime;
  //int FindEndItemIndex = lvFileList->Items->Count;
  //while(FileItem != NULL)
  for(int n = 0;n < frmOpenDialog->lvFiles->Items->Count;n++)
  {
    FileItem = frmOpenDialog->lvFiles->Items->Item[n];
    if(!FileItem->Selected || DirectoryExists(FilePath + FileItem->Caption))
      continue;    
    //检查文件在列表中是否已经存在
    FileName = FileItem->Caption;
    if(SearchExistsFile)
    {
      Item = lvFileList->FindCaption(-1,FileName,false,false,false);
      FileExistsInList = false;
      while(Item != NULL)
      {
        if(Item->SubItems->Strings[5] == FilePath)
        {
          AddItemToWarning("错误","文件加载",FilePath + FileName + "该文件没有被添加到列表中,因为它已经在先前的操作中被加载到了列表中");
          FileExistsInList = true;
          break;
        }
        Item = lvFileList->FindCaption(Item->Index,FileName,false,false,false);
      }
      if(FileExistsInList)
      {
        //FileItem = frmOpenDialog->lvFiles->GetNextItem(FileItem,sdAll,States);
        continue;
      }
    }
    hFile = CreateFile((FilePath + FileName).c_str(),GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if(hFile == INVALID_HANDLE_VALUE)
    {
      //MessageBox(Handle,("不能获取文件 " + FileName + " 的大小").c_str(),"警告",MB_OK | MB_ICONWARNING);
      AddItemToWarning("警告","文件加载","不能获取文件 " + FileName + " 的大小和修改日期");
      FileSize = -1;
      LastModifyTime = StrToDateTime("1990-01-01 00:00:00");
    }
    else
    {
      FileSize = GetFileSize(hFile,NULL);
      memset(&FileTime,0,sizeof(FileTime));
      GetFileTime(hFile,NULL,NULL,&FileTime);
      LastModifyTime = CovFileDate(FileTime);
      CloseHandle(hFile);
    }       
    try
    {
      if(BillFile != NULL)
      {
        BillFile->SetOwnerWnd(Application->MainForm->Handle);
        //BillFile->OpenBillFile(FileList->Strings[n],BillName);
        if(!BillFile->OpenBillFile(FilePath + FileName,BillName))
        {
          SendMessage(BillFile->hWnd,MSG_THREAD_COMPLETED,0,0);
          throw new Exception("加载文件[" + FilePath + FileName + "]出错.");
        }
        SendMessage(BillFile->hWnd,MSG_THREAD_COMPLETED,0,0);
        BillFile->SetOwnerWnd(Handle);
        RecordCount = BillFile->GetRecordCount();
        if(CheckFileFormat)
        {
          for(j = 0; j < RecordCount;j ++)
          {
            BillFile->ReadSingleRecord(j);
            if(AnswerTimeField != NULL)
            {
              try
              {
                if(StrToDateTime(AnswerTimeField->GetFieldValue()) < StrToDateTime("1997-01-01 00:00:00"))
                  throw new Exception("");
              }
              catch(...)
              {
                BillFile->CloseBillFile();
                throw new Exception("尝试对文件[" + FilePath + FileName + "]进行格式验证时出现错误,文件格式可能与所选的格式不一致");
              }
            }
            if(j > 2)
              break;
          }
        }
        if(BillFile->SkipRecordCount > 0)
        {
          for(int j = 0;j < BillFile->FilterList->Count;j++)
          {
            prf = (RecordFilter *)BillFile->FilterList->Items[j];
            if(prf->SkipCount > 0)
            {
              AddItemToWarning("提示","记录过滤","文件" + FileName + "中,有" + IntToStr(prf->SkipCount) + "条记录被过滤(" + AnsiString(prf->Cause) + ").");
            }
          }
          AddItemToWarning("提示","记录过滤","文件" + FileName + "中共有" + IntToStr(BillFile->SkipRecordCount) + "条记录被过滤.");
        }
        BillFile->CloseBillFile();
      }
      else if(IsNonBlockFile)
        RecordCount = (FileSize - FileHeadLength - FileTrailLength) / RecordLength;
      else
        RecordCount = -1;
      Item = lvFileList->Items->Add();
      Item->ImageIndex = 95;
      Item->Caption = FileName;
      Item->SubItems->Add(FileFormatTypeName);
      Item->SubItems->Add(BillName);
      if(FileSize > 1024)
        Item->SubItems->Add(FloatToStr(RoundTo((float)FileSize / 1024,-2)) + "KB");
      else
        Item->SubItems->Add(IntToStr(FileSize) + "B");
      Item->SubItems->Add(RecordCount);
      Item->SubItems->Add(FormatDateTime("yyyy-mm-dd hh:nn:ss",LastModifyTime));//GetModifiedTime(FilePath + FileName)));
      Item->SubItems->Add(FilePath);
      Item->Data = (void *)FileSize;
      if(IsTry && lvFileList->Items->Count >= TRY_VERSION_FILE_COUNT)
      {
        AddItemToWarning("警告","文件加载","试用版软件最多只能同时加载" + IntToStr(TRY_VERSION_FILE_COUNT) + "个文件,加载过程已中止.");
        break;
      }
    }
    catch(Exception * E)
    {
      AddItemToWarning("错误","文件加载",E->Message + ".该文件没有被添加到列表中");
      //FileItem = frmOpenDialog->lvFiles->GetNextItem(FileItem,sdAll,States);
      continue;
    }
    //FileItem = frmOpenDialog->lvFiles->GetNextItem(FileItem,sdAll,States);
  }
  lvFileList->Show();
  delete BillFile;
  return true;
}
bool __fastcall TfrmBillFile::AddFTPFileToList(
    AnsiString BillName,
    TRzListView * FileListView,
    AnsiString Server,
    AnsiString UserName,
    AnsiString Password,
    AnsiString Port,
    AnsiString Pasv)
{
  bool IsTry = EncryRegisteCode(frmMain->RegistUserName).SubString(1,25) != frmMain->SerialNumber;
  if(IsTry && lvFileList->Items->Count >= TRY_VERSION_FILE_COUNT)
    return false;
  TListItem * Item;
  AnsiString FileFormatTypeName;
  try
  {
    FileFormatTypeName = frmMain->BillConfig->TransFormatTypeName(frmMain->BillConfig->GetFileFormatType(BillName));
  }
  catch(Exception * E)
  {
    ShowErrorMessage(Handle,E->Message.c_str(),false,6);
    return false;
  }
  AnsiString FileName;
  AnsiString ModifyTime;
  int pos;
  int FileSize;
  int FileHeadLength   = frmMain->BillConfig->GetFileHeadLength(BillName);
  int FileTrailLength  = frmMain->BillConfig->GetFileTrailLength(BillName);
  int RecordLength     = frmMain->BillConfig->GetFileRecordLength(BillName);
  int FileFormat       = frmMain->BillConfig->GetFileFormatType(BillName);
  bool CalcRecordCount = FileFormat == BIN_BILL_FORMAT && !frmMain->BillConfig->IsBlockFile(BillName);
  AnsiString FilePathHead = "ftp://" + UserName + ":" + Password + "@" + Server + ":" + Port;// + Path + "|" + Pasv;
  AnsiString FilePath;
  TListItem * FileListItem;
  if(FileListView->SelCount > 100)
    lvFileList->Hide();
  bool SearchExistsFile = lvFileList->Items->Count > 0;
  bool FileExistsInList;
  for(int n = 0;n < FileListView->Items->Count;n++)
  {
    FileListItem = FileListView->Items->Item[n];
    if(!FileListItem->Selected)
      continue;
    FileName = FileListItem->Caption;
    FilePath = FileListItem->SubItems->Strings[3];
    if(FilePath.SubString(FilePath.Length(),1) != "/")
      FilePath += "/";
    FilePath = FilePathHead + FilePath + "|" + Pasv;
    if(SearchExistsFile)
    {
      FileExistsInList = false;
      Item = lvFileList->FindCaption(-1,FileName,false,false,false);
      while(Item != NULL)
      {
        if(Item->SubItems->Strings[5] == FilePath)
        {
          AddItemToWarning("错误","文件加载",FilePath + FileName + "  该文件没有被添加到列表中,因为无它已经在先前的操作中被加载到了列表中");
          FileExistsInList = true;
          break;
        }
        Item = lvFileList->FindCaption(Item->Index,FileName,false,false,false);
      }
      if(FileExistsInList)
        continue;
    }
    Item = lvFileList->Items->Add();
    Item->ImageIndex = 95;
    Item->Caption = FileName;
    Item->SubItems->Add(FileFormatTypeName);
    Item->SubItems->Add(BillName);
    FileSize = (int)FileListItem->Data;
    if(FileSize > 1024)
      Item->SubItems->Add(FloatToStr(RoundTo((float)FileSize / 1024,-2)) + "KB");
    else
      Item->SubItems->Add(IntToStr(FileSize) + "B");
    if(CalcRecordCount)
      Item->SubItems->Add((FileSize - FileHeadLength - FileTrailLength) / RecordLength);
    else
      Item->SubItems->Add("-1");
    Item->SubItems->Add(FileListItem->SubItems->Strings[2]);
    Item->SubItems->Add(FilePath);
    Item->Data = (void *)FileSize;
    if(IsTry && lvFileList->Items->Count >= TRY_VERSION_FILE_COUNT)
    {
      AddItemToWarning("警告","文件加载","试用版软件最多只能同时加载" + IntToStr(TRY_VERSION_FILE_COUNT) + "个文件,加载过程已中止.");
      break;
    }
  }
  lvFileList->Show();
  return true;
}
void __fastcall TfrmBillFile::AddItemToWarning(AnsiString Type,AnsiString FileName,AnsiString Comments)
{
  TListItem * Item = lvWarning->Items->Add();
  if(Type == "警告")
    Item->ImageIndex = 74;
  else if(Type == "错误")
    Item->ImageIndex = 113;
  else if(Type == "提示")
    Item->ImageIndex = 122;
  else
    Item->ImageIndex = 46;

  Item->Caption = "";
  Item->SubItems->Add(Type);
  Item->SubItems->Add(FormatDateTime("mm-dd hh:nn:ss",Now()));
  Item->SubItems->Add(FileName);
  Item->SubItems->Add(Comments);
}
//对文件列表进行排序
int __stdcall CustomSortProc(long Item1, long Item2, long ParamSort)

{
  TListColumn * Column = (TListColumn *)ParamSort;
  switch(Column->Index)
  {
    case 0: //按文件名
      return CompareText(((TListItem *)Item1)->Caption,((TListItem *)Item2)->Caption) * Column->Tag;
    case 3: //按文件大小
      {
        int n1,n2;
        n1 = (int)(((TListItem *)Item1)->Data);
        n2 = (int)(((TListItem *)Item2)->Data);
        if(n1 == n2)
          return 0;
        else if(n1 > n2)
          return 1 * Column->Tag;
        else
          return -1 * Column->Tag;
      }
    case 5: //按生成时间
      {
        TDateTime d1,d2;
        d1 = StrToDateTime(((TListItem *)Item1)->SubItems->Strings[4]);
        d2 = StrToDateTime(((TListItem *)Item2)->SubItems->Strings[4]);
        if(d1 == d2)
          return 0;
        else if(d1 > d2)
          return 1 * Column->Tag;
        else
          return -1 * Column->Tag;
      }
    case 4: //按记录数
      {
        int n1,n2;
        n1 = StrToInt(((TListItem *)Item1)->SubItems->Strings[3]);
        n2 = StrToInt(((TListItem *)Item2)->SubItems->Strings[3]);
        if(n1 == n2)
          return 0;
        else if(n1 > n2)
          return 1 * Column->Tag;
        else
          return -1 * Column->Tag;
      }
    default:
      return CompareText(((TListItem *)Item1)->SubItems->Strings[Column->Index - 1],
        ((TListItem *)Item2)->SubItems->Strings[Column->Index - 1]) * Column->Tag;
  }
}

void __fastcall TfrmBillFile::lvFileListColumnClick(TObject *Sender,
      TListColumn *Column)
{
  Screen->Cursor = crHourGlass;
  lvFileList->HeaderSortColumn = Column->Index;
  Column->Tag = Column->Tag * 1;
  if(Column->Tag > 0)
  {
    Column->Tag = -1;
    lvFileList->HeaderSortDirection = sdAscending;
  }
  else
  {
    Column->Tag = 1;
    lvFileList->HeaderSortDirection = sdDescending;
  }
  lvFileList->CustomSort(CustomSortProc,(long)Column);
  Screen->Cursor = crDefault;  
}
//---------------------------------------------------------------------------
void __fastcall TfrmBillFile::lvFileListDblClick(TObject *Sender)
{
  TListItem * Item = lvFileList->Selected;
  if(Item == NULL)
    return;
  TfrmViewBill * frmViewBill = new TfrmViewBill(Application,frmMain->BillConfig);
  if(frmViewBill->OpenBillFile(Item->SubItems->Strings[0],Item->SubItems->Strings[1],Item->SubItems->Strings[5],Item->Caption,(int)Item->Data))
  {
    frmViewBill->Caption = "话单浏览  " + Item->Caption;
    frmMain->AddFormToPageControl(frmViewBill);
    TRzTabSheet * TabSheet = (TRzTabSheet *)frmViewBill->Tag;
    frmMain->RzPageControl1->ActivePage = TabSheet;
  }
  else
  {
    delete frmViewBill;
  }
    
}
void __fastcall TfrmBillFile::btnAddFTPFileClick(TObject *Sender)
{
  if(Sender != NULL)
  {
    if(lvFileList->Items->Count > 0)
    {
      if(!frmMain->ShowOpenFTPDialog(lvFileList->Items->Item[0]->SubItems->Strings[1]))
        return;
    }
    else
    {
      if(!frmMain->ShowOpenFTPDialog())
        return;
    }
  }
  AnsiString Pasv;
  if(frmMain->frmOpenFTPDialog->IdFTP1->Passive)
    Pasv = "PASV";
  else
    Pasv = "";
  Screen->Cursor = crHourGlass;
  AddFTPFileToList(
    frmMain->frmOpenFTPDialog->cbxFileFormat->Text,
    frmMain->frmOpenFTPDialog->lvFiles,
    frmMain->frmOpenFTPDialog->IdFTP1->Host,
    EncryFTPPassword(frmMain->frmOpenFTPDialog->IdFTP1->Username),
    EncryFTPPassword(frmMain->frmOpenFTPDialog->IdFTP1->Password),
    //frmMain->frmOpenFTPDialog->Path,
    IntToStr(frmMain->frmOpenFTPDialog->IdFTP1->Port),
    Pasv);
  AddItemToWarning("信息","文件加载","当前共加载了" + IntToStr(lvFileList->Items->Count) + "个文件"); 
  Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillFile::menuDeleteWarningClick(TObject *Sender)
{
  lvWarning->Items->Clear();  
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillFile::PopupMenu2Popup(TObject *Sender)
{
  menuDeleteWarning->Enabled  = lvWarning->Items->Count > 0;
  menuDeleteAWarning->Enabled = lvWarning->Selected != NULL;
  menuCopyWarning->Enabled    = menuDeleteAWarning->Enabled;
  menuSelectAllWarning->Enabled = menuDeleteWarning->Enabled;
}
//---------------------------------------------------------------------------


void __fastcall TfrmBillFile::FormDestroy(TObject *Sender)
{
  if(frmSearch != NULL)
  {
    //DeleteFile(frmSearch->TempFileName);
    delete frmSearch;
    frmSearch = NULL;
  }
  if(TempFileList != NULL)
  {
    for(int n = 0;n < TempFileList->Count;n++)
      DeleteFile(TempFileList->Strings[n].c_str());
    delete TempFileList;
    TempFileList = NULL;
  }
  if(LoadDirList != NULL)
  {
    delete LoadDirList;
    LoadDirList = NULL;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillFile::menuSearchClick(TObject *Sender)
{
  if(lvFileList->SelCount == 0)
    return;
  Screen->Cursor = crHourGlass;
  if(frmSearch == NULL)
  {
    frmSearch = new TfrmSearch(this);
  }
  else
  {
    frmSearch->FileList->Clear();
    frmSearch->BillNameList->Clear();
  }
  TListItem * Item;
  AnsiString BillName   = lvFileList->Items->Item[0]->SubItems->Strings[1];
  AnsiString FormatType = lvFileList->Items->Item[0]->SubItems->Strings[0];
  frmSearch->lvFileList->Items->Clear();
  TListItem * NewItem;
  for(int n = 0;n < lvFileList->Items->Count;n++)
  {
    Item = lvFileList->Items->Item[n];
    if(!Item->Selected)
      continue;
    if(Item->SubItems->Strings[5].SubString(1,6) == "ftp://")
      frmSearch->FileList->AddObject(Item->SubItems->Strings[5] + "|" + Item->Caption,(TObject *)Item->Data);
    else
      frmSearch->FileList->AddObject(Item->SubItems->Strings[5] + Item->Caption,(TObject *)Item->Data);
    frmSearch->BillNameList->Add(Item->SubItems->Strings[1]);
    NewItem = frmSearch->lvFileList->Items->Add();
    NewItem->SubItems->Add(Item->Caption);
    NewItem->ImageIndex = 128;
    NewItem->SubItems->Add(Item->SubItems->Strings[2]);
    NewItem->SubItems->Add(Item->SubItems->Strings[3]);
    NewItem->SubItems->Add("");
    NewItem->SubItems->Add(Item->SubItems->Strings[5]);
  }
  frmSearch->FormatType = FormatType;
  if(BillName != frmSearch->BillName)
  {
    if(!frmSearch->AddParamsToList(BillName))
    {
      Screen->Cursor = crDefault;
      return;
    }
    frmSearch->cbxBillType->Clear();
    int  FileFormatType = frmMain->BillConfig->GetFileFormatType(BillName);
    bool IsBlockFile    = frmMain->BillConfig->IsBlockFile(BillName);
    if(IsBlockFile)
    {
      for(int j = 0;j < frmMain->BillConfig->BillNameList->Count;j++)
      {
        if((frmMain->BillConfig->GetFileFormatType(frmMain->BillConfig->BillNameList->Strings[j]) == FileFormatType
          &&frmMain->BillConfig->IsBlockFile(frmMain->BillConfig->BillNameList->Strings[j])) ||
          frmMain->BillConfig->BillNameList->Strings[j] == BillName)
        {
          frmSearch->cbxBillType->Items->Add(frmMain->BillConfig->BillNameList->Strings[j]);
        }
      }
    }
    else
      frmSearch->cbxBillType->Items->Add(BillName);
    frmSearch->cbxBillType->ItemIndex = frmSearch->cbxBillType->Items->IndexOf(BillName);
  }
  frmSearch->cbxBillType->Enabled = frmSearch->cbxBillType->Items->Count > 1;
  frmSearch->Label9->Enabled = frmSearch->cbxBillType->Enabled;
  frmSearch->Label1->Caption = "请指定查找条件";
  frmSearch->Label4->Caption = "共有0个文件需要查找,当前完成了0个";
  frmSearch->RzProgressBar1->PartsComplete = 0;
  frmSearch->Label2->Caption = "";
  Screen->Cursor = crDefault;
  frmSearch->ShowModal();
  btnReview->Enabled = TempFileList->Count > 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillFile::btnReviewClick(TObject *Sender)
{
  if(TempFileList->Count == 0)
    return;
  int Index = ((TComponent *)Sender)->Tag;
  if(Index > TempFileList->Count)
    return;
  TfrmViewBill * frmViewBill = new TfrmViewBill(Application,frmMain->BillConfig);
  if(frmViewBill->OpenBillFile(SR_BILL_FORMAT_STR,"",ExtractFilePath(TempFileList->Strings[Index]),
    ExtractFileName(TempFileList->Strings[Index]),GetLocalFileSize(TempFileList->Strings[Index])))
  {
    frmViewBill->StatusBar1->Panels->Items[0]->Text = "话单查找结果 共找到" + IntToStr(frmSearch->FoundCount) + "条记录";
    frmViewBill->Caption = "话单查询结果";
    frmMain->AddFormToPageControl(frmViewBill);
    TRzTabSheet * TabSheet = (TRzTabSheet *)frmViewBill->Tag;
    frmMain->RzPageControl1->ActivePage = TabSheet;
  }
  else
  {
    delete frmViewBill;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillFile::menuExportToFileClick(TObject *Sender)
{
  if(lvFileList->SelCount < 1)
    return;
  if(!frmMain->SaveDialog1->Execute())
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
  }  
  TfrmExportToFile * frmExportToFile = new TfrmExportToFile(this);
  TListItem * Item;
  AnsiString BillName   = "";
  AnsiString FormatType = "";
  int RecordCount = 0;
  for(int n = 0;n < lvFileList->Items->Count;n++)
  {
    Item = lvFileList->Items->Item[n];
    if(!Item->Selected)
      continue;
    if(BillName == "")
      BillName = Item->SubItems->Strings[1];
    if(Item->SubItems->Strings[5].SubString(1,6) == "ftp://")
      frmExportToFile->FileList->AddObject(Item->SubItems->Strings[5] + "|" + Item->Caption,NULL);
    else
      frmExportToFile->FileList->AddObject(Item->SubItems->Strings[5] + Item->Caption,(TObject *)Item->Data);
    frmExportToFile->BillNameList->Add(Item->SubItems->Strings[1]);
    if(RecordCount >= 0)
      RecordCount += StrToInt(Item->SubItems->Strings[3]); 
  }
  frmExportToFile->DestFileName = frmMain->SaveDialog1->FileName;
  frmExportToFile->DestFileType = DestFileType;
  frmExportToFile->AutoClose = true;
  if(RecordCount >= 0)
    AddItemToWarning("信息","开始转换文件","共有" + IntToStr(frmExportToFile->FileList->Count) + "个文件需要转换(" + IntToStr(RecordCount) + "条记录)" +
      "转换后的文件将存放在" + frmExportToFile->DestFileName);
  else
    AddItemToWarning("信息","开始转换文件","共有" + IntToStr(frmExportToFile->FileList->Count) + "个文件需要转换" +
      "转换后的文件将存放在" + frmExportToFile->DestFileName);
  if(frmExportToFile->CreateBillFileClass(BillName))
    frmExportToFile->ShowModal();
  if(frmExportToFile->ErrorMessage != "")
  {
    AddItemToWarning("警告","转换文件时发生异常",frmExportToFile->ErrorMessage);
  }
  else
  {
    AddItemToWarning("信息","转换文件完成","共转换了" + IntToStr(frmExportToFile->FileList->Count) + "个文件," +
      IntToStr(frmExportToFile->Label4->Tag) + "条记录,转换后的文件存放在" + frmExportToFile->DestFileName);
  }
  delete frmExportToFile;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillFile::menuCopyFileLinkClick(TObject *Sender)
{
  if(lvFileList->SelCount == 0)
    return;
  TListItem * Item;
  AnsiString Text = "";
  for(int n = 0;n < lvFileList->Items->Count;n++)
  {
    Item = lvFileList->Items->Item[n];
    if(Item->Selected)
      Text += Item->Caption + "\t" + Item->SubItems->Strings[0] +
        "\t" + Item->SubItems->Strings[1] +
        "\t" + Item->SubItems->Strings[2] +
        "\t" + Item->SubItems->Strings[3] +
        "\t" + Item->SubItems->Strings[4] +
        "\t" + Item->SubItems->Strings[5] + "\r\n";
  }
  TClipboard * Clipboard = new TClipboard();
  Clipboard->AsText = Text;
  delete Clipboard;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillFile::menuStatClick(TObject *Sender)
{
  int FileCount = lvFileList->SelCount;
  if(FileCount == 0)
    return;
  int BillCount = 0;
  int s;
  for(int n= 0;n < lvFileList->Items->Count;n++)
  {
    if(lvFileList->Items->Item[n]->Selected)
    {
      s = StrToInt(lvFileList->Items->Item[n]->SubItems->Strings[3]);
      if(s > -1)
        BillCount += s;
      else
      {
        BillCount = 0;
        break;
      }
    }
  }
  MessageBox(Handle,("共选择了" + IntToStr(FileCount) + "个文件\n\n" + IntToStr(BillCount) + "条记录").c_str(),"统计结果",MB_OK | MB_ICONINFORMATION);
  //TMemoryStream * Stream = new TMemoryStream;
  //lvFileList->Items->WriteData(Stream);
  //Stream->SaveToFile("c:\\jlj.dat");
  //delete Stream;
}
//---------------------------------------------------------------------------


void __fastcall TfrmBillFile::menuCopyFileListClick(TObject *Sender)
{
  if(lvFileList->SelCount == 0)
    return;
  TListItem * Item;
  AnsiString Text = "";
  for(int n = 0;n < lvFileList->Items->Count;n++)
  {
    Item = lvFileList->Items->Item[n];
    if(Item->Selected)
    {
      if(Item->SubItems->Strings[5].SubString(1,6) == "ftp://")
        Text += Item->SubItems->Strings[5] + "|" + Item->Caption + "\r\n";
      else
        Text += Item->SubItems->Strings[5] + Item->Caption + "\r\n";
    }
  }
  TClipboard * Clipboard = new TClipboard();
  Clipboard->AsText = Text;
  delete Clipboard; 
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillFile::menuDeleteAWarningClick(TObject *Sender)
{
  for(int n = lvWarning->Items->Count - 1;n > -1;n--)
  {
    if(lvWarning->Items->Item[n]->Selected)
      lvWarning->Items->Delete(n);  
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmBillFile::menuCopyWarningClick(TObject *Sender)
{
  if(lvWarning->Items->Count == 0)
    return;
  TListItem * Item;
  AnsiString Text = "";
  for(int n = 0;n < lvWarning->Items->Count;n++)
  {
    Item = lvWarning->Items->Item[n];
    if(Item->Selected)
      Text += Item->SubItems->Strings[0] +
        "\t" + Item->SubItems->Strings[1] +
        "\t" + Item->SubItems->Strings[2] +
        "\t" + Item->SubItems->Strings[3] + "\r\n";
  }
  TClipboard * Clipboard = new TClipboard();
  Clipboard->AsText = Text;
  delete Clipboard;  
}
//---------------------------------------------------------------------------


void __fastcall TfrmBillFile::N3Click(TObject *Sender)
{
  TListItem * Item;
  for(int n = 0;n < lvFileList->Items->Count;n++)
  {
    Item = lvFileList->Items->Item[n];
    if(Item->Selected)
      Item->Selected = false;
    else
      Item->Selected = true;
  }
}
//---------------------------------------------------------------------------



void __fastcall TfrmBillFile::menuSelectAllWarningClick(TObject *Sender)
{
  lvWarning->SelectAll();
}
//---------------------------------------------------------------------------





