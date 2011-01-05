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
      "    感谢您使用\"智胜通用话单查询转换系统\".您现在使用的是本"\
      "软件的一份未注册的副本,在没有注册的情况下,软件部份功能"\
      "将会限制(具体的试用期功能限制请参阅帮助文档),并且在超出"\
      "试用期(40天)后,软件将不能再继续使用."\
      "您需要注册并向软件的作者支付一定的费用后才能无限期的使用本软件的全部功能.注册的方法是:\n"\
      "1-向作者发送邮件,将附上您的\"注册用户名\";\n"\
      "2-从作者向您发送的邮件中提取出注册码并输入到\"软件注册码\"(我们将在收到您的付款通知后24小时内将您的注册码发送到您的邮箱中);\n"\
      "3-点击\"注册\"按钮完成注册.";
  }
  else*/
#ifdef SCTELE_COM_VERSION
  Label1->Caption = "机器码";
  txtRegisterUserName->ReadOnly = true;
  txtRegisterUserName->Color = clBtnFace;
  txtRegisterUserName->Text = frmMain->RegistUserName;
#endif
  if(EncryRegisteCode(frmMain->RegistUserName) == frmMain->SerialNumber)
  {
    Label2->Visible = false;
    txtRegistNumber->Visible = false;
    btnRegiste->Caption    = "关闭";
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
    MessageBox(Handle,("请输入" + Label1->Caption + "!").c_str(),"提示",MB_OK | MB_ICONINFORMATION);
    txtRegisterUserName->SetFocus();
    return;
  }
  if(txtRegistNumber->Text == "")
  {
    MessageBox(Handle,"请输入注册码!","提示",MB_OK | MB_ICONINFORMATION);
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
        frmMain->Caption = "Power Bill 通用话单查询转换系统 V2.0 授权给 " + frmMain->RegistUserName + " 使用";
      #else
        frmMain->Caption = "Power Bill 通用话单查询转换系统 V2.0 四川电信专用版本 凉山电信 蒋利军 保留所有权利";
      #endif
    }
    catch(...)
    {
    }
    delete IniFile;
    MessageBox(Handle,"注册成功,感谢您的支持!","注册成功",MB_OK | MB_ICONINFORMATION);
    Close();
  }
  catch(...)
  {
    MessageBox(Handle,"输入的注册码不正确!","提示",MB_OK | MB_ICONWARNING);
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

