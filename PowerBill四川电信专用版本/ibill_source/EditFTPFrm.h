//---------------------------------------------------------------------------

#ifndef EditFTPFrmH
#define EditFTPFrmH
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
#include "FTPConfig.h"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdFTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
//---------------------------------------------------------------------------
class TfrmEditFTP : public TForm
{
__published:	// IDE-managed Components
  TRzPanel *RzPanel1;
  TRzBitBtn *btnCancel;
  TRzBitBtn *btnOk;
  TLabel *Label1;
  TLabel *Label5;
  TRzComboBox *cbxMode;
  TRzEdit *txtServer;
  TLabel *Label2;
  TRzEdit *txtUserName;
  TLabel *Label3;
  TRzEdit *txtPassword;
  TLabel *Label10;
  TRzEdit *txtPort;
  TImage *Image1;
  TLabel *Label4;
  TRzEdit *txtServerName;
  TLabel *Label6;
  TLabel *Label7;
  TRzEdit *txtTimeOut;
  TRzCheckBox *chkNoUserName;
  TIdFTP *IdFTP1;
  TLabel *Label8;
  void __fastcall Label6Click(TObject *Sender);
  void __fastcall btnOkClick(TObject *Sender);
  void __fastcall chkNoUserNameClick(TObject *Sender);
private:	// User declarations
  int DBType;
  bool __fastcall OpenConnection();
  bool __fastcall CheckParameters();
  TFTPConfig * FTPConfig;
  bool ForEdit;
public:		// User declarations
  __fastcall TfrmEditFTP(TComponent* Owner,bool AForEdit);
  AnsiString OldFTPName;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmEditFTP *frmEditFTP;
//---------------------------------------------------------------------------
#endif
