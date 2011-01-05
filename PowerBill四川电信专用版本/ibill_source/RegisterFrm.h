//---------------------------------------------------------------------------

#ifndef RegisterFrmH
#define RegisterFrmH
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
#include "RzLabel.hpp"
#include <jpeg.hpp>
#include "CWGIFImage.hpp"
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TfrmRegister : public TForm
{
__published:	// IDE-managed Components
  TRzPanel *RzPanel1;
  TImage *Image2;
  TRzURLLabel *RzURLLabel1;
  TLabel *Label2;
  TLabel *Label1;
  TRzEdit *txtRegisterUserName;
  TRzEdit *txtRegistNumber;
  TRzBitBtn *btnRegiste;
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
  void __fastcall btnRegisteClick(TObject *Sender);
  void __fastcall RzURLLabel1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmRegister(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRegister *frmRegister;
//---------------------------------------------------------------------------
#endif
