using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;

namespace softerCell_U3_v0._01
{
    public partial class softerCell : Form
    {
        public softerCell()
        {
            InitializeComponent();

        }

        private allMessageFlow f0 = new allMessageFlow();
        private msMessageFlow f1 = new msMessageFlow();
        private messageParameterList f2 = new messageParameterList();

        private void allMessageFlowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            allMessageFlow fr0 = new allMessageFlow();
            fr0.MdiParent = this;
            fr0.Show();
        }

        private void msMessageFlowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            msMessageFlow fr1 = new msMessageFlow();
            fr1.MdiParent = this;
            fr1.Show();
        }

        private void messageParameterListToolStripMenuItem_Click(object sender, EventArgs e)
        {
            messageParameterList fr2 = new messageParameterList();
            fr2.MdiParent = this;
            fr2.Show();
        }

        private void dataAnlysisToolStripMenuItem_Click(object sender, EventArgs e)
        {
            dataAnlysis fr3 = new dataAnlysis();
            fr3.MdiParent = this;
            fr3.Show();
        }
        private void softerCell_Load(object sender, EventArgs e)
        {
            streamMessagePool.appPath = Application.StartupPath.ToString();

            f0.MdiParent = this;
            f0.Show();
            //f1.MdiParent = this;
            //f1.Show();
            //f2.MdiParent = this;
            //f2.Show();

            middleDelegate.sendPEvent += new middleDelegate.SendPMessage(this.fileConvertListProgress);

            toolStripStatusLabel1.Text = string.Empty;
            toolStripStatusLabel2.Text = string.Empty;
            toolStripStatusLabel3.Text = string.Empty;
            toolStripStatusLabel5.Text = string.Empty;
            statusStrip1.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow; 
            toolStripStatusLabel4.Text = "http://www.softercell.com";
            toolStripStatusLabel4.IsLink = true;
            toolStripStatusLabel4.Alignment = ToolStripItemAlignment.Right;

            toolStripProgressBar1.Maximum = fileConvertList.line_total_number;
            toolStripProgressBar1.Minimum = 0;
            toolStripProgressBar1.Value = 0;
        }

        string startTime = string.Empty;
        private void openFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
               
                streamMessagePool.signallingFilePath = openFileDialog1.FileName;

                toolStripStatusLabel1.Text = streamMessagePool.signallingFilePath;

                startTime = ProcessTime();

                fileConvertList.open_iu_cs_file();

                f0.Show();

                middleDelegate.DoSendListMessage();//----------------------------------------->

                string licPath = streamMessagePool.appPath + "\\log\\OpenFileRecord";
                File.AppendAllText(licPath, streamMessagePool.signallingFilePath + Environment.NewLine);

            }
        }

        private void fileConvertListProgress(int i)//<-----------------------------------------
        {
            toolStripProgressBar1.Value = i;
        }
        private bool dropTable(string tableName)
        {
            bool b = false;
            DataTable dt = streamMessagePool.Acc.Tables;
            string tb = null;
            for (int i = 0; i < dt.Rows.Count; i++)
            {
                tb = dt.Rows[i]["TABLE_NAME"].ToString();
                if (tb.IndexOf(tableName) != -1)
                {
                    streamMessagePool.Acc.RunNoQuery("drop table " + tb);
                    //Application.DoEvents();
                    b = true;
                }
            }
            return b;
        }
        private void listInsertSqlToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (dropTable("softerCell_have_read")) { toolStripStatusLabel5.Text = "softerCell_have_read" + "： delete"; }
            toolStripProgressBar1.Maximum = streamMessagePool.ml.Count();
            startTime = ProcessTime();
            //middleDelegate.DoSendListMessage();//----------------------------------------->
            listConvertSql.saveStreamMessageSql("softerCell_have_read");
            toolStripStatusLabel5.Text = "softerCell_have_read" + "： OK";
        }

        private void kpiInsertSqlToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (dropTable("softerCell_kpi_stat")) { toolStripStatusLabel5.Text = "softerCell_kpi_stat" + "： delete"; }
            toolStripProgressBar1.Maximum = streamMessagePool.ml.Count();
            startTime = ProcessTime();
            //middleDelegate.DoSendListMessage();//----------------------------------------->
            kpiConvertSql.mobile_kpi_stat_result();
            toolStripStatusLabel5.Text = "softerCell_kpi_stat" + "： OK";
        }

        private void msHandoverInSqlToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (dropTable("softerCell_ho_in")) { toolStripStatusLabel5.Text = "softerCell_ho_in" + "： delete"; }
            toolStripProgressBar1.Maximum = streamMessagePool.ml.Count();
            startTime = ProcessTime();
            //middleDelegate.DoSendListMessage();//----------------------------------------->
            msHandoverInRNCflow.mobile_handover_in_rnc_flow();
            toolStripStatusLabel5.Text = "softerCell_ho_in" + "： OK";
        }

        private void msHandoverOutSqlToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (dropTable("softerCell_ho_out")) { toolStripStatusLabel5.Text = "softerCell_ho_out" + "： delete"; }
            toolStripProgressBar1.Maximum = streamMessagePool.ml.Count();
            startTime = ProcessTime();
            //middleDelegate.DoSendListMessage();//----------------------------------------->
            msHandoverOutRNCflow.mobile_handover_out_rnc_flow();
            toolStripStatusLabel5.Text = "softerCell_ho_out" + "： OK";
        }

        private void msIuReleaseRequestSqlToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (dropTable("softerCell_iu_rr")) { toolStripStatusLabel5.Text = "softerCell_iu_rr" + "： delete"; }
            toolStripProgressBar1.Maximum = streamMessagePool.ml.Count();
            startTime = ProcessTime();
            //middleDelegate.DoSendListMessage();//----------------------------------------->
            msIuReleaseRequestFlow.mobile_iu_release_request_flow();
            toolStripStatusLabel5.Text = "softerCell_iu_rr" + "： OK";
        }

        private void msLocationUpToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (dropTable("softerCell_lau")) { toolStripStatusLabel5.Text = "softerCell_lau" + "： delete"; }
            toolStripProgressBar1.Maximum = streamMessagePool.ml.Count();
            startTime = ProcessTime();
            //middleDelegate.DoSendListMessage();//----------------------------------------->
            msLocationUpdateFlow.mobile_location_update_flow();
            toolStripStatusLabel5.Text = "softerCell_lau" + "： OK";
        }

        private void msOriginalCallSqlToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (dropTable("softerCell_moc")) { toolStripStatusLabel5.Text = "softerCell_moc" + "： delete"; }
            toolStripProgressBar1.Maximum = streamMessagePool.ml.Count();
            startTime = ProcessTime();
            //middleDelegate.DoSendListMessage();//----------------------------------------->
            msOriginalCallFlow.mobile_original_call_flow();
            toolStripStatusLabel5.Text = "softerCell_moc" + "： OK";
        }

        private void msTerminalCallSqlToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //GC.Collect();
            if (dropTable("softerCell_mtc")) { toolStripStatusLabel5.Text = "softerCell_mtc" + "： delete"; }
            toolStripProgressBar1.Maximum = streamMessagePool.ml.Count();
            startTime = ProcessTime();
            //middleDelegate.DoSendListMessage();//----------------------------------------->
            msTerminalCallFlow.mobile_terminal_call_flow();
            toolStripStatusLabel5.Text = "softerCell_mtc" + "： OK";
        }

        private void exitEToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            msHandoverInSqlToolStripMenuItem.PerformClick();
            msHandoverOutSqlToolStripMenuItem.PerformClick();
            msIuReleaseRequestSqlToolStripMenuItem.PerformClick();
            msLocationUpToolStripMenuItem.PerformClick();
            msOriginalCallSqlToolStripMenuItem.PerformClick();
            msTerminalCallSqlToolStripMenuItem.PerformClick();       
            isupCallFlowSqlToolStripMenuItem.PerformClick();
            kpiInsertSqlToolStripMenuItem.PerformClick();
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            openFileToolStripMenuItem.PerformClick();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (sender == timer1)
            {
                toolStripStatusLabel2.Text = " Time now: " + ProcessTime();

                if (startTime != string.Empty)
                {
                    TimeSpan dt = DateTime.Parse(ProcessTime()) - DateTime.Parse(startTime);
                    toolStripStatusLabel3.Text = " Time elapsed: "
                         + dt.Hours.ToString()
                         + ":" + dt.Minutes.ToString()
                         + ":" + dt.Seconds.ToString();
                }
            }
        }

        public string ProcessTime()
        {
            string sTime;
            // Get the current time
            int iHour = DateTime.Now.Hour;
            int iMin = DateTime.Now.Minute;
            int iSec = DateTime.Now.Second;
            // Format the current time into a string...
            sTime = (iHour < 10) ? "0" + iHour.ToString() : iHour.ToString();
            sTime += ":" + ((iMin < 10) ? "0" + iMin.ToString() : iMin.ToString());
            sTime += ":" + ((iSec < 10) ? "0" + iSec.ToString() : iSec.ToString());
            return sTime;
        }

        //解决路径名中不能有空格的问题
        [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
        public static extern int GetShortPathName([MarshalAs(UnmanagedType.LPTStr)]   string path,
        [MarshalAs(UnmanagedType.LPTStr)]   StringBuilder shortPath, int shortPathLength);
        private void openPCAPToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //文件过滤且写入
            //cd   D:\Program Files\Wireshark\
            //tshark -V   -R "m3ua"  -r  D:\DX188\merge_cap.pcap > D:\DX188\merge_cap
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                string pcapFile = openFileDialog1.FileName;
                string _pcapFile = pcapFile.Replace(".pcap", "_pcap");
                StringBuilder shortPath = new StringBuilder(80);
                int result = GetShortPathName( @"D:\Program Files\Wireshark\tshark.exe", shortPath, shortPath.Capacity);
                string s = shortPath.ToString();
                //MessageBox.Show(s.ToString());
                string openPCAP = s + " -V   -R  " + "\"" + "m3ua" + "\"" + "  -r  " + pcapFile + ">" + _pcapFile;
                Thread th = new Thread(delegate() { fileConvertList.ExecuteCmd(openPCAP); });
                th.Name = "openPCAP";
                th.Start();
                MessageBox.Show(openPCAP);
            }
        }

        private void mergePCAPToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //文件合并
            //cd   D:\Program Files\Wireshark\
            //mergecap -v -T ether -w D:\DX188\merge_cap.pcap   
            //D:\DX188\0pcap.pcap  D:\DX188\1pcap.pcap  D:\DX188\2pcap.pcap   D:\DX188\3pcap.pcap
            this.openFileDialog1.Multiselect = true;
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                string pcapFile = string.Empty;
                string _pcapFile = string.Empty;
                foreach (String file in openFileDialog1.FileNames)
                {
                    _pcapFile = file.Replace(".pcap", "_pcap");
                    pcapFile += file + " ";
                }
                StringBuilder shortPath = new StringBuilder(80);
                int result = GetShortPathName(@"D:\Program Files\Wireshark\mergecap.exe", shortPath, shortPath.Capacity);
                string s = shortPath.ToString();
                string openPCAP = s + "  -v -T ether -w  " + _pcapFile + " " + pcapFile;
                Thread th = new Thread(delegate() { fileConvertList.ExecuteCmd(openPCAP); });
                th.Name = "openPCAP";
                th.Start();
                MessageBox.Show(openPCAP);
            }
        }

        private void toolStripStatusLabel4_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("http://www.softercell.com");
        }

        private void isupCallFlowSqlToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (dropTable("softerCell_isup_iam")) { toolStripStatusLabel5.Text = "softerCell_isup_iam" + "： delete"; }
            toolStripProgressBar1.Maximum = streamMessagePool.ml.Count();
            startTime = ProcessTime();
            //middleDelegate.DoSendListMessage();//----------------------------------------->
            isupCallFlow.isup_iam_acm_flow();
            toolStripStatusLabel5.Text = "softerCell_isup_iam" + "： OK";
        }
    }
}
