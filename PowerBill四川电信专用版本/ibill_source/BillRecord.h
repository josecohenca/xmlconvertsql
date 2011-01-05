//话单记录类

#ifndef BILLRECORD_H
#define BILLRECORD_H

#include "public.h"
#include "BillFile.h"
//话单记录定义
class TBillRecord
{
private:
  //AnsiString    strBuffer;
  bool          BufferFilled;
  TBillFile   * BillFile;
  int           Separator;
  char *        TextBuffer;
  AnsiString    GetTxtBuffer(int Offset);
public:
  int RecordLength;
  char * Buffer;
  int SetBinBuffer(const char * chSrc);
  int SetTxtBuffer(const char * chSrc,int Length);
  const char * GetBuffer(int Offset);

  TBillRecord(TBillFile * ABillFile);
  ~TBillRecord();
};


#endif
