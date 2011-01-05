//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "OpenDialogFrm.h"
#include "public.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzCmboBx"
#pragma link "RzShellCtrls"
#pragma link "RzGroupBar"
#pragma link "RzPanel"
#pragma link "RzListVw"
#pragma link "RzButton"
#pragma link "dcOutPanel"
#pragma link "RzTreeVw"
#pragma link "dcOutPanel"
#pragma link "RzRadChk"
#pragma resource "*.dfm"
TfrmOpenDialog *frmOpenDialog;
//---------------------------------------------------------------------------
__fastcall TfrmOpenDialog::TfrmOpenDialog(TComponent* Owner)
  : TForm(Owner)
{
  //Files    = new TStringList;
  PathName = "";
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenDialog::btnCancelClick(TObject *Sender)
{
  ModalResult = mrCancel;  
}
//---------------------------------------------------------------------------
bool __fastcall TfrmOpenDialog::Execute()
{
  //Files->Clear();
  return ShowModal() == mrOk;
}
void __fastcall TfrmOpenDialog::btnOpenClick(TObject *Sender)
{
  if(lvFiles->SelCount == 0)
    return;
  if(cbxFileFormat->ItemIndex < 0)
  {
    MessageBox(Handle,"请指定当前将要打开的文件的格式!","提示",MB_OK | MB_ICONWARNING);
    cbxFileFormat->SetFocus();
    return;
  }
  PathName = lvFiles->Folder->PathName;
  if(PathName.SubString(PathName.Length(),1) != "\\")
    PathName += "\\";
/*
  TListItem * Item;
  TItemStates States = TItemStates() << isSelected;
  PathName = lvFiles->Folder->PathName;
  if(PathName.SubString(PathName.Length(),1) != "\\")
    PathName += "\\";
  Item = lvFiles->GetNextItem(NULL,sdAll,States);
  Screen->Cursor = crHourGlass;
  //Files->Clear();
  //unsigned int FileSize;
  //HANDLE hFile;
  //AnsiString FileName;
  while(Item != NULL)
  {
    FileName = PathName + Item->Caption;
    hFile = CreateFile(FileName.c_str(),GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if(hFile == INVALID_HANDLE_VALUE)
    {
      if(DirectoryExists(FileName)) //如果是文件夹
      {
        continue;
      }
      else
      {
        MessageBox(Handle,("不能获取文件 " + FileName + " 的大小").c_str(),"警告",MB_OK | MB_ICONWARNING);
        FileSize = -1;
      }
    }
    else
    {
      FileSize = GetFileSize(hFile,NULL);
      CloseHandle(hFile);
    }
    Files->AddObject(FileName,(TObject *)FileSize);
    Item = lvFiles->GetNextItem(Item,sdAll,States);
  }
  Screen->Cursor = crDefault;
  if(Files->Count == 0)
    return;
*/
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenDialog::cbxFolderChange(TObject *Sender)
{
  lvFiles->Folder->PathName = cbxFolder->SelectedFolder->PathName;
  tvFolder->SelectedPathName = cbxFolder->SelectedFolder->PathName;
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenDialog::lvFilesFolderChanged(TObject *Sender)
{
  cbxFolder->SelectedFolder->PathName = lvFiles->Folder->PathName;
  tvFolder->SelectedPathName = lvFiles->Folder->PathName;
}
//---------------------------------------------------------------------------
void __fastcall TfrmOpenDialog::ToolButton1Click(TObject *Sender)
{
  lvFiles->GoUp(1);
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenDialog::menuIconClick(TObject *Sender)
{
  lvFiles->ViewStyle = vsIcon;
  menuIcon->Checked = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenDialog::menuSmallClick(TObject *Sender)
{
  lvFiles->ViewStyle = vsSmallIcon;
  menuSmall->Checked = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenDialog::menuListClick(TObject *Sender)
{
    lvFiles->ViewStyle = vsList;
    menuList->Checked = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenDialog::menuDetailClick(TObject *Sender)
{
    lvFiles->ViewStyle = vsReport;
    menuDetail->Checked = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenDialog::FormDestroy(TObject *Sender)
{
  //delete Files;  
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenDialog::lvFilesDblClickOpen(TObject *Sender,
      bool &Handled)
{
  btnOpenClick(NULL);
  Handled = true;
}
//---------------------------------------------------------------------------



void __fastcall TfrmOpenDialog::tvFolderChange(TObject *Sender,
      TTreeNode *Node)
{
  cbxFolder->SelectedFolder->PathName = tvFolder->SelectedPathName;
  lvFiles->Folder->PathName = tvFolder->SelectedPathName;
}
//---------------------------------------------------------------------------


bool __fastcall TfrmOpenDialog::FormHelp(WORD Command, int Data,
      bool &CallHelp)
{
    if(Command != HELP_CONTEXTPOPUP)
    {
        CallHelp = false;
        return true;
    }
    WinHelp(Handle,(HelpFilePath + "opendialog.hlp").c_str(),HELP_CONTEXTPOPUP,Data);
    CallHelp = false;
    return true;
}
//---------------------------------------------------------------------------

