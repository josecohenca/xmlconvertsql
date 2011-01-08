using System;
using System.ComponentModel;
using System.Configuration;
using System.Drawing;
using System.Windows.Forms;

namespace CustomConfigExample
{
	public partial class Form1 : Form
	{
		MySection settings;

		public Form1()
		{
			InitializeComponent();
			this.txtPort.Text = ConfigurationManager.AppSettings["Port"];
			this.txtLevel.Text = ConfigurationManager.AppSettings["Level"];
		}

		private void btnOpenSection_Click(object sender, EventArgs e)
		{
			settings = MySection.Open();
			this.btnSaveSection.Enabled = true;
			this.btnOpenSection.Enabled = false;
			this.txtDevAShape.Text = settings.UnitShapeConfig.DevA.Shape;
			this.txtDevBShape.Text = settings.UnitShapeConfig.DevB.Shape;
			this.chkEnableShapes.Checked = settings.UnitShapeConfig.Enable;
		}

		private void btnSaveSection_Click(object sender, EventArgs e)
		{
			settings.UnitShapeConfig.DevA.Shape = this.txtDevAShape.Text;
			settings.UnitShapeConfig.DevB.Shape = this.txtDevBShape.Text;
			settings.UnitShapeConfig.Enable = this.chkEnableShapes.Checked;
			settings.Save();
		}

		private void btnSaveAppSettings_Click(object sender, EventArgs e)
		{
			Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
			config.AppSettings.Settings["Port"].Value = this.txtPort.Text;
			config.AppSettings.Settings["Level"].Value = this.txtLevel.Text;
			config.Save(ConfigurationSaveMode.Modified);
		}

		private void btnViewFile_Click(object sender, EventArgs e)
		{
			string path = Application.ExecutablePath + ".config";
			System.Diagnostics.Process proc = new System.Diagnostics.Process();
			proc.StartInfo = new System.Diagnostics.ProcessStartInfo("Notepad.exe", '\"' + path + '\"');
			proc.Start();
		}

        private void Form1_Load(object sender, EventArgs e)
        {

        }

	}
}