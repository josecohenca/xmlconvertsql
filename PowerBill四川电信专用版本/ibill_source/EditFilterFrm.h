//---------------------------------------------------------------------------

#ifndef EditFilterFrmH
#define EditFilterFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RzPanel.hpp"
#include <ExtCtrls.hpp>
#include "RzButton.hpp"
#include "RzCmboBx.hpp"
#include "RzEdit.hpp"
#include "RzSpnEdt.hpp"
#include <Mask.hpp>
#include "BillConfig.h"
//---------------------------------------------------------------------------
class TfrmEditFilter : public TForm
{
__published:	// IDE-managed Components
  TRzGroupBox *RzGroupBox1;
  TRzBitBtn *btnOk;
  TRzBitBtn *btnCancel;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TLabel *Label4;
  TLabel *Label5;
  TRzComboBox *cbxAction;
  TRzSpinEdit *txtWidth;
  TRzSpinEdit *txtPos;
  TRzEdit *txtIs;
  TRzEdit *txtLength;
  TLabel *Label6;
  TLabel *Label7;
  TRzComboBox *cbxCause;
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
  void __fastcall btnOkClick(TObject *Sender);
  bool __fastcall FormHelp(WORD Command, int Data, bool &CallHelp);
private:	// User declarations
  RecordFilter * Filter;
  int BillRecordLength;
public:		// User declarations
  __fastcall TfrmEditFilter(TComponent* Owner,RecordFilter * AFilter,int ABillRecordLength);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmEditFilter *frmEditFilter;
//---------------------------------------------------------------------------
#endif
