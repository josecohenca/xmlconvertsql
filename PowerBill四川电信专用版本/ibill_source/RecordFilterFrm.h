//---------------------------------------------------------------------------

#ifndef RecordFilterFrmH
#define RecordFilterFrmH
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
#include "RzListVw.hpp"
#include <ComCtrls.hpp>
#include "BillConfig.h"
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TfrmRecordFilter : public TForm
{
__published:	// IDE-managed Components
  TRzGroupBox *RzGroupBox1;
  TRzBitBtn *btnOk;
  TRzBitBtn *btnCancel;
  TLabel *Label1;
  TRzEdit *txtBillName;
  TRzListView *lvFilters;
  TPopupMenu *PopupMenu1;
  TMenuItem *menuAddNew;
  TMenuItem *menuDelete;
  TMenuItem *menuEdit;
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
  void __fastcall lvFiltersDblClick(TObject *Sender);
  void __fastcall menuAddNewClick(TObject *Sender);
  void __fastcall btnOkClick(TObject *Sender);
  void __fastcall menuDeleteClick(TObject *Sender);
  bool __fastcall FormHelp(WORD Command, int Data, bool &CallHelp);
private:	// User declarations
  TList * FilterList;
  int BillRecordLength;
  bool    Changed;
  void __fastcall FillFilterList(TListItem * Item,RecordFilter * Filter);
public:		// User declarations
  __fastcall TfrmRecordFilter(TComponent* Owner,TList * AFilterList,int ABillRecordLength);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRecordFilter *frmRecordFilter;
//---------------------------------------------------------------------------
#endif
