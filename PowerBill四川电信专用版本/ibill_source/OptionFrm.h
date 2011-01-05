//---------------------------------------------------------------------------
#ifndef OptionFrmH
#define OptionFrmH
//---------------------------------------------------------------------------


#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RzButton.hpp"
#include "RzPanel.hpp"
#include <ExtCtrls.hpp>
#include "RzBtnEdt.hpp"
#include "RzEdit.hpp"
#include <Mask.hpp>
#include <Dialogs.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TfrmOption : public TForm
{
__published:	// IDE-managed Components
  TRzBitBtn *btnOk;
  TRzBitBtn *btnCancel;
  TRzGroupBox *RzGroupBox1;
  TLabel *Label1;
  TRzButtonEdit *txtBCP;
  TLabel *Label2;
  TRzButtonEdit *txtSQLLDR;
  TLabel *Label3;
  TRzButtonEdit *txtMySQLImport;
  TLabel *Label4;
  TRzButtonEdit *txtTempDir;
  TOpenDialog *OpenDialog1;
  TLabel *Label5;
  TRzEdit *txtSortConfirm;
  TLabel *Label6;
  TLabel *Label7;
  TLabel *Label8;
  TRzEdit *txtFTPTimeout;
  TLabel *Label9;
  TLabel *Label10;
  TRzEdit *txtRetryOpenCount;
  TLabel *Label11;
  TImage *Image1;
  TLabel *Label12;
  void __fastcall txtBCPButtonClick(TObject *Sender);
  void __fastcall txtSQLLDRButtonClick(TObject *Sender);
  void __fastcall txtMySQLImportButtonClick(TObject *Sender);
  void __fastcall txtTempDirButtonClick(TObject *Sender);
  void __fastcall btnOkClick(TObject *Sender);
  bool __fastcall FormHelp(WORD Command, int Data, bool &CallHelp);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmOption(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmOption *frmOption;
//---------------------------------------------------------------------------
#endif
