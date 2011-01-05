
#include "public.h"

//写日志
void __fastcall SaveLog(const char * chMessage)
{
  if(LogFile != NULL)
  {
    fprintf(LogFile,"%s:%s\n",FormatDateTime("yyyy-mm-dd hh:nn:ss",Now()),chMessage);
  }
  if(SlentMode)
  {
    printf("%s:%s\n",FormatDateTime("yyyy-mm-dd hh:nn:ss",Now()),chMessage);
  }
}
//出错时写日志并显示错误信息
void __fastcall ShowErrorMessage(HWND hWnd,const char * chMessage,bool DoExitProcess,int ExitCode)
{
  SaveLog(chMessage);
  if(!SlentMode)
    MessageBox(hWnd,chMessage,"错误",MB_OK | MB_ICONSTOP);
  if(InCmdMode && DoExitProcess)
  {
    fclose(LogFile);
    ExitProcess(ExitCode);
  }
}
//Param1是结束符
AnsiString __fastcall RightBCDToStr(const unsigned char chSource[],int len,int Param1,int Param2)
{
  char str[50] = {0};
  char c;
  int  nPos = 0;
  for(int i = 0;i < len;i++)
  {
    c = chSource[i] >> 4 & 0x0f;
    if(c >= 0x0a && c <= 0x0e)
      str[nPos] = c + 65;
    else
      str[nPos] = c + 48;
    if(c == Param1)
      break;
    ++nPos;
    c = chSource[i] & 0x0f;
    if(c == Param1)
      break;
    if(c >= 0x0a && c <= 0x0e)
      str[nPos] = c + 65;
    else
      str[nPos] = c + 48;
    ++nPos;
  }
  str[nPos] = 0x00;
  return AnsiString(str);
}
void __fastcall RightBCDToStr(char chDest[],const unsigned char chSource[],int len,int Param1,int Param2)
{
  char c;
  int  pos = 0;
  for(int i = 0;i < len;i++)
  {
    c = chSource[i] >> 4 & 0x0f;
    if(c >= 0x0a && c <= 0x0e)
      chDest[pos] = c + 65;
    else
      chDest[pos] = c + 48;
    if(c == Param1)
      break;
    ++pos;
    c = chSource[i] & 0x0f;
    if(c == Param1)
      break;
    if(c >= 0x0a && c <= 0x0e)
      chDest[pos] = c + 65;
    else
      chDest[pos] = c + 48;
    ++pos;
  }
  chDest[pos] = 0x00;
}
//将字符串转换为BCD右对齐格式
//返回转换后的数组长度
//chSource : 要转换的字符串.字符串中只允许0-9和BCDE字符,不在此范围内的字符将被忽略
//Param1 : 是BCD的终止符
//len    : 传入的chDest的长度
int __fastcall StrToRightBCD(char chDest[],const char * chSource,int len,int Param1,int Param2)
{
  memset(&chDest[0],(char)Param1,len);
  unsigned char ch1,ch2; //ch1是源,ch2是目标
  int pos = 0;
  int SourceLen = strlen(chSource);
  int n = 0;
  while(true)
  {
    ch1 = * (chSource + n);
    if(ch1 >= '0' && ch1 <= '9')      //数字
    {
      ch2 = ch1 - 48;
    }
    else if(ch1 >= 'A' && ch1 <= 'E') //字符
    {
      ch2 = ch1 - 55;
    }
    ch2 = (ch2 << 4);
    ++n;
    if(n >= SourceLen)
    {
      * (chDest + pos) = ch2 + 0x0f;
      break;
    }
    ch1 = * (chSource + n);
    if(ch1 >= '0' && ch1 <= '9')      //数字
    {
      ch2 = ch2 + (ch1 - 48);
    }
    else if(ch1 >= 'A' && ch1 <= 'E') //字符
    {
      ch2 = ch2 + (ch1 - 55);
    }
    * (chDest + pos) = ch2;
    ++n;
    if(n >= SourceLen)
      break;
    ++pos;
  }
  return pos;
}
//将一个字符串转换为BCD左对齐格式
//返回转换后的数组长度
//chSource : 要转换的字符串.字符串中只允许0-9和BCDE字符,不在此范围内的字符将被忽略
//Param1   : 是BCD的终止符
//len      : 传入的chDest的长度
int __fastcall StrToLeftBCD(char chDest[],const char * chSource,int len,int Param1,int Param2)
{
  memset(&chDest[0],(char)Param1,len);
  unsigned char ch1,ch2; //ch1是源,ch2是目标
  int pos = 0;
  int SourceLen = strlen(chSource);
  int n = 0;
  while(true)
  {
    ch1 = * (chSource + n);
    if(ch1 >= '1' && ch1 <= '9')      //数字
    {
      ch2 = ch1 - 48;
    }
    else if(ch1 >= 'B' && ch1 <= 'E') //字符
    {
      ch2 = ch1 - 55;
    }
    else if(ch1 == '0')
    {
      ch2 = 0x0a;
    }
    ++n;
    if(n >= SourceLen)
    {
      * (chDest + pos) = 0xf0 + ch2;
      break;
    }
    ch1 = * (chSource + n);
    if(ch1 >= '1' && ch1 <= '9')      //数字
    {
      ch2 = ch2 + ((ch1 - 48) << 4);
    }
    else if(ch1 >= 'B' && ch1 <= 'E') //字符
    {
      ch2 = ch2 + ((ch1 - 55) << 4);
    }
    else if(ch1 == '0')
    {
      ch2 = ch2 + 0xa0;
    }
    if(ch2 < 0x0f)
      ch2 = 0xf0 + ch2;
    * (chDest + pos) = ch2;
    ++n;
    if(n >= SourceLen)
      break;
    ++pos;
  }
  return pos;
}
//BCD左对齐转换为字符串
//Param1是终止符
AnsiString __fastcall LeftBCDToStr(const unsigned char chSource[],int len,int Param1,int Param2)
{
  char str[20] = {0};
  char c;
  int  pos = 0;
  for(int i = 0;i < len;i++)
  {
    //处理低位
    if(chSource[i] == 0xff)
      break;
    c = (chSource[i] & 0x0f);
    if(c == Param1)
      break;
     if(c > 0x0a)
      c += 55;
    else
      c = (c % 10) + 48;
    str[pos] = c;
    ++pos;
    //处理高位
    c = (chSource[i] >> 4) & 0x0f;
    if(c == Param1)
      break;
    if(c > 0x0a)
      c += 55;
    else
      c = (c % 10) + 48;
    str[pos] = c;
    ++pos;
  }
  str[pos] = 0x00;
  return AnsiString(str);
}

//转换char 为 short int
AnsiString __fastcall ConvertToWord(const unsigned char chSource[],int len,int Param1,int Param2)
{
  unsigned short int * Value = (unsigned short int *)chSource;
  try
  {
    return StrToInt(* Value);
  }
  catch(...)
  {
    return 32767;
  }
}
//转换char 为 unsigned int
AnsiString __fastcall ConvertToUINT(const unsigned char chSource[],int len,int Param1,int Param2)
{
  unsigned long int * Value = (unsigned long int *)chSource;
  try
  {
    return (* Value);
  }
  catch(...)
  {
    return 2147483648;
  }
}
//转换char 为 int
AnsiString __fastcall ConvertToInt(const unsigned char chSource[],int len,int Param1,int Param2)
{
  long int * Value = (long int *)chSource;
  try
  {
    return (* Value);
  }
  catch(...)
  {
    return 2147483648;
  }
}

//转换char 为 byte
AnsiString __fastcall ConvertToBYTE(const unsigned char chSource[],int len,int Param1,int Param2)
{
  //unsigned long int * Value = (unsigned long int *)chSource;
  try
  {
    return StrToInt(* chSource);
  }
  catch(...)
  {
    return 255;
  }
}
//将话单中的秒数转换为日期时间
//Param1是基准时间
AnsiString __fastcall ConvertBasedDateTime(const unsigned char chSource[],int len,int Param1,int Param2)
{
  int * Seconds = (int *)chSource;
  return FormatDateTime("yyyy-mm-dd hh:nn:ss",IncSecond(* ((TDateTime *)Param1),* (Seconds)));
}
//转换RBCD_YYYYMMDDHHMMSS为日期时间
AnsiString __fastcall ConvertRBCDYYYYMMDDHHMMSS(const unsigned char chSource[],int len,int Param1,int Param2)
{
  char Year[5]   = {0};
  char Month[3]  = {0};
  char Day[3]    = {0};
  char Hour[3]   = {0};
  char Minute[3] = {0};
  char Second[3] = {0};
  RightBCDToStr(Year,chSource,2,Param1,0);
  RightBCDToStr(Month,(unsigned char *)(chSource + 2),1,Param1,0);
  RightBCDToStr(Day,(unsigned char *)(chSource + 3),1,Param1,0);
  RightBCDToStr(Hour,(unsigned char *)(chSource + 4),1,Param1,0);
  RightBCDToStr(Minute,(unsigned char *)(chSource + 5),1,Param1,0);
  RightBCDToStr(Second,(unsigned char *)(chSource + 6),1,Param1,0);
  char chResult[18] = {0};
  strcat(&chResult[0],&Year[0]);
  strcat(&chResult[0],"-");
  strcat(&chResult[0],&Month[0]);
  strcat(&chResult[0],"-");
  strcat(&chResult[0],&Day[0]);
  strcat(&chResult[0]," ");
  strcat(&chResult[0],&Hour[0]);
  strcat(&chResult[0],":");
  strcat(&chResult[0],&Minute[0]);
  strcat(&chResult[0],":");
  strcat(&chResult[0],&Second[0]);
  return AnsiString(chResult);
}

//转换RBCD_YYMMDDHHMMSS为日期时间
AnsiString __fastcall ConvertRBCDYYMMDDHHMMSS(const unsigned char chSource[],int len,int Param1,int Param2)
{
  char Year[5]   = {'2','0',0,0,0};
  char Month[3]  = {0};
  char Day[3]    = {0};
  char Hour[3]   = {0};
  char Minute[3] = {0};
  char Second[3] = {0};
  RightBCDToStr(&Year[2],chSource,1,Param1,0);
  RightBCDToStr(Month,(unsigned char *)(chSource + 1),1,Param1,0);
  RightBCDToStr(Day,(unsigned char *)(chSource + 2),1,Param1,0);
  RightBCDToStr(Hour,(unsigned char *)(chSource + 3),1,Param1,0);
  RightBCDToStr(Minute,(unsigned char *)(chSource + 4),1,Param1,0);
  RightBCDToStr(Second,(unsigned char *)(chSource + 5),1,Param1,0);
  char chResult[18] = {0};
  strcat(&chResult[0],&Year[0]);
  strcat(&chResult[0],"-");
  strcat(&chResult[0],&Month[0]);
  strcat(&chResult[0],"-");
  strcat(&chResult[0],&Day[0]);
  strcat(&chResult[0]," ");
  strcat(&chResult[0],&Hour[0]);
  strcat(&chResult[0],":");
  strcat(&chResult[0],&Minute[0]);
  strcat(&chResult[0],":");
  strcat(&chResult[0],&Second[0]);
  return AnsiString(chResult);
}
AnsiString __fastcall ConvertStr(const unsigned char chSource[],int len,int Param1,int Param2)
{
  char * Buffer = new char[++len];
  memset(Buffer,0,len);
  memcpy(Buffer,chSource,--len);
  AnsiString Result = Buffer;
  delete[] Buffer;
  return Result;
}
//转换STR_YYYYMMDDHHMMSS为日期时间格式
AnsiString __fastcall ConvertSTRYYYYMMDDHHMMSS(const unsigned char chSource[],int len,int Param1,int Param2)
{
/*
  char Year[5]   = {0};
  char Month[3]  = {0};
  char Day[3]    = {0};
  char Hour[3]   = {0};
  char Minute[3] = {0};
  char Second[3] = {0};
  
  strncpy(&Year[0],chSource,4);
  strncpy(&Month[0],(unsigned char *)(chSource + 4),2);
  strncpy(&Day[0],(unsigned char *)(chSource + 6),2);
  strncpy(&Hour[0],(unsigned char *)(chSource + 8),2);
  strncpy(&Minute[0],(unsigned char *)(chSource + 10),2);
  strncpy(&Second[0],(unsigned char *)(chSource + 12),2);
  
  char chResult[18] = {0};
  strcat(&chResult[0],&Year[0]);
  strcat(&chResult[0],"-");
  strcat(&chResult[0],&Month[0]);
  strcat(&chResult[0],"-");
  strcat(&chResult[0],&Day[0]);
  strcat(&chResult[0]," ");
  strcat(&chResult[0],&Hour[0]);
  strcat(&chResult[0],":");
  strcat(&chResult[0],&Minute[0]);
  strcat(&chResult[0],":");
  strcat(&chResult[0],&Second[0]);
  return AnsiString(chResult);
*/
  char chResult[20] = {0,0,0,0,'-',0,0,'-',0,0,' ',0,0,':',0,0,':',0,0,0};
  strncpy(&chResult[0],chSource,4);
  strncpy(&chResult[5],(unsigned char *)(chSource + 4),2);
  strncpy(&chResult[8],(unsigned char *)(chSource + 6),2);
  strncpy(&chResult[11],(unsigned char *)(chSource + 8),2);
  strncpy(&chResult[14],(unsigned char *)(chSource + 10),2);
  strncpy(&chResult[17],(unsigned char *)(chSource + 12),2);
  return AnsiString(chResult);
}
//转换二进制日期和时间
AnsiString __fastcall ConvertBINYYMMDDHHMMSS(const unsigned char * chSource,int len,int Param1,int Param2)
{
  short int Year   = 2000 + * (chSource);
  short int Month  = * (chSource + 1);
  short int Day    = * (chSource + 2);
  short int Hour   = * (chSource + 3);
  short int Minute = * (chSource + 4);
  short int Second = * (chSource + 5);
  short int MilliSecond = 0;
  
  
  try
  {
    return FormatDateTime("yyyy-mm-dd hh:nn:ss",EncodeDateTime(Year,Month,Day,Hour,Minute,Second,MilliSecond));
  }
  catch(...)
  {
    return "1900-01-01 00:00:00";
  }
}
//判断给定的字符串中是否含有非数字字符
bool __fastcall IsNumber(const char * chStr)
{
  char ch;
  int len = strlen(chStr);
  if(len == 0)
    return false;
  for(int n = 0;n < len;n++)
  {
    ch = *(chStr + n);
    if(n == 0 && (ch == '-' || ch == '+'))
      continue; 
    if(ch < '0' || ch > '9')
      return false;
  }
  return true;
}
int __fastcall StrToIntEx(const char * chStr)
{
  char ch1[11] = {0};
  ch1[0]   = '0';
  char ch2 = 0;
  for(int n = 0;n < (int)strlen(chStr);n++)
  {
    memcpy(&ch2,(chStr + n),1);
    if(ch2 < '0' || ch2 > '9')
      break;
    ch1[n] = ch2;
  }
  return StrToInt(ch1);
}
//返回字符串chScan在chStr中最后一将出现的基于0的位置
//如果chStr中没有找到chScan,返回-1.
int StrRScan(const char * chStr,const char * chScan,int len)
{
  if(len == -1)
    len = strlen(chScan);
  for(int n = strlen(chStr) - len/* - 1*/;n > -1;n--)
  {
    if(chStr[n] == *(chScan) && memcmp((void *)(chStr + n),chScan,len) == 0)
      return n;
  }
  return -1;
}
//搜索一个字符在字符串中第一次出现的位置
//如果成功返回该字符在字符串中出现的基于0的位置
//否则返回-1.
int StrLScan(const char * chStr,const char * chScan,int len)
{
  //int len = strlen(chScan);
  if(len == -1)
    len = strlen(chScan);
  for(int n = 0;n < (int)strlen(chStr);n++)
  {
    if(chStr[n] == *(chScan) && memcmp(chStr + n,chScan,len) == 0)
      return n;
  }
  return -1;
}

AnsiString EncryFTPPassword(AnsiString Password)
{
  TWCDESComp * WCDESComp = new TWCDESComp(NULL);
  AnsiString Result = WCDESComp->EncryStrHex(Password,FTP_PASSWORD_KEY);
  delete WCDESComp;
  return Result;
}
AnsiString DecryFTPPassword(AnsiString Password)
{
  TWCDESComp * WCDESComp = new TWCDESComp(NULL);
  AnsiString Result = WCDESComp->DecryStrHex(Password,FTP_PASSWORD_KEY);
  delete WCDESComp;
  return Result;
}
void Split(AnsiString str,AnsiString Separator,TStringList * List)
{
  AnsiString str1 = str;
  int pos1 = str.Pos(Separator);
  while(pos1 > 0)
  {
    List->Add(str1.SubString(0,pos1 - 1));
    str1 = str1.SubString(pos1 + 1,str1.Length() - pos1);
    pos1 = str1.Pos(Separator);
  }
  if(str1 != "")
    List->Add(str1);
}
AnsiString __fastcall XMLDecode(AnsiString str)
{
  str = AnsiReplaceText(str,"[ASCII=124]","|");
  str = AnsiReplaceText(str,"[ASCII=32]"," ");
  str = AnsiReplaceText(str,"[ASCII=13]","[回车符]");
  str = AnsiReplaceText(str,"[ASCII=10]","[换行符]");
  str = AnsiReplaceText(str,"[ASCII=9]","[制表符]");
  return str;
}
AnsiString __fastcall XMLDecodeForField(AnsiString str)
{
  str = AnsiReplaceText(str,"[ASCII=124]","|");
  str = AnsiReplaceText(str,"[ASCII=32]"," ");
  str = AnsiReplaceText(str,"[ASCII=13]","\n");
  str = AnsiReplaceText(str,"[ASCII=10]","\r");
  str = AnsiReplaceText(str,"[ASCII=9]","\t");
  return str;
}
AnsiString __fastcall XMLEncode(AnsiString str)
{
  str = AnsiReplaceText(str,"|","[ASCII=124]");
  str = AnsiReplaceText(str," ","[ASCII=32]");
  str = AnsiReplaceText(str,"\t","[ASCII=9]");
  str = AnsiReplaceText(str,"[回车符]","[ASCII=13]");
  str = AnsiReplaceText(str,"[换行符]","[ASCII=10]");
  str = AnsiReplaceText(str,"[制表符]","[ASCII=9]");
  return str;
}
AnsiString __fastcall GetErrorString(int nCode)
{
	LPVOID lpMsgBuf;
	lpMsgBuf=LocalLock(LocalAlloc(LMEM_MOVEABLE|LMEM_ZEROINIT,1000));
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM|
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		nCode,
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL);
    AnsiString ErrorString = AnsiString((char *)lpMsgBuf).Trim();
	if(ErrorString=="")
	{
		ErrorString = "未知错误:" + IntToStr(nCode);
	}
	LocalFree(lpMsgBuf);
    return ErrorString;
}
AnsiString DecrySysInfo(AnsiString String)
{
  TWCDESComp * WCDESComp = new TWCDESComp(NULL);
  AnsiString Result = WCDESComp->DecryStrHex(String,SYS_INFO_KEY);
  delete WCDESComp;
  return Result;
}
//加密系统信息
AnsiString EncrySysInfo(AnsiString String)
{
  TWCDESComp * WCDESComp = new TWCDESComp(NULL);
  AnsiString Result = WCDESComp->EncryStrHex(String,SYS_INFO_KEY);
  delete WCDESComp;
  return Result;
}
AnsiString EncryRegisteCode(AnsiString String)
{
  TWCDESComp * WCDESComp = new TWCDESComp(NULL);
  AnsiString Result = WCDESComp->EncryStrHex(String,"ACCSEED\r\n\t_@($*!&MM<ASJJKDSA\t0x28KHFTAL98213897(*&$#($&@!)234hdslfsarewq90");
  delete WCDESComp;
  return Result;
}
AnsiString GetATempFileName(AnsiString TempDirectory,AnsiString FileName)
{
  if(!DirectoryExists(TempDirectory)&&!CreateDir(TempDirectory))
  {
    return "";
  }
  else if(TempDirectory.Pos("\\") < 1)
  {
    TempDirectory = TempDirectory + "\\";
  }
  char chTempFileName[MAX_PATH];
  GetTempFileName(TempDirectory.c_str(),FileName.c_str(),0,&chTempFileName[0]);
  //MessageBox(NULL,FileName.c_str(),"",MB_OK);
  AnsiString Result = chTempFileName;
  DeleteFile(Result);
  //memset(&chTempFileName[0],0,MAX_PATH);
  //GetTempFileName(TempDirectory.c_str(),FileName.c_str(),0,&chTempFileName[0]);
  //Result = chTempFileName;
  return Result;
}
unsigned int GetLocalFileSize(AnsiString FileName)
{
  HANDLE hFile = CreateFile(FileName.c_str(),GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
  if(hFile == INVALID_HANDLE_VALUE)
  {
    return -1;
  }
  unsigned int FileSize = GetFileSize(hFile,NULL);
  CloseHandle(hFile);
  return FileSize;
}
AnsiString GetFullFTPFileName(AnsiString FileName)
{
  if(FileName.SubString(1,6) != "ftp://")
    return FileName;
  AnsiString Str   = FileName.SubString(7,FileName.Length() - 6);
  int pos = Str.Pos("@");
  Str     = Str.SubString(pos + 1,Str.Length() - pos);
  return "ftp://" + AnsiReplaceStr(AnsiReplaceStr(Str,"||",""),"|PASV|","");
}

 