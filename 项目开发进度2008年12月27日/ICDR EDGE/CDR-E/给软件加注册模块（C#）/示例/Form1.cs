using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using System.IO;

using System.Web.Security;
using System.Management;

namespace 示例
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        string serialNum;
        string machineNum;
        string regNum;

        bool bRegOK = true;
        private void Form1_Load(object sender, EventArgs e)
        {
            string path = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "LAY3-T.lic");
            注册.Form1 form = new 注册.Form1();
            if( !File.Exists(path))
            {
                form.ShowDialog();
                if (!File.Exists(path))
                {
                    this.Close();
                    return;
                }
            }

            StreamReader reader = new StreamReader(path);
            serialNum = reader.ReadLine();
            machineNum = reader.ReadLine();
            regNum = reader.ReadLine();
            reader.Close();
            if( !form.CheckRegHead(machineNum,regNum) )
            {
                //这儿会对序列号正确性做简单校验，不合格的会有提示。以后再校验出问题就判定为恶意破解，不再有提示，直接修改系统运行结果。
                MessageBox.Show("序列号不正确，系统将退出，请与软件供应商联系！");  
                this.Close();
            }
        }

        private void 校验注册号代码Btn_Click(object sender, EventArgs e)
        {
            //检验注册号，把检验结果找一内存保存起来。不要在校验结果代码附近做任何影响程序正常工作的处理，这样不易被跟踪。
            string regTail3 = GetRegTail3ByMac(serialNum);
            if (string.Compare(regTail3, 0, regNum, 12, 17) == 0)
                bRegOK = true;
            else
                bRegOK = false;
        }

        private void 核心功能代码代表Btn_Click(object sender, EventArgs e)
        {
            int result;
            if (bRegOK)
            {
                result = 10 + 10;
            }
            else
            {
                result = 10 + 10 + 1;  //发现注册码正常的标记为false时,对核心功能代码进行不易发觉的修改，导致结果无法使用，注：不要弹出易被跟踪的消息事件等。
            }
            textBox1.Text = result.ToString();
        }

        
        #region  真正的注册号检验代码，直接拷贝到需要校验的类中做为私有变量，不要单独打包成dll和做为公共变量。
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
        #endregion



    }
}