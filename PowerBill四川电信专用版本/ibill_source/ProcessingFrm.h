//---------------------------------------------------------------------------

#ifndef ProcessingFrmH
#define ProcessingFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RzPrgres.hpp"
#include "RzButton.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmProcessing : public TForm
{
__published:	// IDE-managed Components
  TLabel *labInformation;
  TRzProgressBar *RzProgressBar1;
  TRzBitBtn *btnCancel;
  TImage *Image1;
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
  bool CloseByUser;
public:		// User declarations
  __fastcall TfrmProcessing(TComponent* Owner);
  void __fastcall CloseWindow(TModalResult Result);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmProcessing *frmProcessing;
//---------------------------------------------------------------------------
#endif
