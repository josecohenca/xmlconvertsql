//---------------------------------------------------------------------------

#ifndef EditDBFrmH
#define EditDBFrmH
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
#include <Mask.hpp>
#include <Graphics.hpp>
#include "RzRadChk.hpp"
#include <ADODB.hpp>
#include <DB.hpp>
#include "DBConfig.h"
//---------------------------------------------------------------------------
class TfrmEditDB : public TForm
{
__published:	// IDE-managed Components
  TRzPanel *RzPanel1;
  TRzBitBtn *btnCancel;
  TRzBitBtn *btnOk;
  TLabel *Label1;
  TLabel *Label5;
  TRzComboBox *cbxDBType;
  TRzEdit *txtDBServer;
  TLabel *Label2;
  TRzEdit *txtDBUserName;
  TLabel *Label3;
  TRzEdit *txtDBPassword;
  TLabel *Label9;
  TRzComboBox *cbxDataBases;
  TLabel *Label10;
  TRzEdit *txtDBPort;
  TImage *Image1;
  TLabel *Label4;
  TRzEdit *txtDBName;
  TADOConnection *ADOConnection1;
  TADODataSet *ADODataSet1;
  TLabel *Label6;
  void __fastcall cbxDBTypeChange(TObject *Sender);
  void __fastcall cbxDataBasesDropDown(TObject *Sender);
  void __fastcall Label6Click(TObject *Sender);
  void __fastcall btnOkClick(TObject *Sender);
private:	// User declarations
  int DBType;
  bool __fastcall OpenConnection();
  TDBConfig * DBConfig;
  bool ForEdit;
public:		// User declarations
  __fastcall TfrmEditDB(TComponent* Owner,bool AForEdit,TDBConfig * ADBConfig);
  AnsiString OldDBName;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmEditDB *frmEditDB;
//---------------------------------------------------------------------------
#endif
