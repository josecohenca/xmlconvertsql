//---------------------------------------------------------------------------

//#include <vcl.h>
#pragma hdrstop

#include "ThreadConvertForDB.h"
#include <ComCtrls.hpp>
#include "DBConfig.h"
#include <StrUtils.hpp>

#pragma package(smart_init)
//---------------------------------------------------------------------------
//本线程将话单文件转换为目标数据库要求的文本文件格式
//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TThreadConvertForDB::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TThreadConvertForDB::TThreadConvertForDB(bool CreateSuspended)
  : TThread(CreateSuspended)
{
  TransFieldValue = true;
  BillFileList    = NULL;
  DestFileFormat  = 0;
  DestDBType = 0;
  hWnd = NULL;
  ErrorMessage = "";
  BillConfig = NULL;
  BillName = "";
  CreateTableSQL = "";
  SQLCommandList = NULL;
  file = NULL;
  RetryCount = 0;
}
//---------------------------------------------------------------------------
void __fastcall TThreadConvertForDB::Execute()
{
  if(BillFileList == NULL)
  {
    SendMessage(BillFile->hWnd,MSG_THREAD_COMPLETED,(WPARAM)this,0);
    return;
  }
  SQLCommandList->Clear();
  switch(DestFileFormat)
  {
  case BIN_BILL_FORMAT:
    BillFile = new TBinBillFile(BillName,BillConfig,hWnd,TempDirectory);
    break;
  case TXT_BILL_FORMAT:
    BillFile = new TTxtBillFile(BillName,BillConfig,hWnd,TempDirectory);
    break;
  case SEARCH_RESULT_FORMAT:
    BillFile = new TSearchResultBillFile(BillFileList->Strings[0],BillConfig,hWnd,TempDirectory);
    break;
  }
  switch(DestDBType)
  {
  case DB_ORACLE:
    TotalRecordCount = ConvertToOracleFormat();
    break;
  case DB_SQLSERVER:
    TotalRecordCount = ConvertToSQLServerFormat();
    break;
  case DB_MYSQL:
    TotalRecordCount = ConvertToMySQL();
    break;
  default:
    ErrorMessage = "目标数据库的类型不正确.";
    break;
  }
  delete BillFile;
  if(hWnd != NULL)
    SendMessage(hWnd,MSG_THREAD_COMPLETED,(WPARAM)this,0);
}
int __fastcall TThreadConvertForDB::ConvertToMySQL()
{
  file = fopen(TempFileName.c_str(),"w");
  if(file == NULL)
  {
    ErrorMessage = "无法创建临时文件" + TempFileName + ".";
    return -1;
  }
  TBillField * BillField;
  CreateTableSQL = "CREATE TABLE " + DestTableName + "(\n";
  AnsiString FieldDataTypeStr = ""; 
  for(int n = 0;n < BillFile->FieldCount;n++)
  {
    BillField = BillFile->GetBillField(n);
    if(!BillField->ImportToDB)
      continue;
    CreateTableSQL += BillField->FieldName + " ";
    if(BillField->CreateIndex)
    {
      SQLCommandList->Add(
        "CREATE INDEX IDX_" + DestTableName + "_" + IntToStr(BillField->FieldIndex) + " ON " + DestTableName + " (" +
        BillField->FieldName + ")");
    }
    if((TransFieldValue && BillField->GetValueTransCount() > 0)||BillField->DisplayFormat == FIELD_DISPLAY_AS_BITS)
    {
      FieldDataTypeStr = "VARCHAR(" + IntToStr(BillField->ExportLength) + ")";
    }
    else
    {
      switch(BillField->SortType)
      {
      case SORT_AS_DATETIME:
        FieldDataTypeStr = "DATETIME";
        break;
      case SORT_AS_NUMBER:
        FieldDataTypeStr = "BIGINT";
        break;
      case SORT_AS_CURRENCY:
        FieldDataTypeStr = "NUMERIC(10,2)";
        break;        
      default:
        if(BillField->GetValueTransCount() > 0 && !TransFieldValue)
        {
          FieldDataTypeStr = "BIGINT";
        }
        else
        {
          FieldDataTypeStr = "VARCHAR(" + IntToStr(BillField->ExportLength) + ")";
        }
        break;
      }
    }
    CreateTableSQL += FieldDataTypeStr + ",";
  }
  CreateTableSQL = CreateTableSQL.SubString(1,CreateTableSQL.Length() - 1) + "\n)";
  int TotalCount = ConvertCSVFile(file);
  fclose(file);
  file = NULL;
  return TotalCount;
}
int __fastcall TThreadConvertForDB::ConvertToSQLServerFormat()
{
  TBillField * BillField;
  //AnsiString FormatString = "8.0\r\n";
  CreateTableSQL = "CREATE TABLE [dbo].[" + DestTableName + "] (\n";
  AnsiString FieldDataTypeStr = "";
  //FormatString += IntToStr(BillFile->FieldCount) + "\r\n";
  TStringList * FormatStrings = new TStringList;
  FormatStrings->Add("");
  int Index = 1; 
  for(int n = 0;n < BillFile->FieldCount;n++)
  {
    BillField = BillFile->GetBillField(n);
    if(!BillField->ImportToDB)
      continue;
    CreateTableSQL += "[" + BillField->FieldName + "] ";
    if(BillField->CreateIndex)
    {
      SQLCommandList->Add(
        "CREATE INDEX IDX_" + DestTableName + "_" + IntToStr(BillField->FieldIndex) + " ON " + DestTableName + " (" +
        BillField->FieldName + ")");
    }
    if((TransFieldValue && BillField->GetValueTransCount() > 0)||BillField->DisplayFormat == FIELD_DISPLAY_AS_BITS)
    {
      FieldDataTypeStr = "[VARCHAR](" + IntToStr(BillField->ExportLength) + ")";
    }
    else
    {
      switch(BillField->SortType)
      {
      case SORT_AS_DATETIME:
        FieldDataTypeStr = "[DATETIME]";
        break;
      case SORT_AS_NUMBER:
        FieldDataTypeStr = "[INT]";
        break;
      case SORT_AS_CURRENCY:
        FieldDataTypeStr = "[NUMERIC](10, 2)";
        break;
      default:
        if(BillField->GetValueTransCount() > 0 && !TransFieldValue)
        {
          FieldDataTypeStr = "[INT]";
        }
        else
        {
          FieldDataTypeStr = "[VARCHAR](" + IntToStr(BillField->ExportLength) + ")";
        }
        break;
      }
    }
    CreateTableSQL += FieldDataTypeStr + ",";
    FormatStrings->Add(IntToStr(Index) + "\tSQLCHAR\t0\t1024\t\",\"\t" + IntToStr(Index) + "\t" + BillField->FieldName + "\tChinese_PRC_CI_AS");
    ++Index;
    /*if(n < BillFile->FieldCount - 1)
    {
      FormatString += IntToStr(n + 1) + "\tSQLCHAR\t0\t1024\t\",\"\t" + IntToStr(n + 1) + "\t" + BillField->FieldName + "\tChinese_PRC_CI_AS\r\n";
      CreateTableSQL += ",";
    }
    else
    {
      FormatString += IntToStr(n + 1) + "\tSQLCHAR\t0\t1024\t\"\\r\\n\"\t" + IntToStr(n + 1) + "\t" + BillField->FieldName + "\tChinese_PRC_CI_AS\n";
    }*/
  }
  FormatStrings->Strings[0] = "8.0\r\n" + IntToStr(Index - 1);
  if(FormatStrings->Strings[FormatStrings->Count - 1].Pos("\\r\\n") < 1)
  {
    FormatStrings->Strings[FormatStrings->Count - 1] =
      AnsiReplaceText(FormatStrings->Strings[FormatStrings->Count - 1],"\",\"","\"\\r\\n\"");  
  }
  CreateTableSQL = CreateTableSQL.SubString(1,CreateTableSQL.Length() - 1) + "\n)";
  //写格式文件
  file = fopen((ExtractFilePath(TempFileName) + DestTableName + ".fmt").c_str(),"w");
  if(file == NULL)
  {
    ErrorMessage = "创建BCP格式文件失败.";
    delete FormatStrings;
    return 0;
  }
  fprintf(file,"%s\n",FormatStrings->Text);//FormatString);
  fclose(file);
  delete FormatStrings;

  file = fopen(TempFileName.c_str(),"w");
  if(file == NULL)
  {
    ErrorMessage = "无法创建临时文件" + TempFileName + ".";
    return -1;
  }
  int TotalCount = ConvertCSVFile(file);
  fclose(file);
  file = NULL;
  return TotalCount;
}
//---------------------------------------------------------------------------
int __fastcall TThreadConvertForDB::ConvertToOracleFormat()
{
  file = fopen(TempFileName.c_str(),"w");
  if(file == NULL)
  {
    ErrorMessage = "无法创建临时文件" + TempFileName + ".";
    return -1;
  }
  //写文件头
  fprintf(file,"Load DATA\n"\
               "INFILE *\n"\
               "INTO TABLE %s\n",DestTableName);
  fprintf(file,"APPEND\n");
  fprintf(file,"FIELDS TERMINATED BY ','\n"\
               "TRAILING NULLCOLS\n"\
               "(\n");
  TBillField * BillField;
  AnsiString TableField = "";
  CreateTableSQL = "CREATE TABLE " + DestTableName + "(\n";
  AnsiString FieldDataTypeStr = "";
  for(int n = 0;n < BillFile->FieldCount;n++)
  {
    BillField = BillFile->GetBillField(n);
    if(!BillField->ImportToDB)
      continue;
    TableField += BillField->FieldName;
    CreateTableSQL += BillField->FieldName + " ";
    if(BillField->CreateIndex)
    {
      SQLCommandList->Add(
        "CREATE INDEX IDX_" + DestTableName + "_" + IntToStr(BillField->FieldIndex) + " ON " + DestTableName + " (" +
        BillField->FieldName + ")");
    }
    if((TransFieldValue && BillField->GetValueTransCount() > 0)||BillField->DisplayFormat == FIELD_DISPLAY_AS_BITS)
    {
      FieldDataTypeStr = "VARCHAR2(" + IntToStr(BillField->ExportLength) + ")";
    }
    else
    {
      switch(BillField->SortType)
      {
      case SORT_AS_DATETIME:
        FieldDataTypeStr = "DATE";
        TableField += " DATE \"yyyy-mm-dd hh24:mi:ss\"";
        break;
      case SORT_AS_NUMBER:
        FieldDataTypeStr = "NUMBER(10)";
        break;
      case SORT_AS_CURRENCY:
        FieldDataTypeStr = "NUMBER(10,2)";
        //FieldDataTypeStr = "VARCHAR(20)";
        //TableField += " CHAR \"to_number(:" + BillField->FieldName + ",'9999D0')\"";// NULLIF " + BillField->FieldName + "=BLANKS";
        break;
      default:
        if(BillField->GetValueTransCount() > 0 && !TransFieldValue)
        {
          FieldDataTypeStr = "NUMBER(10)";
        }
        else
        {
          FieldDataTypeStr = "VARCHAR2(" + IntToStr(BillField->ExportLength) + ")";
        }
        //TableField += " VARCHAR2(" + IntToStr(BillField->ExportLength) + ")";
        break;
      }
    }
    SQLCommandList->Add("COMMENT ON COLUMN " + DestTableName + "." + BillField->FieldName +
      " IS '" + BillField->Label + "'");
    CreateTableSQL += FieldDataTypeStr + ",";
    TableField += ",";
  }
  CreateTableSQL = CreateTableSQL.SubString(1,CreateTableSQL.Length() - 1) + "\n)";
  fprintf(file,"%s\n)\nBEGINDATA\n",TableField.SubString(1,TableField.Length() - 1));
  int TotalCount = ConvertCSVFile(file);
  fclose(file);
  file = NULL;
  return TotalCount;
}
//---------------------------------------------------------------------------
int __fastcall TThreadConvertForDB::ConvertCSVFile(FILE * file)
{
  int Result = 0;
  int pos,RecCount;
  HANDLE hOldOwnerWnd;
  hOldOwnerWnd = BillFile->hWnd;
  AnsiString FileName;
  for(int n = 0;n < BillFileList->Count;n++)
  {
    FileName = BillFileList->Strings[n];
    if(FileName.SubString(1,6).LowerCase() == "ftp://")
    {
      pos = StrRScan(FileName.c_str(),"|",-1);
      if(pos < 0)
        throw new Exception(FileName + " 格式不正确");
      FileName = FileName.SubString(pos + 2,FileName.Length() - pos);
    }
    else
    {
      FileName = ExtractFileName(FileName);
    }
    try
    {
      if(SlentMode)
      {
        printf("正在转换文件%s",FileName);
      }
      pos = MessageTextList->Add("正在转换文件" + FileName);
      if(InCmdMode)
      {
          BillFile->OpenBillFile(BillFileList->Strings[n],BillNameList->Strings[n]);
      }
      else
      {
        BillFile->SetOwnerWnd(Application->MainForm->Handle);
        BillFile->OpenBillFile(BillFileList->Strings[n],BillNameList->Strings[n]);
        SendMessage(BillFile->hWnd,MSG_THREAD_COMPLETED,0,0);
        BillFile->SetOwnerWnd(hOldOwnerWnd);
      }
      RecCount = BillFile->ConvertToDBFile(file,",",TransFieldValue);
      Result += RecCount;
      BillFile->CloseBillFile();
      if(SlentMode)
      {
        printf("  转换完成,共转换了%d条记录,跳过了%d条记录\n",RecCount,BillFile->SkipRecordCount);
      }
      MessageTextList->Strings[pos] = MessageTextList->Strings[pos] + " 转换完成,共转换了" + IntToStr(RecCount) + "条记录," +
        "跳过了" + IntToStr(BillFile->SkipRecordCount)  + "条记录";
      SaveLog(MessageTextList->Strings[pos].c_str());
      if(BillFile->AbortOperation)
      {
        throw new Exception("操作被用户中止");
      }
    }
    catch(Exception * E)
    {
      SendMessage(BillFile->hWnd,MSG_THREAD_COMPLETED,0,0);
      ErrorMessage = E->Message;
      BillFile->CloseBillFile();
      break;
    }
    SendMessage(hWnd,MSG_SET_PROCESS_POS,Result,0);
  }
  return Result;
}
