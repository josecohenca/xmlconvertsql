

#include "FolderBrowseDialog.h"
#include <stdio.h>
#define INFO_BUFFER_SIZE 32767
//以下程序通SHBrowseForFolder返回用户选择的目录
//具有以下几个特性。
//1。有初始化目录
//2。每次选择的目录自动增加字符\sictech
AnsiString InitFolder;
int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)   
{
  switch(uMsg)
  {
  case   BFFM_INITIALIZED:
    ::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)InitFolder.c_str());
    break;
  case   BFFM_SELCHANGED:
    {
      char curr[MAX_PATH];
      SHGetPathFromIDList((LPCITEMIDLIST)lParam,curr);
      if(curr[strlen(curr)-1]==92)
        sprintf(curr,"%sSictech",curr);
      else
        sprintf(curr,"%s\\Sictech",curr);
      ::SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)curr);
    }
    break;
  }
  return   0;
}  
FolderBrowseDialog::FolderBrowseDialog(HWND HwndOwner)
{
  memset(&FInfo,0,sizeof(BROWSEINFO));
  memset(FFolderName,0,260);
  FInfo.hwndOwner      = HwndOwner;
  FInfo.pszDisplayName = FFolderName;
  FInfo.lpszTitle = "请选择目录";
  FInfo.ulFlags   = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
  FInfo.lpfn      =BrowseCallbackProc;
}
//---------------------------------------------------------------------------
FolderBrowseDialog::FolderBrowseDialog()
{
  memset(&FInfo,0,sizeof(BROWSEINFO));
  memset(FFolderName,0,260);
  FInfo.pszDisplayName = FFolderName;
  FInfo.lpszTitle = "请选择目录";
  FInfo.ulFlags   = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
  FInfo.lpfn      =BrowseCallbackProc;   
}
//---------------------------------------------------------------------------
void __fastcall FolderBrowseDialog::SetBrowseInfoFlags(UINT ulFlags)
{
   FInfo.ulFlags = ulFlags;
}
void __fastcall FolderBrowseDialog::SetFolderPath(AnsiString APath)
{
  InitFolder  = APath;
  FFolderPath = APath;
}
//---------------------------------------------------------------------------
bool __fastcall FolderBrowseDialog::Execute()
{
   LPITEMIDLIST ItemID;
   char SelectDir[INFO_BUFFER_SIZE];
   memset(SelectDir,0,INFO_BUFFER_SIZE);
   ItemID = SHBrowseForFolder(&FInfo);
   if(ItemID)
   {
      SHGetPathFromIDList(ItemID,SelectDir);
      GlobalFree(ItemID);
      FFolderPath = AnsiString(SelectDir);
      return true;
   }
   else
   {
      return false;
   }
}
//---------------------------------------------------------------------------
bool __fastcall FolderBrowseDialog::Execute(HWND HwndOwner)
{
   FInfo.hwndOwner = HwndOwner;
   if(Execute())
   {
      return true;
   }
   else
   {
      return false;
   }
}
//---------------------------------------------------------------------------
AnsiString __fastcall FolderBrowseDialog::GetDialogTitle()
{
   return FInfo.lpszTitle;
}
//---------------------------------------------------------------------------
AnsiString __fastcall FolderBrowseDialog::GetFolderName()
{
   return AnsiString(FFolderName);
}
//---------------------------------------------------------------------------
void __fastcall FolderBrowseDialog::SetDialogTitle(AnsiString title)
{
   FInfo.lpszTitle = title.c_str();
}
//---------------------------------------------------------------------------
AnsiString __fastcall FolderBrowseDialog::GetFolderPath()
{
   return FFolderPath;
}
//---------------------------------------------------------------------------
