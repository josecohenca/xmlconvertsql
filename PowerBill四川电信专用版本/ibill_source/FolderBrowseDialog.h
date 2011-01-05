#define NO_WIN32_LEAN_AND_MEAN
#ifndef FolderBrowseDialogH
#define FolderBrowseDialogH
#include <shlobj.h>
#include <vcl.h>
//---------------------------------------------------------------------------
class FolderBrowseDialog
{
 //for user
 //usage:
 //declare:
 //   FolderBrowseDialog fd;
 //set properties:
 //   fd.Title = "选择目录";
 //   fd.Execute(Application->Handle);
 //result:
 //   fd.FolderName
 //   fd.FolderPath

private:
   BROWSEINFO FInfo;

protected:
   char FFolderName[260];        /**保存返回的目录名称*/
   AnsiString FFolderPath;       /**保存路径名*/
   AnsiString __fastcall GetDialogTitle(void);
   void __fastcall SetDialogTitle(AnsiString title);
   AnsiString __fastcall GetFolderPath(void);
   AnsiString __fastcall GetFolderName(void);
   void __fastcall SetFolderPath(AnsiString APath);
public:
   /**初始化调用句柄
   @HwndOwner 调用者句柄
   */
  FolderBrowseDialog(HWND HwndOwner);
  FolderBrowseDialog();    //如果没有指定句柄，则需使用带参数的Execute函数
  void __fastcall SetBrowseInfoFlags(UINT ulFlags);     /**提供对FInfo的自定义*/
  bool __fastcall Execute(void);       //打开对话框
  bool __fastcall Execute(HWND HwndOwner);
   __property AnsiString Title={read=GetDialogTitle, write=SetDialogTitle};
   __property AnsiString FolderName={read=GetFolderName};
   __property AnsiString FolderPath={read=GetFolderPath,write=SetFolderPath};
};
#endif
 