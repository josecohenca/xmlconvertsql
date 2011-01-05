//---------------------------------------------------------------------------

#ifndef FieldFrmH
#define FieldFrmH
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
#include <Graphics.hpp>
#include "BillConfig.h"
#include "RzSpnEdt.hpp"
//---------------------------------------------------------------------------
class TfrmField : public TForm
{
__published:	// IDE-managed Components
  TRzBitBtn *btnOk;
  TRzBitBtn *btnCancel;
  TRzGroupBox *RzGroupBox1;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TLabel *Label4;
  TLabel *Label5;
  TLabel *Label6;
  TLabel *Label8;
  TLabel *Label9;
  TLabel *Label10;
  TLabel *Label11;
  TLabel *Label12;
  TLabel *Label13;
  TLabel *Label17;
  TLabel *Label18;
  TLabel *Label7;
  TLabel *Label19;
  TRzEdit *txtBillName;
  TRzEdit *txtFieldName;
  TRzEdit *txtFieldLable;
  TRzComboBox *cbxDataType;
  TRzCheckBox *chkDisabled;
  TRzEdit *txtEndChar;
  TRzCheckBox *chkDisplayInList;
  TRzComboBox *cbxSortType;
  TRzCheckBox *chkImportToDB;
  TRzButton *btnReplaceRule;
  TRzButton *btnTrans;
  TRzEdit *txtBaseDateTime;
  TRzComboBox *cbxFieldType;
  TRzEdit *txtFixedValue;
  TRzSpinEdit *txtOffset;
  TRzSpinEdit *txtLength;
  TRzSpinEdit *txtBitPosition;
  TRzSpinEdit *txtBitWidth;
  TRzPanel *RzPanel1;
  TLabel *Label14;
  TRzSpinEdit *txtDisplayWidth;
  TLabel *Label15;
  TRzComboBox *cbxAlign;
  TRzPanel *RzPanel2;
  TRzCheckBox *chkCreateIndex;
  TLabel *Label20;
  TRzSpinEdit *txtExportLength;
  TLabel *Label16;
  TRzComboBox *cbxDisplayFormat;
  void __fastcall chkDisabledClick(TObject *Sender);
  void __fastcall chkDisplayInListClick(TObject *Sender);
  void __fastcall cbxDataTypeChange(TObject *Sender);
  void __fastcall btnOkClick(TObject *Sender);
  void __fastcall btnReplaceRuleClick(TObject *Sender);
  void __fastcall btnTransClick(TObject *Sender);
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall cbxSortTypeChange(TObject *Sender);
  bool __fastcall FormHelp(WORD Command, int Data, bool &CallHelp);
  void __fastcall chkImportToDBClick(TObject *Sender);
private:	// User declarations
  _di_IXMLNode FieldNode;
  AnsiString BillName;
  AnsiString FieldName;
  AnsiString ReplaceRules;
  int FileFormatType;
  int RecordLength;
  TBillConfig * BillConfig;
  TStringList * TransList;
  bool TransListChanged;
  bool ReadOnly;
  bool __fastcall CheckParameter();
  void __fastcall SetControlStateByDataType();
public:		// User declarations
  __fastcall TfrmField(TComponent* Owner,AnsiString ABillName,TBillConfig * ABillConfig,_di_IXMLNode AFieldNode,bool AReadOnly = false);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmField *frmField;
//---------------------------------------------------------------------------
#endif
