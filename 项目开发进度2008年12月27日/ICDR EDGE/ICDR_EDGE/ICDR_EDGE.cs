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
//using LAY3_R;


namespace ICDR
{
    public partial class ICDR_EDGE : Form
    {
        public ICDR_EDGE()
        {
            InitializeComponent();
        }
        private void ICDR_EDGE_Load(object sender, EventArgs e)
        {
            //加密模块开始 
            appPath = Application.StartupPath.ToString ();
            string licPath = appPath + "\\LAY3-T.lic";
            LAY3_R.frmRegister form = new LAY3_R.frmRegister();
            if (!File.Exists(licPath))
            {
                form.ShowDialog();
                if (!File.Exists(licPath))
                {
                    this.Close();
                    return;
                }
            }
            //加密模块检查序列号 
            StreamReader reader = new StreamReader(licPath, System.Text.Encoding.GetEncoding("utf-8"));
            serialNum = reader.ReadLine();
            machineNum = reader.ReadLine();
            regNum = reader.ReadLine();
            reader.Close();
            if (!CheckRegHead(machineNum, regNum))
            {
                //这儿会对序列号正确性做简单校验，不合格的会有提示。以后再校验出问题就判定为恶意破解，不再有提示，直接修改系统运行结果。 
                MessageBox.Show("序列号不正确，系统将退出，请与软件供应商联系！");
                this.Close();
            }
            else
            {
                this.WindowState = FormWindowState.Maximized;
            }
            //加载ACCESS
            Acc.Init(serialNum, appPath + "\\iCDR.accdb", serialNum, serialNum);
            Acc.Open();
              
            
            ////加载XML
            //XmlDocument doc = new XmlDocument();
            //doc.Load(appPath + "\\CongfigSql.xml");
            //var node = doc.DocumentElement.SelectSingleNode("//Variable");
            //string attributeValue = node.Attributes(0).Value;
            //加载XML到TREE
            XmlDocument document = new XmlDocument();
            //document.Load(appPath + "\\CdrAppTreeSql.xml");
            //ICDR.ModXml.populateTreeControl(document.DocumentElement, TreeView1.Nodes);
            //TreeView1.ExpandAll();
           //初始化窗口展开
            RadioButton1.Checked = true ;
            RadioButton7.Checked = true ;
            RadioButton9.Checked = true ;
            this.WindowState = FormWindowState.Maximized;
        }
        Icdr.Conn.IDBAccess Acc = Icdr.Conn.DBAccessFactory.Create(Icdr.Conn.DBType.Access);
        string appPath;
        string serialNum;
        string machineNum;
        string regNum;
        bool bRegOK = true;
        private void TreeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {

            //try
            ////{
            //    TreeNode Node1 = this.TreeView1.GetNodeAt(this.TreeView1.PointToClient(Control.MousePosition));
            //    TextBox1.Text = (Node1.Index + 1).ToString();
            //    TextBox2.Text = Node1.Text;
            //    XmlDocument document = new XmlDocument();
            //    document.Load(appPath + "\\CdrAppTreeSql.xml");
            //    XmlNodeList nodes = document.GetElementsByTagName("CdrApp");
                //XmlNode node = default(XmlNode);
                //node = null;
            //    for (int i = 0; i <= nodes.Count - 1; i++)
            //    {
            //        if (nodes.Item(i).Attributes.Item(0).Value== Node1.Index + 1)
            //        {
            //            node = nodes.Item(i);
            //        }
            //    }
            //    if ((Node1 != null))
            //    {
            //        openRs(node.Attributes.Item(2).Value.ToString);
            //        dts = RecordsetToDataTable(rs, "CdrApp");
            //        {
            //            this.DataGridView3.AutoGenerateColumns = true;
            //            this.DataGridView3.DataSource = dts.DefaultView;
            //            this.DataGridView3.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells;
            //            this.DataGridView3.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.DisplayedCellsExceptHeaders;
            //            this.DataGridView3.BorderStyle = BorderStyle.Fixed3D;
            //            this.DataGridView3.EditMode = DataGridViewEditMode.EditOnEnter;
            //            this.DataGridView3.GridColor = Color.Blue;
            //        }
            //    }
            //}
            //catch (Exception exp)
            //{
            //    MessageBox.Show(exp.ToString());
            //}
            //finally
            //{
            //    cloRs();
            //}
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

            }
        }

        private void ttipbtnXML_Click(object sender, EventArgs e)
        {
            //检验注册 
            if (bRegOK == true)
            {

                //Xml->Csv 
                toolStripStatusLabel3.Text = "Xml To Csv......";
                toolStripStatusLabel4.Text = "xml2csv.exe Asn1OutXml.xml ";

                Icdr.Conn.ModFuc.ExecuteCmd(toolStripStatusLabel4.Text + "SGSNPDPRecord.csv SGSNPDPRecord.txt ");
                Icdr.Conn.ModFuc.ExecuteCmd(toolStripStatusLabel4.Text + "GGSNPDPRecord.csv GGSNPDPRecord.txt ");
                Icdr.Conn.ModFuc.ExecuteCmd(toolStripStatusLabel4.Text + "SGSNSMORecord.csv SGSNSMORecord.txt ");
                Icdr.Conn.ModFuc.ExecuteCmd(toolStripStatusLabel4.Text + "SGSNSMTRecord.csv SGSNSMTRecord.txt ");
            }
        }

        private void ttipbtnCSV_Click(object sender, EventArgs e)
        {
            //Txt->Acc 
            toolStripStatusLabel3.Text = "Csv To Sql......";

            string strsql = null;
  
            int JJJ = 0;

            object[] vv = new object[51];
            object[] v = new object[51];
            object[] fd = new object[51];
            string[] fl = new string[4];
       
            int j = 0;
            fl[0] = "\\SGSNPDPRecord.csv";
            fl[1] = "\\GGSNPDPRecord.csv";
            fl[2] = "\\SGSNSMORecord.csv";
            fl[3] = "\\SGSNSMTRecord.csv";
             
            for (j = 0; j <= 3; j++)
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
                    if (aa[0].Length < 5)
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
                                strsql = "insert into sgsnmo values(" + strsql + ")";
                                break;
                            case 3:
                                strsql = "insert into sgsnmt values(" + strsql + ")";
                                break;
                        }
                        JJJ = JJJ + 1;
                        Acc.RunNoQuery(strsql);
                    }
                    else
                    {
                        strsql = "";
                        //创建数据表字段 
                        for (int i = aa.GetLowerBound(0); i <= aa.GetUpperBound(0); i++)
                        {
                            //字段名称 
                            XmlDocument document = new XmlDocument();
                            document.Load(appPath + "\\CdrFieldName.xml");
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
                        if (this.RadioButton1.Checked == true | this.RadioButton2.Checked == false)
                        {
                            switch (j)
                            {
                                case 0:
                                    //判断表是否存在
                                    if (Acc.GetColumns("sgsnpdp").Rows.Count !=-1 ) Acc.RunNoQuery("drop table sgsnpdp");
                                    strsql = "create table sgsnpdp (" + strsql + " ) ";
                                    break;
                                case 1:
                                    if (Acc.GetColumns("ggsnpdp").Rows.Count != -1) Acc.RunNoQuery("drop table ggsnpdp");
                                    strsql = "create table ggsnpdp (" + strsql + " ) ";
                                    break;
                                case 2:
                                    if (Acc.GetColumns("sgsnmo").Rows.Count != -1) Acc.RunNoQuery("drop table sgsnmo");
                                    strsql = "create table sgsnmo (" + strsql + " ) ";
                                    break;
                                case 3:
                                    if (Acc.GetColumns("sgsnmt").Rows.Count != -1) Acc.RunNoQuery("drop table sgsnmt");
                                    strsql = "create table sgsnmt (" + strsql + " ) ";
                                    break;
                            }
                        }
                        //执行
                        Acc.RunNoQuery(strsql);
                    }
                }
            }

            if (RadioButton7.Checked)
            {
                for (int i = 0; i <= 3; i++)
                {
                    FileInfo xFile1 = new FileInfo(appPath + fl[i]);
                    xFile1.Delete();
                }
            }

            this.RadioButton1.Checked = false;
            this.RadioButton2.Checked = true;


        }

        private void lbASN1FILE_SelectedIndexChanged(object sender, EventArgs e)
        {
            //检验注册 
            if (bRegOK == true)
            {

            //Asn->Xml 
            toolStripStatusLabel3.Text = "Asn To Xml......";
            toolStripStatusLabel4.Text = "asn2xml.exe  " + lbASN1FILE.SelectedItem.ToString() + "  -o Asn1OutXml.xml";
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
                if (TabControl1.TabPages[i].Name == "话单分析")
                {
                    TabControl1.SelectedIndex = i;
                }
            }
        }

        private void mtipbtnOPEN_Click(object sender, EventArgs e)
        {
            for (int i = 0; i <= 4; i++)
            {
                if (TabControl1.TabPages[i].Name == "话单导入")
                {
                    TabControl1.SelectedIndex = i;
                }
            }
        }

        private void ttipbtnQUERY_Click(object sender, EventArgs e)
        {
            //选中相应的工作空间 
            mtipbtnQUERY.PerformClick();
            DataTable dt = Acc.RunQuery("select top 100 * from sgsnpdp");
            this.dataGridView2.AutoGenerateColumns = true;
            this.dataGridView2.DataSource = dt.DefaultView;
            this.dataGridView2.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells;
            this.dataGridView2.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.DisplayedCellsExceptHeaders;
            this.dataGridView2.BorderStyle = BorderStyle.Fixed3D;
            this.dataGridView2.EditMode = DataGridViewEditMode.EditOnEnter;
            this.dataGridView2.GridColor = Color.Blue;

        }

        private void mtipbtnEXIT_Click_1(object sender, EventArgs e)
        {
            this.Close();
        }

       private void mtipbtnTXT_Click(object sender, EventArgs e)
        {

        
            for (int i = 0; i <= 4; i++)
            {
                if (TabControl1.TabPages[i].Name == "字段格式")
                {
                    TabControl1.SelectedIndex = i;
                }
            }

        }

        private void mtipFIELD_Click(object sender, EventArgs e)
        {
            for (int i = 0; i <= 4; i++)
            {
                if (TabControl1.TabPages[i].Name == "字段设置")
                {
                    TabControl1.SelectedIndex = i;
                }
            }
        }

        private void mtipbtnTREE_Click(object sender, EventArgs e)
        {
            for (int i = 0; i <= 4; i++)
            {
                if (TabControl1.TabPages[i].Name == "话单应用")
                {
                    TabControl1.SelectedIndex = i;
                }
            }
        }

        private void ttipbtnTXT_Click(object sender, EventArgs e)
        {
            //选中相应的工作空间 
            mtipFIELD.PerformClick();
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
                RichTextBox1.LoadFile(strFileFullName, RichTextBoxStreamType.PlainText);
            }
        }

        private void ttipbtnFIELD_Click(object sender, EventArgs e)
        {
            //选中相应的工作空间 
            mtipbtnTXT.PerformClick();
            //执行相应的查询 
            DataSet ds = new DataSet();
            if (RadioButton9.Checked)
            {
                ds.ReadXml(appPath + "\\CdrFieldName.xml");
            }
            if (RadioButton10.Checked)
            {
                ds.ReadXml(appPath + "\\CdrAppTreeSql.xml");
            }
            if (RadioButton11.Checked)
            {
                ds.ReadXml(appPath + "\\CdrFieldContent.xml");
            }
            this.dataGridView1.AutoGenerateColumns = true;
            this.dataGridView1.DataSource = new DataView(ds.Tables[1]);
            this.dataGridView1.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells;
            this.dataGridView1.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.DisplayedCellsExceptHeaders;
            this.dataGridView1.BorderStyle = BorderStyle.Fixed3D;
            this.dataGridView1.EditMode = DataGridViewEditMode.EditOnEnter;
            this.dataGridView1.GridColor = Color.Blue;
        }

 
    }
}

