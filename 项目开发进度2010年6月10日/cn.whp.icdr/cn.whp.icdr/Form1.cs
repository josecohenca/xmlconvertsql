using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Configuration;
using System.Windows.Forms;
using System.Xml;


namespace cn.whp.icdr
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string conn =ConfigurationManager.ConnectionStrings["conn"].ConnectionString;

            string app = ConfigurationManager.AppSettings ["app"];

            MessageBox.Show(conn);
            MessageBox.Show(app);

        }
    }
}
