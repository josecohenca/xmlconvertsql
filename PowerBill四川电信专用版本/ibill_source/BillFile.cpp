

#include "BillFile.h"
#include <sysopen.h>


int RetryCount;

/*************************************************************************
*                                 话单文件基类
*
*
*************************************************************************/
//BillName:要初始化的话单类型名称.如果为空,则初始化一个StdBill(标准话单类型).
TBillFile::TBillFile(AnsiString ABillName,TBillConfig * ABillConfig,HANDLE hParentWnd,AnsiString ATempDirectory)
{
  BillConfig = ABillConfig;
  Buffer            = NULL;
  BillRecord        = NULL;
  AnswerTimeField   = NULL;
  EndTimeField      = NULL;
  FieldList         = NULL;
  FilterList        = NULL;
  FileLoaded        = false;
  AbortOperation    = false;
  IsBlockBinFile    = false;
  FTPFile           = NULL;
  TempDirectory     = ATempDirectory;
  hWnd              = hParentWnd;
  FieldList         = new TStringList;
  SkipRecordCount   = 0;
  if(ABillName != "")
  {
    BillName = ABillName;
    FileHeadLength    = BillConfig->GetFileHeadLength(BillName);
    FileTrailLength   = BillConfig->GetFileTrailLength(BillName);
    FileBlockLength   = BillConfig->GetFileBlockLength(BillName);
    RecordLength      = BillConfig->GetFileRecordLength(BillName);
    if(FileBlockLength == 0)
        FileBlockLength = 1024;
    FileType  = BillConfig->GetFileFormatType(BillName);
    Separator = BillConfig->GetFieldSeparator(BillName);

    if(RecordLength == 0)
    {
      if(FileType == BIN_BILL_FORMAT)
        throw new Exception("没有为话单类型" + BillName + "定义记录长度!");
      else
        RecordLength = 2048;
    }
    if(FileBlockLength == 0)
      FileBlockLength = RecordLength;
    _di_IXMLNode RecordNode = BillConfig->GetRecordNode(BillName);
    _di_IXMLNode FieldNode;
    _di_IXMLNode AttributeNode;
    TBillField * BillField;

    for(int n = 0;n < RecordNode->ChildNodes->Count;n++)
    {
      FieldNode = RecordNode->ChildNodes->Nodes[n];
      if(BillConfig->GetFieldAttributeValue(FieldNode,"Disabled","0") != "0")
        continue;
      BillField = new TBillField(this,FieldNode);
      if(AnswerTimeField == NULL && BillField->FieldType == FIELDTYPE_STARTTIME)
        AnswerTimeField = BillField;
      else if(EndTimeField == NULL && BillField->FieldType == FIELDTYPE_ENDTIME)
        EndTimeField = BillField;
      BillField->FieldIndex = FieldList->AddObject(BillField->FieldName,(TObject *)BillField);
    }
    FieldCount = FieldList->Count;
    FilterList = new TList;
    BillConfig->GetRecordFilter(BillName,FilterList);
    BillRecord = new TBillRecord(this);
  }
}
TBillFile::~TBillFile()
{
  if(FilterList != NULL)
  {
    for(int n = 0;n < FilterList->Count;n++)
    {
      delete ((RecordFilter *)FilterList->Items[n]);
    }
    delete FilterList;
    FilterList = NULL;
  }
  if(FieldList != NULL)
  {
    for(int n = 0;n < FieldList->Count;n++)
    {
      delete (TBillField *)FieldList->Objects[n];
    }
    delete FieldList;
    FieldList = NULL;
  }
  if(Buffer != NULL)
  {
    delete[] Buffer;
    Buffer = NULL;
  }
  if(BillRecord != NULL)
  {
    delete BillRecord;
    BillRecord = NULL;
  }
  if(FTPFile != NULL)
  {
    delete FTPFile;
    FTPFile = NULL;
  }
}
void TBillFile::SetOwnerWnd(HANDLE hNewWnd)
{
  hWnd = hNewWnd;
  if(FTPFile != NULL)
  {
    FTPFile->hWnd = hNewWnd;
  }
}
void TBillFile::TermCurOperation()
{
  AbortOperation = true;
  if(FTPFile != NULL)
  {
    try
    {
      FTPFile->IdFTP->Abort();
      FTPFile->IdFTP->Quit();
      DeleteFile(FTPFile->CurTempFileName);
    }
    catch(...)
    {
    }
  }
}
AnsiString __fastcall TBillFile::TransFileName(AnsiString AFileName)
{
  if(AFileName.SubString(1,6) == "ftp://")
  {
    int pos = StrRScan(AFileName.c_str(),"|");
    if(pos < 0)
      throw new Exception("FTP连接字符串不正确");
    AnsiString ServerString = AFileName.SubString(1,pos);
    FileName      = AFileName.SubString(pos + 2,AFileName.Length() - pos - 1);
    ShortFileName = FileName;

    if(FTPFile == NULL)
      FTPFile = new TFTPFile(hWnd,TempDirectory,FTPTimeout);
    TempFileName  = FTPFile->GetFile(FileName,ServerString);
    if(TempFileName == "")
      throw new Exception(FTPFile->LastErrorMessage);
    FileName = "ftp://" + FTPFile->GetServerName(ServerString) + FTPFile->GetFilePath(ServerString) + FileName;
    return TempFileName;
  }
  else
  {
    FileName      = AFileName;
    ShortFileName = ExtractFileName(FileName);
    return AFileName;
  }
}
TBillField * TBillFile::GetBillField(int Index)
{
  if(FieldList == NULL || Index > (FieldList->Count - 1))
    return NULL;
  else
    return (TBillField *)FieldList->Objects[Index];
}
bool TBillFile::OpenBillFile(AnsiString AFileName,AnsiString ABillName)
{
  return false;
}
TBillField * TBillFile::GetBillField(AnsiString FieldName)
{
  if(FieldList == NULL)
    return NULL;
  int Index = FieldList->IndexOf(FieldName);
  if(Index < 0)
    return NULL;
  return (TBillField *)FieldList->Objects[Index];
}

int TBillFile::CalcDuration()
{
  if(AnswerTimeField!=NULL && EndTimeField != NULL)
  {
    try
    {
      return RoundTo(SecondSpan(StrToDateTime(EndTimeField->GetFieldValue()),StrToDateTime(AnswerTimeField->GetFieldValue())),0);
    }
    catch(...)
    {
      return -1;
    }
  }
  else
    return -1;
}
bool TBillFile::ReadSingleRecord(int RecordIndex)
{
  return false;
}
int TBillFile::GetRecordCount()
{
  return 0;
}
int TBillFile::ConvertToDBFile(FILE * file,AnsiString FieldTermChar, bool TransFieldValue)
{
  if(!FileLoaded)
    return 0;
  int n,k;
  AnsiString FieldDataTypeStr;
  TList * List = new TList;
  TBillField * BillField;
  for(n = 0;n < FieldCount;n++)
  {
    BillField = (TBillField * )FieldList->Objects[n];
    if(BillField->ImportToDB)
    {
      List->Add((TObject *)BillField);  
    }  
  }
  int Count = List->Count;
  SendMessage(hWnd,MSG_SET_PROCESS_MAX_COUNT,GetRecordCount(),(int)ShortFileName.c_str());
  for(n = 0;n < GetRecordCount();n++)
  {
    FieldDataTypeStr = "";
    ReadSingleRecord(n);
    for(k = 0;k < Count - 1;k++)
    {
      FieldDataTypeStr += ((TBillField * )List->Items[k])->GetFieldValue(TransFieldValue) + FieldTermChar;
    }
    FieldDataTypeStr += ((TBillField * )List->Items[k])->GetFieldValue(TransFieldValue);
    if(FieldDataTypeStr.Pos("\n") > 0)
      fprintf(file,"%s",FieldDataTypeStr);
    else
      fprintf(file,"%s\n",FieldDataTypeStr);
    if(n > 1000 && n % 1000 == 0)
      SendMessage(hWnd,MSG_SET_PROCESS_POS,n,0);
    if(AbortOperation)
      break;
  }
  SendMessage(hWnd,MSG_SET_PROCESS_POS,n,0);
  delete List;
  return n;
}
void TBillFile::EndOfDBFFile(FILE * file,int RecordCount)
{
  fseek(file,4,SEEK_SET);
  fwrite(&RecordCount,4,1,file);
}
void TBillFile::WriteDBFHead(FILE * file,TList * DBFFieldList,TList * BillFieldList)
{
  DBFHead    Head;
  memset(&Head,0,32);
  Head.FileType = 0x03;       //文件版本:FoxBASE+/dBASE III PLUS，无备注
  AnsiString str = FormatDateTime("yyyymmdd",Now());
  char ch[2] = {0};
  memcpy(&ch[0],str.SubString(3,2).c_str(),2);
  ch[0] = ch[0] - 48;
  ch[1] = ch[1] - 48;
  Head.LastModify[0] = (ch[0] << 4) + ch[1];

  memcpy(&ch[0],str.SubString(5,2).c_str(),2);
  ch[0] = ch[0] - 48;
  ch[1] = ch[1] - 48;
  Head.LastModify[1] = (ch[0] << 4) + ch[1];

  memcpy(&ch[0],str.SubString(7,2).c_str(),2);
  ch[0] = ch[0] - 48;
  ch[1] = ch[1] - 48;
  Head.LastModify[2] = (ch[0] << 4) + ch[1];

  Head.RecordCount   = 0x00;//GetRecordCount();
  Head.TableFlag     = 0x00;
  Head.CodePage      = 0x7A;

  DBFField * Field;
  //生成字段结构
  int Offset = 1;
  TBillField * BillField;
  for(int n = 0;n < FieldList->Count;n++)
  {
    BillField = (TBillField * )FieldList->Objects[n];
    if(!BillField->ImportToDB)
      continue;
    Field = new DBFField;
    memset(Field,0x00,32);
    Field->Offset          = Offset;
    Field->FieldFlag       = 0x02;
    Field->ProductionIndex = 0x01;
    Field->Decimal         = 0x00;
    switch(BillField->SortType)
    {
    case SORT_AS_NUMBER:
      Field->FieldType = 'N';
      Field->Decimal   = 0;
      break;
    case SORT_AS_CURRENCY:
      Field->FieldType = 'N';
      Field->Decimal   = 2;
      break;
    default:
      Field->FieldType = 'C';
      break;
    }
    Field->Length    = BillField->ExportLength;
    strncpy(&Field->FieldName[0],BillField->FieldName.c_str(),11);
    Offset += Field->Length;
    DBFFieldList->Add((TObject *)Field);
    BillFieldList->Add((TObject *)BillField);
  }
  Head.RecordLength  = Offset;
  Head.HeadLength    = 32 * (DBFFieldList->Count + 1) + 1;
  //写文件头
  fwrite(&Head,32,1,file);
  //写字段定义
  for(int n = 0;n < DBFFieldList->Count;n++)
  {
    Field = (DBFField *)DBFFieldList->Items[n];
    fwrite((void *)Field,32,1,file);
  }
  fprintf(file,"%c",0x0d);
}
void TBillFile::ExportDataToDBFFile(FILE * file,TList * DBFFieldList,TList * BillFieldList)
{
  //写字段内容
  int k;
  int len;
  char ch[256];
  char * pch;
  char * pch0 = &ch[0];
  char * pch1 = &ch[1];
  int ExportFieldCount = DBFFieldList->Count;
  AnsiString str;
  for(int n = 0;n < GetRecordCount();n++)
  {
    if(AbortOperation)
      return;
    ReadSingleRecord(n);
    pch = pch1;
    for(k = 0;k < ExportFieldCount;k++)
    {
      memset(&ch[0],0x20,256);
      len = ((DBFField * )DBFFieldList->Items[k])->Length;
      str = ((TBillField * )BillFieldList->Items[k])->GetFieldValue();
      if(str.Length() > len)
        memcpy(pch,str.c_str(),len);
      else
        memcpy(pch,str.c_str(),str.Length());
      if(k == 0)
      {
        pch = pch0;
        fwrite(&ch[0],len + 1,1,file);
      }
      else
        fwrite(&ch[0],len,1,file);
    }
    if(n > 1000 && n % 1000 == 0)
    {
      SendMessage(hWnd,MSG_SET_PROCESS_POS,n,0);
    }
  }
}
void TBillFile::ExportDataToTxtFile(FILE * file,AnsiString Separator)
{
  int ExportFieldCount = FieldList->Count;
  int k;
  int n;
  for(n = 0;n < GetRecordCount();n++)
  {
    if(AbortOperation)
      return;
    ReadSingleRecord(n);
    for(k = 0;k < ExportFieldCount - 1;k++)
    {
      fprintf(file,"%s%s",((TBillField * )FieldList->Objects[k])->GetFieldValue().c_str(),Separator);
    }
    fprintf(file,"%s\r\n",((TBillField * )FieldList->Objects[k])->GetFieldValue());
    if(n > 1000 && n % 1000 == 0)
    {
      SendMessage(hWnd,MSG_SET_PROCESS_POS,n,0);
    }
  }
  SendMessage(hWnd,MSG_SET_PROCESS_POS,n,0);  
}
void TBillFile::ExportDataToOriFile(FILE * file)
{
  long pos;
  char * pch;
  if(IsBlockBinFile)
  {
    //写块头
    pch = new char[BlockHeadLength];
    memset(pch,0xff,BlockHeadLength);
    fwrite(pch,BlockHeadLength,1,file);
    delete[] pch;
  }
  int len;
  int WriteCount = 0;
  for(int n = 0;n < GetRecordCount();n++)
  {
    ReadSingleRecord(n);
    SaveRecordToFile(file);
    WriteCount += RecordLength;
    if(IsBlockBinFile)
    {
      if(WriteCount + RecordLength >= FileBlockLength)
      {
        len = FileBlockLength - WriteCount;
        if(len > 0)
        {
          pch = new char[len];
          memset(pch,0xff,len);
          fwrite(pch,len,1,file);
          delete[] pch;
        }
        WriteCount = 0;
      }
    }
  }
  if(IsBlockBinFile)
  {
    //填充剩余的空间
    pos = ftell(file);
    len = (((pos  - FileHeadLength + FileBlockLength - 1)  / FileBlockLength) * FileBlockLength + FileHeadLength) - pos;
    pch = new char[len];
    memset(pch,0xff,len);
    fwrite(pch,len,1,file);
    delete[] pch;
  }
}
void * TBillFile::GetRecordBuffer()
{
  return BillRecord->Buffer;
}
/*************************************************************************
*                                 二进制话单文件类
*
*
*************************************************************************/
TBinBillFile::TBinBillFile(AnsiString ABillName,TBillConfig * ABillConfig,HANDLE hParentWnd,AnsiString ATempDirectory)
  : TBillFile(ABillName,ABillConfig,hParentWnd,ATempDirectory)
{
  BlockHeadLength  = BillConfig->GetBlockHeadLength(ABillName);
  BlockTrailLength = BillConfig->GetBlockTrailLength(ABillName);
  FileStream       = NULL;
  Buffer           = new char[RecordLength];
  IsBlockBinFile   = BillConfig->IsBlockFile(BillName);//FileBlockLength != RecordLength;//BlockHeadLength != 0 || BlockTrailLength != 0;
  IndexStream      = NULL;
  EmptyRecord      = NULL;
  memset(&ri,0,sizeof(ri));
}
TBinBillFile::~TBinBillFile()
{
  if(FileLoaded)
    CloseBillFile();
  if(FileStream != NULL)
  {
    delete FileStream;
    FileStream = NULL;
  }
  if(IndexStream != NULL)
  {
    delete IndexStream;
    IndexStream = NULL;
  }
  if(EmptyRecord != NULL)
  {
    delete[] EmptyRecord;
    EmptyRecord = NULL;
  }
}
bool TBinBillFile::OpenBillFile(AnsiString AFileName,AnsiString ABillName)
{
  AbortOperation = false;
  if(FileStream == NULL)
  {
    FileStream = new TMemoryStream();
  }
  FileLoaded = false;
  if(IsBlockBinFile)
  {
    if(IndexStream == NULL)
      IndexStream = new TMemoryStream;
    else
      IndexStream->Clear();
    for(int n = 0;n < FilterList->Count;n++)
    {
      ((RecordFilter *)FilterList->Items[n])->SkipCount = 0;
    }
  }
  SkipRecordCount = 0;
  int ErrCount = 0;
  while(true)
  {
    try
    {
      FileStream->LoadFromFile(TransFileName(AFileName));
      break;
    }
    catch(Exception * E)
    {
      if(AbortOperation)
        break;
      ++ErrCount;
      if(hWnd != NULL)
        SendMessage(hWnd,MSG_OPEN_FILE_ERROR,(int)ShortFileName.c_str(),(int)(E->Message + " 重试" + IntToStr(ErrCount)).c_str());
      if((ErrCount > RetryCount && RetryCount > 0) || ErrCount > 1000)
      {
        throw new Exception(E->Message);
      }
    }
  }
  if(AbortOperation)
  {
    FileStream->Clear();
    throw new Exception("操作被用户取消");
  }
  try
  {
    
    if(IsBlockBinFile)    //生成索引 /****如果一条记录中的所有位都是FF,则认为该记录是无效的(转换时填入的0xFF)***/
    {
      if(EmptyRecord == NULL)
      {
        EmptyRecord = new char[RecordLength];
        memset(EmptyRecord,0xff,RecordLength);
      }
      FileStream->Position = FileHeadLength + BlockHeadLength;
      int ReadLength = BlockHeadLength;
      BinRecordIndex ri;
      RecordFilter * prf;
      bool Read;
      if(hWnd != NULL)
      {
        SendMessage(hWnd,MSG_SET_PROCESS_MAX_COUNT,FileStream->Size,(int)ShortFileName.c_str());
      }
      while(FileStream->Read(Buffer,RecordLength) == RecordLength)
      {
        ReadLength += RecordLength;
        if(memcmp(EmptyRecord,Buffer,RecordLength) != 0)  //如果不是空记录
        {
          Read = true;
          //检查读取控制
          for(int n = 0;n < FilterList->Count;n++)
          {
            prf = ((RecordFilter *)FilterList->Items[n]);
            if(prf->Pos >= RecordLength)
              continue;
            unsigned char * pch = new unsigned char[prf->Width];
            memcpy(pch,(Buffer + prf->Pos),prf->Width);
            if((int)*(pch) == prf->Is)
            {
              switch(prf->Action)
              {
              case RECORD_FILTER_SKIP:  //跳过
                FileStream->Position += prf->Length - RecordLength;
                ReadLength += prf->Length - RecordLength;
                Read = false;
                ++SkipRecordCount;
                ++prf->SkipCount;
                break;
              /*case RECORD_FILTER_READ:  //读取
                FileStream->Position += prf->Length - RecordLength;
                ReadLength += prf->Length - RecordLength;
                Read = false;
                break;*/
              default:
                break;
              }
            }
            delete[] pch;
          }
          if(Read)
          {
            memset(&ri,0,sizeof(ri));
            ri.RecordPosition = FileStream->Position - RecordLength;
            ri.RecordLength   = RecordLength;
            IndexStream->Write(&ri,sizeof(ri));
          }
        }
        if(ReadLength + RecordLength > FileBlockLength)
        {
          FileStream->Position += (FileBlockLength - ReadLength) + BlockHeadLength;
          ReadLength = BlockHeadLength;
        }
        if(hWnd != NULL && FileStream->Position > 1000 && FileStream->Position % 1000 == 0)
        {
          SendMessage(hWnd,MSG_SET_PROCESS_POS,FileStream->Position,0);
        }
        if(AbortOperation)
          break;
      }
    }
    //SendMessage(hWnd,MSG_SET_PROCESS_POS,FileStream->Position,0);
  }
  catch(Exception * E)
  {
    throw new Exception(E->Message);
  }
  FileLoaded = true;
  return true;
}
void TBinBillFile::CloseBillFile()
{
  if(FileStream == NULL || !FileLoaded)
    return;
  FileStream->Clear();
  if(IndexStream != NULL)
    IndexStream->Clear();
  if(EmptyRecord != NULL)
  {
    delete[] EmptyRecord;
    EmptyRecord = NULL;
  }
  if(TempFileName != "")
    DeleteFile(TempFileName);
  TempFileName  = "";
  FileName      = "";
  ShortFileName = "";
  FileLoaded = false;
}
//设置BillRecord的当前缓冲区.在二进制文件中,Length参数无效,始终为0
int TBinBillFile::SetRecordBuffer(const char * chSrc,int Length)
{
  return BillRecord->SetBinBuffer(chSrc);
}
int TBinBillFile::ManualSetRecordBuffer(const char * ABuffer)
{
  if(IsBlockBinFile)
  {
    BinRecordIndex * pbri = (BinRecordIndex *)ABuffer;
    FileStream->Position = pbri->RecordPosition;
    if(FileStream->Read(Buffer,RecordLength) != RecordLength)
      return 0;
    return SetRecordBuffer(Buffer,0);
  }
  else
  {
    return SetRecordBuffer(ABuffer,0);
  }
}
//返回当前记录在文件中的基于0的起始位置
int TBinBillFile::GetCurRecordPosition()
{
  if(IsBlockBinFile)
  {
    return ri.RecordPosition;
  }
  else
  {
    return FileStream->Position - RecordLength;
  }
}
int TBinBillFile::GetCurRecordLength()
{
  return RecordLength;
}
//RecordIndex:基于0的记录索引号
bool TBinBillFile::ReadSingleRecord(int RecordIndex)
{
  int pos;
  if(IsBlockBinFile)
  {
    memset(&ri,0,sizeof(ri));
    IndexStream->Position = RecordIndex * sizeof(ri);
    if(IndexStream->Read(&ri,sizeof(ri)) != sizeof(ri))
      return false;
    pos = ri.RecordPosition;
  }
  else
  {
    pos = FileHeadLength + RecordLength * RecordIndex;
    if(pos >= FileStream->Size - FileTrailLength)
      throw new Exception("文件指针试图定位到超出文件大小的位置!");
  }
  FileStream->Position = pos;
  if(FileStream->Read(Buffer,RecordLength) == RecordLength)
  {
    SetRecordBuffer((char *)Buffer,0);
    return true;
  }
  else
  {
    return false;
  }
}
int TBinBillFile::GetRecordCount()
{
  if(FileStream->Size == 0)
    return 0;
  else if(IsBlockBinFile)
  {
    if(IndexStream == NULL)
      return 0;
    else
      return IndexStream->Size / sizeof(RecordIndex);
  }
  else
    return (FileStream->Size - FileHeadLength - FileTrailLength) / RecordLength;
}
void TBinBillFile::SortFile(TBillField * BillField,int Direction)
{
  if(!FileLoaded)
    return;
  CompareField = BillField;
  CompareField->CompareDirection = Direction;
  CompareField->CompareLen = CompareField->FieldLength;
  if(IsBlockBinFile)
  {
    qsort((void *)((char *)IndexStream->Memory),GetRecordCount(),sizeof(BinRecordIndex),BillField->CompareFieldData);
  }
  else
  {
    qsort((void *)((char *)FileStream->Memory + FileHeadLength),GetRecordCount(),RecordLength,BillField->CompareFieldData);
  }
}
void TBinBillFile::SaveRecordToFile(FILE * file)
{
  fwrite(BillRecord->Buffer,RecordLength,1,file);
}
void TBinBillFile::SaveFileHeadToFile(FILE * file)
{
  if(FileHeadLength < 1)
    return;
  char * pch = new char[FileHeadLength];
  memset(pch,0xff,FileHeadLength);
  fwrite(pch,FileHeadLength,1,file);
  delete[] pch;
}
void TBinBillFile::SaveFileTrailToFile(FILE * file)
{
  if(FileTrailLength < 1)
    return;
  char * pch = new char[FileTrailLength];
  memset(pch,0xff,FileTrailLength);
  fwrite(pch,FileTrailLength,1,file);
  delete[] pch;
}
/*----------------------------------------------------------------------------
*                          文本文件话单记录操作类
*
*----------------------------------------------------------------------------*/
TTxtBillFile::TTxtBillFile(AnsiString ABillName,TBillConfig * ABillConfig,HANDLE hParentWnd,AnsiString ATempDirectory)
  : TBillFile(ABillName,ABillConfig,hParentWnd,ATempDirectory)
{
  FileStream  = new TMemoryStream;
  IndexStream = new TMemoryStream;
}
TTxtBillFile::~TTxtBillFile()
{
  if(FileLoaded)
    CloseBillFile();
  if(FileStream != NULL)
  {
    delete FileStream;
    FileStream = NULL;
  }
}
int TTxtBillFile::GetRecordCount()
{
  if(IndexStream == NULL)
    return 0;
  else
    return IndexStream->Size / sizeof(TxtFileRecord);
}
void TTxtBillFile::CloseBillFile()
{
  FileStream->Clear();
  IndexStream->Clear();
  if(Buffer != NULL)
  {
    delete[] Buffer;
    Buffer = NULL;
  }
  if(TempFileName != "")
    DeleteFile(TempFileName);
  TempFileName  = "";
  FileName      = "";
  ShortFileName = "";
  FileLoaded = false;
}
bool TTxtBillFile::OpenBillFile(AnsiString AFileName,AnsiString ABillName)
{
  AbortOperation = false;
  CloseBillFile();
  FILE * file;// = NULL;
  int ErrCount = 0;
  while(true)
  {
    try
    {
      file = fopen(TransFileName(AFileName).c_str(),"r");
      if(file == NULL)
        throw new Exception("不能打开文件" + AFileName);
      break;
    }
    catch(Exception * E)
    {
      if(AbortOperation)
        break;
      ++ErrCount;
      if(hWnd != NULL)
        SendMessage(hWnd,MSG_OPEN_FILE_ERROR,(int)ShortFileName.c_str(),(int)(E->Message + " 重试" + IntToStr(ErrCount)).c_str());      
      //if(ErrCount > RetryCount && RetryCount > 0)
      if((ErrCount > RetryCount && RetryCount > 0) || ErrCount > 1000)
      {
        throw new Exception(E->Message);
      }
    }
  }
  if(AbortOperation)
  {
    if(file != NULL)
    {
      fclose(file);
    }
    throw new Exception("操作被用户取消");
  }
  char * Buffer = new char[RecordLength];
  int line = 0;
  TxtFileRecord tfr;
  int l;
  if(hWnd != NULL)
  {
    fseek(file,0L,SEEK_END);
    SendMessage(hWnd,MSG_SET_PROCESS_MAX_COUNT,ftell(file),(int)ShortFileName.c_str());
    fseek(file,0L,SEEK_SET);
  }
  int FilePos;
  while(!feof(file) && fgets(Buffer,RecordLength,file) != NULL)
  {
    if(++line > FileHeadLength && strlen(Buffer) > 0)
    {
      memset(&tfr,0,sizeof(tfr));
      tfr.RecordPosition = FileStream->Size;
      l = strlen(Buffer);
      tfr.RecordLength   = l;
      IndexStream->Write(&tfr,sizeof(tfr));
      FileStream->Write(Buffer,l);
    }
	  if(hWnd != NULL)
    {
      FilePos = ftell(file);
      if(FilePos % 1000 == 0 && FilePos > 1000)
      {
        PostMessage(hWnd,MSG_SET_PROCESS_POS,FilePos,0);
      }
    }
    if(AbortOperation)
      break;
  }
  IndexStream->Size -= FileTrailLength * sizeof(tfr);
  delete[] Buffer;
  fclose(file);
  FileLoaded = true;
  return true;
}
//设置BillRecord的当前缓冲区,Length是记录的长度
int TTxtBillFile::SetRecordBuffer(const char * chSrc,int Length)
{
  return BillRecord->SetTxtBuffer(chSrc,Length);
}
//返回当前记录在文件中的基于0的起始位置,对于文本文件，表示记录在文件中的行号
int TTxtBillFile::GetCurRecordPosition()
{
  return (IndexStream->Position -  sizeof(ri)) /sizeof(ri);
}
//返回当前记录的长度
int TTxtBillFile::GetCurRecordLength()
{
  return BillRecord->RecordLength;
}
//读取单条的记录
bool TTxtBillFile::ReadSingleRecord(int RecordIndex)
{
  if(!FileLoaded || RecordIndex >= GetRecordCount())
    return false;
  memset(&ri,0,sizeof(ri));
  IndexStream->Position = RecordIndex * sizeof(ri);
  if(IndexStream->Read(&ri,sizeof(ri)) != sizeof(ri))
    return false;
  return ManualSetRecordBuffer((const char *)&ri) > 0;
}
//对于文本文件类,这里的ABuffer实际上是一个指向TxtFileRecord结构的指针
int TTxtBillFile::ManualSetRecordBuffer(const char * ABuffer)
{
  TxtFileRecord * ptfr = (TxtFileRecord *)ABuffer;
  FileStream->Position = ptfr->RecordPosition;
  if(Buffer != NULL)
  {
    delete[] Buffer;
    Buffer = NULL;
  }
  Buffer = new char [ptfr->RecordLength];
  FileStream->Read(Buffer,ptfr->RecordLength);  
  return SetRecordBuffer(Buffer,ptfr->RecordLength);
}

void TTxtBillFile::SortFile(TBillField * BillField,int Direction)
{
  if(!FileLoaded)
    return;
  CompareField = BillField;
  CompareField->CompareDirection = Direction;
  CompareField->CompareLen = CompareField->FieldLength;
  qsort((void *)((char *)IndexStream->Memory),GetRecordCount(),sizeof(TxtFileRecord),BillField->CompareFieldData);  
}
void TTxtBillFile::SaveRecordToFile(FILE * file)
{
  fwrite(BillRecord->Buffer,BillRecord->RecordLength,1,file);
}
void TTxtBillFile::SaveFileHeadToFile(FILE * file)
{
  for(int n = 0;n < FileHeadLength;n++)
  {
    fprintf(file,"\n");
  }
}

/*
* 话单查找结果类
*
*
*
*/
TSearchResultBillFile::TSearchResultBillFile(AnsiString AFileName,TBillConfig * ABillConfig,HANDLE hParentWnd,AnsiString ATempDirectory)
  : TBillFile("",ABillConfig,hParentWnd,ATempDirectory)
{
  RealBillFile   = NULL;
  FileStream     = NULL;
  LastRecordLength = 0;
  Buffer = NULL;
  FileLoaded = false;
  //打开文件读文件头
  /*FileStream = new TMemoryStream;
  FileStream->LoadFromFile(AFileName);
  FileLoaded = false;
  if(FileStream->Read(&FileHead,sizeof(FileHead)) != sizeof(FileHead))
  {
    delete FileStream;
    FileStream = NULL;
    throw new Exception("文件格式不正确!");
  }
  FileLoaded = true;*/
  FILE * file = fopen(AFileName.c_str(),"r+b");
  if(file == NULL)
    throw new Exception("无法打开文件!");
  if(fread(&FileHead,sizeof(FileHead),1,file) != 1)
  {
    fclose(file);
    throw new Exception("无法打开文件!");
  }
  fclose(file);
  FileType = BillConfig->GetFileFormatType(FileHead.BillName);
  switch(FileType)
  {
    case BIN_BILL_FORMAT:
      RealBillFile = new TBinBillFile(FileHead.BillName,ABillConfig,hParentWnd,ATempDirectory);
      break;
    case TXT_BILL_FORMAT:
      RealBillFile = new TTxtBillFile(FileHead.BillName,ABillConfig,hParentWnd,ATempDirectory);
      break;
  }
  RecordLength     = RealBillFile->RecordLength;
  FileHeadLength   = RealBillFile->FileHeadLength;
  FileBlockLength  = RealBillFile->FileBlockLength;
  BlockHeadLength  = RealBillFile->BlockHeadLength;
  BlockTrailLength = RealBillFile->BlockTrailLength;
  IsBlockBinFile   = RealBillFile->IsBlockBinFile;
  Separator        = RealBillFile->Separator;
  //填充字段结构
  TBillField * Field;
  for(int n = 0;n < RealBillFile->FieldCount;n++)
  {
    Field = (TBillField *)RealBillFile->FieldList->Objects[n];
    if(Field->DataType == DATATYPE_SHORTFILENAME ||
       Field->DataType == DATATYPE_BILLNAME ||
       Field->DataType == DATATYPE_FILENAME ||
       Field->DataType == DATATYPE_RECORD_POSITION)
      continue;
    if(Field->FieldType == FIELDTYPE_STARTTIME)
      AnswerTimeField = Field;
    else if(Field->FieldType == FIELDTYPE_ENDTIME)
      EndTimeField = Field;
    Field->DataBillFile = this;
    FieldList->AddObject(RealBillFile->FieldList->Strings[n],RealBillFile->FieldList->Objects[n]);
  }
  //添加固定值字段-原始文件名
  Field = new TBillField(this,NULL);
  Field->FieldName = "file_name";
  Field->Label     = "原始文件名";
  Field->Offset    = -1;
  Field->DataType  = DATATYPE_STR;
  Field->DisplayAlign  = ALIGN_LEFT;
  Field->FieldLength   = 0;
  Field->DisplayWidth  = 100;
  Field->DisplayFormat = FIELD_DISPLAY_AS_TEXT;
  Field->SortType      = SORT_AS_STR;
  Field->DisplayInList = true;
  Field->ImportToDB    = true;
  Field->CreateIndex   = true;
  Field->ExportLength  = 255;
  Field->FieldType     = FIELDTYPE_SEARCH_RESULT_FILENAME;
  Field->SetDataType(Field->DataType);
  FieldList->AddObject(Field->FieldName,(TObject *)Field);
  //添加固定值字段-原始记录在原始文件中的位置
  Field = new TBillField(this,NULL);
  Field->FieldName = "record_position";
  Field->Label     = "记录位置";
  Field->Offset    = -1;
  Field->FieldLength   = 0;
  Field->DataType      = DATATYPE_UINT;
  Field->DisplayAlign  = ALIGN_RIGHT;
  Field->DisplayWidth  = 100;
  Field->DisplayFormat = FIELD_DISPLAY_AS_TEXT;
  Field->SortType      = SORT_AS_NUMBER;
  Field->DisplayInList = true;
  Field->ImportToDB    = true;
  Field->CreateIndex   = true;
  Field->ExportLength  = 10;
  Field->FieldType     = FIELDTYPE_SEARCH_RESULT_POS;
  Field->SetDataType(Field->DataType);
  FieldList->AddObject(Field->FieldName,(TObject *)Field);
  FieldCount = FieldList->Count;
  BillRecord = new TBillRecord(this);
}
TSearchResultBillFile::~TSearchResultBillFile()
{
  if(FileStream != NULL)
  {
    delete FileStream;
    FileStream = NULL;
  }
  if(RealBillFile != NULL)
  {
    delete RealBillFile;
    RealBillFile = NULL;
  }
  if(Buffer != NULL)
  {
    delete[] Buffer;
    Buffer = NULL;
  }
  if(FieldList != NULL)
  {
    for(int n = FieldCount - 2;n < FieldCount;n++)
    {
      delete (TBillField *)FieldList->Objects[n];
    }
    delete FieldList;
    FieldList = NULL;
  }
}
AnsiString TSearchResultBillFile::GetBillName()
{
  return RealBillFile->GetBillName();
}
int  TSearchResultBillFile::GetRecordCount()
{
  return FileHead.RecordCount;
}
bool TSearchResultBillFile::ReadSingleRecord(int RecordIndex)
{
  if(RecordIndex >= FileHead.RecordCount)
    return false;
  int pos = sizeof(SearchResultDataIndex) * RecordIndex + sizeof(FileHead) ;
  if(pos >= FileStream->Size)
    throw new Exception("文件指针试图定义到超出文件大小的位置!");
  FileStream->Position = pos;
  memset(&srd,0,sizeof(srd));
  SearchResultDataIndex srdi = {0};
  if(FileStream->Read(&srdi,sizeof(srdi)) != sizeof(srdi))
  {
    return false;
  }
  return ManualSetRecordBuffer((char *)&srdi) > 0;
}
/*
*对于查询结果集,ABuffer是一个指向SearchResultDataIndex结构的指针
*/
int TSearchResultBillFile::ManualSetRecordBuffer(const char * ABuffer)
{
  SearchResultDataIndex * psrdi = (SearchResultDataIndex *)ABuffer;
  FileStream->Position = psrdi->Position + sizeof(FileHead) + FileHead.IndexAreaLength;
  if(FileStream->Read(&srd,sizeof(srd)) != sizeof(srd))
  {
    return 0;
  }
  if(srd.DataLength != LastRecordLength)
  {
    if(Buffer != NULL)
    {
      delete[] Buffer;
      Buffer = NULL;
    }
    LastRecordLength = srd.DataLength;
    Buffer = new unsigned char[LastRecordLength];
  }
  if(ReadBlock())
  {
    return SetRecordBuffer((char *)Buffer,srd.DataLength);
  }
  else
  {
    return 0;
  }
}
int TSearchResultBillFile::SetRecordBuffer(const char * chSrc,int Length)
{
  return RealBillFile->SetRecordBuffer(chSrc,Length);
}
bool TSearchResultBillFile::ReadBlock()
{
  return FileStream->Read(Buffer,LastRecordLength) == LastRecordLength;  
}
bool TSearchResultBillFile::OpenBillFile(AnsiString AFileName,AnsiString ABillName)
{
  //return FileStream != NULL;
  FileName      = AFileName;
  ShortFileName = ExtractFileName(FileName);
  FileLoaded     = false;
  AbortOperation = false;
  if(FileStream == NULL)
    FileStream = new TMemoryStream();
  else
    FileStream->Clear();
  int FileSize = GetLocalFileSize(AFileName);
  if(hWnd != NULL)
  {
    SendMessage(hWnd,MSG_SET_PROCESS_MAX_COUNT,FileSize,(int)ExtractFileName(AFileName).c_str());
  }

  FILE * file = fopen(AFileName.c_str(),"r+b");
  if(file == NULL)
    throw new Exception("打开文件失败,请检查文件是否存在,并且可以被打开");
  #define BUFFER_LENGTH 20480
  FileStream->SetSize(FileSize);
  char chBuffer[BUFFER_LENGTH];
  int ReadLen = fread(&chBuffer[0],sizeof(char),BUFFER_LENGTH,file);
  int WriteCount = 0;
  while(ReadLen > 0)
  {
    FileStream->Write(&chBuffer[0],ReadLen);
    WriteCount += ReadLen;
    if(hWnd != NULL && WriteCount > 1000 && WriteCount % 1000 == 0)
    {
      SendMessage(hWnd,MSG_SET_PROCESS_POS,WriteCount,0);
    }
    if(AbortOperation)
      break;
    ReadLen = fread(&chBuffer[0],sizeof(char),BUFFER_LENGTH,file);
  }
  fclose(file);
  if(AbortOperation)
  {
    FileStream->Clear();
    return false;
  }
  FileLoaded = true;
  return true;
}
void TSearchResultBillFile::SaveFileHeadToFile(FILE * file)
{
  RealBillFile->SaveFileHeadToFile(file);
}
void TSearchResultBillFile::SaveFileTrailToFile(FILE * file)
{
  RealBillFile->SaveFileTrailToFile(file);
}
void TSearchResultBillFile::SortFile(TBillField * BillField,int Direction)
{
  if(!FileLoaded)
    return;
  CompareField = BillField;
  CompareField->CompareDirection = Direction;
  CompareField->CompareLen = CompareField->FieldLength;
  char * chIndexes = new char[FileHead.IndexAreaLength];
  FileStream->Position = sizeof(FileHead);
  FileStream->Read(chIndexes,FileHead.IndexAreaLength);
  qsort((void *)(chIndexes),GetRecordCount(),sizeof(SearchResultDataIndex),BillField->CompareFieldData);
  FileStream->Position = sizeof(FileHead);
  FileStream->Write(chIndexes,FileHead.IndexAreaLength);
  delete[] chIndexes;
}
void TSearchResultBillFile::SaveRecordToFile(FILE * file)
{
  RealBillFile->SaveRecordToFile(file);  
}
//返回当前记录在文件中的基于0的起始位置
int TSearchResultBillFile::GetCurRecordPosition()
{
  return srd.OriDataIndex;
}
//返回当前记录的长度
int TSearchResultBillFile::GetCurRecordLength()
{
  return srd.DataLength;
}
AnsiString TSearchResultBillFile::GetShortFileName()
{
  return srd.OriFileName;
}
void * TSearchResultBillFile::GetRecordBuffer()
{
  return RealBillFile->GetRecordBuffer();
}
