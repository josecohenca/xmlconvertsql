//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EditDBFrm.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzButton"
#pragma link "RzCmboBx"
#pragma link "RzEdit"
#pragma link "RzRadChk"
#pragma resource "*.dfm"
TfrmEditDB *frmEditDB;
//---------------------------------------------------------------------------
__fastcall TfrmEditDB::TfrmEditDB(TComponent* Owner,bool AForEdit,TDBConfig * ADBConfig)
  : TForm(Owner)
{
  DBConfig = ADBConfig;
  ForEdit  = AForEdit;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditDB::cbxDBTypeChange(TObject *Sender)
{
  cbxDataBases->Enabled = cbxDBType->ItemIndex > 0;
  if(cbxDBType->ItemIndex == 0)
  {
    Label5->Caption = "TNS名称";
  }
  else
  {
    Label5->Caption = "服务器地址";
  }
  Label9->Enabled       = cbxDataBases->Enabled;
  Label10->Enabled      = cbxDBType->ItemIndex == 2;
  txtDBPort->Enabled    = Label10->Enabled;
  cbxDataBases->Items->Clear();    
}
bool __fastcall TfrmEditDB::OpenConnection()
{
  if(ADOConnection1->Connected)
    ADOConnection1->Close();
  switch(cbxDBType->ItemIndex)
  {
  case 0:
    DBType = DB_ORACLE;
    break;
  case 1:
    DBType = DB_SQLSERVER;
    break;
  case 2:
    DBType = DB_MYSQL;
    if(!IsNumber(txtDBPort->Text.c_str()))
      txtDBPort->Text = "3306";
    break;
  default:
    return false;
  }
  ADOConnection1->ConnectionString =
    frmMain->DBConfig->GetDBConnectionString(DBType,txtDBServer->Text,txtDBUserName->Text,
      txtDBPassword->Text,cbxDataBases->Text,txtDBPort->Text);
  try
  {
    ADOConnection1->Open();
  }
  catch(Exception * E)
  {
    MessageBox(Handle,(E->Message + "  请确认已安装了所选数据库的驱动程序并检查数据库服务器、用户名、密码是否设置正确.").c_str(),"错误",MB_OK | MB_ICONSTOP);
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditDB::cbxDataBasesDropDown(TObject *Sender)
{
  if(cbxDBType->ItemIndex == 0)
    return;
  Screen->Cursor = crHourGlass;
  AnsiString Text = cbxDataBases->Text;
  cbxDataBases->Items->Clear();
  if(!OpenConnection())
  {
    Screen->Cursor = crDefault;
    return;
  }
  if(ADODataSet1->Active)
    ADODataSet1->Close();
  switch(DBType)
  {
  case DB_SQLSERVER:
    ADODataSet1->CommandText = "SELECT name FROM master..sysdatabases ORDER BY name";
    break;
  case DB_MYSQL:
    ADODataSet1->CommandText = "show databases";
    break;
  }
  try
  {
    ADODataSet1->Open();
    while(!ADODataSet1->Eof)
    {
      cbxDataBases->Items->Add(ADODataSet1->Fields->Fields[0]->AsString);
      ADODataSet1->Next();  
    }
    ADODataSet1->Close();
    ADOConnection1->Close();
  }
  catch(Exception * E)
  {
    Screen->Cursor = crDefault;
    ADOConnection1->Close();
    MessageBox(Handle,E->Message.c_str(),"警告",MB_OK | MB_ICONSTOP);
  }
  ADOConnection1->Close();
  cbxDataBases->ItemIndex = cbxDataBases->Items->IndexOf(Text);
  Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditDB::Label6Click(TObject *Sender)
{
  Screen->Cursor = crHourGlass;
  if(OpenConnection())
  {
    ADOConnection1->Close();
    MessageBox(Handle,"测试成功完成.","信息",MB_OK | MB_ICONINFORMATION);
  }
  Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditDB::btnOkClick(TObject *Sender)
{
  if(cbxDBType->ItemIndex < 0)
  {
    MessageBox(Handle,"请选择一种数据库类型!","提示",MB_OK | MB_ICONWARNING);
    cbxDBType->SetFocus();
    return;
  }
  if(txtDBName->Text == "")
  {
    MessageBox(Handle,"连接名称不能为空!","提示",MB_OK | MB_ICONWARNING);
    txtDBName->SetFocus();
    return;
  }
  if(txtDBServer->Text == "")
  {
    MessageBox(Handle,"服务器地址不能为空!","提示",MB_OK | MB_ICONWARNING);
    txtDBServer->SetFocus();
    return;
  }
  //if(MessageBox(Handle,"您要保存所做的修改吗?","问题",MB_YESNO | MB_ICONWARNING) == IDNO)
  //  return;
  if(ForEdit)
  {
    if(OldDBName != txtDBName->Text)
    {
      if(DBConfig->IsDBNameExists(txtDBName->Text))
      {
        MessageBox(Handle,"输入的数据库连接名称在配置文件中已经存在!","提示",MB_OK | MB_ICONWARNING);
        txtDBName->SetFocus();
        return;
      }
    }
    DBConfig->SetDBAttributeValue(OldDBName,"DBType",cbxDBType->Text);
    DBConfig->SetDBAttributeValue(OldDBName,"Server",txtDBServer->Text);
    DBConfig->SetDBAttributeValue(OldDBName,"DataBase",cbxDataBases->Text);
    DBConfig->SetDBAttributeValue(OldDBName,"Port",txtDBPort->Text);
    DBConfig->SetDBAttributeValue(OldDBName,"UserName",txtDBUserName->Text);
    DBConfig->SetDBAttributeValue(OldDBName,"Password",txtDBPassword->Text,true);
    DBConfig->SetDBAttributeValue(OldDBName,"DBName",txtDBName->Text);    
  }
  else
  {
    if(DBConfig->IsDBNameExists(txtDBName->Text))
    {
      MessageBox(Handle,"输入的数据库连接名称在配置文件中已经存在!","提示",MB_OK | MB_ICONWARNING);
      txtDBName->SetFocus();
      return;
    }
    DBConfig->AddNewDBServer(
        txtDBName->Text,
        cbxDBType->Text,
        txtDBServer->Text,
        txtDBUserName->Text,
        txtDBPassword->Text,
        cbxDataBases->Text,
        txtDBPort->Text);
  }
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
