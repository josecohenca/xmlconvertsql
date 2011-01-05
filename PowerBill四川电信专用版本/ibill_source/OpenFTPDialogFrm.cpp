//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "OpenFTPDialogFrm.h"
#include <Math.hpp>
#include "MainFrm.h"
#include "public.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzCmboBx"
#pragma link "dcOutPanel"
#pragma link "RzButton"
#pragma link "RzListVw"
#pragma link "RzPanel"
#pragma link "RzShellCtrls"
#pragma link "RzTreeVw"
#pragma link "RzEdit"
#pragma link "RzRadChk"
#pragma resource "*.dfm"
TfrmOpenFTPDialog *frmOpenFTPDialog;

#define IMAGE_FOLDER 1
#define IMAGE_FILE   0

//---------------------------------------------------------------------------
__fastcall TfrmOpenFTPDialog::TfrmOpenFTPDialog(TComponent* Owner)
  : TForm(Owner)
{
  //FileList = new TStringList;
  frmSearchFile = new TfrmSearchFile(this);
  //frmSearchFile->txtFileName->Text = "*.*";
}
bool __fastcall TfrmOpenFTPDialog::Execute()
{
  return ShowModal() == mrOk;
}
//
void __fastcall TfrmOpenFTPDialog::ListDir(TTreeNode * ParentNode)
{
  Screen->Cursor = crHourGlass;
  if(lvFiles->Items->Count > 100)
    lvFiles->Hide();
  lvFiles->Items->Clear();
  try
  {
    IdFTP1->List(NULL,"",true);
    TListItem * Item;
    TTreeNode * Node;
    AnsiString FileType;
    AnsiString FileName;
    AnsiString Path = IdFTP1->RetrieveCurrentDir();
    int k;
    for(int n = 0;n < IdFTP1->DirectoryListing->Count;n++)
    {
      FileName = IdFTP1->DirectoryListing->Items[n]->FileName;
      if(FileName == "." || FileName == "..")
        continue;
      if(IdFTP1->DirectoryListing->Items[n]->ItemType == ditDirectory)
      {
        for(k = 0;k < ParentNode->Count;k++)
        {
          if(ParentNode->Item[k]->Text == FileName)
            break;
        }
        if(k < ParentNode->Count)
          continue;
        Node = tvFolder->Items->AddChild(ParentNode,FileName);
        Node->ImageIndex    = 1;
        Node->SelectedIndex = 2;
      }
      else
      {
        Item     = lvFiles->Items->Add();
        Item->Caption = FileName;
        Item->ImageIndex = IMAGE_FILE;
        if(IdFTP1->DirectoryListing->Items[n]->Size > 1048576)
          Item->SubItems->Add(FloatToStr(RoundTo((float)IdFTP1->DirectoryListing->Items[n]->Size / 1048576,-2)) + "MB");
        else if(IdFTP1->DirectoryListing->Items[n]->Size > 1024)
          Item->SubItems->Add(FloatToStr(RoundTo((float)IdFTP1->DirectoryListing->Items[n]->Size / 1024,-2)) + "KB");
        else
          Item->SubItems->Add(IntToStr(IdFTP1->DirectoryListing->Items[n]->Size) +"B");
        FileType = ExtractFileExt(FileName);
        if(FileType != "")
          Item->SubItems->Add(FileType.SubString(2,FileType.Length() - 1) + "文件");
        else
          Item->SubItems->Add("");
        Item->SubItems->Add(IdFTP1->DirectoryListing->Items[n]->ModifiedDate.FormatString("yyyy-mm-dd hh:nn:ss"));
        Item->SubItems->Add(Path);
        Item->Data = (void *)IdFTP1->DirectoryListing->Items[n]->Size;
      }
    }
  }
  catch(Exception * E)
  {
    MessageBox(Handle,E->Message.c_str(),"错误",MB_OK | MB_ICONSTOP);
  }
  ParentNode->Expand(false);
  Screen->Cursor = crDefault;
  lvFiles->Show();
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenFTPDialog::menuIconClick(TObject *Sender)
{
    Screen->Cursor = crHourGlass;
    lvFiles->ViewStyle = vsIcon;
    menuIcon->Checked = true;
    Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenFTPDialog::menuSmallClick(TObject *Sender)
{
  Screen->Cursor = crHourGlass;
  lvFiles->ViewStyle = vsSmallIcon;
  menuSmall->Checked = true;
  Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenFTPDialog::menuListClick(TObject *Sender)
{
  Screen->Cursor = crHourGlass;
  lvFiles->ViewStyle = vsList;
  menuList->Checked = true;
  Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenFTPDialog::menuDetailClick(TObject *Sender)
{
  Screen->Cursor = crHourGlass;
  lvFiles->ViewStyle = vsReport;
  menuDetail->Checked = true;
  Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------


void __fastcall TfrmOpenFTPDialog::Action1Execute(TObject *Sender)
{
  for(int n = 0;n < lvFiles->Items->Count;n++)
  {
    lvFiles->Items->Item[n]->Selected = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmOpenFTPDialog::GetNodePath(TTreeNode * Node,AnsiString & Path)
{
  if(Node->Text.SubString(Node->Text.Length(),1) != "/")
    Path = Node->Text + "/" + Path;
  else
    Path = Node->Text + Path;
  if(Node->Parent != NULL && Node->Text != "/")
  {
    GetNodePath(Node->Parent,Path);     
  }
}

void __fastcall TfrmOpenFTPDialog::btnCancelClick(TObject *Sender)
{
  ModalResult = mrCancel;  
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenFTPDialog::btnOpenClick(TObject *Sender)
{
  if(lvFiles->SelCount == 0)
  {
    return;
  }
  if(cbxFileFormat->ItemIndex < 0)
  {
    MessageBox(Handle,"请指定当前将要打开的文件的格式!","提示",MB_OK | MB_ICONWARNING);
    cbxFileFormat->SetFocus();
    return;
  }
  /*Screen->Cursor = crHourGlass;
  Path = IdFTP1->RetrieveCurrentDir();
  FileList->Clear();
  TListItem * Item;
  for(int n = 0;n < lvFiles->Items->Count;n++)
  {
    Item = lvFiles->Items->Item[n];
    if(Item->Selected)
      FileList->AddObject(Item->Caption + "|" + Item->SubItems->Strings[2],(TObject *)Item->Data);
  }
  Screen->Cursor = crDefault;*/
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenFTPDialog::tvFolderChange(TObject *Sender,
      TTreeNode *Node)
{
  AnsiString Path;
  GetNodePath(Node,Path);
  if(Path != IdFTP1->RetrieveCurrentDir())
  {
    IdFTP1->ChangeDir(Path);
    ListDir(Node);
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenFTPDialog::ToolButton2Click(TObject *Sender)
{
  if(cbxServer->Text == "")
  {
    MessageBox(Handle,"请输入服务器地址!","提示",MB_OK | MB_ICONWARNING);
    cbxServer->SetFocus();
    return;
  }
  if(IdFTP1->Connected())
    IdFTP1->Disconnect();
  if(cbxServer->Items->IndexOf(cbxServer->Text) > -1)
    IdFTP1->Host = frmMain->FTPConfig->GetFTPAttributeValue(cbxServer->Text,"Server");
  else
    IdFTP1->Host = cbxServer->Text;
  try
  {
    IdFTP1->Port = StrToInt(txtPort->Text);
  }
  catch(...)
  {
    txtPort->Text = "21";
    IdFTP1->Port  = 21;
  }
  Screen->Cursor = crHourGlass;
  IdFTP1->Username = txtUserName->Text;
  IdFTP1->Password = txtPassword->Text;
  IdFTP1->Passive  = cbPasv->Checked;
  try
  {
    IdFTP1->Connect(true,1000);
  }
  catch(Exception * E)
  {
    Screen->Cursor = crDefault;
    MessageBox(Handle,("连接服务器失败:" + E->Message).c_str(),"错误",MB_OK | MB_ICONSTOP);
    return;
  }
  tvFolder->Items->Clear();
  IdFTP1->ChangeDir("/");
  //IdFTP1->Quote("pwd /usr/catis/DATA/XiChang25_5ES");
  //IdFTP1->Quote("dir");
  TTreeNode * RootNode    = tvFolder->Items->AddChild(NULL,IdFTP1->RetrieveCurrentDir());
  RootNode->ImageIndex    = 1;
  RootNode->SelectedIndex = 2;
  ListDir(RootNode);
}
//---------------------------------------------------------------------------


void __fastcall TfrmOpenFTPDialog::lvFilesColumnClick(TObject *Sender,
      TListColumn *Column)
{
  lvFiles->HeaderSortColumn = Column->Index;
  if(lvFiles->HeaderSortDirection == sdDescending)
    lvFiles->HeaderSortDirection = sdAscending;
  else
    lvFiles->HeaderSortDirection = sdDescending;
  Screen->Cursor = crHourGlass;
  try
  {
    lvFiles->AlphaSort();
  }
  catch(...)
  {
  }
  Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenFTPDialog::FormResize(TObject *Sender)
{
  lvFiles->Update(); 
}
//---------------------------------------------------------------------------


void __fastcall TfrmOpenFTPDialog::lvFilesCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
{
  Compare = 0;
  switch(lvFiles->HeaderSortColumn)
  {
  case 0: //按文件名
    Compare = StrComp(Item1->Caption.c_str(),Item2->Caption.c_str());
    break;
  case 1: //按大小
    if((int)Item1->Data > (int)Item2->Data)
      Compare = 1;
    else if((int)Item1->Data < (int)Item2->Data)
      Compare = -1;
    //if(Compare == 0)
    //  Compare = StrComp(Item1->SubItems->Strings[2].c_str(),Item2->SubItems->Strings[2].c_str());
    break;
  case 2: //按类型
    Compare = StrComp(Item1->SubItems->Strings[1].c_str(),Item2->SubItems->Strings[1].c_str());
    break;
  case 3: //按修改日期
    {
      TDateTime D1,D2;
      D1 = StrToDateTime(Item1->SubItems->Strings[2]);
      D2 = StrToDateTime(Item2->SubItems->Strings[2]);
      if(D1 > D2)
        Compare = 1;
      else if(D1 < D2)
        Compare = -1;
    }
    break;
  case 4: //按类型
    Compare = StrComp(Item1->SubItems->Strings[3].c_str(),Item2->SubItems->Strings[3].c_str());
    break;    
  }
  if(lvFiles->HeaderSortDirection == sdAscending)
    Compare = Compare * -1;
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenFTPDialog::FormDestroy(TObject *Sender)
{
  Screen->Cursor = crHourGlass;
  lvFiles->Hide();
  lvFiles->Items->Clear();
  Screen->Cursor = crDefault;
  //delete FileList;
  delete frmSearchFile;
}
//---------------------------------------------------------------------------
void __fastcall TfrmOpenFTPDialog::cbxServerChange(TObject *Sender)
{
  if(cbxServer->Items->IndexOf(cbxServer->Text) > -1)
  {
    txtUserName->Text = frmMain->FTPConfig->GetFTPAttributeValue(cbxServer->Text,"UserName");
    txtPassword->Text = frmMain->FTPConfig->GetFTPAttributeValue(cbxServer->Text,"Password",true);
    txtPort->Text     = frmMain->FTPConfig->GetFTPAttributeValue(cbxServer->Text,"Port");
    cbPasv->Checked = frmMain->FTPConfig->GetFTPAttributeValue(cbxServer->Text,"Mode").UpperCase() == "PASV";    
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmOpenFTPDialog::SearchFile(AnsiString APath,AnsiString SearchFileName,int Level)
{
  TStringList * DirList = new TStringList;
  AnsiString FileName;
  TListItem * Item;
  AnsiString FileType;
  try
  {
    IdFTP1->ChangeDir(APath);
    AnsiString CurPath = IdFTP1->RetrieveCurrentDir();
    IdFTP1->List(NULL,SearchFileName,true);
    for(int n = 0;n < IdFTP1->DirectoryListing->Count;n++)
    {
      FileName = IdFTP1->DirectoryListing->Items[n]->FileName;
      if(FileName == "." || FileName == "..")
        continue;
      if(IdFTP1->DirectoryListing->Items[n]->ItemType == ditDirectory)
      {
          DirList->Add(APath + FileName);
      }
      else
      {
        //Item = lvFiles->FindCaption(-1,FileName,false,false,false);
        //if(Item != NULL && strcmp(Item->SubItems->Strings[3].c_str(),CurPath.c_str()) == 0)
        //{
        //  continue;
        //}
        Item = lvFiles->Items->Add();
        Item->Caption = FileName;
        Item->ImageIndex = IMAGE_FILE;
        if(IdFTP1->DirectoryListing->Items[n]->Size > 1048576)
          Item->SubItems->Add(FloatToStr(RoundTo((float)IdFTP1->DirectoryListing->Items[n]->Size / 1048576,-2)) + "MB");
        else if(IdFTP1->DirectoryListing->Items[n]->Size > 1024)
          Item->SubItems->Add(FloatToStr(RoundTo((float)IdFTP1->DirectoryListing->Items[n]->Size / 1024,-2)) + "KB");
        else
          Item->SubItems->Add(IntToStr(IdFTP1->DirectoryListing->Items[n]->Size) +"B");
        FileType = ExtractFileExt(FileName);
        if(FileType != "")
          Item->SubItems->Add(FileType.SubString(2,FileType.Length() - 1) + "文件");
        else
          Item->SubItems->Add("");
        Item->SubItems->Add(IdFTP1->DirectoryListing->Items[n]->ModifiedDate.FormatString("yyyy-mm-dd hh:nn:ss"));
        Item->SubItems->Add(CurPath);
        Item->Data = (void *)IdFTP1->DirectoryListing->Items[n]->Size;
      }
    }
  }
  catch(Exception * E)
  {
    Screen->Cursor = crDefault;
    MessageBox(Handle,E->Message.c_str(),"错误",MB_OK | MB_ICONSTOP);
  }
  if(Level > 0)
  {
    for(int n = 0;n < DirList->Count;n++)
    {
      SearchFile(DirList->Strings[n],SearchFileName,Level - 1);
    }
  }
  delete DirList;
}
//生成要列出文件的目录列表
void __fastcall TfrmOpenFTPDialog::CreateListPaths(TTreeNode * ParentNode,
  TStringList * PathList,
  bool Recursion/*是否递归列出子目录下的文件*/
  )
{
  AnsiString APath;
  GetNodePath(ParentNode,APath);
  if(PathList->IndexOf(APath) < 0)
    PathList->Add(APath);
  if(Recursion)
  {
    for(int n = 0;n < ParentNode->Count;n++)
    {
      CreateListPaths(ParentNode->Item[n],PathList,Recursion);
    }
  }
}
void __fastcall TfrmOpenFTPDialog::GetSelectedNodes(TTreeNode * ParentNode,TList * NodeList)
{
  if(ParentNode->Selected) 
  {
    NodeList->Add((TObject *)ParentNode);
  }
  for(int n = 0;n < ParentNode->Count;n++)
  {
    GetSelectedNodes(ParentNode->Item[n],NodeList);
  }
}
void __fastcall TfrmOpenFTPDialog::menuSearchFileClick(TObject *Sender)
{
  if(tvFolder->Selected == NULL)
    return;
  frmSearchFile->chkSearchChildDir->Checked = Recursion;
  if(frmSearchFile->ShowModal() != mrOk)
    return;
  Recursion = frmSearchFile->chkSearchChildDir->Checked;
  Screen->Cursor = crHourGlass;
  lvFiles->Items->Clear();
  lvFiles->Hide();
  TList *       NodeList = new TList;
  TStringList * PathList = new TStringList;
  GetSelectedNodes(tvFolder->Items->GetFirstNode(),NodeList);
  for(int n = 0;n < NodeList->Count;n++)
  {
    CreateListPaths((TTreeNode *)NodeList->Items[n],PathList,Recursion);
  }
  for(int n = 0;n < PathList->Count;n++)
  {
    SearchFile(PathList->Strings[n],"",0);
  }
  delete PathList;
  delete NodeList;
  lvFiles->Show();
  Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenFTPDialog::PopupMenu2Popup(TObject *Sender)
{
  menuSearchFile->Enabled = tvFolder->Selected != NULL;  
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenFTPDialog::tvFolderMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if(Button == mbRight)
  {
    TTreeNode * Node = tvFolder->GetNodeAt(X,Y);
    if(Node == NULL)
      return;
    Node->Selected = true;
    Node->Focused  = true;
  }
}
//---------------------------------------------------------------------------

bool __fastcall TfrmOpenFTPDialog::FormHelp(WORD Command, int Data,
      bool &CallHelp)
{
    if(Command != HELP_CONTEXTPOPUP)
    {
        CallHelp = false;
        return true;
    }
    WinHelp(Handle,(HelpFilePath + "openftpdialog.hlp").c_str(),HELP_CONTEXTPOPUP,Data);
    CallHelp = false;
    return true;
}
//---------------------------------------------------------------------------


