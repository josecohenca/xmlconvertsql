//---------------------------------------------------------------------------

#ifndef BillFormatFrmH
#define BillFormatFrmH
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
#include "RzListVw.hpp"
#include <ComCtrls.hpp>
#include "public.h"
#include "BillConfig.h"
#include <Menus.hpp>
#include "RzRadChk.hpp"
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfrmBillFormat : public TForm
{
__published:	// IDE-managed Components
  TRzGroupBox *RzGroupBox1;
  TRzGroupBox *RzGroupBox2;
  TRzComboBox *cbxFileFormatType;
  TLabel *Label2;
  TLabel *Label4;
  TRzEdit *txtFileHeaderLength;
  TLabel *Label1;
  TRzEdit *txtBillName;
  TLabel *Label3;
  TRzEdit *txtDescription;
  TLabel *Label5;
  TLabel *Label6;
  TRzEdit *txtFileTrailLength;
  TLabel *Label7;
  TLabel *Label8;
  TRzEdit *txtRecordLength;
  TLabel *Label9;
  TLabel *Label10;
  TRzEdit *txtSeparator;
  TLabel *Label12;
  TRzEdit *txtBlockLength;
  TLabel *Label13;
  TLabel *Label11;
  TRzEdit *txtBlockHeaderLength;
  TLabel *Label14;
  TLabel *Label15;
  TRzEdit *txtBlockTrailLength;
  TLabel *Label16;
  TRzButton *btnAction;
  TRzListView *lvFields;
  TRzBitBtn *btnSaveFieldConfig;
  TPopupMenu *PopupMenu1;
  TMenuItem *menuNewField;
  TMenuItem *menuEditField;
  TMenuItem *menuDeleteField;
  TRzCheckBox *chkReadByBlock;
  TCheckBox *cbxDisabled;
  void __fastcall cbxFileFormatTypeChange(TObject *Sender);
  void __fastcall lvFieldsDblClick(TObject *Sender);
  void __fastcall PopupMenu1Popup(TObject *Sender);
  void __fastcall btnSaveFieldConfigClick(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall menuNewFieldClick(TObject *Sender);
  void __fastcall menuDeleteFieldClick(TObject *Sender);
  void __fastcall lvFieldsDragOver(TObject *Sender, TObject *Source, int X,
          int Y, TDragState State, bool &Accept);
  void __fastcall lvFieldsDragDrop(TObject *Sender, TObject *Source, int X,
          int Y);
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
  void __fastcall btnActionClick(TObject *Sender);
  void __fastcall chkReadByBlockClick(TObject *Sender);
  void __fastcall FormDestroy(TObject *Sender);
  bool __fastcall FormHelp(WORD Command, int Data, bool &CallHelp);
private:	// User declarations
  bool Modified;
  bool __fastcall CheckBillParameter();
  bool __fastcall CommitBillFormat();
  bool __fastcall SaveChangeToFile(bool ShowHint);
  void __fastcall ShowFieldInList(_di_IXMLNode FieldNode,TListItem * Item);
public:		// User declarations
  AnsiString BillName;
  TBillConfig * BillConfig;
  TList * FilterList;
  bool BillChanged;
  __fastcall TfrmBillFormat(TComponent* Owner,AnsiString ABillName,TBillConfig * ABillConfig);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmBillFormat *frmBillFormat;
//---------------------------------------------------------------------------
#endif
