using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Web.Security;
using System.Management;
using System.IO;
using System.Security.Cryptography;

namespace LAY3_R
{
    public partial class frmRegister : Form
    {
        public frmRegister()
        {
            InitializeComponent();
        }
        string serialNum;
        string machineNum;
        string regNum;
        string regDate;
        string regTimes;
        string serialNum1;
        string machineNum1;
        string regNum1;
        string regDate1;
        string regTimes1;
        private void 生成机器码Btn()
        {
            textBox2.Text = 获取机器码(textBox1.Text);
        }

        private void 注册Btn_Click(object sender, EventArgs e)
        {
            string appPath = Application.StartupPath.ToString();
            string licPath = appPath + "\\Xml\\LAY3-T.lic";
            string licDate = dt.ToString();


            OpenFileDialog dlgOpenfile = new OpenFileDialog();
            string strFileFullName = null;
            dlgOpenfile.Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
            dlgOpenfile.Title = "Open";
            dlgOpenfile.ShowDialog();
            dlgOpenfile.RestoreDirectory = true;
            if (!string.IsNullOrEmpty(dlgOpenfile.FileName))
            {
                strFileFullName = dlgOpenfile.FileName;

                if (File.Exists(licPath))
                {
                    StreamReader reader = new StreamReader(licPath);
                    machineNum = reader.ReadLine();
                    regNum = reader.ReadLine();
                    regTimes = reader.ReadLine();
                    regDate = reader.ReadLine();
                    serialNum = reader.ReadLine();
                    reader.Close();
                    StreamReader reader1 = new StreamReader(strFileFullName);
                    machineNum1 = reader1.ReadLine();
                    regNum1 = reader1.ReadLine();
                    regTimes1 = reader1.ReadLine();
                    regDate1 = reader1.ReadLine();
                    serialNum1 = reader1.ReadLine();
                    reader1.Close();
                    if (string.Compare(regDate, regDate1) == 0)
                    {
                        MessageBox.Show("注册失败");
                        return;
                    }
                    else
                    {
                        File.Delete(licPath);
                        File.Copy(strFileFullName, licPath);
                        File.AppendAllText(licPath, Encrypt(licDate, "icdredge") + Environment.NewLine);
                        File.AppendAllText(licPath, Encrypt(textBox3.Text, "icdredge") + Environment.NewLine);
                        MessageBox.Show("注册成功");
                        return;
                    }
                }
                else
                {
                    File.Copy(strFileFullName, licPath);
                    File.AppendAllText(licPath, Encrypt(licDate, "icdredge") + Environment.NewLine);
                    File.AppendAllText(licPath, Encrypt(textBox3.Text, "icdredge") + Environment.NewLine);
                    MessageBox.Show("注册成功");
                    return;
                }
            }
            else
            {
                return;
            }

        }

        /// <summary>
        /// 校验注册码头两端是否正确
        /// </summary>
        /// <param name="machineCode">机器码</param>
        /// <param name="regCode">注册码</param>
        /// <returns></returns>
        public bool CheckRegHead(string machineCode, string regCode)
        {
            string regHead2 = GetRegHead2(machineCode);
            return string.Compare(regHead2, 0, regCode, 0, 11) == 0;
        }

        /// <summary>
        /// 获取注册码的头两端。用来做注册码正确性的简单判断，判断失败会给提示。
        /// </summary>
        /// <param name="machineCode"></param>
        /// <returns></returns>
        string GetRegHead2(string machineCode)
        {
            string regHead2 = FormsAuthentication.HashPasswordForStoringInConfigFile(machineCode, "MD5");
            regHead2 = GetCode(regHead2, 2);
            return regHead2;
        }

        /// <summary>
        /// 获取机器码
        /// </summary>
        /// <param name="序列号"></param>
        /// <returns>带“-”的四段数字</returns>
        string 获取机器码(string seriesNum)
        { 
            string macAdd = GetMacAddress();
            string md5Str = FormsAuthentication.HashPasswordForStoringInConfigFile(macAdd + seriesNum, "MD5");
            return GetCode(md5Str,4);
        }

        /// <summary>
        /// 得到以“-”分隔，每段5个字母的编码
        /// </summary>
        /// <param name="src"></param>
        /// <param name="num"></param>
        /// <returns></returns>
        string GetCode(string src,int num)
        {
            string re = "";
            for (int i = 0; i < num; i++)
            {
                string tem = src.Substring(i * num, 5);
                re += tem;
                if (i < num -1)
                    re += "-";
            }
            return re;
        }
        string GetMacAddress()
        {
            try
            {
                string mac = "";
                ManagementClass mc = new ManagementClass("Win32_NetworkAdapterConfiguration");
                ManagementObjectCollection moc = mc.GetInstances();
                foreach (ManagementObject mo in moc)
                {
                    if ((bool)mo["IPEnabled"] == true)
                    {
                        mac = mo["MacAddress"].ToString();
                        break;
                    }
                }
                moc = null;
                mc = null;
                return mac;
            }
            catch
            {
                return "unknow";
            }
            finally
            {
            }
        }

        DateTime dt = DateTime.Now;
        private void frmRegister_Load(object sender, EventArgs e)
        {       
            textBox1 .Text =dt.ToFileTime().ToString();
            textBox3.Text = "ICDR EDGE version 1.0";
            
            Application.DoEvents();
            生成机器码Btn();
        }
        #region 可逆向加密方法，DES，8位key。
        //加密方法  
        public string Encrypt(string pToEncrypt, string sKey)
        {
            DESCryptoServiceProvider des = new DESCryptoServiceProvider();
            //把字符串放到byte数组中  
            //原来使用的UTF8编码，我改成Unicode编码了，不行  
            byte[] inputByteArray = Encoding.Default.GetBytes(pToEncrypt);
            //byte[]  inputByteArray=Encoding.Unicode.GetBytes(pToEncrypt);  

            //建立加密对象的密钥和偏移量  
            //原文使用ASCIIEncoding.ASCII方法的GetBytes方法  
            //使得输入密码必须输入英文文本  
            des.Key = ASCIIEncoding.ASCII.GetBytes(sKey);
            des.IV = ASCIIEncoding.ASCII.GetBytes(sKey);
            MemoryStream ms = new MemoryStream();
            CryptoStream cs = new CryptoStream(ms, des.CreateEncryptor(), CryptoStreamMode.Write);
            //Write  the  byte  array  into  the  crypto  stream  
            //(It  will  end  up  in  the  memory  stream)  
            cs.Write(inputByteArray, 0, inputByteArray.Length);
            cs.FlushFinalBlock();
            //Get  the  data  back  from  the  memory  stream,  and  into  a  string  
            StringBuilder ret = new StringBuilder();
            foreach (byte b in ms.ToArray())
            {
                //Format  as  hex  
                ret.AppendFormat("{0:X2}", b);
            }
            ret.ToString();
            return ret.ToString();
        }
        //解密方法  
        public string Decrypt(string pToDecrypt, string sKey)
        {
            try
            {
                DESCryptoServiceProvider des = new DESCryptoServiceProvider();

                //Put  the  input  string  into  the  byte  array  
                byte[] inputByteArray = new byte[pToDecrypt.Length / 2];
                for (int x = 0; x < pToDecrypt.Length / 2; x++)
                {
                    int i = (Convert.ToInt32(pToDecrypt.Substring(x * 2, 2), 16));
                    inputByteArray[x] = (byte)i;
                }

                //建立加密对象的密钥和偏移量，此值重要，不能修改  
                des.Key = ASCIIEncoding.ASCII.GetBytes(sKey);
                des.IV = ASCIIEncoding.ASCII.GetBytes(sKey);
                MemoryStream ms = new MemoryStream();
                CryptoStream cs = new CryptoStream(ms, des.CreateDecryptor(), CryptoStreamMode.Write);
                //Flush  the  data  through  the  crypto  stream  into  the  memory  stream  
                cs.Write(inputByteArray, 0, inputByteArray.Length);
                cs.FlushFinalBlock();

                //Get  the  decrypted  data  back  from  the  memory  stream  
                //建立StringBuild对象，CreateDecrypt使用的是流对象，必须把解密后的文本变成流对象  
                StringBuilder ret = new StringBuilder();

                return System.Text.Encoding.Default.GetString(ms.ToArray());
            }
            catch { return pToDecrypt; }
        }
        #endregion
    }
}