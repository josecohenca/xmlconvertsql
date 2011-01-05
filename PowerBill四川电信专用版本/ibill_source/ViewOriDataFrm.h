//---------------------------------------------------------------------------

#ifndef ViewOriDataFrmH
#define ViewOriDataFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RzEdit.hpp"
#include <ComCtrls.hpp>
#include "RzPanel.hpp"
#include <ExtCtrls.hpp>
#include "RzButton.hpp"
#include <Mask.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include "RzCmboBx.hpp"
//---------------------------------------------------------------------------
class TfrmViewOriData : public TForm
{
__published:	// IDE-managed Components
  TRzGroupBox *RzGroupBox1;
  TRzRichEdit *txtRecordData;
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
  TPopupMenu *PopupMenu1;
  TMenuItem *menuCopy;
  TMenuItem *N1;
  TMenuItem *menuSaveAsText;
  TMenuItem *menuSaveAsOri;
  TSaveDialog *SaveDialog1;
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
  void __fastcall btnCancelClick(TObject *Sender);
  void __fastcall PopupMenu1Popup(TObject *Sender);
  void __fastcall menuCopyClick(TObject *Sender);
  void __fastcall menuSaveAsTextClick(TObject *Sender);
  void __fastcall menuSaveAsOriClick(TObject *Sender);
  void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
  char * Buffer;
  int RecordLength;
public:		// User declarations
  void __fastcall SetBuffer(char * ABuffer,int Length);
  __fastcall TfrmViewOriData(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmViewOriData *frmViewOriData;
//---------------------------------------------------------------------------
#endif
