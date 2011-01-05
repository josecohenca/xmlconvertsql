//公用

#ifndef PUBLIC_H
#define PUBLIC_H

#define _WINSOCKAPI_

//编译选项
#define SCTELE_COM_VERSION  //四川电信专用版本,用加密码后的BIOS序列号和CPU信息作为序列号

#include <windows.h>

#ifdef SCTELE_COM_VERSION
#include <comdef.h>
#include <wbemidl.h>
#endif

//#include <Classes.hpp>
//#include <msxmldom.hpp>
//#include <XMLDoc.hpp>
//#include <xmldom.hpp>
//#include <XMLIntf.hpp>
//#include <DateUtils.hpp>
#include <stdio.h>
#include "WCDESComp.hpp"
#include <StrUtils.hpp>




#define TRY_VERSION_FILE_COUNT  10    //试用版允许操作的最大文件数


#define MSG_SET_PROCESS_POS       WM_USER + 1000  //设置进度条的位置.wParam是当前的完成量
#define MSG_SET_PROCESS_MAX_COUNT WM_USER + 1005  //通知窗口设置进度条的最大值.WParam是最大值.LParam是附加信息,由发送者和接收者确定
#define MSG_CUSTOMER_KEYDOWN      WM_USER + 1006  //按下了自定义键,WParam是CTRL是否被按下,LParam是键的ASCII码

#define MSG_FTP_WORK_BEGIN        WM_USER + 1002  //通告FTP下载即将开始,wParam是一个指向文件名称字符串的指针,lParam是文件的大小
#define MSG_FTP_WORK              WM_USER + 1004  //通告FTP下载正在进行,wParam是一个指向文件名称字符串的指针,lParam是当前的完成量
#define MSG_THREAD_COMPLETED      WM_USER + 1100  //通知主窗口线程的工作已结束,wParam是线程的指针

#define MSG_SAVE_TO_TEMPFILE      WM_USER + 1200  //通知调用者窗口正在写临时文件

#define MSG_OPEN_FILE_ERROR       WM_USER + 1300  //通知调用者窗口打开文件失败,WParam是已失败的次数,LParam是错误的描述信息

#define BIN_BILL_FORMAT 1             //二进制话单格式
#define TXT_BILL_FORMAT 2             //文本文件的话单格式
#define SEARCH_RESULT_FORMAT 9999     //标准话单格式,程序内部使用

#define BIN_BILL_FORMAT_STR  "二进制文件"
#define TXT_BILL_FORMAT_STR  "文本文件"
#define ANS_BILL_FORMAT_STR  "ANS.1二进制文件"
#define TXT1_BILL_FORMAT_STR "非线性文本文件"
#define SR_BILL_FORMAT_STR   "SearchResult"

#define SORT_AS_STR      1
#define SORT_AS_DATETIME 2
#define SORT_AS_NUMBER   3
#define SORT_AS_CURRENCY 4

#define REPLACE_LEFT  1
#define REPLACE_RIGHT 2
#define REPLACE_ANY   3

//数据类型定义
#define DATATYPE_LBCD                     1   //BCD码左对齐
#define DATATYPE_RBCD                     2   //BCD码右对齐
#define DATATYPE_STR                      3   //字符串
#define DATATYPE_LBCD_YYYYMMDDHHMMSS      4   //BCD码左对齐日期时间(四位年份).
#define DATATYPE_RBCD_YYYYMMDDHHMMSS      5   //BCD码右对齐日期时间(四位年份).
#define DATATYPE_LBCD_YYMMDDHHMMSS		    6   //BCD码左对齐日期时间(两位年份).
#define DATATYPE_RBCD_YYMMDDHHMMSS		    7   //BCD码右对齐日期时间(两位年份).
#define DATATYPE_BASED_DATETIME	  	      8   //有基准时间的日期时间格式
#define DATATYPE_STR_YYYY_MM_DD_HH_MM_SS  9   //文本文件日期时间字段.格式为:YYYY-MM-DD HH:MM:SS
#define DATATYPE_UINT                     10  // 无符号长整数(4字节)
#define DATATYPE_WORD                     11  //无符号短整数(2字节)
#define DATATYPE_BYTE                     12  //字节(1Byte),统一为unsigned char
#define DATATYPE_BIT                      13  //BIT位
#define DATATYPE_CURRENCY                 14  //货币(精确到小数点后两位).
#define DATATYPE_STR_YYYYMMDDHHMMSS       15  //文本文件日期格式字段(四位年份).
#define DATATYPE_BIN_YYMMDDHHMMSS         16  //二进制日期时间
#define DATATYPE_FIXED                    17  //固定值
#define DATATYPE_FILENAME                 18  //取记录所在的文件名
#define DATATYPE_BILLNAME                 19  //取话单名称
#define DATATYPE_SHORTFILENAME            20  //不带路径的文件名
#define DATATYPE_INT                      21  //有符号长整数
#define DATATYPE_DURATION                 22  //通话时长
#define DATATYPE_RECORD_POSITION          23  //记录位置


//字段类型定义
#define FIELDTYPE_NORMAL        1     //普通字段
#define FIELDTYPE_STARTTIME     2     //起始时间
#define FIELDTYPE_ENDTIME       3     //结束时间
#define FIELDTYPE_RECORDTYPE    4     //记录标志
#define FIELDTYPE_RECORDLENGTH  5     //记录长度

#define FIELDTYPE_SEARCH_RESULT_FILENAME 100 //内部类型:查询结果所在的原始文件名
#define FIELDTYPE_SEARCH_RESULT_POS      101 //内部类型:查询结果所在的原始记录位置



#define ALIGN_LEFT    1
#define ALIGN_CENTER  2
#define ALIGN_RIGHT   3

#define FIELD_DISPLAY_AS_TEXT 1
#define FIELD_DISPLAY_AS_BITS 2

#define EXPORT_DBF 1
#define EXPORT_CSV 2
#define EXPORT_TXT 3
#define EXPORT_ORI 4


#define RECORD_FILTER_SKIP 1  //跳过指定的记录
//#define RECORD_FILTER_READ 2  //读取指定的记录

//打开话单文件时出错的处理方式
#define ON_ERROR_RETRY   1 //出错时自动重试
#define ON_ERROR_WARNING 2 //出错时显示MessageBox
#define ON_ERROR_ABORT   3 //出错时终止操作
#define ON_ERROR_IGNORE  4 //忽略错误继续操作


//导出话单到数据库时转换文件类型
//#define CONVERT_TO_STD_FORMAT 1   //转换为标准格式
//#define CONVERT_TO_ORI_BILL   2   //转换为Config.xml中定义的格式(仅在选择了单一话单类型时生效)

//FTP密码密钥
const char FTP_PASSWORD_KEY[] = "APPLICATION_198705#_*@#$(*&$(*@&!($*&#*&(*&(*^123521@(*)!";
//系统信息密钥
const char SYS_INFO_KEY[] = "@($*&(#YRKJDWHVKWJBKVW&R(@#!*&$()@HOJRHDLWQ&R)*($@!&ABCDEFGHIJKLMN";
//WIN32_bios
const char WIN32_BIOS_STRING[] = "8622DB48366E510BC2E2B92AA54E9112";
//"SERIALNUMBER="
const char SERIALNUMBER[] = "EEF56AA860F7F75CB8A21CC2E3C3ECFC";

extern bool SlentMode;
extern FILE * LogFile;
extern bool InCmdMode;

extern AnsiString HelpFilePath;

extern int FTPTimeout;
extern int RetryCount;
//写日志
void __fastcall SaveLog(const char * chMessage);
//出错时写日志并显示错误信息
void __fastcall ShowErrorMessage(HWND hWnd,const char * chMessage,bool DoExitProcess = false,int ExitCode = 0);
//Param1是结束符
AnsiString __fastcall RightBCDToStr(const unsigned char chSource[],int len,int Param1,int Param2);
void __fastcall RightBCDToStr(char chDest[],const unsigned char chSource[],int len,int Param1,int Param2);
//将字符串转换为BCD右对齐格式
//返回转换后的数组长度
//chSource : 要转换的字符串.字符串中只允许0-9和BCDE字符,不在此范围内的字符将被忽略
//Param1 : 是BCD的终止符
//len    : 传入的chDest的长度
int __fastcall StrToRightBCD(char chDest[],const char * chSource,int len,int Param1,int Param2);
//将一个字符串转换为BCD左对齐格式
//返回转换后的数组长度
//chSource : 要转换的字符串.字符串中只允许0-9和BCDE字符,不在此范围内的字符将被忽略
//Param1   : 是BCD的终止符
//len      : 传入的chDest的长度
int __fastcall StrToLeftBCD(char chDest[],const char * chSource,int len,int Param1,int Param2);
//BCD左对齐转换为字符串
//Param1是终止符
AnsiString __fastcall LeftBCDToStr(const unsigned char chSource[],int len,int Param1,int Param2);
//转换char 为 short int
AnsiString __fastcall ConvertToWord(const unsigned char chSource[],int len,int Param1,int Param2);
//转换char 为 unsigned int
AnsiString __fastcall ConvertToUINT(const unsigned char chSource[],int len,int Param1,int Param2);
//转换char 为 int
AnsiString __fastcall ConvertToInt(const unsigned char chSource[],int len,int Param1,int Param2);
//转换char 为 byte
AnsiString __fastcall ConvertToBYTE(const unsigned char chSource[],int len,int Param1,int Param2);
//将话单中的秒数转换为日期时间
//Param1是基准时间
AnsiString __fastcall ConvertBasedDateTime(const unsigned char chSource[],int len,int Param1,int Param2);
//转换RBCD_YYYYMMDDHHMMSS为日期时间
AnsiString __fastcall ConvertRBCDYYYYMMDDHHMMSS(const unsigned char chSource[],int len,int Param1,int Param2);
//转换RBCD_YYMMDDHHMMSS为日期时间
AnsiString __fastcall ConvertRBCDYYMMDDHHMMSS(const unsigned char chSource[],int len,int Param1,int Param2);
AnsiString __fastcall ConvertStr(const unsigned char chSource[],int len,int Param1,int Param2);
//转换STR_YYYYMMDDHHMMSS为日期时间格式
AnsiString __fastcall ConvertSTRYYYYMMDDHHMMSS(const unsigned char chSource[],int len,int Param1,int Param2);
//转换二进制日期和时间
AnsiString __fastcall ConvertBINYYMMDDHHMMSS(const unsigned char * chSource,int len,int Param1,int Param2);
//判断给定的字符串中是否含有非数字字符
bool __fastcall IsNumber(const char * chStr);
int __fastcall StrToIntEx(const char * chStr);
//返回字符串chScan在chStr中最后一将出现的基于0的位置
//如果chStr中没有找到chScan,返回-1.
int StrRScan(const char * chStr,const char * chScan,int len = -1);
//搜索一个字符在字符串中第一次出现的位置
//如果成功返回该字符在字符串中出现的基于0的位置
//否则返回-1.
int StrLScan(const char * chStr,const char * chScan,int len = -1);
AnsiString EncryFTPPassword(AnsiString Password);
AnsiString DecryFTPPassword(AnsiString Password);
void Split(AnsiString str,AnsiString Separator,TStringList * List);
AnsiString __fastcall XMLDecode(AnsiString str);
AnsiString __fastcall XMLDecodeForField(AnsiString str);
AnsiString __fastcall XMLEncode(AnsiString str);

//返回一个去掉用户名和密码之后的FTP全路径文件名
AnsiString GetFullFTPFileName(AnsiString FileName);
//读取Windows的错误描述
AnsiString __fastcall GetErrorString(int nCode);
//解密系统信息
AnsiString DecrySysInfo(AnsiString String);
//加密系统信息
AnsiString EncrySysInfo(AnsiString String);
//加密注册码
AnsiString EncryRegisteCode(AnsiString String);
//获取一个临时文件名
AnsiString GetATempFileName(AnsiString TempDirectory,AnsiString FileName);
//获取一个文件的大小
unsigned int GetLocalFileSize(AnsiString FileName);
//取字段的值
typedef AnsiString __fastcall (* ConvertFieldData)(const unsigned char chData[],int FieldLength,int Param1,int Param2);

class TBillConfig;
class TBillFile;
class TBillField;
class TBillRecord;
class TBinBillFile;

#endif
 