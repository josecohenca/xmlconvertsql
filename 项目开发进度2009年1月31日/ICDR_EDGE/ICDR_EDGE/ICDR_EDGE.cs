using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.Web.Security;
using System.Management;
using System.IO;
using Asn1Processor;
using System.Security.Cryptography;
using System.Runtime.InteropServices;
using System.Net;
using LAY3_R;
using System.Xml.Schema;

namespace ICDR
{
    public partial class ICDR_EDGE : Form
    {
        public ICDR_EDGE()
        {
            InitializeComponent();
        }
        [DllImport("kernel32")]
        private static extern long WritePrivateProfileString(string section, string key, string val, string filePath);
        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);
        private void ICDR_EDGE_Load(object sender, EventArgs e)
        {
            appPath = Application.StartupPath.ToString();
            //文件是否不全
            if (!File.Exists(appPath + "\\LAY3-R.exe"))
            {
                this.Close();
                this.Dispose();
                return;
            }
            //加密模块开始 
            string licPath = appPath + "\\Xml\\LAY3-T.lic";
            LAY3_R.frmRegister form = new LAY3_R.frmRegister();
            if (!File.Exists(licPath))
            {
                form.ShowDialog();
                if (!File.Exists(licPath))
                {
                    this.Close();
                    this.Dispose();
                    return;
                }
            }
            //加密模块检查序列号 
            StreamReader reader = new StreamReader(licPath);
            machineNum = reader.ReadLine();
            regNum = reader.ReadLine();
            regTimes = reader.ReadLine();
            regDate = reader.ReadLine();
            serialNum = reader.ReadLine();
            string licDate = reader.ReadLine();
            string licQQ = reader.ReadLine();
            reader.Close();
            if (machineNum == "" || regNum == "" || regTimes == "" || regDate == "" || licDate == "" || licQQ == "")
            {
                form.ShowDialog();
                this.Close();
                return;
            }
            serialNum = Decrypt(serialNum, "icomicom");
            string thismachineNum = 获取机器码(serialNum);
            machineNum = Decrypt(machineNum, "icomicom");
            if (machineNum != thismachineNum)
            {
                form.ShowDialog();
                this.Close();
                return;
            }
            licQQ = Decrypt(licQQ, "icdredge");//新写入
            if (licQQ != "ICDR EDGE version 1.0")
            {
                MessageBox.Show("注册号不正确，系统将退出，请与软件供应商联系！");
                this.Close();
                return;
            }
            regNum = Decrypt(regNum, "icomicom");
            if (!CheckRegHead(machineNum, regNum))
            {
                MessageBox.Show("序列号不正确，系统将退出，请与软件供应商联系！");
                this.Close();
                return;
            }
            licDate = Decrypt(licDate, "icdredge");//新写入
            regDate = Decrypt(regDate, "icomicom");
            DateTime trial = DateTime.Parse(licDate);
            DateTime YouregDate = DateTime.Parse(regDate);
            if (DateTime.Compare(trial, YouregDate) > 0)
            {
                MessageBox.Show("试用期限超出，系统将退出，请与软件供应商联系！");
                this.Close();
                return;
            }
            regTimes = Decrypt(regTimes, "icomicom");
            long YouregTimes = Int64.Parse(regTimes);
            if (YouregTimes == 0)
            {
                MessageBox.Show("试用次数超出，系统将退出，请与软件供应商联系！");
                this.Close();
                return;
            }
            else
            {
                if (File.Exists(licPath) == true)
                {
                    File.Delete(licPath);
                }
                StreamWriter sw = File.CreateText(licPath);
                sw.WriteLine(Encrypt(machineNum, "icomicom"));
                sw.WriteLine(Encrypt(regNum, "icomicom"));
                sw.WriteLine(Encrypt((YouregTimes - 1).ToString(), "icomicom"));
                sw.WriteLine(Encrypt(regDate, "icomicom"));
                sw.WriteLine(Encrypt(serialNum, "icomicom"));
                DateTime dt = DateTime.Now;
                licDate = dt.ToString();
                sw.WriteLine(Encrypt(licDate, "icdredge"));
                sw.WriteLine(Encrypt(licQQ, "icdredge"));
                sw.Flush();
                sw.Close();
            }

            //初始化
            this.WindowState = FormWindowState.Maximized;
            textBox2.Text = "select top 100 * from sgsnpdp";

            //加载ACCESS,如果目录中有users.ini文件
            if (File.Exists(appPath + "\\Xml\\users.ini") == true)
            {
                //读取INI文件
                string dbdrive = "";
                string sqluser = "";
                string sqlpass = "";
                StringBuilder temp = new StringBuilder(255);
                //读取连接数据库类型
                GetPrivateProfileString("database connection", "database type", "数据库类型", temp, 255, appPath + "\\Xml\\users.ini");
                dbdrive = temp.ToString();
                switch (dbdrive)
                {
                    case "ACCDB":
                        //读取连接数据库类型
                        GetPrivateProfileString("database drive", "ACCDB", "数据库名称", temp, 255, appPath + "\\Xml\\users.ini");
                        dbdrive = temp.ToString();
                        Acc.Init(serialNum, dbdrive, serialNum, serialNum);
                        Acc.Open();
                        break;
                    case "MDB":
                        //读取连接数据库类型
                        GetPrivateProfileString("database drive", "MDB", "数据库名称", temp, 255, appPath + "\\Xml\\users.ini");
                        dbdrive = temp.ToString();
                        Acc.Init(serialNum, dbdrive, serialNum, serialNum);
                        Acc.Open();
                        break;
                    case "MYSQL":
                        //读取连接数据库类型
                        GetPrivateProfileString("database drive", "MYSQL", "数据库名称", temp, 255, appPath + "\\Xml\\users.ini");
                        dbdrive = temp.ToString();
                        Acc.Init(serialNum, dbdrive, serialNum, serialNum);
                        Acc.Open();
                        break;
                    case "SQL_SERVER":
                        //读取连接数据库类型
                        GetPrivateProfileString("database drive", "SQL_SERVER", "数据库名称", temp, 255, appPath + "\\Xml\\users.ini");
                        dbdrive = temp.ToString();
                        GetPrivateProfileString("database drive", "SQL_USER", "数据库名称", temp, 255, appPath + "\\Xml\\users.ini");
                        sqluser = temp.ToString();
                        GetPrivateProfileString("database drive", "SQL_PASSWORD", "数据库名称", temp, 255, appPath + "\\Xml\\users.ini");
                        sqlpass = temp.ToString();
                        Sqlserv.Init ("127.0.0.1", "master", sqluser, sqlpass);
                        Sqlserv.Open();
                        break;
                    case "Oracle":
                        //读取连接数据库类型
                        GetPrivateProfileString("database drive", "Oracle", "数据库名称", temp, 255, appPath + "\\Xml\\users.ini");
                        dbdrive = temp.ToString();
                        Acc.Init(serialNum, dbdrive, serialNum, serialNum);
                        Acc.Open();
                        break;
                }
                toolStripStatusLabel5.Text=dbdrive;
            }


            //加载XML到TREE
            LoadTree();
            rbtnNEW.Checked = true;
            rbtnDEL.Checked = true;
            rbtnTREEAPPLCATION.Checked = true;
            this.WindowState = FormWindowState.Maximized;
        }
        Icdr.Conn.IDBAccess Acc = Icdr.Conn.DBAccessFactory.Create(Icdr.Conn.DBType.Access);
        Icdr.Conn.IDBAccess Sqlserv = Icdr.Conn.DBAccessFactory.Create(Icdr.Conn.DBType.SQL );
        string appPath;
        string serialNum;
        string machineNum;
        string regNum;
        string regDate;
        string regTimes;
        bool bRegOK = true;
        public void LoadTree()
        {
            //增加根节点
            treeView1.Nodes.Clear();
            TreeNode newNode1 = treeView1.Nodes.Add("SGSN CDR");
            TreeNode newNode2 = treeView1.Nodes.Add("GGSN CDR");
            TreeNode newNode3 = treeView1.Nodes.Add("cmwap CDR");
            TreeNode newNode4 = treeView1.Nodes.Add("Intelligent CDR");
            //加载XML
            XmlDocument myDoc = new XmlDocument();
            myDoc.Load(appPath + "\\Workspace\\CdrFieldName.xml");
            //搜索指定的节点
            System.Xml.XmlNodeList nodes1 = myDoc.GetElementsByTagName("GsnFieldName");
            if (nodes1 != null)
            {
                foreach (System.Xml.XmlNode xn in nodes1)
                {
                    if (xn.Attributes.Item(0).Value.Substring(0, 2) == "20")
                    {
                        newNode1.Nodes.Add(xn.Attributes.Item(3).Value.ToString());
                    }
                    if (xn.Attributes.Item(0).Value.Substring(0, 2) == "21")
                    {
                        newNode2.Nodes.Add(xn.Attributes.Item(3).Value.ToString());
                        newNode3.Nodes.Add(xn.Attributes.Item(3).Value.ToString());
                    }
                }
            }
            //加载XML
            XmlDocument myDoc1 = new XmlDocument();
            myDoc1.Load(appPath + "\\Xml\\CdrAppTreeSql.xml");
            //搜索指定的节点
            System.Xml.XmlNodeList nodes2 = myDoc1.GetElementsByTagName("ICDR");
            if (nodes2 != null)
            {
                foreach (System.Xml.XmlNode xn in nodes2)
                {
                    newNode4.Nodes.Add(xn.Attributes.Item(1).Value.ToString());
                }
            }

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


        private void ttipbtnASN1_Click(object sender, EventArgs e)
        {
            //检验注册 
            if (bRegOK == true)
            {

                //大量数据处理期间 

                for (int i = 0; i <= lbASN1FILE.Items.Count - 1; i++)
                {
                    //asn->xml 
                    lbASN1FILE.SelectedIndex = i;

                    //xml->csv 
                    ttipbtnXML.PerformClick();

                    //csv->sql 
                    ttipbtnCSV.PerformClick();
                }
                MessageBox.Show("OK!");
            }
        }

        private void ttipbtnXML_Click(object sender, EventArgs e)
        {
            //检验注册 
            if (bRegOK == true)
            {

                //Xml->Csv 
                toolStripStatusLabel3.Text = "Xml To Csv......";
                toolStripStatusLabel4.Text = appPath + "\\Exe\\xml2csv.exe  " + appPath + "\\Temp\\Asn1OutXml.xml  ";

                Icdr.Conn.ModFuc.ExecuteCmd(toolStripStatusLabel4.Text + appPath + "\\Temp\\SGSNPDPRecord.csv  " + appPath + "\\Workspace\\SGSNPDPRecord.txt ");
                Icdr.Conn.ModFuc.ExecuteCmd(toolStripStatusLabel4.Text + appPath + "\\Temp\\GGSNPDPRecord.csv  " + appPath + "\\Workspace\\GGSNPDPRecord.txt ");
                Icdr.Conn.ModFuc.ExecuteCmd(toolStripStatusLabel4.Text + appPath + "\\Temp\\GGSNPDPRecord1.csv  " + appPath + "\\Workspace\\GGSNPDPRecord1.txt ");
                Icdr.Conn.ModFuc.ExecuteCmd(toolStripStatusLabel4.Text + appPath + "\\Temp\\SGSNSMORecord.csv  " + appPath + "\\Workspace\\SGSNSMORecord.txt ");
                Icdr.Conn.ModFuc.ExecuteCmd(toolStripStatusLabel4.Text + appPath + "\\Temp\\SGSNSMTRecord.csv  " + appPath + "\\Workspace\\SGSNSMTRecord.txt ");
            }
        }

        private void ttipbtnCSV_Click(object sender, EventArgs e)
        {
            //Txt->Acc 
            toolStripStatusLabel3.Text = "Csv To Sql......";

            string strsql = null;
            string strdec = null;

            int JJJ = 0;

            object[] vv = new object[120];
            object[] v = new object[120];
            object[] fd = new object[120];
            string[] fl = new string[5];

            int j = 0;
            fl[0] = "\\Temp\\SGSNPDPRecord.csv";
            fl[1] = "\\Temp\\GGSNPDPRecord.csv";
            fl[2] = "\\Temp\\GGSNPDPRecord1.csv";
            fl[3] = "\\Temp\\SGSNSMORecord.csv";
            fl[4] = "\\Temp\\SGSNSMTRecord.csv";

            for (j = 0; j <= 4; j++)
            {
                if (!System.IO.File.Exists(appPath + fl[j])) return;

                StreamReader reader = new StreamReader(appPath + fl[j], System.Text.Encoding.GetEncoding("utf-8"));
                int n = 0;
                while (!reader.EndOfStream)
                {
                    //状态提示
                    toolStripStatusLabel2.Text = "正在处理" + n.ToString() + "行";
                    n = n + 1;
                    System.Windows.Forms.Application.DoEvents();
                    //写入数据库
                    string JetTemp = reader.ReadLine();
                    string[] aa = JetTemp.Split(Convert.ToChar(','));
                    if (aa[0].Length > 1 && aa[0].Length < 5)
                    {
                        strsql = null;
                        //把每行数据格式化 
                        for (int i = aa.GetLowerBound(0); i <= aa.GetUpperBound(0); i++)
                        {
                            switch (vv[i].ToString())
                            {
                                case "hextodec":
                                    v[i] = Icdr.Conn.HexConversion.hextodec(aa[i]);
                                    break;
                                //dec 
                                case "hextostr":
                                    v[i] = Icdr.Conn.HexConversion.hextostr(aa[i]);
                                    break;
                                //str 
                                case "hextoip":
                                    v[i] = Icdr.Conn.HexConversion.hextoip(aa[i]);
                                    break;
                                //ip 
                                case "hextonip":
                                    v[i] = Icdr.Conn.HexConversion.hextonip(aa[i]);
                                    break;
                                //nip 
                                case "hextotbcd":
                                    v[i] = Icdr.Conn.HexConversion.hextotbcd(aa[i]);
                                    break;
                                //tbcd 
                                case "hextobcd":
                                    v[i] = Icdr.Conn.HexConversion.hextobcd(aa[i]);
                                    break;
                                //bcd 
                                case "hextotime":
                                    v[i] = Icdr.Conn.HexConversion.hextotime(aa[i]);
                                    break;
                                //time 
                                case "hextohex":
                                    v[i] = Icdr.Conn.HexConversion.hextohex(aa[i]);
                                    break;
                                //hex 
                                case "hextoimsi":
                                    v[i] = Icdr.Conn.HexConversion.hextoimsi(aa[i]);
                                    break;
                                //imsi 
                                case "hextomsdn":
                                    v[i] = Icdr.Conn.HexConversion.hextomsdn(aa[i]);
                                    break;
                                //msdn 
                                case "hextoqos":
                                    v[i] = Icdr.Conn.HexConversion.hextoqos(aa[i]);
                                    break;
                                //qos 
                                case "hextootectstr1":
                                    v[i] = Icdr.Conn.HexConversion.hextootectstr1(aa[i]);
                                    break;
                                //octect string size1 
                                case "hextootectstr2":
                                    v[i] = Icdr.Conn.HexConversion.hextootectstr2(aa[i]);
                                    break;
                                //octect string size2 
                                case "Hex2CallEventRecordType":
                                    strdec = Icdr.Conn.HexConversion.hextodec(aa[i]);
                                    Icdr.Conn.ModEum.CallEventRecordType myCallEventRecordType =
                                        (Icdr.Conn.ModEum.CallEventRecordType)Enum.Parse(typeof(Icdr.Conn.ModEum.CallEventRecordType), strdec, true);
                                    v[i] = myCallEventRecordType.ToString() + "(" + strdec + ")";
                                    break;
                                case "Hex2CauseForRecClosing":
                                    strdec = Icdr.Conn.HexConversion.hextodec(aa[i]);
                                    Icdr.Conn.ModEum.CauseForRecClosing myCauseForRecClosing =
                                        (Icdr.Conn.ModEum.CauseForRecClosing)Enum.Parse(typeof(Icdr.Conn.ModEum.CauseForRecClosing), strdec, true);
                                    v[i] = myCauseForRecClosing.ToString() + "(" + strdec + ")";
                                    break;
                                case "Hex2ChangeCondition":
                                    strdec = Icdr.Conn.HexConversion.hextodec(aa[i]);
                                    Icdr.Conn.ModEum.ChangeCondition myChangeCondition =
                                        (Icdr.Conn.ModEum.ChangeCondition)Enum.Parse(typeof(Icdr.Conn.ModEum.ChangeCondition), strdec, true);
                                    v[i] = myChangeCondition.ToString() + "(" + strdec + ")";
                                    break;
                                case "Hex2Diagnostics":
                                    strdec = Icdr.Conn.HexConversion.hextodec(aa[i]);
                                    Icdr.Conn.ModEum.Diagnostics myDiagnostics =
                                        (Icdr.Conn.ModEum.Diagnostics)Enum.Parse(typeof(Icdr.Conn.ModEum.Diagnostics), strdec, true);
                                    v[i] = myDiagnostics.ToString() + "(" + strdec + ")";
                                    break;
                                case "Hex2APNSelectionMode":
                                    strdec = Icdr.Conn.HexConversion.hextodec(aa[i]);
                                    Icdr.Conn.ModEum.APNSelectionMode myAPNSelectionMode =
                                        (Icdr.Conn.ModEum.APNSelectionMode)Enum.Parse(typeof(Icdr.Conn.ModEum.APNSelectionMode), strdec, true);
                                    v[i] = myAPNSelectionMode.ToString() + "(" + strdec + ")";
                                    break;
                                case "Hex2ConsolidationResult":
                                    strdec = Icdr.Conn.HexConversion.hextodec(aa[i]);
                                    Icdr.Conn.ModEum.ConsolidationResult myConsolidationResult =
                                        (Icdr.Conn.ModEum.ConsolidationResult)Enum.Parse(typeof(Icdr.Conn.ModEum.ConsolidationResult), strdec, true);
                                    v[i] = myConsolidationResult.ToString() + "(" + strdec + ")";
                                    break;
                                default:
                                    v[i] = aa[i];
                                    break;
                            }
                            if (i < aa.GetUpperBound(0))
                            {
                                strsql = strsql + "'" + v[i].ToString() + "',";
                            }
                            else
                            {
                                strsql = strsql + "'" + v[i].ToString() + "'";
                            }
                        }

                        //把每行数据格式化后写入数据库 
                        switch (j)
                        {
                            case 0:
                                strsql = "insert into sgsnpdp values(" + strsql + ")";
                                break;
                            case 1:
                                strsql = "insert into ggsnpdp values(" + strsql + ")";
                                break;
                            case 2:
                                strsql = "insert into ggsnpdp1 values(" + strsql + ")";
                                break;
                            case 3:
                                strsql = "insert into sgsnmo values(" + strsql + ")";
                                break;
                            case 4:
                                strsql = "insert into sgsnmt values(" + strsql + ")";
                                break;
                        }
                        JJJ = JJJ + 1;
                        Acc.RunNoQuery(strsql);
                    }
                    if (aa[0].Length > 5)
                    {
                        strsql = "";
                        //创建数据表字段 
                        for (int i = aa.GetLowerBound(0); i <= aa.GetUpperBound(0); i++)
                        {
                            //字段名称 
                            XmlDocument document = new XmlDocument();
                            document.Load(appPath + "\\Workspace\\CdrFieldName.xml");
                            XmlNodeList nodes = document.GetElementsByTagName("GsnFieldName");
                            XmlNode node = default(XmlNode);
                            node = null;
                            for (int x = 0; x <= nodes.Count - 1; x++)
                            {
                                if (nodes.Item(x).Attributes.Item(0).Value == aa[i])
                                {
                                    node = nodes.Item(x);
                                }
                            }
                            //字段名称 
                            if (node == null)
                            {
                                fd[i] = "F" + aa[i];
                                vv[i] = "hextohex";
                            }
                            else
                            {
                                fd[i] = node.Attributes.Item(3).Value.ToString();
                                vv[i] = node.Attributes.Item(2).Value.ToString();
                            }
                            if (i < aa.GetUpperBound(0))
                            {
                                strsql = strsql + "[" + fd[i] + "]" + " TEXT(100),";
                            }
                            else
                            {
                                strsql = strsql + "[" + fd[i] + "]" + " TEXT(100)";
                            }
                        }
                        //创建数据表 
                        try
                        {
                            if (this.rbtnNEW.Checked == true && this.rbtnOLD.Checked == false)
                            {
                                switch (j)
                                {
                                    case 0:
                                        strsql = "create table sgsnpdp (" + strsql + " ) ";
                                        break;
                                    case 1:
                                        strsql = "create table ggsnpdp (" + strsql + " ) ";
                                        break;
                                    case 2:
                                        strsql = "create table ggsnpdp1 (" + strsql + " ) ";
                                        break;
                                    case 3:
                                        strsql = "create table sgsnmo (" + strsql + " ) ";
                                        break;
                                    case 4:
                                        strsql = "create table sgsnmt (" + strsql + " ) ";
                                        break;
                                }
                            }
                            //执行
                            Acc.RunNoQuery(strsql);
                        }
                        catch { }
                    }
                }
                reader.Close();
            }

            if (rbtnDEL.Checked)
            {
                for (int i = 0; i <= 4; i++)
                {
                    FileInfo xFile1 = new FileInfo(appPath + fl[i]);
                    xFile1.Delete();
                }
            }

            this.rbtnNEW.Checked = false;
            this.rbtnOLD.Checked = true;


        }

        private void lbASN1FILE_SelectedIndexChanged(object sender, EventArgs e)
        {
            //检验注册 
            if (bRegOK == true)
            {

                //Asn->Xml 
                toolStripStatusLabel3.Text = "Asn To Xml......";
                toolStripStatusLabel4.Text = appPath + "\\Exe\\asn2xml.exe  " + lbASN1FILE.SelectedItem.ToString() + "  -o  " + appPath + "\\Temp\\Asn1OutXml.xml ";
                Icdr.Conn.ModFuc.ExecuteCmd(toolStripStatusLabel4.Text);
            }
        }

        private void ttipbtnOPEN_Click(object sender, EventArgs e)
        {


            //选中相应的工作空间 
            mtipbtnOPEN.PerformClick();

            //把相应文件夹的文件列出 
            FolderBrowserDialog1.RootFolder = Environment.SpecialFolder.MyComputer;
            //FolderBrowserDialog1.SelectedPath = Environment.SpecialFolder.Recent;
            FolderBrowserDialog1.Description = "Open ASN.1 File";
            if (this.FolderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                this.lbASN1FILE.DataSource = System.IO.Directory.GetFiles(this.FolderBrowserDialog1.SelectedPath);
            }
            toolStripStatusLabel1.Text = "合计" + lbASN1FILE.Items.Count.ToString() + "文件";
            //检验注册号，把检验结果找一内存保存起来。不要在校验结果代码附近做任何影响程序正常工作的处理，这样不易被跟踪。 
            string regTail3 = GetRegTail3ByMac(serialNum);
            if (string.Compare(regTail3, 0, regNum, 12, 17) == 0)
            {
                bRegOK = true;
            }
            else
            {
                bRegOK = false;
            }
        }



        private void mtipbtnEXIT_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void mtipbtnQUERY_Click(object sender, EventArgs e)
        {
            for (int i = 0; i <= 4; i++)
            {
                if (tcICDR.TabPages[i].Name == "话单分析")
                {
                    tcICDR.SelectedIndex = i;
                }
            }
        }

        private void mtipbtnOPEN_Click(object sender, EventArgs e)
        {
            for (int i = 0; i <= 4; i++)
            {
                if (tcICDR.TabPages[i].Name == "话单导入")
                {
                    tcICDR.SelectedIndex = i;
                }
            }
        }

        private void ttipbtnQUERY_Click(object sender, EventArgs e)
        {
            //选中相应的工作空间 
            mtipbtnQUERY.PerformClick();
            try
            {
                DataTable dt = Acc.RunQuery(textBox2.Text);
                this.dtgvQUERY.AutoGenerateColumns = true;
                this.dtgvQUERY.DataSource = dt.DefaultView;
                this.dtgvQUERY.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells;
                this.dtgvQUERY.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.DisplayedCellsExceptHeaders;
                this.dtgvQUERY.BorderStyle = BorderStyle.Fixed3D;
                this.dtgvQUERY.EditMode = DataGridViewEditMode.EditOnEnter;
                this.dtgvQUERY.GridColor = Color.Blue;
            }
            catch { }

        }
        private void mtipbtnTXT_Click(object sender, EventArgs e)
        {
            for (int i = 0; i <= 4; i++)
            {
                if (tcICDR.TabPages[i].Name == "字段设置")
                {
                    tcICDR.SelectedIndex = i;
                }
            }
        }

        private void mtipbtnFIELD_Click(object sender, EventArgs e)
        {
            for (int i = 0; i <= 4; i++)
            {
                if (tcICDR.TabPages[i].Name == "字段格式")
                {
                    tcICDR.SelectedIndex = i;
                }
            }

        }

        private void mtipbtnTREE_Click(object sender, EventArgs e)
        {
            for (int i = 0; i <= 4; i++)
            {
                if (tcICDR.TabPages[i].Name == "话单应用")
                {
                    tcICDR.SelectedIndex = i;
                }
            }
        }

        private void ttipbtnTXT_Click(object sender, EventArgs e)
        {
            //选中相应的工作空间 
            mtipbtnTXT.PerformClick();
            //打开话单导入字段设置 
            OpenFileDialog dlgOpenfile = new OpenFileDialog();
            string strFileFullName = null;
            dlgOpenfile.Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
            dlgOpenfile.Title = "Open";
            dlgOpenfile.ShowDialog();
            dlgOpenfile.RestoreDirectory = true;
            if (!string.IsNullOrEmpty(dlgOpenfile.FileName))
            {
                strFileFullName = dlgOpenfile.FileName;
                rtxtTXTQUERY.LoadFile(strFileFullName, RichTextBoxStreamType.PlainText);
                toolStripStatusLabel4.Text = strFileFullName;
            }
        }

        private void ttipbtnFIELD_Click(object sender, EventArgs e)
        {
            //选中相应的工作空间 
            mtipbtnFIELD.PerformClick();
            //执行相应的查询 
            DataSet ds = new DataSet();
            if (rbtnFIELDCONVERTER.Checked)
            {
                ds.ReadXml(appPath + "\\Workspace\\CdrFieldName.xml");
            }
            if (rbtnTREEAPPLCATION.Checked)
            {
                ds.ReadXml(appPath + "\\Xml\\CdrAppTreeSql.xml");
            }
            if (rbtnFIELDENUM.Checked)
            {
                ds.ReadXml(appPath + "\\Xml\\CdrFieldContent.xml");
            }
            //if (rbtnFIELDENUM.Checked)
            //{
            //    ds.ReadXml(appPath + "\\Temp\\_asn1outxml.xsd");
            //}

            this.dtgvFIELD.AutoGenerateColumns = true;
            this.dtgvFIELD.DataSource = new DataView(ds.Tables[0]);
            this.dtgvFIELD.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells;
            this.dtgvFIELD.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.DisplayedCellsExceptHeaders;
            this.dtgvFIELD.BorderStyle = BorderStyle.Fixed3D;
            this.dtgvFIELD.EditMode = DataGridViewEditMode.EditOnEnter;
            this.dtgvFIELD.GridColor = Color.Blue;
        }
        //保存数据到xml，通过datagridview修改
        private void xMLToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //Messagebox所属于的类
            DialogResult result;
            result = MessageBox.Show(this, "YesOrNo", "你确定要更新xml数据吗？", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            //Messagebox返回的值
            if (result == DialogResult.Yes)
            {
                DataView dv = this.dtgvFIELD.DataSource as DataView;   //dataGridview的数据返回到内存
                DataTable dt = dv.Table.Copy();                          //复制数据到  DATATABLE 中
                if (dt == null)
                {
                    return;
                }
                if (rbtnFIELDCONVERTER.Checked)
                {
                    dt.WriteXml(appPath + "\\Workspace\\CdrFieldName.xml"); //写入XML 
                }
                if (rbtnTREEAPPLCATION.Checked)
                {
                    dt.WriteXml(appPath + "\\Xml\\CdrAppTreeSql.xml"); //写入XML 
                }
                if (rbtnFIELDENUM.Checked)
                {
                    dt.WriteXml(appPath + "\\Xml\\CdrFieldContent.xml"); //写入XML 
                }
            }
        }

        Asn1Parser xParser = new Asn1Parser();
        Asn1Node RootNode
        {
            get
            {
                return xParser.RootNode;
            }
        }



        private void btnOPENASN1FILE_Click(object sender, EventArgs e)
        {
 
        }

        //private void btnLISTDECODER_Click(object sender, EventArgs e)
        //{
        //    for (int m = 0; m < 5; m++)
        //    {
                
        //        MemoryStream ms = new MemoryStream(xParser.RawData);
        //        xParser.LoadData(ms);
        //        Asn1Parser xparser = new Asn1Parser();
               
        //        uint mask = 0;
        //        int i, j, k, l;
        //        for (i = 0; i < RootNode.ChildNodeCount; i++)
        //        {
        //            Asn1TreeNode tmpNode = new Asn1TreeNode(RootNode.GetChildNode(i), mask);
        //            lbxTESTASN1HEX.Items.Add(tmpNode.ANode.Path + "|" + tmpNode.ANode.GetDataStr(true));
        //            ASN12TXT(tmpNode.ANode.Path, tmpNode.ANode.GetDataStr(true));
        //            string hexStr = Asn1Unit.ConvertNodeValue(tmpNode.ANode.Path, tmpNode.ANode.GetDataStr(true));
        //            lbxTESTSTRING.Items.Add(tmpNode.ANode.TagName + "|" + hexStr);

        //            for (j = 0; j < tmpNode.ANode.ChildNodeCount; j++)
        //            {
        //                Asn1TreeNode tmpNodeChild = new Asn1TreeNode(tmpNode.ANode.GetChildNode(j), mask);
        //                lbxTESTASN1HEX.Items.Add(tmpNodeChild.ANode.Path + "|" + tmpNodeChild.ANode.GetDataStr(true));
        //                ASN12TXT(tmpNodeChild.ANode.Path, tmpNodeChild.ANode.GetDataStr(true));
        //                hexStr = Asn1Unit.ConvertNodeValue(tmpNodeChild.ANode.Path, tmpNodeChild.ANode.GetDataStr(true));
        //                lbxTESTSTRING.Items.Add(tmpNodeChild.ANode.TagName + "|" + hexStr);
        //                for (k = 0; k < tmpNodeChild.ANode.ChildNodeCount; k++)
        //                {
        //                    Asn1TreeNode tmpNodeChild1 = new Asn1TreeNode(tmpNodeChild.ANode.GetChildNode(k), mask);
        //                    lbxTESTASN1HEX.Items.Add(tmpNodeChild1.ANode.Path + "|" + tmpNodeChild1.ANode.GetDataStr(true));
        //                    ASN12TXT(tmpNodeChild1.ANode.Path, tmpNodeChild1.ANode.GetDataStr(true));
        //                    hexStr = Asn1Unit.ConvertNodeValue(tmpNodeChild1.ANode.Path, tmpNodeChild1.ANode.GetDataStr(true));
        //                    lbxTESTSTRING.Items.Add(tmpNodeChild1.ANode.TagName + "|" + hexStr);
        //                    for (l = 0; l < tmpNodeChild1.ANode.ChildNodeCount; l++)
        //                    {
        //                        Asn1TreeNode tmpNodeChild2 = new Asn1TreeNode(tmpNodeChild1.ANode.GetChildNode(l), mask);
        //                        lbxTESTASN1HEX.Items.Add(tmpNodeChild2.ANode.Path + "|" + tmpNodeChild2.ANode.GetDataStr(true));
        //                        ASN12TXT(tmpNodeChild2.ANode.Path, tmpNodeChild2.ANode.GetDataStr(true));
        //                        hexStr = Asn1Unit.ConvertNodeValue(tmpNodeChild2.ANode.Path, tmpNodeChild2.ANode.GetDataStr(true));
        //                        lbxTESTSTRING.Items.Add(tmpNodeChild2.ANode.TagName + "|" + hexStr);
        //                    }
        //                }

        //            }

        //        }

        //    }
        //}
        string strASN1;
        string ASN12TXT(string asn1path, string asn1string)
        {

            try
            {
                StreamWriter writer = null;             //方法一：写文本文件.txt
                string filepath = "F:log.txt";
                if (File.Exists(filepath))
                {
                    writer = File.AppendText(filepath);
                }
                else
                {
                    writer = File.CreateText(filepath);
                }
                if (asn1path == "/0")
                {
                    writer.WriteLine(strASN1);
                    strASN1 = Asn1Unit.ConvertNodeValue(asn1path, asn1string);
                }
                else { strASN1 += "," + Asn1Unit.ConvertNodeValue(asn1path, asn1string); }
                writer.Close();
            }
            catch { }
            return strASN1;
        }
        private void rbtnSGSNPDP_CheckedChanged(object sender, EventArgs e)
        {
            try
            {
                textBox2.Text = "select top 100 * from sgsnpdp";
                DataTable dt = Acc.RunQuery("select * from sgsnpdp");
                ttipTXTCOUNT.Text = "记录数量：" + dt.Rows.Count.ToString();
            }
            catch { }
        }

        private void rbtnGGSNPDP_CheckedChanged(object sender, EventArgs e)
        {
            try
            {
                textBox2.Text = "select top 100 * from ggsnpdp";
                DataTable dt = Acc.RunQuery("select * from ggsnpdp");
                ttipTXTCOUNT.Text = "记录数量：" + dt.Rows.Count.ToString();
            }
            catch { }
        }
        private void rbtnGGSNWAP_CheckedChanged(object sender, EventArgs e)
        {
            try
            {
                textBox2.Text = "select top 100 * from ggsnpdp1";
                DataTable dt = Acc.RunQuery("select * from ggsnpdp1");
                ttipTXTCOUNT.Text = "记录数量：" + dt.Rows.Count.ToString();
            }
            catch { }
        }
        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            try
            {
                TreeNode Node1 = this.treeView1.GetNodeAt(this.treeView1.PointToClient(Control.MousePosition));
                txtTREEFIELD.Text = Node1.Text;

                if (Node1.Parent.Text == "SGSN CDR")
                {
                    txtTREEQUERY.Text = "select " + txtTREEFIELD.Text + ",count(*) as 统计时间出现次数 from sgsnpdp group by " + txtTREEFIELD.Text + " order by 2 desc";
                }
                if (Node1.Parent.Text == "GGSN CDR")
                {
                    txtTREEQUERY.Text = "select " + txtTREEFIELD.Text + ",count(*) as 统计时间出现次数 from ggsnpdp group by " + txtTREEFIELD.Text + " order by 2 desc";
                }
                if (Node1.Parent.Text == "cmwap CDR")
                {
                    txtTREEQUERY.Text = "select " + txtTREEFIELD.Text + ",count(*) as 统计时间出现次数 from ggsnpdp1 group by " + txtTREEFIELD.Text + " order by 2 desc";
                }
                if (Node1.Parent.Text == "Intelligent CDR")
                {
                    XmlDocument myDoc1 = new XmlDocument();
                    myDoc1.Load(appPath + "\\Xml\\CdrAppTreeSql.xml");
                    System.Xml.XmlNodeList nodes2 = myDoc1.GetElementsByTagName("ICDR");
                    if (nodes2 != null)
                    {
                        foreach (System.Xml.XmlNode xn in nodes2)
                        {
                            if (txtTREEFIELD.Text == xn.Attributes.Item(1).Value.ToString())
                            {
                                txtTREEQUERY.Text = xn.Attributes.Item(2).Value.ToString();
                            }
                        }
                    }
                }
                if (Node1 != null)
                {
                    DataTable dt = Acc.RunQuery(txtTREEQUERY.Text);
                    {
                        this.dtgvTREE.AutoGenerateColumns = true;
                        this.dtgvTREE.DataSource = dt.DefaultView;
                        this.dtgvTREE.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
                        this.dtgvTREE.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.DisplayedCells;
                        this.dtgvTREE.BorderStyle = BorderStyle.Fixed3D;
                        this.dtgvTREE.EditMode = DataGridViewEditMode.EditOnEnter;
                        this.dtgvTREE.GridColor = Color.Blue;
                    }
                }
            }
            catch (Exception exp)
            {
                MessageBox.Show(exp.ToString());
            }
            //检验注册号，把检验结果找一内存保存起来。不要在校验结果代码附近做任何影响程序正常工作的处理，这样不易被跟踪。 
            string regTail3 = GetRegTail3ByMac(serialNum);
            if (string.Compare(regTail3, 0, regNum, 12, 17) == 0)
            {
                bRegOK = true;
            }
            else
            {
                bRegOK = false;
            }
        }
        private void btnCLEARDB_Click(object sender, EventArgs e)
        {
            //清空数据表的方法，把数据库中的表格读入datatable,循环读取后删除
            try
            {
                DataTable dt = Acc.Tables;
                string tb= null;
                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    tb = dt.Rows[i]["TABLE_NAME"].ToString();
                    Acc.RunNoQuery("drop table " + tb);
                }
            }
            catch { }
            //清空数据表的方法，把数据库中的表格读入datatable,循环读取后删除
            try
            {
                DataTable dt =Sqlserv . Tables;
                string tb = null;
                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    tb = dt.Rows[i]["TABLE_NAME"].ToString();
                    Sqlserv.RunNoQuery("drop table " + tb);
                }
            }
            catch { }
            MessageBox.Show("OK!");
        }
        private void rbtnERICSSON_CheckedChanged(object sender, EventArgs e)
        {
            string[] s = System.IO.Directory.GetFiles(appPath + "\\Ericsson");
            for (int i = 0; i < s.Length; i++)
            {
                string d = appPath + "\\Workspace" + s[i].Substring(s[i].LastIndexOf("\\"));
                if (File.Exists(d)) { File.Delete(d); }
                File.Copy(s[i], d);
            }
        }
        private void rbtnNOKIA_CheckedChanged(object sender, EventArgs e)
        {
            string[] s = System.IO.Directory.GetFiles(appPath + "\\Nokia");
            for (int i = 0; i < s.Length; i++)
            {
                string d = appPath + "\\Workspace" + s[i].Substring(s[i].LastIndexOf("\\"));
                if (File.Exists(d)) { File.Delete(d); }
                File.Copy(s[i], d);
            }
        }
        #region 比较机器码
        /// <summary>
        /// 获取机器码
        /// </summary>
        /// <param name="序列号"></param>
        /// <returns>带“-”的四段数字</returns>
        string 获取机器码(string seriesNum)
        {
            string macAdd = GetMacAddress();
            string md5Str = FormsAuthentication.HashPasswordForStoringInConfigFile(macAdd + seriesNum, "MD5");
            return GetCode(md5Str, 4);
        }
        #endregion

        private void ttipbtnTree_Click(object sender, EventArgs e)
        {
            mtipbtnTREE.PerformClick();
        }

        private void ttipbtnRUN_Click(object sender, EventArgs e)
        {
            try
            {
                string JetTemp = rtxtTXTQUERY.Text;
                string[] aa = JetTemp.Split(Convert.ToChar(';'));
                for (int i = aa.GetLowerBound(0); i <= aa.GetUpperBound(0); i++)
                {
                    if (aa[i] != "")
                    {
                        rtxtTXTEXCUTE.Text = aa[i];
                        if (rtxtTXTEXCUTE.Text.IndexOf("//") == -1)//没有被双斜杠注释的代码,执行。
                        {
                            Acc.RunNoQuery(rtxtTXTEXCUTE.Text);
                        }
                    }
                }
            }
            catch { }
            File.Delete(toolStripStatusLabel4.Text);
            File.AppendAllText(toolStripStatusLabel4.Text, rtxtTXTQUERY.Text);//保存执行脚本
        }

        private void copyToolStripMenuItem_Click(object sender, EventArgs e)
        {

            Clipboard.SetDataObject(rtxtTXTQUERY.SelectedText);//拷贝选中文本
            IDataObject iData = Clipboard.GetDataObject();
            textBox2.Text = (String)iData.GetData(DataFormats.Text); //放到指定的文本控件
        }

        private void mtipbtnHELP_Click(object sender, EventArgs e)
        {
            MessageBox.Show("ICDR_EDGE 版本 1.0");
        }
        private void cmtipTREEEXCELexcel_Click(object sender, EventArgs e)
        {
            try
            {
                    Icdr.Conn.EXL.ExportForDataGridview(dtgvTREE, txtTREEFIELD.Text, true);
            }
            catch { }
        }

        private void cmtipQUERYEXCELexcel_Click(object sender, EventArgs e)
        {
            try
            {
                //如何判断某个radiobutton被选中
                string sheetname = "ICDR_EDGE";
                bool result1 = this.rbtnSGSNPDP.Checked;
                bool result2 = this.rbtnGGSNPDP.Checked;
                bool result3 = this.rbtnGGSNWAP.Checked;
                if (result1 == true) { sheetname = "SGSNPDP"; }
                if (result2 == true) { sheetname = "GGSNPDP"; }
                if (result3 == true) { sheetname = "GGSNWAP"; }
                Icdr.Conn.EXL.ExportForDataGridview(dtgvQUERY, sheetname, true);
            }
            catch { }
        }

        private void rbtnTABLE_CheckedChanged(object sender, EventArgs e)
        {
            try
            {
                DataTable dt = Acc.GetColumns();
                dtgvQUERY.DataSource =dt.DefaultView;
                ttipTXTCOUNT.Text = "记录数量：" + dt.Rows.Count.ToString();
            }
            catch { }
        }

        private void rbtnDATABASE_CheckedChanged(object sender, EventArgs e)
        {
            try
            {
                DataTable dt = Acc.Tables;
                dtgvQUERY.DataSource = dt.DefaultView;
                ttipTXTCOUNT.Text = "记录数量：" + dt.Rows.Count.ToString();
            }
            catch { }
        }


        //运用递归过程遍历treeView的所有的节点
        public void searchNode(Asn1TreeNode tmpNode,TreeNode tn)
        {
            uint mask = 0;
            for (int i = 0; i < tmpNode .ANode .ChildNodeCount ; i++)
            {
                Asn1TreeNode tmpNodeChild = new Asn1TreeNode(tmpNode.ANode.GetChildNode(i), mask);
                string hexStr = Asn1Unit.ConvertNodeValue(tmpNodeChild.ANode.Path, tmpNodeChild.ANode.GetDataStr(true));
                TreeNode tnChild = new TreeNode(tmpNodeChild.ANode.Path + "::" + tmpNodeChild.ANode.GetDataStr(true) + "::"
                     + hexStr);
                tn.Nodes.Add(tnChild );
                searchNode(tmpNodeChild, tnChild);
            }
        }

  

        private void toolStripButtonOpen1BillFile_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    lbxTESTSTRING.Items.Clear();
                    //Asn->Xml 
                    toolStripStatusLabel3.Text = "Asn To Xml......";
                    toolStripStatusLabel4.Text = appPath + "\\Exe\\asn2xml.exe  " + openFileDialog1.FileName + "  -o  " + appPath + "\\Temp\\Asn1OutXml.xml ";
                    Icdr.Conn.ModFuc.ExecuteCmd(toolStripStatusLabel4.Text);
                    StreamReader reader = new StreamReader(appPath + "\\Temp\\Asn1OutXml.xml");
                    int i = 0;
                    while (!reader.EndOfStream)
                    {
                        lbxTESTSTRING.Items.Add(reader.ReadLine());
                        i++;
                        Application.DoEvents();
                        if (i == 100) { break; }
                    }
                    reader.Close();
                    while (!File.Exists(appPath + "\\Temp\\Asn1OutXml.xml"))
                    { Application.DoEvents(); }
                    XmlReader reader1 = XmlReader.Create(appPath + "\\Temp\\Asn1OutXml.xml");
                    XmlSchemaSet schemaSet = new XmlSchemaSet();
                    XmlSchemaInference schema = new XmlSchemaInference();
                    schemaSet = schema.InferSchema(reader1);
                    XmlSchema compiledSchema = null;

                    foreach (XmlSchema s in schemaSet.Schemas())
                    {
                        compiledSchema = s;
                        lbxXsd.Items.Add(s.Elements.Names.ToString());

                        foreach (XmlSchemaObject schemaObject in compiledSchema.Items)
                        {
                            lbxXsd.Items.Add(schemaObject.SourceUri.Length+schemaObject.ToString().Length);
                            if (schemaObject.GetType() == typeof(XmlSchemaSimpleType))
                            {
                                XmlSchemaSimpleType simpleType = (XmlSchemaSimpleType)schemaObject;
                                lbxXsd.Items.Add("{0} {1}" + simpleType.Name + simpleType.Datatype.ValueType);
                            }
                            if (schemaObject.GetType() == typeof(XmlSchemaComplexType))
                            {
                                XmlSchemaComplexType complexType = (XmlSchemaComplexType)schemaObject;
                                lbxXsd.Items.Add("{0} {1}" + complexType.Name + complexType.Datatype.ValueType);
                            }
                        }
                    }

                    //regNum = reader.ReadLine();
                    // DataSet ds = new DataSet();
                    // ds.ReadXml (appPath + "\\Temp\\Asn1OutXml.xml");
                    // DataTable dt=ds.Tables [0];
                    // for (int i = 0; i < dt.Rows.Count; i++)
                    // {
                    //     lbxTESTSTRING.Items.Add(dt.Rows[i].ToString());
                    // }
                    //xParser.LoadData(openFileDialog1.FileName);
                }
                catch (Exception ex)
                {
                    string msg = ex.Message;
                    MessageBox.Show(msg);
                }
            }
        }

        private void toolStripButtonOpen1ASN_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    lbxTESTASN1HEX.Items.Clear();
                    //Asn->Xml 
                    toolStripStatusLabel3.Text = openFileDialog1.FileName;
                    //toolStripStatusLabel4.Text = openFileDialog1.FileName;
                    //Icdr.Conn.ModFuc.ExecuteCmd(toolStripStatusLabel4.Text);
                    StreamReader reader = new StreamReader(toolStripStatusLabel3.Text);
                    //int i = 0;
                    while (!reader.EndOfStream)
                    {
                        lbxTESTASN1HEX.Items.Add(reader.ReadLine());
                        //i++;
                        //Application.DoEvents();
                        //if (i == 100) { break; }
                    }
                    reader.Close();
                }
                catch (Exception ex)
                {
                    string msg = ex.Message;
                    MessageBox.Show(msg);
                }
            }
        }

        private void toolStripButtonExit_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void mtipICDR_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        private void rbtnFIELDCONVERTER_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void rbtnTREEAPPLCATION_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void radioButtonCdrHexField_CheckedChanged(object sender, EventArgs e)
        {

        }
        //private void button1_Click(object sender, EventArgs e)
        //{
        //    for (int m = 0; m < 10; m++)
        //    {
        //        MemoryStream ms = new MemoryStream(xParser.RawData);
        //        xParser.LoadData(ms);
        //        //Asn1Parser xparser = new Asn1Parser();
        //        uint mask = 0;
        //        TreeNode root = new TreeNode(m.ToString());
        //        treeView2.Nodes.Add(root);
        //        for (int i = 0; i < xParser.RootNode.ChildNodeCount; i++)
        //        {
        //            Asn1TreeNode tmpNode = new Asn1TreeNode(xParser.RootNode.GetChildNode(i), mask);
        //            string hexStr = Asn1Unit.ConvertNodeValue(tmpNode.ANode.Path, tmpNode.ANode.GetDataStr(true));
        //            TreeNode tn = new TreeNode(tmpNode.ANode.Path + "::" + tmpNode.ANode.GetDataStr(true) + "::"
        //               + hexStr);
        //            root.Nodes.Add(tn);
        //            searchNode(tmpNode, tn);
        //        }
        //    }
        //}

        //private void button2_Click(object sender, EventArgs e)
        //{
        //    //MemoryStream ms = new MemoryStream(xParser.RawData);
        //    //xParser.LoadData(ms);
        //    //Asn1Parser xparser = new Asn1Parser();
        //    // Asn1TreeNode tmpNode = new Asn1TreeNode();
        //    //treeView2.Nodes.Add("a");
        //    //treeView2.Nodes.Add(tmpNode);
        //}
        }

    }