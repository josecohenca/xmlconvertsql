//����

#ifndef PUBLIC_H
#define PUBLIC_H

#define _WINSOCKAPI_

//����ѡ��
#define SCTELE_COM_VERSION  //�Ĵ�����ר�ð汾,�ü�������BIOS���кź�CPU��Ϣ��Ϊ���к�

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




#define TRY_VERSION_FILE_COUNT  10    //���ð��������������ļ���


#define MSG_SET_PROCESS_POS       WM_USER + 1000  //���ý�������λ��.wParam�ǵ�ǰ�������
#define MSG_SET_PROCESS_MAX_COUNT WM_USER + 1005  //֪ͨ�������ý����������ֵ.WParam�����ֵ.LParam�Ǹ�����Ϣ,�ɷ����ߺͽ�����ȷ��
#define MSG_CUSTOMER_KEYDOWN      WM_USER + 1006  //�������Զ����,WParam��CTRL�Ƿ񱻰���,LParam�Ǽ���ASCII��

#define MSG_FTP_WORK_BEGIN        WM_USER + 1002  //ͨ��FTP���ؼ�����ʼ,wParam��һ��ָ���ļ������ַ�����ָ��,lParam���ļ��Ĵ�С
#define MSG_FTP_WORK              WM_USER + 1004  //ͨ��FTP�������ڽ���,wParam��һ��ָ���ļ������ַ�����ָ��,lParam�ǵ�ǰ�������
#define MSG_THREAD_COMPLETED      WM_USER + 1100  //֪ͨ�������̵߳Ĺ����ѽ���,wParam���̵߳�ָ��

#define MSG_SAVE_TO_TEMPFILE      WM_USER + 1200  //֪ͨ�����ߴ�������д��ʱ�ļ�

#define MSG_OPEN_FILE_ERROR       WM_USER + 1300  //֪ͨ�����ߴ��ڴ��ļ�ʧ��,WParam����ʧ�ܵĴ���,LParam�Ǵ����������Ϣ

#define BIN_BILL_FORMAT 1             //�����ƻ�����ʽ
#define TXT_BILL_FORMAT 2             //�ı��ļ��Ļ�����ʽ
#define SEARCH_RESULT_FORMAT 9999     //��׼������ʽ,�����ڲ�ʹ��

#define BIN_BILL_FORMAT_STR  "�������ļ�"
#define TXT_BILL_FORMAT_STR  "�ı��ļ�"
#define ANS_BILL_FORMAT_STR  "ANS.1�������ļ�"
#define TXT1_BILL_FORMAT_STR "�������ı��ļ�"
#define SR_BILL_FORMAT_STR   "SearchResult"

#define SORT_AS_STR      1
#define SORT_AS_DATETIME 2
#define SORT_AS_NUMBER   3
#define SORT_AS_CURRENCY 4

#define REPLACE_LEFT  1
#define REPLACE_RIGHT 2
#define REPLACE_ANY   3

//�������Ͷ���
#define DATATYPE_LBCD                     1   //BCD�������
#define DATATYPE_RBCD                     2   //BCD���Ҷ���
#define DATATYPE_STR                      3   //�ַ���
#define DATATYPE_LBCD_YYYYMMDDHHMMSS      4   //BCD�����������ʱ��(��λ���).
#define DATATYPE_RBCD_YYYYMMDDHHMMSS      5   //BCD���Ҷ�������ʱ��(��λ���).
#define DATATYPE_LBCD_YYMMDDHHMMSS		    6   //BCD�����������ʱ��(��λ���).
#define DATATYPE_RBCD_YYMMDDHHMMSS		    7   //BCD���Ҷ�������ʱ��(��λ���).
#define DATATYPE_BASED_DATETIME	  	      8   //�л�׼ʱ�������ʱ���ʽ
#define DATATYPE_STR_YYYY_MM_DD_HH_MM_SS  9   //�ı��ļ�����ʱ���ֶ�.��ʽΪ:YYYY-MM-DD HH:MM:SS
#define DATATYPE_UINT                     10  // �޷��ų�����(4�ֽ�)
#define DATATYPE_WORD                     11  //�޷��Ŷ�����(2�ֽ�)
#define DATATYPE_BYTE                     12  //�ֽ�(1Byte),ͳһΪunsigned char
#define DATATYPE_BIT                      13  //BITλ
#define DATATYPE_CURRENCY                 14  //����(��ȷ��С�������λ).
#define DATATYPE_STR_YYYYMMDDHHMMSS       15  //�ı��ļ����ڸ�ʽ�ֶ�(��λ���).
#define DATATYPE_BIN_YYMMDDHHMMSS         16  //����������ʱ��
#define DATATYPE_FIXED                    17  //�̶�ֵ
#define DATATYPE_FILENAME                 18  //ȡ��¼���ڵ��ļ���
#define DATATYPE_BILLNAME                 19  //ȡ��������
#define DATATYPE_SHORTFILENAME            20  //����·�����ļ���
#define DATATYPE_INT                      21  //�з��ų�����
#define DATATYPE_DURATION                 22  //ͨ��ʱ��
#define DATATYPE_RECORD_POSITION          23  //��¼λ��


//�ֶ����Ͷ���
#define FIELDTYPE_NORMAL        1     //��ͨ�ֶ�
#define FIELDTYPE_STARTTIME     2     //��ʼʱ��
#define FIELDTYPE_ENDTIME       3     //����ʱ��
#define FIELDTYPE_RECORDTYPE    4     //��¼��־
#define FIELDTYPE_RECORDLENGTH  5     //��¼����

#define FIELDTYPE_SEARCH_RESULT_FILENAME 100 //�ڲ�����:��ѯ������ڵ�ԭʼ�ļ���
#define FIELDTYPE_SEARCH_RESULT_POS      101 //�ڲ�����:��ѯ������ڵ�ԭʼ��¼λ��



#define ALIGN_LEFT    1
#define ALIGN_CENTER  2
#define ALIGN_RIGHT   3

#define FIELD_DISPLAY_AS_TEXT 1
#define FIELD_DISPLAY_AS_BITS 2

#define EXPORT_DBF 1
#define EXPORT_CSV 2
#define EXPORT_TXT 3
#define EXPORT_ORI 4


#define RECORD_FILTER_SKIP 1  //����ָ���ļ�¼
//#define RECORD_FILTER_READ 2  //��ȡָ���ļ�¼

//�򿪻����ļ�ʱ����Ĵ���ʽ
#define ON_ERROR_RETRY   1 //����ʱ�Զ�����
#define ON_ERROR_WARNING 2 //����ʱ��ʾMessageBox
#define ON_ERROR_ABORT   3 //����ʱ��ֹ����
#define ON_ERROR_IGNORE  4 //���Դ����������


//�������������ݿ�ʱת���ļ�����
//#define CONVERT_TO_STD_FORMAT 1   //ת��Ϊ��׼��ʽ
//#define CONVERT_TO_ORI_BILL   2   //ת��ΪConfig.xml�ж���ĸ�ʽ(����ѡ���˵�һ��������ʱ��Ч)

//FTP������Կ
const char FTP_PASSWORD_KEY[] = "APPLICATION_198705#_*@#$(*&$(*@&!($*&#*&(*&(*^123521@(*)!";
//ϵͳ��Ϣ��Կ
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
//д��־
void __fastcall SaveLog(const char * chMessage);
//����ʱд��־����ʾ������Ϣ
void __fastcall ShowErrorMessage(HWND hWnd,const char * chMessage,bool DoExitProcess = false,int ExitCode = 0);
//Param1�ǽ�����
AnsiString __fastcall RightBCDToStr(const unsigned char chSource[],int len,int Param1,int Param2);
void __fastcall RightBCDToStr(char chDest[],const unsigned char chSource[],int len,int Param1,int Param2);
//���ַ���ת��ΪBCD�Ҷ����ʽ
//����ת��������鳤��
//chSource : Ҫת�����ַ���.�ַ�����ֻ����0-9��BCDE�ַ�,���ڴ˷�Χ�ڵ��ַ���������
//Param1 : ��BCD����ֹ��
//len    : �����chDest�ĳ���
int __fastcall StrToRightBCD(char chDest[],const char * chSource,int len,int Param1,int Param2);
//��һ���ַ���ת��ΪBCD������ʽ
//����ת��������鳤��
//chSource : Ҫת�����ַ���.�ַ�����ֻ����0-9��BCDE�ַ�,���ڴ˷�Χ�ڵ��ַ���������
//Param1   : ��BCD����ֹ��
//len      : �����chDest�ĳ���
int __fastcall StrToLeftBCD(char chDest[],const char * chSource,int len,int Param1,int Param2);
//BCD�����ת��Ϊ�ַ���
//Param1����ֹ��
AnsiString __fastcall LeftBCDToStr(const unsigned char chSource[],int len,int Param1,int Param2);
//ת��char Ϊ short int
AnsiString __fastcall ConvertToWord(const unsigned char chSource[],int len,int Param1,int Param2);
//ת��char Ϊ unsigned int
AnsiString __fastcall ConvertToUINT(const unsigned char chSource[],int len,int Param1,int Param2);
//ת��char Ϊ int
AnsiString __fastcall ConvertToInt(const unsigned char chSource[],int len,int Param1,int Param2);
//ת��char Ϊ byte
AnsiString __fastcall ConvertToBYTE(const unsigned char chSource[],int len,int Param1,int Param2);
//�������е�����ת��Ϊ����ʱ��
//Param1�ǻ�׼ʱ��
AnsiString __fastcall ConvertBasedDateTime(const unsigned char chSource[],int len,int Param1,int Param2);
//ת��RBCD_YYYYMMDDHHMMSSΪ����ʱ��
AnsiString __fastcall ConvertRBCDYYYYMMDDHHMMSS(const unsigned char chSource[],int len,int Param1,int Param2);
//ת��RBCD_YYMMDDHHMMSSΪ����ʱ��
AnsiString __fastcall ConvertRBCDYYMMDDHHMMSS(const unsigned char chSource[],int len,int Param1,int Param2);
AnsiString __fastcall ConvertStr(const unsigned char chSource[],int len,int Param1,int Param2);
//ת��STR_YYYYMMDDHHMMSSΪ����ʱ���ʽ
AnsiString __fastcall ConvertSTRYYYYMMDDHHMMSS(const unsigned char chSource[],int len,int Param1,int Param2);
//ת�����������ں�ʱ��
AnsiString __fastcall ConvertBINYYMMDDHHMMSS(const unsigned char * chSource,int len,int Param1,int Param2);
//�жϸ������ַ������Ƿ��з������ַ�
bool __fastcall IsNumber(const char * chStr);
int __fastcall StrToIntEx(const char * chStr);
//�����ַ���chScan��chStr�����һ�����ֵĻ���0��λ��
//���chStr��û���ҵ�chScan,����-1.
int StrRScan(const char * chStr,const char * chScan,int len = -1);
//����һ���ַ����ַ����е�һ�γ��ֵ�λ��
//����ɹ����ظ��ַ����ַ����г��ֵĻ���0��λ��
//���򷵻�-1.
int StrLScan(const char * chStr,const char * chScan,int len = -1);
AnsiString EncryFTPPassword(AnsiString Password);
AnsiString DecryFTPPassword(AnsiString Password);
void Split(AnsiString str,AnsiString Separator,TStringList * List);
AnsiString __fastcall XMLDecode(AnsiString str);
AnsiString __fastcall XMLDecodeForField(AnsiString str);
AnsiString __fastcall XMLEncode(AnsiString str);

//����һ��ȥ���û���������֮���FTPȫ·���ļ���
AnsiString GetFullFTPFileName(AnsiString FileName);
//��ȡWindows�Ĵ�������
AnsiString __fastcall GetErrorString(int nCode);
//����ϵͳ��Ϣ
AnsiString DecrySysInfo(AnsiString String);
//����ϵͳ��Ϣ
AnsiString EncrySysInfo(AnsiString String);
//����ע����
AnsiString EncryRegisteCode(AnsiString String);
//��ȡһ����ʱ�ļ���
AnsiString GetATempFileName(AnsiString TempDirectory,AnsiString FileName);
//��ȡһ���ļ��Ĵ�С
unsigned int GetLocalFileSize(AnsiString FileName);
//ȡ�ֶε�ֵ
typedef AnsiString __fastcall (* ConvertFieldData)(const unsigned char chData[],int FieldLength,int Param1,int Param2);

class TBillConfig;
class TBillFile;
class TBillField;
class TBillRecord;
class TBinBillFile;

#endif
 