

#include "FolderBrowseDialog.h"
#define INFO_BUFFER_SIZE 32767

FolderBrowseDialog::FolderBrowseDialog(HWND HwndOwner)
{
   memset(&FInfo,0,sizeof(BROWSEINFO));
  memset(FFolderName,0,260);
   FInfo.hwndOwner = HwndOwner;
   FInfo.pszDisplayName = FFolderName;
   FInfo.lpszTitle = "请选择目录";
   FInfo.ulFlags = BIF_RETURNONLYFSDIRS;
}
//---------------------------------------------------------------------------
FolderBrowseDialog::FolderBrowseDialog()
{
   memset(&FInfo,0,sizeof(BROWSEINFO));
   memset(FFolderName,0,260);
   FInfo.pszDisplayName = FFolderName;
   FInfo.lpszTitle = "请选择目录";
   FInfo.ulFlags = BIF_RETURNONLYFSDIRS;
}
//---------------------------------------------------------------------------
void __fastcall FolderBrowseDialog::SetBrowseInfoFlags(UINT ulFlags)
{
   FInfo.ulFlags = ulFlags;
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
