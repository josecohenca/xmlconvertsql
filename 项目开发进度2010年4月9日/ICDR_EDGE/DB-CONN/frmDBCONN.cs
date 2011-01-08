using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Net;
using System.IO;

namespace DB_CONN
{
    public partial class frmDBCONN : Form
    {
        public frmDBCONN()
        {
            InitializeComponent();
        }

        [DllImport("kernel32")]
        private static extern long WritePrivateProfileString(string section, string key, string val, string filePath);
        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);
        string appPath = Application.StartupPath.ToString();

        private void saveconnToolStripMenuItem_Click(object sender, EventArgs e)
        {
            WritePrivateProfileString("database connection", "database type", tbDBCONN.SelectedTab.Name, appPath + "\\Xml\\users.ini");
            for (int i = 0; i <= 4; i++)
            {
                if (tbDBCONN.TabPages[i].Name == "ACCDB")
                {
                    WritePrivateProfileString("database drive", "ACCDB", txtACCDB.Text, appPath + "\\Xml\\users.ini");
                }
                if (tbDBCONN.TabPages[i].Name == "MDB")
                {
                    WritePrivateProfileString("database drive", "MDB", txtMDB.Text , appPath + "\\Xml\\users.ini");
                }
                if (tbDBCONN.TabPages[i].Name == "MYSQL")
                {
                    WritePrivateProfileString("database drive", "MYSQLname", txtMYSQLname.Text, appPath + "\\Xml\\users.ini");
                    WritePrivateProfileString("database drive", "MYSQLdir", txtMYSQLdir.Text, appPath + "\\Xml\\users.ini");
                }
                if (tbDBCONN.TabPages[i].Name == "SQL_SERVER")
                {
                    WritePrivateProfileString("database drive", "SQL_SERVER", txtSQLSERVER.Text, appPath + "\\Xml\\users.ini");
                    WritePrivateProfileString("database drive", "SQL_USER", txtUSER.Text, appPath + "\\Xml\\users.ini");
                    WritePrivateProfileString("database drive", "SQL_PASSWORD", txtPASSWORD.Text, appPath + "\\Xml\\users.ini");
                }
                if (tbDBCONN.TabPages[i].Name == "Oracle")
                {
                    WritePrivateProfileString("database drive", "Oracle", txtSQLSERVER.Text, appPath + "\\Xml\\users.ini");
                    WritePrivateProfileString("database drive", "Oracle_USER", txtUSER.Text, appPath + "\\Xml\\users.ini");
                    WritePrivateProfileString("database drive", "Oracle_PASSWORD", txtPASSWORD.Text, appPath + "\\Xml\\users.ini");
                }
            }
            MessageBox.Show("OK!");
        }


        private void frmDBCONN_Load(object sender, EventArgs e) 
        { 
            txtACCDB.Text=appPath +"\\Db\\iCDR.accdb";
            txtMDB.Text = appPath + "\\Db\\iCDR.mdb";
            txtMYSQLname.Text = "ICDR";
            txtMYSQLdir.Text = appPath;
            txtSQLSERVER.Text = "ICDR";
            txtUSER.Text = "sa";
            txtPASSWORD.Text = "1";
        }

        private void 退出EToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
