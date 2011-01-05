//---------------------------------------------------------------------------

#ifndef RecordDetailFrmH
#define RecordDetailFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RzPanel.hpp"
#include <ExtCtrls.hpp>
#include "RzButton.hpp"
#include "RzListVw.hpp"
#include <ComCtrls.hpp>
#include "RzEdit.hpp"
#include <Mask.hpp>
#include "BillFile.h"
#include "RzSplit.hpp"
#include <Dialogs.hpp>
#include <Menus.hpp>
#include "RzRadChk.hpp"
//---------------------------------------------------------------------------
class TfrmRecordDetail : public TForm
{
__published:	// IDE-managed Components
  TRzGroupBox *RzGroupBox1;
  TRzBitBtn *btnCancel;
  TLabel *Label1;
  TRzEdit *txtBillName;
  TLabel *Label2;
  TRzEdit *txtFileName;
  TLabel *Label3;
  TRzEdit *txtRecordPosition;
  TLabel *Label4;
  TRzEdit *txtRecordLength;
  TLabel *Label5;
  TRzBitBtn *btnCapy;
  TRzSplitter *RzSplitter1;
  TRzPanel *RzPanel1;
  TRzPanel *RzPanel2;
  TRzListView *lvFields;
  TPopupMenu *PopupMenu1;
  TMenuItem *menuCopy;
  TMenuItem *N1;
  TMenuItem *menuSaveAsText;
  TMenuItem *menuSaveAsOri;
  TSaveDialog *SaveDialog1;
  TRzRichEdit *txtRecordData;
  TRzEdit *txtBitContext;
  TLabel *Label6;
  TRzCheckBox *cbxViewAsText;
  void __fastcall btnCancelClick(TObject *Sender);
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
  void __fastcall btnCapyClick(TObject *Sender);
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall PopupMenu1Popup(TObject *Sender);
  void __fastcall menuCopyClick(TObject *Sender);
  void __fastcall menuSaveAsTextClick(TObject *Sender);
  void __fastcall menuSaveAsOriClick(TObject *Sender);
  void __fastcall lvFieldsSelectItem(TObject *Sender, TListItem *Item,
          bool Selected);
  void __fastcall lvFieldsDblClick(TObject *Sender);
  void __fastcall cbxViewAsTextClick(TObject *Sender);
private:	// User declarations
  TBillFile * BillFile;
  char * Buffer;
  int RecordLength;
  int RecordIndex;
public:		// User declarations
  void __fastcall SetBuffer(char * ABuffer,int Length);
  void __fastcall ShowFieldValues(int ARecordIndex);
  void __fastcall SetBillName(TBillFile * ABillFile,AnsiString BillName,AnsiString FileName);
  __fastcall TfrmRecordDetail(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRecordDetail *frmRecordDetail;
//---------------------------------------------------------------------------
#endif
