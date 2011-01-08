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

namespace 注册机
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        /// <summary>
        /// 校验注册码头两端是否正确
        /// </summary>
        /// <param name="machineCode">机器码</param>
        /// <param name="regCode">注册码</param>
        /// <returns></returns>
        bool CheckRegHead(string machineCode, string regCode)
        {
            string regHead2 = GetRegHead2(machineCode);
            return string.Compare(regHead2, 0, regCode, 0, 9) == 0;
        }

        /// <summary>
        /// 通过网卡号和序列号重新获取注册码后三段。此函数给程序用来抗破解之用
        /// </summary>
        /// <returns></returns>
        string GetRegTail3ByMac(string seriesNum)
        {
            string machineCode = GetMachineCode(seriesNum);
            return GetRegTail3(machineCode);
        }

        /// <summary>
        /// 获取注册码的后三段。此函数不能流失出去
        /// </summary>
        /// <param name="machineCode"></param>
        /// <returns></returns>
        string GetRegTail3(string machineCode)
        {
            string regTail3;
            regTail3 = FormsAuthentication.HashPasswordForStoringInConfigFile(machineCode, "MD5");
            regTail3 = FormsAuthentication.HashPasswordForStoringInConfigFile(regTail3, "SHA1");
            regTail3 = FormsAuthentication.HashPasswordForStoringInConfigFile(regTail3, "SHA1");
            regTail3 = FormsAuthentication.HashPasswordForStoringInConfigFile(regTail3, "MD5");
            regTail3 = FormsAuthentication.HashPasswordForStoringInConfigFile(regTail3, "SHA1");
            regTail3 = FormsAuthentication.HashPasswordForStoringInConfigFile(regTail3, "MD5");
            regTail3 = FormsAuthentication.HashPasswordForStoringInConfigFile(regTail3, "MD5");

            regTail3 = GetCode(regTail3, 3);
            return regTail3;
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
        string GetMachineCode(string seriesNum)
        {
            string macAdd = GetMacAddress();
            string md5Str = FormsAuthentication.HashPasswordForStoringInConfigFile(macAdd + seriesNum, "MD5");
            return GetCode(md5Str, 4);
        }

        /// <summary>
        /// 得到以“-”分隔，每段5个字母的编码
        /// </summary>
        /// <param name="src"></param>
        /// <param name="num"></param>
        /// <returns></returns>
        string GetCode(string src, int num)
        {
            string re = "";
            for (int i = 0; i < num; i++)
            {
                string tem = src.Substring(i * num, 5);
                re += tem;
                if (i < num - 1)
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
        string strPath = Application.StartupPath;
        void 生成注册号Btn_Click(object sender, EventArgs e)
        {
            textBox2.Text += GetRegHead2(textBox1.Text);
            textBox2.Text += "-";
            textBox2.Text += GetRegTail3(textBox1.Text);


            if (File.Exists("regInfo.txt") == true)
            {
                File.Delete("regInfo.txt");
            }

            StreamWriter sw = File.CreateText("regInfo.txt");

            sw.WriteLine(Encrypt(textBox1.Text,"icomicom"));
            sw.WriteLine(Encrypt(textBox2.Text, "icomicom"));
            sw.WriteLine(Encrypt(textBox3.Text, "icomicom"));
            sw.WriteLine(Encrypt(textBox4.Text, "icomicom"));
            sw.WriteLine(Encrypt(textBox5.Text, "icomicom"));
            sw.Flush();
            sw.Close();



        }

        private void Form1_Load(object sender, EventArgs e)
        {
            DateTime dt = DateTime.Now;
            DateTime dt1 = DateTime.Now.AddDays(7); 
            textBox3.Text = "30";
            textBox4.Text = dt1.ToString();
            textBox5.Text = dt.ToFileTime().ToString();
           //textBox5.Text = "shanghai china mobile 1";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (File.Exists("regInfo.txt") == true)
            {
                StreamReader reader = new StreamReader("regInfo.txt");
                for (int i = 0; i < 5; i++)
                {
                    string line1 = reader.ReadLine();
                    richTextBox1.AppendText(line1 + Environment.NewLine);
                    line1 = Decrypt(line1, "icomicom");
                    richTextBox1.AppendText(line1 + Environment.NewLine);
                }

                for (int j = 0; j < 2; j++)
                {
                    string line2 = reader.ReadLine();
                    richTextBox1.AppendText(line2 + Environment.NewLine);
                    line2 = Decrypt(line2, "icdredge");
                    richTextBox1.AppendText(line2 + Environment.NewLine);
                }


            }
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