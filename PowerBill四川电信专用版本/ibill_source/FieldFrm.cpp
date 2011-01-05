//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FieldFrm.h"
#include "MainFrm.h"
#include "BillFormatFrm.h"
#include "ReplaceRuleFrm.h"
#include "ValueTransFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzButton"
#pragma link "RzEdit"
#pragma link "RzCmboBx"
#pragma link "RzRadChk"
#pragma link "RzSpnEdt"
#pragma resource "*.dfm"
TfrmField *frmField;
//---------------------------------------------------------------------------
__fastcall TfrmField::TfrmField(TComponent* Owner,AnsiString ABillName,TBillConfig * ABillConfig,_di_IXMLNode AFieldNode,bool AReadOnly)
  : TForm(Owner)
{
  BillConfig   = ABillConfig;
  BillName     = ABillName;
  FieldNode    = AFieldNode;
  ReadOnly = AReadOnly;
  RecordLength      = BillConfig->GetFileRecordLength(BillName);
  txtBillName->Text = BillName;

  FileFormatType    = BillConfig->GetFileFormatType(BillName);
  switch(FileFormatType)
  {
  case BIN_BILL_FORMAT:
    cbxDataType->Items->AddObject("BCD��������ַ���",(TObject *)DATATYPE_LBCD);
    cbxDataType->Items->AddObject("BCD���Ҷ����ַ���",(TObject *)DATATYPE_RBCD);
    cbxDataType->Items->AddObject("ASCII�ַ���",(TObject *)DATATYPE_STR);
    cbxDataType->Items->AddObject("BCD�����������ʱ��(YYYY-MM-DD HH:MM:SS)",(TObject *)DATATYPE_LBCD_YYYYMMDDHHMMSS);
    cbxDataType->Items->AddObject("BCD���Ҷ�������ʱ��(YYYY-MM-DD HH:MM:SS)",(TObject *)DATATYPE_RBCD_YYYYMMDDHHMMSS);
    cbxDataType->Items->AddObject("BCD���Ҷ�������ʱ��(YY-MM-DD HH:MM:SS)",(TObject *)DATATYPE_RBCD_YYMMDDHHMMSS);
    //cbxDataType->Items->AddObject("BCD�����������ʱ��(YY-MM-DD HH:MM:SS)",(TObject *)DATATYPE_LBCD_YYMMDDHHMMSS);
    //cbxDataType->Items->AddObject("BCD���Ҷ�������ʱ��(YY-MM-DD HH:MM:SS)",(TObject *)DATATYPE_RBCD_YYMMDDHHMMSS);
    cbxDataType->Items->AddObject("����������ʱ��(YY-MM-DD HH:MM:SS)",(TObject *)DATATYPE_BIN_YYMMDDHHMMSS);
    //cbxDataType->Items->AddObject("����������ʱ��(YY-MM-DD HH:MM:SS)",(TObject *)DATATYPE_BIN_YYMMDDHHMMSS);
    cbxDataType->Items->AddObject("�л�׼ʱ�������ʱ���ʽ",(TObject *)DATATYPE_BASED_DATETIME);
    cbxDataType->Items->AddObject("ͨ��ʱ��(�����Զ�����)",(TObject *)DATATYPE_DURATION);
    cbxDataType->Items->AddObject("�ı��ļ�����ʱ���ֶ�(YYYY-MM-DD HH:MM:SS)",(TObject *)DATATYPE_STR_YYYY_MM_DD_HH_MM_SS);
    cbxDataType->Items->AddObject("�ı��ļ�����ʱ���ʽ�ֶ�(YYYYMMDDHHMMSS)",(TObject *)DATATYPE_STR_YYYYMMDDHHMMSS);
    cbxDataType->Items->AddObject("�޷��ų�����(4Byte)",(TObject *)DATATYPE_UINT);
    cbxDataType->Items->AddObject("�з��ų�����(4Byte)",(TObject *)DATATYPE_INT);
    cbxDataType->Items->AddObject("�޷��Ŷ�����(2Byte)",(TObject *)DATATYPE_WORD);
    cbxDataType->Items->AddObject("�ֽ�(1Byte)",(TObject *)DATATYPE_BYTE);
    cbxDataType->Items->AddObject("BITλ",(TObject *)DATATYPE_BIT);
    cbxDataType->Items->AddObject("�̶�ֵ",(TObject *)DATATYPE_FIXED);
    cbxDataType->Items->AddObject("��·�����ļ�����",(TObject *)DATATYPE_FILENAME);
    cbxDataType->Items->AddObject("����·�����ļ���",(TObject *)DATATYPE_SHORTFILENAME);
    cbxDataType->Items->AddObject("��������",(TObject *)DATATYPE_BILLNAME);
    //cbxDataType->Items->AddObject("��¼λ��",(TObject *)DATATYPE_RECORD_POSITION);
    break;
  case TXT_BILL_FORMAT:
    cbxDataType->Items->AddObject("ASCII�ַ���",(TObject *)DATATYPE_STR);
    cbxDataType->Items->AddObject("�ı��ļ�����ʱ���ֶ�(YYYY-MM-DD HH:MM:SS)",(TObject *)DATATYPE_STR_YYYY_MM_DD_HH_MM_SS);
    cbxDataType->Items->AddObject("�ı��ļ�����ʱ���ʽ�ֶ�(YYYYMMDDHHMMSS)",(TObject *)DATATYPE_STR_YYYYMMDDHHMMSS);
    cbxDataType->Items->AddObject("ͨ��ʱ��(�����Զ�����)",(TObject *)DATATYPE_DURATION);
    cbxDataType->Items->AddObject("����(��ȷ��С�������λ)",(TObject *)DATATYPE_CURRENCY);
    cbxDataType->Items->AddObject("�̶�ֵ",(TObject *)DATATYPE_FIXED);
    cbxDataType->Items->AddObject("��·�����ļ�����",(TObject *)DATATYPE_FILENAME);
    cbxDataType->Items->AddObject("����·�����ļ���",(TObject *)DATATYPE_SHORTFILENAME);
    cbxDataType->Items->AddObject("��������",(TObject *)DATATYPE_BILLNAME);
    //cbxDataType->Items->AddObject("��¼λ��",(TObject *)DATATYPE_RECORD_POSITION);
    break;
  }
  cbxAlign->Items->AddObject("�������",(TObject *)ALIGN_LEFT);
  cbxAlign->Items->AddObject("���ж���",(TObject *)ALIGN_CENTER);
  cbxAlign->Items->AddObject("���Ҷ���",(TObject *)ALIGN_RIGHT);
  cbxDisplayFormat->Items->AddObject("Ĭ��ת��",(TObject *)FIELD_DISPLAY_AS_TEXT);
  cbxDisplayFormat->Items->AddObject("ת��Ϊ�����ƴ���",(TObject *)FIELD_DISPLAY_AS_BITS);

  cbxSortType->Items->AddObject("�ı�",(TObject *)SORT_AS_STR);
  cbxSortType->Items->AddObject("����ʱ��",(TObject *)SORT_AS_DATETIME);
  cbxSortType->Items->AddObject("����",(TObject *)SORT_AS_NUMBER);
  cbxSortType->Items->AddObject("����",(TObject *)SORT_AS_CURRENCY);

  cbxFieldType->Items->AddObject("��ͨ",(TObject *)FIELDTYPE_NORMAL);
  cbxFieldType->Items->AddObject("ͨ����ʼʱ��",(TObject *)FIELDTYPE_STARTTIME);
  cbxFieldType->Items->AddObject("ͨ������ʱ��",(TObject *)FIELDTYPE_ENDTIME);
  //cbxFieldType->Items->AddObject("��¼����",(TObject *)FIELDTYPE_RECORDTYPE);   

  TransList = new TStringList();
  if(FieldNode != NULL)
  {
    txtFieldName->Text    = BillConfig->GetFieldAttributeValue(FieldNode,"FieldName");
    FieldName             = txtFieldName->Text;
    txtFieldLable->Text   = BillConfig->GetFieldAttributeValue(FieldNode,"Label");
    chkDisabled->Checked  = BillConfig->GetFieldAttributeValue(FieldNode,"Disabled","0") == "0";
    txtOffset->Text       = BillConfig->GetFieldAttributeValue(FieldNode,"Offset","0");
    txtLength->Text       = BillConfig->GetFieldAttributeValue(FieldNode,"FieldLength","0");
    txtBitPosition->Text  = BillConfig->GetFieldAttributeValue(FieldNode,"BitPosition","0");
    txtBitWidth->Text     = BillConfig->GetFieldAttributeValue(FieldNode,"BitWidth","0");
    txtEndChar->Text      = BillConfig->GetFieldAttributeValue(FieldNode,"EndChar","F");
    txtDisplayWidth->Text = BillConfig->GetFieldAttributeValue(FieldNode,"DisplayWidth","100");
    txtBaseDateTime->Text = BillConfig->GetFieldAttributeValue(FieldNode,"BaseDateTime","1900-01-01 00:00:00");
    chkImportToDB->Checked    = BillConfig->GetFieldAttributeValue(FieldNode,"Import","1") == "1";
    chkCreateIndex->Checked   = BillConfig->GetFieldAttributeValue(FieldNode,"CreateIndex","0") == "1";
    chkDisplayInList->Checked = BillConfig->GetFieldAttributeValue(FieldNode,"DisplayInList","1") == "1";
    txtFixedValue->Text       = BillConfig->GetFieldAttributeValue(FieldNode,"FixedValue");
    txtExportLength->Text     = BillConfig->GetFieldAttributeValue(FieldNode,"ExportLength","255");
    int DataType   = BillConfig->TransFieldDataType(BillConfig->GetFieldAttributeValue(FieldNode,"DataType","STR"));
    cbxDataType->ItemIndex = cbxDataType->Items->IndexOfObject((TObject *)DataType);

    int AlignMode = BillConfig->TransAlignMode(BillConfig->GetFieldAttributeValue(FieldNode,"DisplayAlign","LEFT"));
    cbxAlign->ItemIndex = cbxAlign->Items->IndexOfObject((TObject *)AlignMode);

    int DisplayFormat = BillConfig->TransDisplayFormat(BillConfig->GetFieldAttributeValue(FieldNode,"DisplayFormat","TEXT"));
    cbxDisplayFormat->ItemIndex = cbxDisplayFormat->Items->IndexOfObject((TObject *)DisplayFormat);

    int SortType = BillConfig->TransSortType(BillConfig->GetFieldAttributeValue(FieldNode,"SortType","STR"));
    cbxSortType->ItemIndex = cbxSortType->Items->IndexOfObject((TObject *)SortType);

    int FieldType = BillConfig->TransFieldType(BillConfig->GetFieldAttributeValue(FieldNode,"FieldType","NORMAL"));
    cbxFieldType->ItemIndex = cbxFieldType->Items->IndexOfObject((TObject *)FieldType);
    ReplaceRules = BillConfig->GetFieldReplaceRules(BillName,FieldName);
    BillConfig->GetFieldTrans(BillName,FieldName,TransList);
  }
  else
  {
    chkDisabled->Checked        = true;
    chkImportToDB->Checked      = true;
    chkDisplayInList->Checked   = true;
    cbxDataType->ItemIndex      = 0;
    cbxFieldType->ItemIndex     = 0;
    cbxAlign->ItemIndex         = 0;
    cbxDisplayFormat->ItemIndex = 0;
    cbxSortType->ItemIndex      = 0;
    txtDisplayWidth->Text       = "100";
    txtEndChar->Text            = "E";
    txtExportLength->Text       = "255";
    Caption = "������ֶ�";
    ReplaceRules = "";
  }
  if(FileFormatType == TXT_BILL_FORMAT)
    cbxDisplayFormat->ItemIndex = 0;
  if(ReadOnly)
  {
    Caption = "�ֶ�����";
    btnOk->Visible = false;
    btnCancel->Caption = "�ر�";
    btnReplaceRule->Enabled = true;
    btnTrans->Enabled = true;
    for(int n = 0;n < RzGroupBox1->ControlCount;n++)
    {
      if(RzGroupBox1->Controls[n]->Name != "btnReplaceRule" && RzGroupBox1->Controls[n]->Name != "btnTrans")
        RzGroupBox1->Controls[n]->Enabled = false;
    }
  }
  else
    SetControlStateByDataType();
  TransListChanged = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmField::chkDisabledClick(TObject *Sender)
{
/*
  for(int n = 0;n < RzGroupBox1->ControlCount;n++)
  {
    if(RzGroupBox1->Controls[n]->Tag == 999)
      RzGroupBox1->Controls[n]->Enabled = chkDisabled->Checked;
  }
  Label5->Enabled    = chkDisabled->Checked && FileFormatType != TXT_BILL_FORMAT;
  txtLength->Enabled = Label5->Enabled;
  Label6->Enabled    = Label5->Enabled;
  cbxDisplayFormat->Enabled = Label5->Enabled;
  cbxDataTypeChange(NULL);
  txtOffset->Enabled       = chkDisabled->Checked;
  cbxSortType->Enabled     = chkDisabled->Checked;
  txtExportLength->Enabled = chkDisabled->Checked;
*/
  Label2->Enabled    = chkDisabled->Checked;
  txtOffset->Enabled = Label2->Enabled;
  Label5->Enabled = Label2->Enabled;
  txtLength->Enabled = Label2->Enabled;
  Label6->Enabled = Label2->Enabled;
  Label7->Enabled = Label2->Enabled;
  cbxFieldType->Enabled = Label2->Enabled;
  Label8->Enabled = Label2->Enabled;
  cbxDataType->Enabled = Label2->Enabled;
  Label9->Enabled = Label2->Enabled;
  txtBitPosition->Enabled = Label2->Enabled;
  Label10->Enabled = Label2->Enabled;
  Label11->Enabled = Label2->Enabled;
  txtBitWidth->Enabled = Label2->Enabled;
  Label12->Enabled = Label2->Enabled;
  Label13->Enabled = Label2->Enabled;
  txtEndChar->Enabled = Label2->Enabled;
  Label19->Enabled = Label2->Enabled;
  txtFixedValue->Enabled = Label2->Enabled;
  Label18->Enabled = Label2->Enabled;
  txtBaseDateTime->Enabled = Label2->Enabled;
  Label17->Enabled = Label2->Enabled;
  cbxSortType->Enabled = Label2->Enabled;

  chkImportToDB->Enabled = Label2->Enabled;
  RzPanel2->Enabled = chkImportToDB->Checked;
/*
  chkCreateIndex->Enabled = Label2->Enabled;
  Label20->Enabled = Label2->Enabled;
  txtExportLength->Enabled = Label2->Enabled;
*/

  btnReplaceRule->Enabled = Label2->Enabled;
  btnTrans->Enabled = Label2->Enabled;

  chkDisplayInList->Enabled = Label2->Enabled;
  RzPanel1->Enabled = chkDisplayInList->Checked;
  Label16->Enabled = Label2->Enabled;
  cbxDisplayFormat->Enabled = Label2->Enabled;

/*
  Label14->Enabled = Label2->Enabled;
  txtDisplayWidth->Enabled = Label2->Enabled;
  Label15->Enabled = Label2->Enabled;
  cbxAlign->Enabled = Label2->Enabled;
*/
  //cbxDataTypeChange(NULL);
  //cbxSortTypeChange(NULL);
  //chkImportToDBClick(NULL);
  //chkDisplayInListClick(NULL);
  if(chkDisabled->Checked)
  {
    /*
    Label14->Enabled = chkDisplayInList->Checked;
    txtDisplayWidth->Enabled = chkDisplayInList->Checked;
    Label15->Enabled  = chkDisplayInList->Checked;
    cbxAlign->Enabled = chkDisplayInList->Checked;
    Label16->Enabled  = chkDisplayInList->Checked;
    cbxDisplayFormat->Enabled = Label2->Enabled;
    */
    SetControlStateByDataType();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmField::chkDisplayInListClick(TObject *Sender)
{
  RzPanel1->Enabled = chkDisplayInList->Checked;
  //SetControlStateByDataType(); 
}
//---------------------------------------------------------------------------

void __fastcall TfrmField::SetControlStateByDataType()
{
  if(cbxDataType->ItemIndex < 0)
    return;
  int DataType = (int)(cbxDataType->Items->Objects[cbxDataType->ItemIndex]);
  switch(DataType)
  {
  case DATATYPE_LBCD:
  case DATATYPE_RBCD:
    Label2->Enabled = true; //ƫ����
    txtOffset->Enabled = true;

    Label5->Enabled = true; //�ֶγ���
    txtLength->Enabled = true;
    Label6->Enabled = true; //Byte


    Label9->Enabled = false; //BITλ��
    txtBitPosition->Enabled = false;
    Label10->Enabled = false; //Bit

    Label11->Enabled = false; //BIT���
    txtBitWidth->Enabled = false;
    Label12->Enabled = false; //Bit

    Label13->Enabled = true; //BCD��ֹ��
    txtEndChar->Enabled = true;

    Label19->Enabled = false; //�̶�ֵ
    txtFixedValue->Enabled = false;

    Label18->Enabled = false; //��׼ʱ��
    txtBaseDateTime->Enabled = false;

    Label17->Enabled = true; //�߼���������
    cbxSortType->Enabled = true;


    btnReplaceRule->Enabled = true;

    Label16->Enabled = false;//FileFormatType != TXT_BILL_FORMAT && RzPanel1->Enabled; //��ʾ��ʽ
    cbxDisplayFormat->Enabled = false;//FileFormatType != TXT_BILL_FORMAT && RzPanel1->Enabled;
    cbxDisplayFormat->ItemIndex = 0;
    btnTrans->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_NUMBER;

    Label20->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_STR;
    txtExportLength->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_STR;

    Label7->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_DATETIME; //�ֶ�����
    cbxFieldType->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_DATETIME;
    if(!cbxFieldType->Enabled)
      cbxFieldType->ItemIndex = 0;
    break;
  case DATATYPE_STR:
    Label2->Enabled = true; //ƫ����
    txtOffset->Enabled = true;

    Label5->Enabled = FileFormatType != TXT_BILL_FORMAT; //�ֶγ���
    txtLength->Enabled = Label5->Enabled;
    Label6->Enabled = Label5->Enabled; //Byte


    Label9->Enabled = false; //BITλ��
    txtBitPosition->Enabled = false;
    Label10->Enabled = false; //Bit

    Label11->Enabled = false; //BIT���
    txtBitWidth->Enabled = false;
    Label12->Enabled = false; //Bit

    Label13->Enabled = false; //BCD��ֹ��
    txtEndChar->Enabled = false;

    Label19->Enabled = false; //�̶�ֵ
    txtFixedValue->Enabled = false;

    Label18->Enabled = false; //��׼ʱ��
    txtBaseDateTime->Enabled = false;

    Label17->Enabled = true; //�߼���������
    cbxSortType->Enabled = true;


    btnReplaceRule->Enabled = true;

    Label16->Enabled = FileFormatType != TXT_BILL_FORMAT; //��ʾ��ʽ
    cbxDisplayFormat->Enabled = FileFormatType != TXT_BILL_FORMAT;
    //cbxDisplayFormat->ItemIndex = 0;
    btnTrans->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_NUMBER;

    Label20->Enabled = RzPanel2->Enabled && (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_STR; //��������
    txtExportLength->Enabled = Label20->Enabled;
    
    Label7->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_DATETIME; //�ֶ�����
    cbxFieldType->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_DATETIME;
    if(!cbxFieldType->Enabled)
      cbxFieldType->ItemIndex = 0;
    break;
  case DATATYPE_RECORD_POSITION:
  case DATATYPE_DURATION:
    Label2->Enabled = false; //ƫ����
    txtOffset->Enabled = false;
    txtOffset->Text = 0;

    Label5->Enabled = false; //�ֶγ���
    txtLength->Enabled = false;
    txtLength->Text = 0;
    Label6->Enabled = false; //Byte

    Label7->Enabled = false; //�ֶ�����
    cbxFieldType->Enabled = false;
    cbxFieldType->ItemIndex = 0;

    Label9->Enabled = false; //BITλ��
    txtBitPosition->Enabled = false;
    Label10->Enabled = false; //Bit

    Label11->Enabled = false; //BIT���
    txtBitWidth->Enabled = false;
    Label12->Enabled = false; //Bit

    Label13->Enabled = false; //BCD��ֹ��
    txtEndChar->Enabled = false;

    Label19->Enabled = false; //�̶�ֵ
    txtFixedValue->Enabled = false;

    Label18->Enabled = false; //��׼ʱ��
    txtBaseDateTime->Enabled = false;

    Label17->Enabled = false; //�߼���������
    cbxSortType->Enabled = false;

    Label20->Enabled = false; //��������
    txtExportLength->Enabled = false;

    btnReplaceRule->Enabled = true;
    btnTrans->Enabled = false;

    Label16->Enabled = false; //��ʾ��ʽ
    cbxDisplayFormat->Enabled = false;
    cbxDisplayFormat->ItemIndex = 0;
    cbxSortType->ItemIndex = cbxSortType->Items->IndexOfObject((TObject * )SORT_AS_NUMBER);
    break;
  case DATATYPE_LBCD_YYYYMMDDHHMMSS:
  case DATATYPE_RBCD_YYYYMMDDHHMMSS:
    Label2->Enabled = true; //ƫ����
    txtOffset->Enabled = true;

    Label5->Enabled = false; //�ֶγ���
    txtLength->Enabled = false;
    txtLength->Text = 7;
    Label6->Enabled = false; //Byte

    Label9->Enabled = false; //BITλ��
    txtBitPosition->Enabled = false;
    Label10->Enabled = false; //Bit

    Label11->Enabled = false; //BIT���
    txtBitWidth->Enabled = false;
    Label12->Enabled = false; //Bit

    Label13->Enabled = true; //BCD��ֹ��
    txtEndChar->Enabled = true;

    Label19->Enabled = false; //�̶�ֵ
    txtFixedValue->Enabled = false;

    Label18->Enabled = false; //��׼ʱ��
    txtBaseDateTime->Enabled = false;

    Label17->Enabled = false; //�߼���������
    cbxSortType->Enabled = false;

    Label20->Enabled = false; //��������
    txtExportLength->Enabled = false;

    btnReplaceRule->Enabled = true;
    btnTrans->Enabled = false;

    Label16->Enabled = false; //��ʾ��ʽ
    cbxDisplayFormat->Enabled = false;
    cbxDisplayFormat->ItemIndex = 0;
    
    cbxSortType->ItemIndex = cbxSortType->Items->IndexOfObject((TObject * )SORT_AS_DATETIME);
    cbxDisplayFormat->ItemIndex = cbxDisplayFormat->Items->IndexOfObject((TObject *)FIELD_DISPLAY_AS_TEXT);
    Label7->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_DATETIME; //�ֶ�����
    cbxFieldType->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_DATETIME;
    if(!cbxFieldType->Enabled)
      cbxFieldType->ItemIndex = 0;
    break;
  case DATATYPE_STR_YYYYMMDDHHMMSS:
  case DATATYPE_STR_YYYY_MM_DD_HH_MM_SS:
    Label5->Enabled    = true;
    Label2->Enabled = true; //ƫ����
    txtOffset->Enabled = true;

    Label5->Enabled = FileFormatType != TXT_BILL_FORMAT; //�ֶγ���
    txtLength->Enabled = Label5->Enabled;
    Label6->Enabled = Label5->Enabled; //Byte

    Label9->Enabled = false; //BITλ��
    txtBitPosition->Enabled = false;
    Label10->Enabled = false; //Bit

    Label11->Enabled = false; //BIT���
    txtBitWidth->Enabled = false;
    Label12->Enabled = false; //Bit

    Label13->Enabled = false; //BCD��ֹ��
    txtEndChar->Enabled = false;

    Label19->Enabled = false; //�̶�ֵ
    txtFixedValue->Enabled = false;

    Label18->Enabled = false; //��׼ʱ��
    txtBaseDateTime->Enabled = false;

    Label17->Enabled = false; //�߼���������
    cbxSortType->Enabled = false;

    Label20->Enabled = false; //��������
    txtExportLength->Enabled = false;

    btnReplaceRule->Enabled = true;
    btnTrans->Enabled = false;

    Label16->Enabled = false; //��ʾ��ʽ
    cbxDisplayFormat->Enabled = false;
    cbxDisplayFormat->ItemIndex = 0;
    cbxSortType->ItemIndex = cbxSortType->Items->IndexOfObject((TObject * )SORT_AS_DATETIME);
    cbxDisplayFormat->ItemIndex = cbxDisplayFormat->Items->IndexOfObject((TObject *)FIELD_DISPLAY_AS_TEXT);
    Label7->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_DATETIME; //�ֶ�����
    cbxFieldType->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_DATETIME;
    if(!cbxFieldType->Enabled)
      cbxFieldType->ItemIndex = 0;
    break;
  case DATATYPE_BIN_YYMMDDHHMMSS:
    Label2->Enabled = true; //ƫ����
    txtOffset->Enabled = true;

    Label5->Enabled = false; //�ֶγ���
    txtLength->Enabled = false;
    txtLength->Text = 6;
    Label6->Enabled = false; //Byte

    Label9->Enabled = false; //BITλ��
    txtBitPosition->Enabled = false;
    Label10->Enabled = false; //Bit

    Label11->Enabled = false; //BIT���
    txtBitWidth->Enabled = false;
    Label12->Enabled = false; //Bit

    Label13->Enabled = false; //BCD��ֹ��
    txtEndChar->Enabled = false;

    Label19->Enabled = false; //�̶�ֵ
    txtFixedValue->Enabled = false;

    Label18->Enabled = false; //��׼ʱ��
    txtBaseDateTime->Enabled = false;

    Label17->Enabled = false; //�߼���������
    cbxSortType->Enabled = false;

    Label20->Enabled = false; //��������
    txtExportLength->Enabled = false;

    btnReplaceRule->Enabled = true;
    btnTrans->Enabled = false;

    Label16->Enabled = false; //��ʾ��ʽ
    cbxDisplayFormat->Enabled = false;
    cbxDisplayFormat->ItemIndex = 0;
    
    cbxSortType->ItemIndex = cbxSortType->Items->IndexOfObject((TObject * )SORT_AS_DATETIME);
    cbxDisplayFormat->ItemIndex = cbxDisplayFormat->Items->IndexOfObject((TObject *)FIELD_DISPLAY_AS_TEXT);
    Label7->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_DATETIME; //�ֶ�����
    cbxFieldType->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_DATETIME;
    if(!cbxFieldType->Enabled)
      cbxFieldType->ItemIndex = 0;
    break;  
  case DATATYPE_LBCD_YYMMDDHHMMSS:
  case DATATYPE_RBCD_YYMMDDHHMMSS:
    Label2->Enabled = true; //ƫ����
    txtOffset->Enabled = true;

    Label5->Enabled = false; //�ֶγ���
    txtLength->Enabled = false;
    txtLength->Text = 6;
    Label6->Enabled = false; //Byte

    Label9->Enabled = false; //BITλ��
    txtBitPosition->Enabled = false;
    Label10->Enabled = false; //Bit

    Label11->Enabled = false; //BIT���
    txtBitWidth->Enabled = false;
    Label12->Enabled = false; //Bit

    Label13->Enabled = true; //BCD��ֹ��
    txtEndChar->Enabled = true;

    Label19->Enabled = false; //�̶�ֵ
    txtFixedValue->Enabled = false;

    Label18->Enabled = false; //��׼ʱ��
    txtBaseDateTime->Enabled = false;

    Label17->Enabled = false; //�߼���������
    cbxSortType->Enabled = false;

    Label20->Enabled = false; //��������
    txtExportLength->Enabled = false;

    btnReplaceRule->Enabled = true;
    btnTrans->Enabled = false;

    Label16->Enabled = false; //��ʾ��ʽ
    cbxDisplayFormat->Enabled = false;
    cbxDisplayFormat->ItemIndex = 0;

    cbxSortType->ItemIndex = cbxSortType->Items->IndexOfObject((TObject * )SORT_AS_DATETIME);
    cbxDisplayFormat->ItemIndex = cbxDisplayFormat->Items->IndexOfObject((TObject *)FIELD_DISPLAY_AS_TEXT);
    Label7->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_DATETIME; //�ֶ�����
    cbxFieldType->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_DATETIME;
    if(!cbxFieldType->Enabled)
      cbxFieldType->ItemIndex = 0;
    break;
  case DATATYPE_BASED_DATETIME:
    Label2->Enabled = true; //ƫ����
    txtOffset->Enabled = true;

    Label5->Enabled = true; //�ֶγ���
    txtLength->Enabled = true;
    Label6->Enabled = true; //Byte

    Label9->Enabled = false; //BITλ��
    txtBitPosition->Enabled = false;
    Label10->Enabled = false; //Bit

    Label11->Enabled = false; //BIT���
    txtBitWidth->Enabled = false;
    Label12->Enabled = false; //Bit

    Label13->Enabled = false; //BCD��ֹ��
    txtEndChar->Enabled = false;

    Label19->Enabled = false; //�̶�ֵ
    txtFixedValue->Enabled = false;

    Label18->Enabled = true; //��׼ʱ��
    txtBaseDateTime->Enabled = true;

    Label17->Enabled = false; //�߼���������
    cbxSortType->Enabled = false;

    Label20->Enabled = false; //��������
    txtExportLength->Enabled = false;

    btnReplaceRule->Enabled = true;
    btnTrans->Enabled = false;

    Label16->Enabled = false; //��ʾ��ʽ
    cbxDisplayFormat->Enabled = false;
    cbxDisplayFormat->ItemIndex = 0;

    cbxSortType->ItemIndex = cbxSortType->Items->IndexOfObject((TObject * )SORT_AS_DATETIME);
    cbxDisplayFormat->ItemIndex = cbxDisplayFormat->Items->IndexOfObject((TObject *)FIELD_DISPLAY_AS_TEXT);    
    Label7->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_DATETIME; //�ֶ�����
    cbxFieldType->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_DATETIME;
    if(!cbxFieldType->Enabled)
      cbxFieldType->ItemIndex = 0;
    break;
  case DATATYPE_UINT:
  case DATATYPE_INT:
    Label2->Enabled = true; //ƫ����
    txtOffset->Enabled = true;

    Label5->Enabled = false; //�ֶγ���
    txtLength->Enabled = false;
    txtLength->Text = 4;
    Label6->Enabled = false; //Byte


    Label9->Enabled = false; //BITλ��
    txtBitPosition->Enabled = false;
    Label10->Enabled = false; //Bit

    Label11->Enabled = false; //BIT���
    txtBitWidth->Enabled = false;
    Label12->Enabled = false; //Bit

    Label13->Enabled = false; //BCD��ֹ��
    txtEndChar->Enabled = false;

    Label19->Enabled = false; //�̶�ֵ
    txtFixedValue->Enabled = false;

    Label18->Enabled = false; //��׼ʱ��
    txtBaseDateTime->Enabled = false;

    Label17->Enabled = false; //�߼���������
    cbxSortType->Enabled = false;

    Label20->Enabled = false; //��������
    txtExportLength->Enabled = false;
    txtExportLength->Text = 10; 

    btnReplaceRule->Enabled = true;

    Label16->Enabled = false;//FileFormatType != TXT_BILL_FORMAT; //��ʾ��ʽ
    cbxDisplayFormat->Enabled = false;//FileFormatType != TXT_BILL_FORMAT;
    cbxSortType->ItemIndex = cbxSortType->Items->IndexOfObject((TObject * )SORT_AS_NUMBER);

    cbxDisplayFormat->ItemIndex = 0;
    btnTrans->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_NUMBER;

    Label7->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_DATETIME; //�ֶ�����
    cbxFieldType->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_DATETIME;
    if(!cbxFieldType->Enabled)
      cbxFieldType->ItemIndex = 0;
    if(!cbxDisplayFormat->Enabled)
      cbxDisplayFormat->ItemIndex = 0;
    break;
  case DATATYPE_WORD:
    Label2->Enabled = true; //ƫ����
    txtOffset->Enabled = true;

    Label5->Enabled = false; //�ֶγ���
    txtLength->Enabled = false;
    txtLength->Text = 2;
    Label6->Enabled = false; //Byte

    Label7->Enabled = false; //�ֶ�����
    cbxFieldType->Enabled = false;
    cbxFieldType->ItemIndex = 0;

    Label9->Enabled = false; //BITλ��
    txtBitPosition->Enabled = false;
    Label10->Enabled = false; //Bit

    Label11->Enabled = false; //BIT���
    txtBitWidth->Enabled = false;
    Label12->Enabled = false; //Bit

    Label13->Enabled = false; //BCD��ֹ��
    txtEndChar->Enabled = false;

    Label19->Enabled = false; //�̶�ֵ
    txtFixedValue->Enabled = false;

    Label18->Enabled = false; //��׼ʱ��
    txtBaseDateTime->Enabled = false;

    Label17->Enabled = false; //�߼���������
    cbxSortType->Enabled = false;

    Label20->Enabled = false; //��������
    txtExportLength->Enabled = false;
    txtExportLength->Text = 10; 

    btnReplaceRule->Enabled = true;

    Label16->Enabled = false;//FileFormatType != TXT_BILL_FORMAT; //��ʾ��ʽ
    cbxDisplayFormat->Enabled = false;//FileFormatType != TXT_BILL_FORMAT;
    cbxSortType->ItemIndex = cbxSortType->Items->IndexOfObject((TObject * )SORT_AS_NUMBER);
    cbxFieldType->ItemIndex = 0;
    cbxDisplayFormat->ItemIndex = 0;
    btnTrans->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_NUMBER;

    Label7->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_DATETIME; //�ֶ�����
    cbxFieldType->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_DATETIME;
    if(!cbxFieldType->Enabled)
      cbxFieldType->ItemIndex = 0;
    break;
  case DATATYPE_BYTE:
    Label2->Enabled = true; //ƫ����
    txtOffset->Enabled = true;

    Label5->Enabled = false; //�ֶγ���
    txtLength->Enabled = false;
    txtLength->Text = 1;
    Label6->Enabled = false; //Byte

    Label7->Enabled = false; //�ֶ�����
    cbxFieldType->Enabled = false;
    cbxFieldType->ItemIndex = 0;

    Label9->Enabled = false; //BITλ��
    txtBitPosition->Enabled = false;
    Label10->Enabled = false; //Bit

    Label11->Enabled = false; //BIT���
    txtBitWidth->Enabled = false;
    Label12->Enabled = false; //Bit

    Label13->Enabled = false; //BCD��ֹ��
    txtEndChar->Enabled = false;

    Label19->Enabled = false; //�̶�ֵ
    txtFixedValue->Enabled = false;

    Label18->Enabled = false; //��׼ʱ��
    txtBaseDateTime->Enabled = false;

    Label17->Enabled = false; //�߼���������
    cbxSortType->Enabled = false;

    Label20->Enabled = false; //��������
    txtExportLength->Enabled = false;
    txtExportLength->Text = 10; 

    btnReplaceRule->Enabled = true;

    Label16->Enabled = false;//FileFormatType != TXT_BILL_FORMAT; //��ʾ��ʽ
    cbxDisplayFormat->Enabled = false;//FileFormatType != TXT_BILL_FORMAT;
    cbxSortType->ItemIndex = cbxSortType->Items->IndexOfObject((TObject * )SORT_AS_NUMBER);
    cbxDisplayFormat->ItemIndex = 0;
    cbxFieldType->ItemIndex = 0;
    btnTrans->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_NUMBER;
    Label7->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_DATETIME; //�ֶ�����
    cbxFieldType->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_DATETIME;
    if(!cbxFieldType->Enabled)
      cbxFieldType->ItemIndex = 0;
    break;
  case DATATYPE_BIT:
    Label2->Enabled = true; //ƫ����
    txtOffset->Enabled = true;

    Label5->Enabled = false; //�ֶγ���
    txtLength->Enabled = false;
    txtLength->Text = 1;
    Label6->Enabled = false; //Byte

    Label7->Enabled = false; //�ֶ�����
    cbxFieldType->Enabled = false;
    cbxFieldType->ItemIndex = 0;

    Label9->Enabled = true; //BITλ��
    txtBitPosition->Enabled = true;
    Label10->Enabled = true; //Bit

    Label11->Enabled = true; //BIT���
    txtBitWidth->Enabled = true;
    Label12->Enabled = true; //Bit

    Label13->Enabled = false; //BCD��ֹ��
    txtEndChar->Enabled = false;

    Label19->Enabled = false; //�̶�ֵ
    txtFixedValue->Enabled = false;

    Label18->Enabled = false; //��׼ʱ��
    txtBaseDateTime->Enabled = false;

    Label17->Enabled = false; //�߼���������
    cbxSortType->Enabled = false;

    Label20->Enabled = false; //��������
    txtExportLength->Enabled = false;
    txtExportLength->Text = 10; 

    btnReplaceRule->Enabled = true;

    Label16->Enabled = false;//FileFormatType != TXT_BILL_FORMAT; //��ʾ��ʽ
    cbxDisplayFormat->Enabled = false;//FileFormatType != TXT_BILL_FORMAT;
    cbxSortType->ItemIndex = cbxSortType->Items->IndexOfObject((TObject * )SORT_AS_NUMBER);
    cbxDisplayFormat->ItemIndex = 0;
    cbxFieldType->ItemIndex = 0;
    btnTrans->Enabled = (int)cbxSortType->Items->Objects[cbxSortType->ItemIndex] == SORT_AS_NUMBER;
    break;  
  case DATATYPE_CURRENCY:
    Label2->Enabled = true; //ƫ����
    txtOffset->Enabled = true;

    Label5->Enabled = false; //�ֶγ���
    txtLength->Enabled = false;
    Label6->Enabled = false; //Byte

    Label7->Enabled = false; //�ֶ�����
    cbxFieldType->Enabled = false;
    cbxFieldType->ItemIndex = 0;

    Label9->Enabled = false; //BITλ��
    txtBitPosition->Enabled = false;
    Label10->Enabled = false; //Bit

    Label11->Enabled = false; //BIT���
    txtBitWidth->Enabled = false;
    Label12->Enabled = false; //Bit

    Label13->Enabled = false; //BCD��ֹ��
    txtEndChar->Enabled = false;

    Label19->Enabled = false; //�̶�ֵ
    txtFixedValue->Enabled = false;

    Label18->Enabled = false; //��׼ʱ��
    txtBaseDateTime->Enabled = false;

    Label17->Enabled = false; //�߼���������
    cbxSortType->Enabled = false;

    Label20->Enabled = false; //��������
    txtExportLength->Enabled = false;
    txtExportLength->Text = 10; 

    btnReplaceRule->Enabled = true;
    btnTrans->Enabled = false;

    Label16->Enabled = false; //��ʾ��ʽ
    cbxDisplayFormat->Enabled = false;
    cbxSortType->ItemIndex = cbxSortType->Items->IndexOfObject((TObject * )SORT_AS_CURRENCY);
    cbxFieldType->ItemIndex = 0;
    cbxDisplayFormat->ItemIndex = 0;
    break;  
  case DATATYPE_FIXED:
    Label2->Enabled = false; //ƫ����
    txtOffset->Enabled = false;
    txtOffset->Text = 0;

    Label5->Enabled = false; //�ֶγ���
    txtLength->Enabled = false;
    txtLength->Text = 0;
    Label6->Enabled = false; //Byte

    Label7->Enabled = false; //�ֶ�����
    cbxFieldType->Enabled = false;
    cbxFieldType->ItemIndex = 0;

    Label9->Enabled = false; //BITλ��
    txtBitPosition->Enabled = false;
    Label10->Enabled = false; //Bit

    Label11->Enabled = false; //BIT���
    txtBitWidth->Enabled = false;
    Label12->Enabled = false; //Bit

    Label13->Enabled = false; //BCD��ֹ��
    txtEndChar->Enabled = false;

    Label19->Enabled = true; //�̶�ֵ
    txtFixedValue->Enabled = true;

    Label18->Enabled = false; //��׼ʱ��
    txtBaseDateTime->Enabled = false;

    Label17->Enabled = true; //�߼���������
    cbxSortType->Enabled = true;

    Label20->Enabled = RzPanel2->Enabled; //��������
    txtExportLength->Enabled = RzPanel2->Enabled;

    btnReplaceRule->Enabled = false;


    Label16->Enabled = false; //��ʾ��ʽ
    cbxDisplayFormat->Enabled = false;
    cbxDisplayFormat->ItemIndex = 0;
    cbxFieldType->ItemIndex = 0;
    break;  
  case DATATYPE_FILENAME:
  case DATATYPE_BILLNAME:
  case DATATYPE_SHORTFILENAME:
    Label2->Enabled = false; //ƫ����
    txtOffset->Enabled = false;
    txtOffset->Text = 0;

    Label5->Enabled = false; //�ֶγ���
    txtLength->Enabled = false;
    txtLength->Text = 0;
    Label6->Enabled = false; //Byte

    Label7->Enabled = false; //�ֶ�����
    cbxFieldType->Enabled = false;
    cbxFieldType->ItemIndex = 0;

    Label9->Enabled = false; //BITλ��
    txtBitPosition->Enabled = false;
    Label10->Enabled = false; //Bit

    Label11->Enabled = false; //BIT���
    txtBitWidth->Enabled = false;
    Label12->Enabled = false; //Bit

    Label13->Enabled = false; //BCD��ֹ��
    txtEndChar->Enabled = false;

    Label19->Enabled = false; //�̶�ֵ
    txtFixedValue->Enabled = false;

    Label18->Enabled = false; //��׼ʱ��
    txtBaseDateTime->Enabled = false;

    Label17->Enabled = false; //�߼���������
    cbxSortType->Enabled = false;

    Label20->Enabled = RzPanel2->Enabled; //��������
    txtExportLength->Enabled = RzPanel2->Enabled;
    txtExportLength->Text = 255; 

    btnReplaceRule->Enabled = false;
    btnTrans->Enabled = false;

    Label16->Enabled = false; //��ʾ��ʽ
    cbxDisplayFormat->Enabled = false;
    cbxDisplayFormat->ItemIndex = 0;
    cbxSortType->ItemIndex = cbxSortType->Items->IndexOfObject((TObject * )SORT_AS_STR);
    cbxFieldType->ItemIndex = 0;
    break;
  }
  RzPanel1->Enabled = chkDisplayInList->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmField::cbxDataTypeChange(TObject *Sender)
{
  SetControlStateByDataType();
/*
  Label9->Enabled = DataType == DATATYPE_BIT && chkDisabled->Checked;
  txtBitPosition->Enabled = Label9->Enabled;
  Label10->Enabled        = Label9->Enabled;
  Label11->Enabled        = Label9->Enabled;
  txtBitWidth->Enabled    = Label9->Enabled;
  Label12->Enabled        = Label9->Enabled;
  Label13->Enabled    = (DataType == DATATYPE_LBCD ||
        DataType == DATATYPE_RBCD ||
        DataType == DATATYPE_LBCD_YYYYMMDDHHMMSS ||
        DataType == DATATYPE_RBCD_YYYYMMDDHHMMSS ||
        DataType == DATATYPE_LBCD_YYMMDDHHMMSS ||
        DataType == DATATYPE_RBCD_YYMMDDHHMMSS)&& chkDisabled->Checked;
  txtEndChar->Enabled      = Label13->Enabled;
  txtFixedValue->Enabled   = DataType == DATATYPE_FIXED && chkDisabled->Checked;
  Label19->Enabled         = txtFixedValue->Enabled;
  Label18->Enabled         = DataType == DATATYPE_BASED_DATETIME && chkDisabled->Checked;
  txtBaseDateTime->Enabled = Label18->Enabled;
  txtOffset->Enabled = DataType != DATATYPE_DURATION;
  cbxSortType->Enabled     = txtOffset->Enabled;
  Label5->Enabled    = DataType != DATATYPE_DURATION && chkDisabled->Checked;
  txtLength->Enabled = Label5->Enabled;
  if(DataType == DATATYPE_DURATION)
  {
    cbxSortType->ItemIndex   = cbxSortType->Items->IndexOfObject((TObject *)SORT_AS_NUMBER);
    Label20->Enabled         = chkDisabled->Checked;
    txtExportLength->Enabled = chkDisabled->Checked;
  }
  if(DataType != DATATYPE_LBCD_YYYYMMDDHHMMSS &&
        DataType != DATATYPE_RBCD_YYYYMMDDHHMMSS &&
        DataType != DATATYPE_LBCD_YYMMDDHHMMSS &&
        DataType != DATATYPE_RBCD_YYMMDDHHMMSS &&
        DataType != DATATYPE_RBCD_YYMMDDHHMMSS &&
        DataType != DATATYPE_STR_YYYY_MM_DD_HH_MM_SS &&
        DataType != DATATYPE_STR_YYYYMMDDHHMMSS &&
        DataType != DATATYPE_BASED_DATETIME
        )
  {
    cbxFieldType->Enabled = chkDisabled->Checked;
    //cbxFieldType->ItemIndex = 0;
  }
  else
    cbxFieldType->Enabled = true;
  cbxDisplayFormat->Enabled = FileFormatType != TXT_BILL_FORMAT;
  cbxSortTypeChange(NULL);
*/  
}
//---------------------------------------------------------------------------
void __fastcall TfrmField::btnOkClick(TObject *Sender)
{
  if(ReadOnly)
    return;
  if(FieldNode == NULL) //������ֶ�
  {
    if(!CheckParameter())
      return;
    BillConfig->AddNewField(
      txtBillName->Text,
      txtFieldName->Text,
      !chkDisabled->Checked,
      txtFieldLable->Text,
      StrToInt(txtOffset->Text),
      StrToInt(txtLength->Text),
      (int)(cbxFieldType->Items->Objects[cbxFieldType->ItemIndex]),
      (int)(cbxDataType->Items->Objects[cbxDataType->ItemIndex]),
      StrToInt(txtBitPosition->Text),
      StrToInt(txtBitWidth->Text),
      txtEndChar->Text,
      txtFixedValue->Text,
      StrToDateTime(txtBaseDateTime->Text),
      (int)(cbxSortType->Items->Objects[cbxSortType->ItemIndex]),
      chkImportToDB->Checked,
      chkCreateIndex->Checked,
      chkDisplayInList->Checked,
      StrToInt(txtDisplayWidth->Text),
      (int)(cbxAlign->Items->Objects[cbxAlign->ItemIndex]),
      (int)(cbxDisplayFormat->Items->Objects[cbxDisplayFormat->ItemIndex]),
      StrToInt(txtExportLength->Text),
      TransList);
    BillConfig->SetFiledReplaceRule(txtBillName->Text,txtFieldName->Text,ReplaceRules);
  }
  else
  {
    //����Ƿ��޸Ĺ��ֶ�����
    bool Changed =
      (FieldName != txtFieldName->Text) ||
      (BillConfig->GetFieldAttributeValue(FieldNode,"Label") != txtFieldLable->Text) ||
      ((BillConfig->GetFieldAttributeValue(FieldNode,"Disabled","0") == "0") != chkDisabled->Checked) ||
      (BillConfig->GetFieldAttributeValue(FieldNode,"Offset") != txtOffset->Text) ||
      (BillConfig->GetFieldAttributeValue(FieldNode,"FieldLength","0") != txtLength->Text) ||
      (BillConfig->TransFieldType(BillConfig->GetFieldAttributeValue(FieldNode,"FieldType","NORMAL")) !=
        (int)(cbxFieldType->Items->Objects[cbxFieldType->ItemIndex])) ||
      (BillConfig->TransFieldDataType(BillConfig->GetFieldAttributeValue(FieldNode,"DataType")) !=
        (int)(cbxDataType->Items->Objects[cbxDataType->ItemIndex])) ||
      (BillConfig->GetFieldAttributeValue(FieldNode,"BitPosition","") != txtBitPosition->Text) ||
      (BillConfig->GetFieldAttributeValue(FieldNode,"BitWidth","") != txtBitWidth->Text) ||      
      (BillConfig->GetFieldAttributeValue(FieldNode,"EndChar","") != txtEndChar->Text) ||      
      (BillConfig->GetFieldAttributeValue(FieldNode,"FixedValue","") != txtFixedValue->Text) ||      
      (BillConfig->GetFieldAttributeValue(FieldNode,"BaseDateTime","") != txtBaseDateTime->Text) ||
      (BillConfig->GetFieldAttributeValue(FieldNode,"BaseDateTime","") != txtBaseDateTime->Text) ||
      (BillConfig->TransSortType(BillConfig->GetFieldAttributeValue(FieldNode,"SortType","STR")) !=
        (int)(cbxSortType->Items->Objects[cbxSortType->ItemIndex])) ||
      (BillConfig->TransAlignMode(BillConfig->GetFieldAttributeValue(FieldNode,"DisplayAlign","LEFT")) !=
        (int)(cbxAlign->Items->Objects[cbxAlign->ItemIndex])) ||
      ((BillConfig->GetFieldAttributeValue(FieldNode,"Import","1") == "1") != chkImportToDB->Checked) ||
      ((BillConfig->GetFieldAttributeValue(FieldNode,"CreateIndex","1") == "1") != chkCreateIndex->Checked) ||
      ((BillConfig->GetFieldAttributeValue(FieldNode,"DisplayInList","1") == "1") != chkDisplayInList->Checked) ||
      (BillConfig->GetFieldAttributeValue(FieldNode,"DisplayWidth","") != txtDisplayWidth->Text) ||
      (BillConfig->TransAlignMode(BillConfig->GetFieldAttributeValue(FieldNode,"DisplayAlign","LEFT")) !=
        (int)(cbxAlign->Items->Objects[cbxAlign->ItemIndex])) ||
      (BillConfig->TransDisplayFormat(BillConfig->GetFieldAttributeValue(FieldNode,"DisplayFormat","TEXT")) !=
        (int)(cbxDisplayFormat->Items->Objects[cbxDisplayFormat->ItemIndex]) ||
      ReplaceRules != BillConfig->GetFieldReplaceRules(BillName,FieldName) || TransListChanged ||
      txtExportLength->Text != BillConfig->GetFieldAttributeValue(FieldNode,"ExportLength"));
    if(Changed)
    {
      if(!CheckParameter())
        return;
      BillConfig->ModifyField(
        txtBillName->Text,
        FieldName,
        txtFieldName->Text,
        !chkDisabled->Checked,
        txtFieldLable->Text,
        StrToInt(txtOffset->Text),
        StrToInt(txtLength->Text),
        (int)(cbxFieldType->Items->Objects[cbxFieldType->ItemIndex]),
        (int)(cbxDataType->Items->Objects[cbxDataType->ItemIndex]),
        StrToInt(txtBitPosition->Text),
        StrToInt(txtBitWidth->Text),
        txtEndChar->Text,
        txtFixedValue->Text,
        StrToDateTime(txtBaseDateTime->Text),
        (int)(cbxSortType->Items->Objects[cbxSortType->ItemIndex]),
        chkImportToDB->Checked,
        chkCreateIndex->Checked,
        chkDisplayInList->Checked,
        StrToInt(txtDisplayWidth->Text),
        (int)(cbxAlign->Items->Objects[cbxAlign->ItemIndex]),
        (int)(cbxDisplayFormat->Items->Objects[cbxDisplayFormat->ItemIndex]),
        StrToInt(txtExportLength->Text),
        TransList);
      BillConfig->SetFiledReplaceRule(BillName,txtFieldName->Text,ReplaceRules);
    }
  }
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
bool __fastcall TfrmField::CheckParameter()
{
  if(txtFieldName->Text == "")
  {
    MessageBox(Handle,"���������ֶ�����!","��ʾ",MB_OK | MB_ICONWARNING);
    txtFieldName->SetFocus();
    return false;
  }
  char chFieldName[51];
  strcpy(&chFieldName[0],txtFieldName->Text.c_str());
  for(int n = 0;n < txtFieldName->Text.Length();n++)
  {
    if((chFieldName[n] > 47 && chFieldName[n] < 58) ||
       (chFieldName[n] > 64 && chFieldName[n] < 91) ||
       (chFieldName[n] > 96 && chFieldName[n] < 123) ||
       (chFieldName[n] == 95))
      continue;
    else
    {
      MessageBox(Handle,"�ֶ�������ֻ���������ĸ�����ֺ��»���!","��ʾ",MB_OK | MB_ICONWARNING);
      txtFieldName->SetFocus();
      txtFieldName->SelectAll();
      return false;
    }
  }
  if(chFieldName[0] > 47 && chFieldName[0] < 58)  
  {
      MessageBox(Handle,"�ֶ����Ʊ�������ĸ��ͷ!","��ʾ",MB_OK | MB_ICONWARNING);
      txtFieldName->SetFocus();
      txtFieldName->SelectAll();
      return false;
  }
  if(txtFieldLable->Text != "" && (txtFieldLable->Text.Pos("'") > 0 || txtFieldLable->Text.Pos("\"") > 0))
  {
      MessageBox(Handle,"�ֶα�ǩ�в��ܰ��������ź�˫����!","��ʾ",MB_OK | MB_ICONWARNING);
      txtFieldLable->SetFocus();
      txtFieldLable->SelectAll();
      return false;
  }
  int DataType = (int)(cbxDataType->Items->Objects[cbxDataType->ItemIndex]);
  if(FileFormatType == BIN_BILL_FORMAT)
  {
    if(DataType != DATATYPE_BIT)
    {
      if(txtBitPosition->Text == "")
        txtBitPosition->Text = "";
      if(txtBitPosition->Text == "")
        txtBitPosition->Text = "0";
      if(txtBitWidth->Text == "")
        txtBitWidth->Text = "0";
    }
    if((int)(cbxDataType->Items->Objects[cbxDataType->ItemIndex]) != DATATYPE_BASED_DATETIME)
    {
      if(txtBaseDateTime->Text == "")
        txtBaseDateTime->Text = "1900-01-01 00:00:00";
    }
  }
  else if(FileFormatType == TXT_BILL_FORMAT)
  {
    if(txtBitPosition->Text == "")
      txtBitPosition->Text = "0";
    if(txtBitWidth->Text == "")
      txtBitWidth->Text = "0";
    if(txtBaseDateTime->Text == "")
      txtBaseDateTime->Text = "1900-01-01 00:00:00";
    if(txtLength->Text == "")
      txtLength->Text = "0";
  }
  if(FieldName.LowerCase() != txtFieldName->Text.LowerCase() && BillConfig->GetFieldNode(BillName,txtFieldName->Text) != NULL)
  {
    MessageBox(Handle,"�����ֶε��ֶ������ڸ��������Ѿ�����!","��ʾ",MB_OK | MB_ICONWARNING);
    txtFieldName->SetFocus();
    txtFieldName->SelectAll();
    return false;
  }
  int n;
  if(DataType == DATATYPE_DURATION)
  {
    try
    {
      n = StrToInt(txtOffset->Text);
      if(n < 0 || n > RecordLength - 1)
         txtOffset->Text = "0";
    }
    catch(...)
    {
      txtOffset->Text = "0";
    }
  }
  try
  {
    n = StrToInt(txtOffset->Text);
  }
  catch(...)
  {
    MessageBox(Handle,"�ֶ�ƫ����������һ������!","��ʾ",MB_OK | MB_ICONWARNING);
    txtOffset->SetFocus();
    txtOffset->SelectAll();
    return false;
  }
  if((FileFormatType == BIN_BILL_FORMAT && n > RecordLength - 1) || n < 0)
  {
    MessageBox(Handle,("�����ֶ�ƫ���������˷�Χ(0-" + IntToStr(RecordLength - 1) + ")!").c_str(),"��ʾ",MB_OK | MB_ICONWARNING);
    txtOffset->SetFocus();
    txtOffset->SelectAll();
    return false;
  }
  try
  {
    StrToInt(txtLength->Text);
  }
  catch(...)
  {
    MessageBox(Handle,"�ֶγ��ȱ�����һ������!","��ʾ",MB_OK | MB_ICONWARNING);
    txtLength->SetFocus();
    txtLength->SelectAll();
    return false;
  }
  //int DataType = (int)(cbxDataType->Items->Objects[cbxDataType->ItemIndex]);
  if(DataType == DATATYPE_BIT)
  {
    try
    {
      n = StrToInt(txtBitPosition->Text);
      if(n < 0 || n > 7)
        throw new Exception("");
    }
    catch(...)
    {
      MessageBox(Handle,"BITλ�ñ�����һ��0-7֮�������!","��ʾ",MB_OK | MB_ICONWARNING);
      txtBitPosition->SetFocus();
      txtBitPosition->SelectAll();
      return false;
    }
    try
    {
      n = StrToInt(txtBitWidth->Text);
      if(n < 1 || n > 8)
        throw new Exception("");
    }
    catch(...)
    {
      MessageBox(Handle,"BIT��ȱ�����һ��1-8֮�������!","��ʾ",MB_OK | MB_ICONWARNING);
      txtBitWidth->SetFocus();
      txtBitWidth->SelectAll();
      return false;
    }
  }
  else if(DataType == DATATYPE_BASED_DATETIME)
  {
    try
    {
      StrToDateTime(txtBaseDateTime->Text);
    }
    catch(...)
    {
      MessageBox(Handle,"��׼ʱ�������һ����ʽΪ\"YYYY-MM-DD HH:MM:SS\"���ַ���!","��ʾ",MB_OK | MB_ICONWARNING);
      txtBaseDateTime->SetFocus();
      txtBaseDateTime->SelectAll();
      return false;
    }
  }
  switch((int)(cbxSortType->Items->Objects[cbxSortType->ItemIndex]))
  {
  case SORT_AS_STR:
    try
    {
      n = StrToInt(txtExportLength->Text);
      if(n < 1 || n > 255)
        throw new Exception("");
    }
    catch(...)
    {
      MessageBox(Handle,"�������ȱ�����һ��1-255֮�������!","��ʾ",MB_OK | MB_ICONWARNING);
      txtExportLength->SetFocus();
      txtExportLength->SelectAll();
      return false;
    }
    break;
  case SORT_AS_DATETIME:
    txtExportLength->Text = "20";
    break;
  case SORT_AS_NUMBER:
  case SORT_AS_CURRENCY:
    txtExportLength->Text = "10";
    break;
  }
  try
  {
    n = StrToInt(txtDisplayWidth->Text);
    if(n <= 0)
      throw new Exception("");
  }
  catch(...)
  {
    MessageBox(Handle,"��ʾ��ȱ�����һ������0������!","��ʾ",MB_OK | MB_ICONWARNING);
    txtDisplayWidth->SetFocus();
    txtDisplayWidth->SelectAll();
    return false;
  }
  return true;
}
void __fastcall TfrmField::btnReplaceRuleClick(TObject *Sender)
{
  TfrmReplaceRule * frmReplaceRule = new TfrmReplaceRule(this,BillName,FieldName,ReplaceRules,ReadOnly);
  if(frmReplaceRule->ShowModal() == mrOk && !ReadOnly)
  {
    ReplaceRules = frmReplaceRule->ReplayRules;
  }
  delete frmReplaceRule;  
}
//---------------------------------------------------------------------------

void __fastcall TfrmField::btnTransClick(TObject *Sender)
{
  TfrmValueTrans * frmValueTrans = new TfrmValueTrans(this,BillName,FieldName,TransList,ReadOnly);
  if(frmValueTrans->ShowModal() == mrOk && !ReadOnly)
  {
    TransListChanged = frmValueTrans->ConfigChanged || TransListChanged;
  }
  delete frmValueTrans;  
}
//---------------------------------------------------------------------------

void __fastcall TfrmField::FormDestroy(TObject *Sender)
{
  delete TransList;  
}
//---------------------------------------------------------------------------

void __fastcall TfrmField::cbxSortTypeChange(TObject *Sender)
{
/*
  if(cbxSortType->ItemIndex < 0)
    return;
  switch((int)(cbxSortType->Items->Objects[cbxSortType->ItemIndex]))
  {
  case SORT_AS_STR:
    try
    {
      int n = StrToInt(txtExportLength->Text);
      if(n < 1 || n > 255)
        throw new Exception("");
    }
    catch(...)
    {
      txtExportLength->Text = "255";
    }
    txtExportLength->Enabled = true;
    break;
  case SORT_AS_DATETIME:
    txtExportLength->Text = "20";
    txtExportLength->Enabled = false;
    break;
  case SORT_AS_NUMBER:
  case SORT_AS_CURRENCY:
    txtExportLength->Text = "10";
    txtExportLength->Enabled = false;
    break;
  }*/
  SetControlStateByDataType();   
}
//---------------------------------------------------------------------------

bool __fastcall TfrmField::FormHelp(WORD Command, int Data, bool &CallHelp)
{
  if(Command != HELP_CONTEXTPOPUP)
  {
    CallHelp = false;
    return true;
  }
  switch(Data)
  {
  case 999999:  //������������
#ifdef SCTELE_COM_VERSION
    ShellExecute(NULL,"open","hh.exe",(ExtractFilePath(Application->ExeName) + "Help\\ibillsc.chm::/datatype.htm").c_str(),"",SW_NORMAL);
#else
    ShellExecute(NULL,"open","hh.exe",(ExtractFilePath(Application->ExeName) + "Help\\ibill.chm::/datatype.htm").c_str(),"",SW_NORMAL);
#endif
    break;
  case 999998:  //�߼���������
#ifdef SCTELE_COM_VERSION
    ShellExecute(NULL,"open","hh.exe",(ExtractFilePath(Application->ExeName) + "Help\\ibillsc.chm::/logicdatatype.htm").c_str(),"",SW_NORMAL);
#else
    ShellExecute(NULL,"open","hh.exe",(ExtractFilePath(Application->ExeName) + "Help\\ibill.chm::/logicdatatype.htm").c_str(),"",SW_NORMAL);
#endif
    break;
  default:
    WinHelp(Handle,(HelpFilePath + "field.hlp").c_str(),HELP_CONTEXTPOPUP,Data);
    break;
  }
  CallHelp = false;
  return true;    
}
//---------------------------------------------------------------------------

void __fastcall TfrmField::chkImportToDBClick(TObject *Sender)
{
  RzPanel2->Enabled  = chkImportToDB->Checked;
  SetControlStateByDataType();
}
//---------------------------------------------------------------------------


