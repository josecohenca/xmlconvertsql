//---------------------------------------------------------------------------

#ifndef MonitorFrmH
#define MonitorFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RzEdit.hpp"
#include <ComCtrls.hpp>
#include "RzButton.hpp"
#include <ImgList.hpp>
#include <Dialogs.hpp>
#include "RzPanel.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmMonitor : public TForm
{
__published:	// IDE-managed Components
  TRzBitBtn *btnClose;
  TRzBitBtn *btnSaveAS;
  TImageList *ImageList1;
  TSaveDialog *SaveDialog1;
  TRzPanel *RzPanel1;
  TRzMemo *txtMessages;
  void __fastcall btnCloseClick(TObject *Sender);
  void __fastcall btnSaveASClick(TObject *Sender);
  void __fastcall txtMessagesChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmMonitor(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMonitor *frmMonitor;
//---------------------------------------------------------------------------
#endif
