//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RecordDetailFrm.h"
#include "MainFrm.h"
#include <Clipbrd.hpp>
#include "FieldFrm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzButton"
#pragma link "RzListVw"
#pragma link "RzEdit"
#pragma link "RzSplit"
#pragma link "RzRadChk"
#pragma resource "*.dfm"
TfrmRecordDetail *frmRecordDetail;
//---------------------------------------------------------------------------
__fastcall TfrmRecordDetail::TfrmRecordDetail(TComponent* Owner)
  : TForm(Owner)
{
  Buffer      = NULL;
  RecordIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecordDetail::btnCancelClick(TObject *Sender)
{
  Close();  
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecordDetail::SetBuffer(char * ABuffer,int Length)
{
  if(Buffer != NULL)
  {
    delete[] Buffer;
    Buffer = NULL;
  }
  Buffer = new char[Length];
  memcpy(Buffer,ABuffer,Length);
  RecordLength = Length;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecordDetail::FormKeyPress(TObject *Sender, char &Key)
{
  if(Key == 27)
  {
    Key = 0;
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecordDetail::SetBillName(TBillFile * ABillFile,AnsiString BillName,AnsiString FileName)
{
  BillFile = ABillFile;
  if(BillName != txtBillName->Text)
  {
    lvFields->Items->Clear();
    TBillField * BillField;
    TListItem * Item;
    for(int n = 0;n < BillFile->FieldCount;n++)
    {
      BillField = BillFile->GetBillField(n);
      Item = lvFields->Items->Add();
      Item->Caption = BillField->Label;
      Item->SubItems->Add("");
      Item->SubItems->Add("");
      Item->SubItems->Add("");
      Item->Data = (void *)BillField;
    }
    txtBillName->Text = BillName;
  }
  txtFileName->Text = FileName;
}
void __fastcall TfrmRecordDetail::ShowFieldValues(int ARecordIndex)
{
  RecordIndex = ARecordIndex; 
  TBillField * BillField;
  TListItem * Item;
  for(int n = 0;n < lvFields->Items->Count;n++)
  {
    Item = lvFields->Items->Item[n];
    Item->SubItems->Strings[0] = "";
    Item->SubItems->Strings[1] = "";
    Item->SubItems->Strings[2] = "";
    BillField = (TBillField *)Item->Data;
    Item->SubItems->Strings[0] = BillField->GetFieldValue(false);
    Item->SubItems->Strings[1] = BillField->GetFieldValue(true);
    /*switch(BillFile->FileType)
    {
    case TXT_BILL_FORMAT:
      {
        char * Buffer = (char *)BillField->GetFieldOriData();
        Item->SubItems->Strings[2] = AnsiString(Buffer);
      }
      break;
    case BIN_BILL_FORMAT:
      {
        int Len = BillField->FieldLength;
        unsigned char * Buffer = (unsigned char *)BillField->GetFieldOriData();
        int Position = -1;
        AnsiString str;
        do
        {
          ++Position;
          if(Position >= Len)
            break;
          str = str + IntToHex( * (Buffer + Position),2) + " ";
        }while(Position < Len);
        Item->SubItems->Strings[2] = str;
      }
      break;
    }*/
  }
  lvFieldsSelectItem(NULL,lvFields->Selected,true);
}
void __fastcall TfrmRecordDetail::btnCapyClick(TObject *Sender)
{
  AnsiString Text = "������¼��ϸ��Ϣ\r\n"\
    "��������:" + txtBillName->Text + "\r\n"\
    "�����ļ�:" + txtFileName->Text + "\r\n"\
    "��¼λ��:" + txtRecordPosition->Text + "\r\n"\
    "��¼����:" + txtRecordLength->Text + "\r\n"\
    "�����Ǹü�¼�и��ֶε�ֵ\r\n\r\n"\
    "�ֶ�����\t�ֶ�����(����ǰ)\t�ֶ�����(�����)\r\n";
  TListItem * Item;
  for(int n = 0;n < lvFields->Items->Count;n++)
  {
    Item = lvFields->Items->Item[n];
    Text += Item->Caption + "\t" 
      + Item->SubItems->Strings[0] + "\t"\
      + Item->SubItems->Strings[1] + "\t"\
      + Item->SubItems->Strings[2] + "\r\n";
  }
  Text += "ԭʼ��������:\r\n" + txtRecordData->Text;
  TClipboard * Clipboard = new TClipboard();
  Clipboard->AsText = Text;
  delete Clipboard;
  MessageBox(Handle,"��¼����ϸ��Ϣ�Ѹ��Ƶ�������!","��Ϣ",MB_OK | MB_ICONINFORMATION);    
}
//---------------------------------------------------------------------------


void __fastcall TfrmRecordDetail::FormDestroy(TObject *Sender)
{
  if(Buffer != NULL)
  {
    delete[] Buffer;
    Buffer = NULL;
  }  
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecordDetail::PopupMenu1Popup(TObject *Sender)
{
  menuCopy->Enabled = txtRecordData->SelLength > 0;
  menuSaveAsOri->Enabled = Buffer != NULL;    
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecordDetail::menuCopyClick(TObject *Sender)
{
  if(txtRecordData->SelLength == 0)
    return;
  txtRecordData->CopyToClipboard();    
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecordDetail::menuSaveAsTextClick(TObject *Sender)
{
  SaveDialog1->Title = "����Ϊ�ı�";
  SaveDialog1->DefaultExt = "txt";
  SaveDialog1->Filter = "�ı��ļ�(*.txt)|*.txt";
  if(!SaveDialog1->Execute())
    return;
  FILE * file = fopen(SaveDialog1->FileName.c_str(),"w");
  if(file == NULL)
  {
    MessageBox(Handle,"д���ļ�ʧ��!","����",MB_OK | MB_ICONSTOP);
    return;
  }
  fprintf(file,"%s",txtRecordData->Text);
  fclose(file);  
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecordDetail::menuSaveAsOriClick(TObject *Sender)
{
  SaveDialog1->Title = "����Ϊԭʼ����";
  SaveDialog1->DefaultExt = "";
  SaveDialog1->Filter = "ԭʼ�����ļ�(*.*)|*.*";
  if(!SaveDialog1->Execute())
    return;
  FILE * file = fopen(SaveDialog1->FileName.c_str(),"w+b");
  if(file == NULL)
  {
    MessageBox(Handle,"д���ļ�ʧ��!","����",MB_OK | MB_ICONSTOP);
    return;
  }
  fwrite(Buffer,RecordLength,1,file);
  fclose(file);    
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecordDetail::lvFieldsSelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{
  if(Item == NULL)
    return;
  TBillField * BillField = (TBillField * )Item->Data;
  if(Selected)
  {
    if(BillFile->FileType == TXT_BILL_FORMAT|| cbxViewAsText->Checked)
    {
      if(BillField->Offset < 0)
        return;
      TStringList * List = new TStringList;
      Split(txtRecordData->Text,BillFile->Separator,List);
      int pos = 1;
      int n;
      for(n = 0;n < BillField->Offset;n++)
      {
        pos += List->Strings[n].Length();
      }
      if(pos > 0)
      {
        txtRecordData->SelStart  = pos + BillField->Offset - 1;
        txtRecordData->SelLength = List->Strings[n].Length();
      }
      else
      {
        txtRecordData->SelStart  = 0;
        txtRecordData->SelLength = 0;
      }
      delete List;
    }
    else //BIN_BILL_FORMAT:
    {
      if((BillField->Offset * 2 + BillField->Offset < 0) ||
         (BillField->FieldLength * 2 + BillField->FieldLength - 1) < 0)
      {
        txtRecordData->SelStart  = 0;
        txtRecordData->SelLength = 0;
      }
      else
      {
        Label6->Enabled        = BillField->DataType == DATATYPE_BIT;
        txtBitContext->Enabled = Label6->Enabled;
        if(Label6->Enabled)
        {
          if(!BillFile->ReadSingleRecord(RecordIndex))
          {
            return;
          }
          txtBitContext->Text = GetFieldValue_FIELD_DISPLAY_AS_BITS(BillField);
          txtBitContext->SelStart  = 8 - BillField->BitPosition - BillField->BitWidth;
          txtBitContext->SelLength = BillField->BitWidth;
          Label6->Caption = "Bitλ��:" + IntToStr(BillField->BitPosition) + ",Bit���:" + IntToStr(BillField->BitWidth);
        }
        else
        {
          txtBitContext->Text = "";
          Label6->Caption     = "";
        }
        txtRecordData->SelStart  = BillField->Offset * 2 + BillField->Offset;
        txtRecordData->SelLength = BillField->FieldLength * 2 + BillField->FieldLength - 1;
      }
    }
  }  
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecordDetail::lvFieldsDblClick(TObject *Sender)
{
  if(lvFields->Selected == NULL)
    return;
  AnsiString FieldName = ((TBillField * )lvFields->Selected->Data)->FieldName;
  _di_IXMLNode FieldNode = frmMain->BillConfig->GetFieldNode(BillFile->GetBillName(),FieldName);
  if(FieldNode == NULL)
  {
    MessageBox(Handle,("�Ҳ����ֶνڵ�[" + FieldName + "]!").c_str(),"����",MB_OK | MB_ICONSTOP);
    return;
  }    
  TfrmField * frmField = new TfrmField(this,BillFile->GetBillName(),frmMain->BillConfig,FieldNode,true);
  frmField->ShowModal();
  delete frmField;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecordDetail::cbxViewAsTextClick(TObject *Sender)
{
  //lvFieldsSelectItem(NULL,lvFields->Selected,true);  
}
//---------------------------------------------------------------------------

