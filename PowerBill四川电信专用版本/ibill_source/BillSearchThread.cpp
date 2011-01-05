//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BillSearchThread.h"
#include "BillField.h"
#include "MainFrm.h"


#pragma package(smart_init)
//详单搜索线程

//匹配内容
//Str1是要查找的字符串,Str2是字段值
typedef bool __fastcall (* MatchProcedure)(TBillField * BillField,const AnsiString & Str1,const AnsiString & Str2,int MatchMode);
//A < B
bool __fastcall LessThanMatch(TBillField * BillField,const AnsiString & Str1,const AnsiString & Str2,int MatchMode)
{
  return BillField->MatchFieldData(Str2,Str1,Str1.Length()) < 0;
}
//A > B
bool __fastcall GreaterThanMatch(TBillField * BillField,const AnsiString & Str1,const AnsiString & Str2,int MatchMode)
{
  return BillField->MatchFieldData(Str2,Str1,Str1.Length()) > 0;
}

//A = B
bool __fastcall EqualsMatch(TBillField * BillField,const AnsiString & Str1,const AnsiString & Str2,int MatchMode)
{
  if(MatchMode == 0)
    return strcmp(Str2.c_str(),Str1.c_str()) == 0;
  else
    return BillField->MatchFieldData(Str2,Str1,Str1.Length()) == 0;
}
//A != B
bool __fastcall NotEqualsMatch(TBillField * BillField,const AnsiString & Str1,const AnsiString & Str2,int MatchMode)
{
  if(MatchMode == 0)
    return strcmp(Str2.c_str(),Str1.c_str()) != 0;
  else
    return BillField->MatchFieldData(Str2,Str1,Str1.Length()) != 0;
}

//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TBillSearchThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TBillSearchThread::TBillSearchThread(bool CreateSuspended)
  : TThread(CreateSuspended)
{
  TempFile    = NULL;
  FoundCount  = 0;
  Continue    = true;
  AutoScroll  = true;
  ErrorCount  = 0;
}
//---------------------------------------------------------------------------
void __fastcall TBillSearchThread::QueryByFields()
{
  /*
  *TempFile1:保存索引
  *TempFile2:保存数据
  *TempFile3:保存当前正在查找的文件的索引
  *TempFile4:保存当前正在查找的文件的数据
  */
  AnsiString TempFileName1 = TempFileName + "__index_"  + FormatDateTime("yyyymmddhhnnss",Now());
  AnsiString TempFileName2 = TempFileName + "__data_"  + FormatDateTime("yyyymmddhhnnss",Now());
  AnsiString TempFileName3 = TempFileName + "__indexcur_"  + FormatDateTime("yyyymmddhhnnss",Now());
  AnsiString TempFileName4 = TempFileName + "__datacur_"  + FormatDateTime("yyyymmddhhnnss",Now());
  FILE * TempFile1 = fopen(TempFileName1.c_str(),"w+b");
  FILE * TempFile2 = fopen(TempFileName2.c_str(),"w+b");
  FILE * TempFile3;
  FILE * TempFile4;
  TempFile = fopen(TempFileName.c_str(),"w+b");
  if(TempFile1 == NULL || TempFile2 == NULL || TempFile == NULL)
  {
    ErrorMessage = "不能创建临时文件";
    if(TempFile1 != NULL)
      fclose(TempFile1);
    if(TempFile2 != NULL)
      fclose(TempFile2);
    return;
  }
  TList * MatchProcList = new TList;                  //匹配函数列表
  for(int n = 0;n < MatchFieldList->Count;n++)
  {
    //BillField = (TBillField * )MatchFieldList->Objects[n];
    if(MatchFieldOperatorList->Strings[n] == "等于")
      //BillField->Tag = (int)EqualsMatch;
      MatchProcList->Add((TObject *)EqualsMatch);
    else if(MatchFieldOperatorList->Strings[n] == "小于")
      //BillField->Tag = (int)LessThanMatch;
      MatchProcList->Add((TObject *)LessThanMatch);
    else if(MatchFieldOperatorList->Strings[n] == "不等于")
      //BillField->Tag = (int)NotEqualsMatch;
      MatchProcList->Add((TObject *)NotEqualsMatch);
    else if(MatchFieldOperatorList->Strings[n] == "大于")
      //BillField->Tag = (int)GreaterThanMatch;
      MatchProcList->Add((TObject *)GreaterThanMatch);
  }
  bool Found;
  MatchProcedure MatchProc;
  int n,l,k;
  int RecordCount;
  SearchResultFileHead  srfh;
  memset(&srfh,0,sizeof(srfh));
  SearchResultDataIndex srdi;
  SearchResultData      srd;
  strcpy(&srfh.BillName[0],BillFile->GetBillName().c_str());
  int RecordLength = BillFile->RecordLength;
  int DataPosition = 0;
  TMemoryStream * FileStream = new TMemoryStream();
  TListItem * Item;
  int CurFileFoundCount;  //当前文件中找到的记录数
  TBillField * BillField;
  //AndOrList->Items[0] = (TObject *)0;  //第一个条件总是OR
  //bool CurAndOr;  //当前的上下文关系操作符,true代表AND,false代表OR
  //TfrmOpeningFile * frmOpeningFile = NULL;
  HANDLE hOldOwnerWnd;
  ErrorCount = 0;
  for(n = 0;n < FileList->Count;n++)
  {
    ErrorMessage = "";
    Item = ListView->Items->Item[n];
    if(AutoScroll)
      Item->MakeVisible(true);
    CurFileFoundCount = 0;
    try
    {
      Item->ImageIndex = IMAGE_CUR_ITEM;
      TempFile3 = fopen(TempFileName3.c_str(),"w+b");
      TempFile4 = fopen(TempFileName4.c_str(),"w+b");
      if(TempFile3 == NULL || TempFile4 == NULL)
        throw new Exception("不能创建临时文件!");
      /*if(FileList->Strings[n].SubString(1,6) == "ftp://" || (unsigned int)FileList->Objects[n] > 1024000)
      {
        if(frmOpeningFile == NULL)
        {
          frmOpeningFile = new TfrmOpeningFile(Application);
        }
        frmOpeningFile->BillFile = BillFile;
        frmOpeningFile->FileName = FileList->Strings[n];
        frmOpeningFile->BillName = BillNameList->Strings[n];
        //frmOpeningFile->Canvas->TryLock();
        if(frmOpeningFile->ShowModal() != mrOk)
        {
          //frmOpeningFile->Canvas->Unlock();
          throw new Exception("不能打开文件" + FileList->Strings[n] + ":" + frmOpeningFile->ErrorMessage);
        }
        //frmOpeningFile->Canvas->Unlock();
      }
      else
      {*/
      hOldOwnerWnd = BillFile->hWnd;
      BillFile->SetOwnerWnd(Application->MainForm->Handle);
      BillFile->OpenBillFile(FileList->Strings[n],BillNameList->Strings[n]);
      SendMessage(BillFile->hWnd,MSG_THREAD_COMPLETED,0,0);
      BillFile->SetOwnerWnd(hOldOwnerWnd);
      //}
      RecordCount = BillFile->GetRecordCount();
      SendMessage(BillFile->hWnd,MSG_SET_PROCESS_MAX_COUNT,RecordCount,(LPARAM)BillFile->GetOriShortFileName().c_str());
      Item->SubItems->Strings[2] = RecordCount;
      
      for(k = 0;k < RecordCount;k++)
      {
        BillFile->ReadSingleRecord(k);
        if(MatchFieldList->Count > 0)
        {
          for(l = 0;l < MatchFieldList->Count;l++)
          {
            BillField = (TBillField * )MatchFieldList->Objects[l];
            MatchProc = (MatchProcedure)(MatchProcList->Items[l]);
            Found = MatchProc(BillField,MatchFieldValueList->Strings[l],BillField->GetFieldValue(),(int)MatchModeList->Items[l]);
            if((int)AndOrList->Items[l] == 1)
            {
              if(!Found) //如果当前的上下文关系是AND,并且没有匹配到符合条件的字段,则停止
                break;
            }
            else
            {
              if(Found)  //如果当前的上下文关系是OR,并且匹配到了符合条件的字段,则停止
                break;
            }
          }
        }
        else
          Found = true;
        if(k > 1000 && k % 1000 == 0)
        {
          SendMessage(BillFile->hWnd,MSG_SET_PROCESS_POS,k,0);
        }
        if(Found)
        {
          //写入到索引文件
          memset(&srdi,0,sizeof(srdi));
          srdi.RecordIndex = FoundCount;
          srdi.Position    = DataPosition;
          fwrite(&srdi,sizeof(srdi),1,TempFile3);
          //写入到数据文件
          memset(&srd,0,sizeof(srd));
          switch(BillFile->FileType)
          {
          case BIN_BILL_FORMAT:
            srd.DataLength = RecordLength;
            break;
          case TXT_BILL_FORMAT:
            srd.DataLength = BillFile->ri.RecordLength;
            break;
          }
          strcpy(&srd.OriFileName[0],BillFile->GetShortFileName().c_str());
          srd.OriDataIndex = BillFile->GetCurRecordPosition();
          fwrite(&srd,sizeof(srd),1,TempFile4);
          fwrite(BillFile->BillRecord->Buffer,srd.DataLength,1,TempFile4);
          ++FoundCount;
          ++CurFileFoundCount;
          DataPosition += srd.DataLength + sizeof(srd);
        }
        if(!Continue)
          break;
      }
      BillFile->CloseBillFile();
      SendMessage(BillFile->hWnd,MSG_SET_PROCESS_MAX_COUNT,0,NULL);
      fclose(TempFile3);
      fclose(TempFile4);
      //将数据写入到临时文件
      FileStream->LoadFromFile(TempFileName3);
      fwrite(FileStream->Memory,FileStream->Size,1,TempFile1);
      FileStream->Clear();
      FileStream->LoadFromFile(TempFileName4);
      fwrite(FileStream->Memory,FileStream->Size,1,TempFile2);
      FileStream->Clear();
      Item->ImageIndex = IMAGE_SUCCEED_ITEM;
      Item->SubItems->Strings[3] = CurFileFoundCount;
      SendMessage(hWnd,MSG_FILE_PROCESSED,n + 1,NULL);
      if(!Continue)
        break;
    }
    catch(Exception * E)
    {
      SendMessage(hWnd,MSG_FILE_PROCESSED,n + 1,NULL);
      BillFile->CloseBillFile();
      if(TempFile3 != NULL)
      {
        fclose(TempFile3);
        TempFile3 = NULL;
      }
      if(TempFile4 != NULL)
      {
        fclose(TempFile4);
        TempFile4 = NULL;
      }
      if(FileStream->Size > 0)
        FileStream->Clear();
      ErrorMessage = E->Message;
      Item->SubItems->Strings[3] = "打开文件失败:" + E->Message;
      Item->ImageIndex = IMAGE_FAILED_ITEM;
      ++ErrorCount;
      if(!Continue)
        break;
      continue;
    }
  }
  delete MatchProcList;
  if(TempFile3 != NULL)
  {
    fclose(TempFile3);
    TempFile3 = NULL;
  }
  if(TempFile4 != NULL)
  {
    fclose(TempFile4);
    TempFile4 = NULL;
  }
  if(FileStream != NULL)
  {
    delete FileStream;
    FileStream = NULL;
  }
  if(TempFile1 != NULL && TempFile2 != NULL && TempFile != NULL)
  {
    SendMessage(BillFile->hWnd,MSG_SAVE_TO_TEMPFILE,FoundCount,0);
    fclose(TempFile1);
    fclose(TempFile2);

    TMemoryStream * Stream = new TMemoryStream;
    Stream->LoadFromFile(TempFileName1);

    srfh.RecordCount     = FoundCount;
    srfh.IndexAreaLength = Stream->Size;
    fwrite(&srfh,sizeof(srfh),1,TempFile);
    fwrite(Stream->Memory,Stream->Size,1,TempFile);
    Stream->Clear();

    Stream->LoadFromFile(TempFileName2);
    fwrite(Stream->Memory,Stream->Size,1,TempFile);
    delete Stream;
    //BillFile->SaveFileTrailToFile(TempFile);
    fclose(TempFile);
    TempFile = NULL;
    DeleteFile(TempFileName1);
    DeleteFile(TempFileName2);
    DeleteFile(TempFileName3);
    DeleteFile(TempFileName4);
  }
}
void __fastcall TBillSearchThread::QueryByScript()  //按脚本查询
{
  /*
  *TempFile1:保存索引
  *TempFile2:保存数据
  *TempFile3:保存当前正在查找的文件的索引
  *TempFile4:保存当前正在查找的文件的数据
  */
  AnsiString TempFileName1 = TempFileName + "__index_"  + FormatDateTime("yyyymmddhhnnss",Now());
  AnsiString TempFileName2 = TempFileName + "__data_"  + FormatDateTime("yyyymmddhhnnss",Now());
  AnsiString TempFileName3 = TempFileName + "__indexcur_"  + FormatDateTime("yyyymmddhhnnss",Now());
  AnsiString TempFileName4 = TempFileName + "__datacur_"  + FormatDateTime("yyyymmddhhnnss",Now());
  FILE * TempFile1 = fopen(TempFileName1.c_str(),"w+b");
  FILE * TempFile2 = fopen(TempFileName2.c_str(),"w+b");
  FILE * TempFile3;
  FILE * TempFile4;
  TempFile = fopen(TempFileName.c_str(),"w+b");
  if(TempFile1 == NULL || TempFile2 == NULL || TempFile == NULL)
  {
    ErrorMessage = "不能创建临时文件";
    if(TempFile1 != NULL)
      fclose(TempFile1);
    if(TempFile2 != NULL)
      fclose(TempFile2);
    return;
  }
  bool Found;
  int n,l,k,p;
  int RecordCount;
  SearchResultFileHead  srfh;
  memset(&srfh,0,sizeof(srfh));
  SearchResultDataIndex srdi;
  SearchResultData      srd;
  strcpy(&srfh.BillName[0],BillFile->GetBillName().c_str());
  int RecordLength = BillFile->RecordLength;
  int DataPosition = 0;
  TMemoryStream * FileStream = new TMemoryStream();
  TListItem * Item;
  int CurFileFoundCount;  //当前文件中找到的记录数
  TBillField * BillField;
  HANDLE hOldOwnerWnd;
  ErrorCount = 0;
  CoInitialize(NULL);
  TScriptControl * ScriptControl = new TScriptControl(frmMain);
  ScriptControl->Language = "VBScript";
  ScriptControl->AllowUI  = false;
  LPSAFEARRAY Params; //传递给脚本的参数
  Params = SafeArrayCreateVector(VT_VARIANT, 0, MatchFieldList->Count);
  VARIANT * pData = NULL;
  SafeArrayAccessData(Params, (void **)&pData);     //锁定数据
  TList * ParamList = new TList;                    //参数列表
  VARIANT * pDataTmp;
  for(n = 0;n < MatchFieldList->Count;n++)
  {
    pDataTmp = new VARIANT;
    pDataTmp->vt = VT_BSTR;
    ParamList->Add((TObject *)pDataTmp);
    //* pData = * pDataTmp;
    //pData++;
  }
  ScriptControl->AddCode(VBScript.c_bstr());
  bool ScriptSucceed;
  if(ScriptControl->Error->Number > 0)
  {
    ErrorMessage = "查询脚本错误:第 " + AnsiString(ScriptControl->Error->Line) \
                       + " 行，第 " + AnsiString(ScriptControl->Error->Column) \
                       + "列出现错误 " + AnsiString(ScriptControl->Error->Number) \
                       + " (" + AnsiString(ScriptControl->Error->Description) + ")";
  }
  else
  {
    for(n = 0;n < FileList->Count;n++)
    {
      ErrorMessage = "";
      Item = ListView->Items->Item[n];
      if(AutoScroll)
        Item->MakeVisible(true);
      CurFileFoundCount = 0;
      try
      {
        Item->ImageIndex = IMAGE_CUR_ITEM;
        TempFile3 = fopen(TempFileName3.c_str(),"w+b");
        TempFile4 = fopen(TempFileName4.c_str(),"w+b");
        if(TempFile3 == NULL || TempFile4 == NULL)
          throw new Exception("不能创建临时文件!");
        hOldOwnerWnd = BillFile->hWnd;
        BillFile->SetOwnerWnd(Application->MainForm->Handle);
        BillFile->OpenBillFile(FileList->Strings[n],BillNameList->Strings[n]);
        SendMessage(BillFile->hWnd,MSG_THREAD_COMPLETED,0,0);
        BillFile->SetOwnerWnd(hOldOwnerWnd);
        RecordCount = BillFile->GetRecordCount();
        SendMessage(BillFile->hWnd,MSG_SET_PROCESS_MAX_COUNT,RecordCount,(LPARAM)BillFile->GetOriShortFileName().c_str());
        Item->SubItems->Strings[2] = RecordCount;

        for(k = 0;k < RecordCount;k++)
        {
          BillFile->ReadSingleRecord(k);
          //生成参数列表
          for(p = 0;p < MatchFieldList->Count;p++)
          {
            pDataTmp = (VARIANT *)ParamList->Items[p];
            pDataTmp->bstrVal = SysAllocString(WideString(((TBillField *)MatchFieldList->Objects[p])->GetFieldValue()).c_bstr());
            * (pData + p) = * pDataTmp;
          }
          //执行脚本
          try
          {
            Found = ScriptControl->Run(WideString("MatchBill").c_bstr(),&Params) == 1;
            ScriptSucceed = true;
          }
          catch(Exception & EE)
          {
            ScriptSucceed = false;
            if(ScriptControl->Error->Number != 0)
              ErrorMessage = "查询脚本错误:第 " + AnsiString(ScriptControl->Error->Line) \
                         + " 行，第 " + AnsiString(ScriptControl->Error->Column) \
                         + "列出现错误 " + AnsiString(ScriptControl->Error->Number) \
                       + " (" + AnsiString(ScriptControl->Error->Description) + ")";

          }
          //释放字符串占用的空间
          for(p = 0;p < MatchFieldList->Count;p++)
          {
            pDataTmp = (VARIANT *)ParamList->Items[p];
            SysFreeString(pDataTmp->bstrVal);
          }
          if(k > 1000 && k % 1000 == 0)
          {
            SendMessage(BillFile->hWnd,MSG_SET_PROCESS_POS,k,0);
          }
          if(!ScriptSucceed)
          {
            throw new Exception(ErrorMessage);
          }
          if(Found)
          {
            //写入到索引文件
            memset(&srdi,0,sizeof(srdi));
            srdi.RecordIndex = FoundCount;
            srdi.Position    = DataPosition;
            fwrite(&srdi,sizeof(srdi),1,TempFile3);
            //写入到数据文件
            memset(&srd,0,sizeof(srd));
          switch(BillFile->FileType)
          {
          case BIN_BILL_FORMAT:
            srd.DataLength = RecordLength;
            break;
          case TXT_BILL_FORMAT:
            srd.DataLength = BillFile->ri.RecordLength;
            break;
          }
          strcpy(&srd.OriFileName[0],BillFile->GetShortFileName().c_str());
          srd.OriDataIndex = BillFile->GetCurRecordPosition();
          fwrite(&srd,sizeof(srd),1,TempFile4);
          fwrite(BillFile->BillRecord->Buffer,srd.DataLength,1,TempFile4);
          ++FoundCount;
          ++CurFileFoundCount;
          DataPosition += srd.DataLength + sizeof(srd);
        }
        if(!Continue)
          break;
      }
      BillFile->CloseBillFile();
      SendMessage(BillFile->hWnd,MSG_SET_PROCESS_MAX_COUNT,0,NULL);
      fclose(TempFile3);
      fclose(TempFile4);
      //将数据写入到临时文件
      FileStream->LoadFromFile(TempFileName3);
      fwrite(FileStream->Memory,FileStream->Size,1,TempFile1);
      FileStream->Clear();
      FileStream->LoadFromFile(TempFileName4);
      fwrite(FileStream->Memory,FileStream->Size,1,TempFile2);
      FileStream->Clear();
      Item->ImageIndex = IMAGE_SUCCEED_ITEM;
      Item->SubItems->Strings[3] = CurFileFoundCount;
      SendMessage(hWnd,MSG_FILE_PROCESSED,n + 1,NULL);
      if(!Continue)
        break;
    }
    catch(Exception * E)
    {
      SendMessage(hWnd,MSG_FILE_PROCESSED,n + 1,NULL);
      BillFile->CloseBillFile();
      if(TempFile3 != NULL)
      {
        fclose(TempFile3);
        TempFile3 = NULL;
      }
      if(TempFile4 != NULL)
      {
        fclose(TempFile4);
        TempFile4 = NULL;
      }
      if(FileStream->Size > 0)
        FileStream->Clear();
      ErrorMessage = E->Message;
      Item->SubItems->Strings[3] = "查询失败:" + E->Message;
      Item->ImageIndex = IMAGE_FAILED_ITEM;
      ++ErrorCount;
      if(E->Message.Pos("查询脚本错误") > 0)
        break;
      if(!Continue)
        break;
      continue;
    }
  }
  }
  SafeArrayUnaccessData(Params);
  for(int n = 0;n < ParamList->Count;n++)
  {
    delete (VARIANT *)ParamList->Items[n];
  }
  delete ParamList;
  delete ScriptControl;
  CoUninitialize();
  if(TempFile3 != NULL)
  {
    fclose(TempFile3);
    TempFile3 = NULL;
  }
  if(TempFile4 != NULL)
  {
    fclose(TempFile4);
    TempFile4 = NULL;
  }
  if(FileStream != NULL)
  {
    delete FileStream;
    FileStream = NULL;
  }
  if(TempFile1 != NULL && TempFile2 != NULL && TempFile != NULL)
  {
    SendMessage(BillFile->hWnd,MSG_SAVE_TO_TEMPFILE,FoundCount,0);
    fclose(TempFile1);
    fclose(TempFile2);

    TMemoryStream * Stream = new TMemoryStream;
    Stream->LoadFromFile(TempFileName1);

    srfh.RecordCount     = FoundCount;
    srfh.IndexAreaLength = Stream->Size;
    fwrite(&srfh,sizeof(srfh),1,TempFile);
    fwrite(Stream->Memory,Stream->Size,1,TempFile);
    Stream->Clear();

    Stream->LoadFromFile(TempFileName2);
    fwrite(Stream->Memory,Stream->Size,1,TempFile);
    delete Stream;
    fclose(TempFile);
    TempFile = NULL;
    DeleteFile(TempFileName1);
    DeleteFile(TempFileName2);
    DeleteFile(TempFileName3);
    DeleteFile(TempFileName4);
  }
}
void __fastcall TBillSearchThread::Execute()
{
  if(UseScript) //按脚本查询
  {
    QueryByScript();
  }
  else
  {
    QueryByFields();
  }
}
//---------------------------------------------------------------------------
