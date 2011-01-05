//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EditFTPFrm.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzButton"
#pragma link "RzCmboBx"
#pragma link "RzEdit"
#pragma link "RzRadChk"
#pragma resource "*.dfm"
TfrmEditFTP *frmEditFTP;
//---------------------------------------------------------------------------
__fastcall TfrmEditFTP::TfrmEditFTP(TComponent* Owner,bool AForEdit)
  : TForm(Owner)
{
  FTPConfig = frmMain->FTPConfig;
  ForEdit = AForEdit;
}
//---------------------------------------------------------------------------
bool __fastcall TfrmEditFTP::OpenConnection()
{
  if(IdFTP1->Connected())
    IdFTP1->Disconnect();
  IdFTP1->Host = txtServer->Text;
  try
  {
    IdFTP1->Port = StrToInt(txtPort->Text);
  }
  catch(...)
  {
    if(txtPort->Text == "")
      txtPort->Text = "21";
    else
    {
      MessageBox(Handle,"�˿ںű�����һ������!","����",MB_OK | MB_ICONSTOP);
      txtPort->SetFocus();
      return false;
    }
    IdFTP1->Port = 21;
  }
  IdFTP1->ReadTimeout = StrToInt(txtTimeOut->Text) * 1000;
  IdFTP1->Passive     = cbxMode->ItemIndex == 1;
  if(chkNoUserName->Checked)
  {
    IdFTP1->Username    = "Anonymous";
    IdFTP1->Password    = "";
  }
  else
  {
    IdFTP1->Username    = txtUserName->Text;
    IdFTP1->Password    = txtPassword->Text;
  }
  try
  {
    IdFTP1->Connect(true,IdFTP1->ReadTimeout);
  }
  catch(Exception * E)
  {
    MessageBox(Handle,("���ӵ�������ʱ��������:" + E->Message).c_str(),"����",MB_OK | MB_ICONSTOP);
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditFTP::Label6Click(TObject *Sender)
{
  Screen->Cursor = crHourGlass;
  if(OpenConnection())
  {
    MessageBox(Handle,"���Գɹ����.","��Ϣ",MB_OK | MB_ICONINFORMATION);
  }
  Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------
bool __fastcall TfrmEditFTP::CheckParameters()
{
  if(txtServerName->Text == "")
  {
    MessageBox(Handle,"������FTP����������!","��ʾ",MB_OK | MB_ICONWARNING);
    txtServerName->SetFocus();
    return false;
  }
  if(txtServer->Text == "")
  {
    MessageBox(Handle,"��������ַ����Ϊ��!","��ʾ",MB_OK | MB_ICONWARNING);
    txtServer->SetFocus();
    return false;
  }
  try
  {
    int n = StrToInt(txtTimeOut->Text);
    if(n < 0 || n > 65535)
      throw new Exception("");
  }
  catch(...)
  {
    MessageBox(Handle,"������һ��0-65535֮�������!","��ʾ",MB_OK | MB_ICONWARNING);
    txtTimeOut->SetFocus();
    txtTimeOut->SelectAll();
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditFTP::btnOkClick(TObject *Sender)
{
  if(!CheckParameters())
    return;
  AnsiString Mode;
  if(cbxMode->ItemIndex == 0)
    Mode = "PASV";
  else
    Mode = "NORMAL";
  AnsiString UserName;
  if(chkNoUserName->Checked)
    UserName = "Anonymous";
  else
    UserName = txtUserName->Text;  
  if(ForEdit)
  {
    if(OldFTPName != txtServerName->Text)
    {
      if(FTPConfig->IsFTPNameExists(txtServerName->Text))
      {
        MessageBox(Handle,"����ķ����������������ļ����Ѿ�����!","��ʾ",MB_OK | MB_ICONWARNING);
        txtServerName->SetFocus();
        txtServerName->SelectAll();
        return;
      }
    }
    FTPConfig->SetFTPAttributeValue(OldFTPName,"ServerName",txtServerName->Text);
    FTPConfig->SetFTPAttributeValue(OldFTPName,"Server",txtServer->Text);
    FTPConfig->SetFTPAttributeValue(OldFTPName,"Port",txtPort->Text);
    FTPConfig->SetFTPAttributeValue(OldFTPName,"UserName",UserName);
    FTPConfig->SetFTPAttributeValue(OldFTPName,"Password",txtPassword->Text,true);
    FTPConfig->SetFTPAttributeValue(OldFTPName,"Mode",Mode);
    //FTPConfig->SetFTPAttributeValue(OldFTPName,"TimeOut",txtTimeOut->Text);
  }
  else
  {
    if(FTPConfig->IsFTPNameExists(txtServerName->Text))
    {
      MessageBox(Handle,"����ķ����������������ļ����Ѿ�����!","��ʾ",MB_OK | MB_ICONWARNING);
      txtServerName->SetFocus();
      txtServerName->SelectAll();
      return;
    }
    FTPConfig->AddNewFTPServer(
        txtServerName->Text,
        txtServer->Text,
        StrToInt(txtPort->Text),
        UserName,
        txtPassword->Text,
        Mode);//,StrToInt(txtTimeOut->Text));
  }
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEditFTP::chkNoUserNameClick(TObject *Sender)
{
  txtUserName->Enabled = !chkNoUserName->Checked;
}
//---------------------------------------------------------------------------

