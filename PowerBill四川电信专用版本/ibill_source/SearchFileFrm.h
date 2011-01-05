//---------------------------------------------------------------------------

#ifndef SearchFileFrmH
#define SearchFileFrmH
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
#include "RzSpnEdt.hpp"
#include "RzRadChk.hpp"
//---------------------------------------------------------------------------
class TfrmSearchFile : public TForm
{
__published:	// IDE-managed Components
  TRzGroupBox *RzGroupBox1;
  TRzBitBtn *btnOk;
  TRzBitBtn *btnCancel;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TRzEdit *txtFileName;
  TRzSpinEdit *txtSearchLevel;
  TRzCheckBox *chkSearchChildDir;
  void __fastcall btnOkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmSearchFile(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSearchFile *frmSearchFile;
//---------------------------------------------------------------------------
#endif
