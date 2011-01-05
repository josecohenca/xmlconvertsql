//---------------------------------------------------------------------------

#ifndef AboutFrmH
#define AboutFrmH
//---------------------------------------------------------------------------
//#include <Classes.hpp>
//#include <Controls.hpp>
//#include <StdCtrls.hpp>
//#include <Forms.hpp>
//#include <ExtCtrls.hpp>
//#include <Graphics.hpp>
//#include "CWGIFImage.hpp"
//#include "RzPanel.hpp"
//---------------------------------------------------------------------------
class TfrmAbout : public TForm
{
__published:	// IDE-managed Components
  TRzPanel *RzPanel1;
  TImage *Image1;
  TImage *Image2;
  TLabel *Label5;
  TLabel *Label1;
  TLabel *Label3;
  TLabel *Label2;
  TLabel *Label4;
  void __fastcall Label1Click(TObject *Sender);
  void __fastcall FormClick(TObject *Sender);
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
  void __fastcall Label5Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmAbout(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAbout *frmAbout;
//---------------------------------------------------------------------------
#endif
