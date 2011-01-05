//---------------------------------------------------------------------------

#ifndef EditSearchParamFrmH
#define EditSearchParamFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RzPanel.hpp"
#include <ExtCtrls.hpp>
#include "RzButton.hpp"
#include "RzEdit.hpp"
#include <Mask.hpp>
#include "RzCmboBx.hpp"
#include "RzRadChk.hpp"
#include <ActnList.hpp>
#include "RzDTP.hpp"
#include <ComCtrls.hpp>
#include "BillField.h"
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TfrmEditSearchParam : public TForm
{
__published:	// IDE-managed Components
  TRzPanel *RzPanel1;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TLabel *Label4;
  TRzComboBox *cbxOperator;
  TRzComboBox *cbxValues;
  TRzCheckBox *cbxMatch;
  TRzEdit *txtContext;
  TRzDateTimePicker *dtDate;
  TRzDateTimePicker *dtTime;
  TRzComboBox *cbxField;
  TRzComboBox *cbxAndOr;
  TRzBitBtn *btnQuery;
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
  void __fastcall btnQueryClick(TObject *Sender);
  void __fastcall cbxFieldChange(TObject *Sender);
  bool __fastcall FormHelp(WORD Command, int Data, bool &CallHelp);
private:	// User declarations
public:		// User declarations
  void __fastcall SetSelectedFieldIndex(TBillField * Field);
  __fastcall TfrmEditSearchParam(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmEditSearchParam *frmEditSearchParam;
//---------------------------------------------------------------------------
#endif
