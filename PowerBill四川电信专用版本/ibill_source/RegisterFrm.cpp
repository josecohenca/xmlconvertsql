//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RegisterFrm.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzButton"
#pragma link "RzEdit"
#pragma link "RzLabel"
#pragma link "CWGIFImage"
#pragma resource "*.dfm"
TfrmRegister *frmRegister;
//---------------------------------------------------------------------------
__fastcall TfrmRegister::TfrmRegister(TComponent* Owner)
  : TForm(Owner)
{
  txtRegisterUserName->Text = frmMain->RegistUserName;

  /*{
    Label3->Caption =
      "    ��л��ʹ��\"��ʤͨ�û�����ѯת��ϵͳ\".������ʹ�õ��Ǳ�"\
      "�����һ��δע��ĸ���,��û��ע��������,������ݹ���"\
      "��������(����������ڹ�����������İ����ĵ�),�����ڳ���"\
      "������(40��)��,����������ټ���ʹ��."\
      "����Ҫע�Ტ�����������֧��һ���ķ��ú���������ڵ�ʹ�ñ������ȫ������.ע��ķ�����:\n"\
      "1-�����߷����ʼ�,����������\"ע���û���\";\n"\
      "2-�������������͵��ʼ�����ȡ��ע���벢���뵽\"���ע����\"(���ǽ����յ����ĸ���֪ͨ��24Сʱ�ڽ�����ע���뷢�͵�����������);\n"\
      "3-���\"ע��\"��ť���ע��.";
  }
  else*/
#ifdef SCTELE_COM_VERSION
  Label1->Caption = "������";
  txtRegisterUserName->ReadOnly = true;
  txtRegisterUserName->Color = clBtnFace;
  txtRegisterUserName->Text = frmMain->RegistUserName;
#endif
  if(EncryRegisteCode(frmMain->RegistUserName) == frmMain->SerialNumber)
  {
    Label2->Visible = false;
    txtRegistNumber->Visible = false;
    btnRegiste->Caption    = "�ر�";
    btnRegiste->ImageIndex = 49;
    txtRegisterUserName->ReadOnly = true;
    txtRegisterUserName->Color = clBtnFace;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmRegister::FormKeyPress(TObject *Sender, char &Key)
{
  if(Key == 27)
  {
    Key = 0;
    Close();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRegister::btnRegisteClick(TObject *Sender)
{
  if(EncryRegisteCode(frmMain->RegistUserName).SubString(1,25) == frmMain->SerialNumber)
  {
    Close();
    return;
  }
  if(txtRegisterUserName->Text == "")
  {
    MessageBox(Handle,("������" + Label1->Caption + "!").c_str(),"��ʾ",MB_OK | MB_ICONINFORMATION);
    txtRegisterUserName->SetFocus();
    return;
  }
  if(txtRegistNumber->Text == "")
  {
    MessageBox(Handle,"������ע����!","��ʾ",MB_OK | MB_ICONINFORMATION);
    txtRegistNumber->SetFocus();
    return;
  }
  try
  {
    if(EncryRegisteCode(txtRegisterUserName->Text).SubString(1,25) != txtRegistNumber->Text)
    {
      throw new Exception("");
    }
    TIniFile * IniFile;
    try
    {
      IniFile = new TIniFile(ExtractFilePath(Application->ExeName) + "ibill.ini");
      IniFile->WriteString("System","SerialNumber",txtRegistNumber->Text);
      frmMain->RegistUserName = txtRegisterUserName->Text;
      frmMain->SerialNumber   = txtRegistNumber->Text;
      #ifndef SCTELE_COM_VERSION
        IniFile->WriteString("System","RegisterUserName",txtRegisterUserName->Text);
        frmMain->Caption = "Power Bill ͨ�û�����ѯת��ϵͳ V2.0 ��Ȩ�� " + frmMain->RegistUserName + " ʹ��";
      #else
        frmMain->Caption = "Power Bill ͨ�û�����ѯת��ϵͳ V2.0 �Ĵ�����ר�ð汾 ��ɽ���� ������ ��������Ȩ��";
      #endif
    }
    catch(...)
    {
    }
    delete IniFile;
    MessageBox(Handle,"ע��ɹ�,��л����֧��!","ע��ɹ�",MB_OK | MB_ICONINFORMATION);
    Close();
  }
  catch(...)
  {
    MessageBox(Handle,"�����ע���벻��ȷ!","��ʾ",MB_OK | MB_ICONWARNING);
    txtRegistNumber->SetFocus();
    return;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRegister::RzURLLabel1Click(TObject *Sender)
{
#ifdef SCTELE_COM_VERSION
  ShellExecute(NULL,"open","hh.exe",(ExtractFilePath(Application->ExeName) + "Help\\ibillsc.chm::/reg_sc.htm").c_str(),"",SW_NORMAL);
#else
  ShellExecute(NULL,"open","hh.exe",(ExtractFilePath(Application->ExeName) + "Help\\ibill.chm::/reg.htm").c_str(),"",SW_NORMAL);
#endif
}
//---------------------------------------------------------------------------

