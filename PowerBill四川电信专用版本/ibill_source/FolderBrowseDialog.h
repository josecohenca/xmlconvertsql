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
 //   fd.Title = "ѡ��Ŀ¼";
 //   fd.Execute(Application->Handle);
 //result:
 //   fd.FolderName
 //   fd.FolderPath

private:
   BROWSEINFO FInfo;

protected:
   char FFolderName[260];        /**���淵�ص�Ŀ¼����*/
   AnsiString FFolderPath;       /**����·����*/
   AnsiString __fastcall GetDialogTitle(void);
   void __fastcall SetDialogTitle(AnsiString title);
   AnsiString __fastcall GetFolderPath(void);
   AnsiString __fastcall GetFolderName(void);
   void __fastcall SetFolderPath(AnsiString APath);
public:
   /**��ʼ�����þ��
   @HwndOwner �����߾��
   */
  FolderBrowseDialog(HWND HwndOwner);
  FolderBrowseDialog();    //���û��ָ�����������ʹ�ô�������Execute����
  void __fastcall SetBrowseInfoFlags(UINT ulFlags);     /**�ṩ��FInfo���Զ���*/
  bool __fastcall Execute(void);       //�򿪶Ի���
  bool __fastcall Execute(HWND HwndOwner);
   __property AnsiString Title={read=GetDialogTitle, write=SetDialogTitle};
   __property AnsiString FolderName={read=GetFolderName};
   __property AnsiString FolderPath={read=GetFolderPath,write=SetFolderPath};
};
#endif
 