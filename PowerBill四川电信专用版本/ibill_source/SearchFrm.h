//---------------------------------------------------------------------------

#ifndef SearchFrmH
#define SearchFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RzPanel.hpp"
#include <ExtCtrls.hpp>
#include "RzButton.hpp"
#include <Buttons.hpp>
#include "RzCmboBx.hpp"
#include "RzEdit.hpp"
#include <Mask.hpp>
#include "RzListVw.hpp"
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include "RzPrgres.hpp"
#include <Graphics.hpp>
#include "public.h"
#include "EditSearchParamFrm.h"
#include <Menus.hpp>
#include "BillSearchThread.h"
#include "ViewBillFrm.h"
#include <ActnList.hpp>
#include "RzRadChk.hpp"
#include "SkyEdit.hpp"
#include "SkyParser.hpp"
#include <Dialogs.hpp>
#include "MSScriptControl_OCX.h"
#include <OleCtrls.hpp>
#include "RzSplit.hpp"
//---------------------------------------------------------------------------
class TfrmSearch : public TForm
{
__published:	// IDE-managed Components
  TPopupMenu *PopupMenu1;
  TMenuItem *menuClearParam;
  TActionList *ActionList1;
  TAction *Action2;
  TPopupMenu *PopupMenu2;
  TMenuItem *menuCopyFileList;
  TMenuItem *menuAutoScroll;
  TMenuItem *menuAddParameter;
  TMenuItem *menuEditParameter;
  TSaveDialog *SaveDialog1;
  TOpenDialog *OpenDialog1;
  TSkyParser *VBParser;
  TPopupMenu *PopupMenu3;
  TMenuItem *menuCopy;
  TMenuItem *menuCut;
  TMenuItem *menuPaste;
  TMenuItem *menuRedo;
  TRzGroupBox *RzGroupBox1;
  TRzGroupBox *RzGroupBox2;
  TLabel *Label5;
  TLabel *Label6;
  TLabel *Label7;
  TRzEdit *txtRetryCount;
  TRzRadioButton *rbRetry;
  TRzRadioButton *rbShowWarning;
  TRzRadioButton *rbIgnore;
  TRzRadioButton *rbAbort;
  TLabel *Label8;
  TRzBitBtn *btnQuery;
  TRzBitBtn *btnClose;
  TLabel *Label9;
  TRzComboBox *cbxBillType;
  TImage *Image1;
  TLabel *Label1;
  TLabel *Label10;
  TRzProgressBar *RzProgressBar1;
  TLabel *Label11;
  TRzProgressBar *RzProgressBar2;
  TLabel *Label4;
  TLabel *Label2;
  TRzSplitter *RzSplitter10;
  TRzPanel *RzPanel1;
  TRzPanel *RzPanel20;
  TRzListView *lvFileList;
  TToolBar *ToolBar1;
  TToolButton *btnAddParameter;
  TToolButton *btnDelParameter;
  TToolButton *btnVBScript;
  TLabel *labScriptFile;
  TRzRadioButton *rbAnd;
  TRzRadioButton *rbOr;
  TRzPanel *RzPanel2;
  TToolBar *ToolBar2;
  TToolButton *btnNewScript;
  TToolButton *btnOpenScript;
  TToolButton *btnSaveScript;
  TRzSplitter *RzSplitter1;
  TRzPanel *RzPanel3;
  TSkyEdit *SkyEdit2;
  TRzPanel *RzPanel4;
  TSkyEdit *SkyEdit1;
  TRzListView *lvParams;
  TToolButton *btnScriptHelp;
  TToolButton *btnVBHelp;
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall btnCloseClick(TObject *Sender);
  void __fastcall menuClearParamClick(TObject *Sender);
  void __fastcall PopupMenu1Popup(TObject *Sender);
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall btnQueryClick(TObject *Sender);
  void __fastcall rbRetryClick(TObject *Sender);
  void __fastcall Action2Execute(TObject *Sender);
  void __fastcall PopupMenu2Popup(TObject *Sender);
  void __fastcall menuCopyFileListClick(TObject *Sender);
  void __fastcall lvFileListCustomDrawItem(TCustomListView *Sender,
          TListItem *Item, TCustomDrawState State, bool &DefaultDraw);
  void __fastcall menuAutoScrollClick(TObject *Sender);
  void __fastcall menuAddParameterClick(TObject *Sender);
  void __fastcall menuEditParameterClick(TObject *Sender);
  void __fastcall lvParamsDragOver(TObject *Sender, TObject *Source, int X,
          int Y, TDragState State, bool &Accept);
  void __fastcall lvParamsDragDrop(TObject *Sender, TObject *Source, int X,
          int Y);
  void __fastcall cbxBillTypeChange(TObject *Sender);
  bool __fastcall FormHelp(WORD Command, int Data, bool &CallHelp);
  void __fastcall btnVBScriptClick(TObject *Sender);
  void __fastcall menuCopyClick(TObject *Sender);
  void __fastcall menuCutClick(TObject *Sender);
  void __fastcall menuPasteClick(TObject *Sender);
  void __fastcall menuRedoClick(TObject *Sender);
  void __fastcall menuNewScriptClick(TObject *Sender);
  void __fastcall menuSaveClick(TObject *Sender);
  void __fastcall menuOpenScriptClick(TObject *Sender);
  void __fastcall btnScriptHelpClick(TObject *Sender);
  void __fastcall btnVBHelpClick(TObject *Sender);
private:	// User declarations
  TBillFile * BillFile;
  TfrmEditSearchParam * frmEditSearchParam;
  TBillSearchThread   * BillSearchThread;
  TStringList * MatchFieldList;           //要查找的字段列表
  TStringList * MatchFieldOperatorList;   //查找时的操作符列表
  TStringList * MatchFieldValueList;      //查找的值列表
  TList *       MatchModeList;            //查找的匹配方式列表
  TList *       AndOrList;                //查找的上下文关系列表
  TfrmViewBill *frmViewBill;
  AnsiString    TempFileName;
  AnsiString CurScriptFileName;
  bool __fastcall SaveScript();
  AnsiString __fastcall AnalyseScript();
  TDateTime StartTime;  
public:		// User declarations
  void __fastcall ShowSearchResult();
  TStringList * FileList;                 //要查找的文件列表
  TStringList * BillNameList;             //要查找的文件的BillName列表
  __fastcall TfrmSearch(TComponent* Owner);
  AnsiString BillName;
  AnsiString FormatType;
  int FoundCount;
  bool __fastcall AddParamsToList(AnsiString ABillName);
  void __fastcall OnSearchComplete(TObject * Sender);
  //void __fastcall OnFTPWorkBegin(TMessage Message);
  //void __fastcall OnFTPWork(TMessage Message);
  void __fastcall OnSetProcessMaxCount(TMessage Message);
  void __fastcall OnSetProcessPos(TMessage Message);
  void __fastcall OnSaveTempFile(TMessage Message);
  void __fastcall OnFileProcessed(TMessage Message);
  BEGIN_MESSAGE_MAP
    //MESSAGE_HANDLER(MSG_FTP_WORK_BEGIN,TMessage,OnFTPWorkBegin)
    //MESSAGE_HANDLER(MSG_FTP_WORK,TMessage,OnFTPWork)
    MESSAGE_HANDLER(MSG_SET_PROCESS_MAX_COUNT,TMessage,OnSetProcessMaxCount)
    MESSAGE_HANDLER(MSG_SET_PROCESS_POS,TMessage,OnSetProcessPos)
    MESSAGE_HANDLER(MSG_SAVE_TO_TEMPFILE,TMessage,OnSaveTempFile)
    MESSAGE_HANDLER(MSG_FILE_PROCESSED,TMessage,OnFileProcessed)
  END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSearch *frmSearch;
//---------------------------------------------------------------------------
#endif
