//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ViewOriDataFrm.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzEdit"
#pragma link "RzPanel"
#pragma link "RzButton"
#pragma link "RzCmboBx"
#pragma resource "*.dfm"
TfrmViewOriData *frmViewOriData;
//---------------------------------------------------------------------------
__fastcall TfrmViewOriData::TfrmViewOriData(TComponent* Owner)
  : TForm(Owner)
{
  Buffer = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfrmViewOriData::FormKeyPress(TObject *Sender, char &Key)
{
  if(Key == 27)
  {
    Key = 0;
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmViewOriData::btnCancelClick(TObject *Sender)
{
  Close();  
}
//---------------------------------------------------------------------------
void __fastcall TfrmViewOriData::PopupMenu1Popup(TObject *Sender)
{
  menuCopy->Enabled = txtRecordData->SelLength > 0;
  menuSaveAsOri->Enabled = Buffer != NULL;  
}
//---------------------------------------------------------------------------
void __fastcall TfrmViewOriData::menuCopyClick(TObject *Sender)
{
  if(txtRecordData->SelLength == 0)
    return;
  txtRecordData->CopyToClipboard();  
}
//---------------------------------------------------------------------------
void __fastcall TfrmViewOriData::menuSaveAsTextClick(TObject *Sender)
{
  SaveDialog1->Title = "保存为文本";
  SaveDialog1->DefaultExt = "txt";
  SaveDialog1->Filter = "文本文件(*.txt)|*.txt";
  if(!SaveDialog1->Execute())
    return;
  FILE * file = fopen(SaveDialog1->FileName.c_str(),"w");
  if(file == NULL)
  {
    MessageBox(Handle,"写入文件失败!","错误",MB_OK | MB_ICONSTOP);
    return;
  }
  fprintf(file,"%s",txtRecordData->Text);
  fclose(file);
}
void __fastcall TfrmViewOriData::SetBuffer(char * ABuffer,int Length)
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
void __fastcall TfrmViewOriData::menuSaveAsOriClick(TObject *Sender)
{
  SaveDialog1->Title = "保存为原始数据";
  SaveDialog1->DefaultExt = "";
  SaveDialog1->Filter = "原始数据文件(*.*)|*.*";
  if(!SaveDialog1->Execute())
    return;
  FILE * file = fopen(SaveDialog1->FileName.c_str(),"w+b");
  if(file == NULL)
  {
    MessageBox(Handle,"写入文件失败!","错误",MB_OK | MB_ICONSTOP);
    return;
  }
  fwrite(Buffer,RecordLength,1,file);
  fclose(file);  
}
//---------------------------------------------------------------------------
void __fastcall TfrmViewOriData::FormDestroy(TObject *Sender)
{
  if(Buffer != NULL)
  {
    delete[] Buffer;
    Buffer = NULL;
  }
}
//---------------------------------------------------------------------------


