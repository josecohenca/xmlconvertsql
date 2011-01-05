//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ExportToFileThread.h"


#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TExportToFileThead::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TExportToFileThead::TExportToFileThead(bool CreateSuspended)
  : TThread(CreateSuspended)
{
  DestFile   = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TExportToFileThead::Execute()
{
  DestFile = fopen(DestFileName.c_str(),"w+b");
  if(DestFile == NULL)
  {
    ErrorMessage = "不能创建文件" + DestFileName;
    SendMessage(BillFile->hWnd,MSG_THREAD_COMPLETED,(WPARAM)this,0);
    return;
  }
  TList * List1 = NULL;
  TList * List2 = NULL;
  int n;
  switch(DestFileType)
  {
  case EXPORT_DBF:
    List1 = new TList;
    List2 = new TList;
    BillFile->WriteDBFHead(DestFile,List1,List2);
    break;
  case EXPORT_ORI:
    //写原始文件头
    BillFile->SaveFileHeadToFile(DestFile);
    break;
  case EXPORT_CSV:
    for(n = 0;n < BillFile->FieldCount - 1;n++)
    {
      fprintf(DestFile,"%s%s",BillFile->GetBillField(n)->Label,",");
    }
    fprintf(DestFile,"%s\r\n",BillFile->GetBillField(n)->Label);
    break;
  case EXPORT_TXT:
    for(n = 0;n < BillFile->FieldCount - 1;n++)
    {
      fprintf(DestFile,"%s%s",BillFile->GetBillField(n)->FieldName,"|");
    }
    fprintf(DestFile,"%s\r\n",BillFile->GetBillField(n)->FieldName);
    break;
  }
  int RecordCount = 0;
  int l;
  HANDLE hOldOwnerWnd;
  try
  {
    for(n = 0;n < FileList->Count;n++)
    {
//      if(FileList->Strings[n].SubString(1,6) == "ftp://" || (unsigned int)FileList->Objects[n] > 1024000)
      hOldOwnerWnd = BillFile->hWnd;
      BillFile->SetOwnerWnd(Application->MainForm->Handle);
      try
      {
        BillFile->OpenBillFile(FileList->Strings[n],BillNameList->Strings[n]);
      }
      catch(Exception * E)
      {
        BillFile->SetOwnerWnd(hOldOwnerWnd);
        ErrorMessage = "不能打开文件" + FileList->Strings[n];
        break;
      }
      SendMessage(BillFile->hWnd,MSG_THREAD_COMPLETED,0,0);
      BillFile->SetOwnerWnd(hOldOwnerWnd);
      l = BillFile->GetRecordCount();
      SendMessage(BillFile->hWnd,MSG_SET_PROCESS_MAX_COUNT,l,(LPARAM)BillFile->GetOriShortFileName().c_str());
      switch(DestFileType)
      {
      case EXPORT_DBF:
        BillFile->ExportDataToDBFFile(DestFile,List1,List2);
        break;
      case EXPORT_CSV:
        BillFile->ExportDataToTxtFile(DestFile,",");
        break;
      case EXPORT_TXT:
        BillFile->ExportDataToTxtFile(DestFile,"|");
        break;
      case EXPORT_ORI:
        BillFile->ExportDataToOriFile(DestFile);
        break;
      default:
        ErrorMessage = "目标文件类型不正确!";
        break;
      }
      RecordCount += l;
      BillFile->CloseBillFile();
      SendMessage(BillFile->hWnd,MSG_SET_PROCESS_MAX_COUNT,0,NULL);
      if(BillFile->IsTermBuUser())
      {
        ErrorMessage = "操作被用户中止!";
        break;
      }
    }
  }
  catch(Exception * E)
  {
    ErrorMessage = E->Message;
  }
  if(DestFileType == EXPORT_DBF)
  {
    BillFile->EndOfDBFFile(DestFile,RecordCount);
    delete List1;
    delete List2;
  }
  else if(DestFileType == EXPORT_ORI)
  {
    //写原始文件尾
    BillFile->SaveFileTrailToFile(DestFile);
  }
  fclose(DestFile);
  SendMessage(BillFile->hWnd,MSG_THREAD_COMPLETED,(WPARAM)this,0);
}
//---------------------------------------------------------------------------
