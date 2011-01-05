//话单记录类

#include "BillRecord.h"

TBillRecord::TBillRecord(TBillFile * ABillFile)
{
  BillFile = ABillFile;
  RecordLength = BillFile->RecordLength;
  if(BillFile->FileType == BIN_BILL_FORMAT)
    Buffer = new char[RecordLength];
  else
    Buffer = NULL;
  BufferFilled = false;
  TextBuffer = NULL;
  Separator = (int)BillFile->Separator;
}

TBillRecord::~TBillRecord()
{
  if(Buffer != NULL)
  {
   delete[] Buffer;
   Buffer = NULL;
  }
  if(TextBuffer != NULL)
  {
    delete[] TextBuffer;
    TextBuffer = NULL;
  }
}
int TBillRecord::SetBinBuffer(const char * chSrc)
{
  BufferFilled = false;
  memcpy(Buffer,&chSrc[0],BillFile->RecordLength);
  BufferFilled = true;
  return BillFile->RecordLength;
}
int TBillRecord::SetTxtBuffer(const char * chSrc,int Length)
{
  BufferFilled = false;
  if(Length != RecordLength)
  {
    if(Buffer != NULL)
    {
      delete[] Buffer;
    }
    Buffer = new char[Length];
  }
  strncpy(Buffer,chSrc,Length);
  RecordLength = Length;
  BufferFilled = true;
  return RecordLength;
}
const char * TBillRecord::GetBuffer(int Offset)
{
  if(BillFile->FileType == BIN_BILL_FORMAT)
  {
   return (const char *)(Buffer + Offset);
  }
  else
  {
    int n;
    char * pos1 = Buffer;
    char * pos2;
    for(n = 0;n < Offset;n++)
    {
      pos2 = strchr(pos1,Separator);
      if(pos2 == NULL)
        break;
      pos1 = ++pos2;
    }
    if(TextBuffer != NULL)
    {
      delete[] TextBuffer;
      TextBuffer = NULL;
    }
    int len = strchr(pos1,Separator) - pos1;
    if(len < 0)
    {
      if(Offset > n)
        return "";
      else
        len = Buffer + RecordLength - pos1;
    }
    TextBuffer = new char[++len];
    memset(TextBuffer,0,len);
    strncpy(TextBuffer,pos1,--len);

    pos1 = strchr(TextBuffer,'\n');
    if(pos1 != NULL)
      * pos1 = 0;
    pos1 = strchr(TextBuffer,'\r');
    if(pos1 != NULL)
      * pos1 = 0;
    return TextBuffer;
  }
}
/*
AnsiString TBillRecord::GetTxtBuffer(int Offset)
{

  /*if(!BufferFilled)
    return "";
  if(Offset >= TxtFields->Count)
    return "";
  else
    return TxtFields->Strings[Offset];*/
    //return "";
/*
  const char * pos1;
  const char * pos2;
  int BufferLen = strlen(Buffer);
  pos1 = strchr((char *)Buffer,(int)BillFile->Separator);
  pos2 = pos1;
  int n = 0;
  while(pos1 != NULL && n < Offset)
  {
    pos1 = strchr(pos2 + 1,(int)BillFile->Separator);
    ++n;
    if(pos1 == NULL || n >= Offset)
      break;
    pos2 = pos1;
  }
  int count = 0;
  AnsiString Result = "";
  const char * pos3;
  if(pos1 > pos2)
  {
    count = pos1 - pos2 - 1;
    pos3 = pos2 + 1;
  }
  else if(pos1 < pos2)
  {
    if(pos1 == NULL)
    {
      count = Buffer + BufferLen - pos2;
      pos3 = pos2 + 1;
    }
    else
    {
      count = pos2 - pos1;
      pos3 = pos1;
    }
  }
  else if(pos1 == pos2)
  {
    if(pos1 == NULL)
      count = BufferLen;
    else
      count = pos1 - Buffer;
    pos3 = Buffer;
  }
  if(count > 0)
  {
    char * ch = new char[count + 1];
    memcpy(ch,pos3,count);
    * (ch + count) = 0;
    Result = ch;
    delete[] ch;
  }
  return AnsiReplaceText(AnsiReplaceText(Result,"\n",""),"\r","");
*/
/*}*/
 