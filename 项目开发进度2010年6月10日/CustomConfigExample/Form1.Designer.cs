namespace CustomConfigExample
{
	partial class Form1
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.txtLevel = new System.Windows.Forms.TextBox();
            this.btnSaveAppSettings = new System.Windows.Forms.Button();
            this.txtPort = new System.Windows.Forms.TextBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.txtDevBShape = new System.Windows.Forms.TextBox();
            this.txtDevAShape = new System.Windows.Forms.TextBox();
            this.chkEnableShapes = new System.Windows.Forms.CheckBox();
            this.btnSaveSection = new System.Windows.Forms.Button();
            this.btnOpenSection = new System.Windows.Forms.Button();
            this.btnViewFile = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.txtLevel);
            this.groupBox1.Controls.Add(this.btnSaveAppSettings);
            this.groupBox1.Controls.Add(this.txtPort);
            this.groupBox1.Location = new System.Drawing.Point(12, 11);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(224, 73);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "AppSettings";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(111, 22);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 12);
            this.label2.TabIndex = 8;
            this.label2.Text = "Level";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(16, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(29, 12);
            this.label1.TabIndex = 7;
            this.label1.Text = "Port";
            // 
            // txtLevel
            // 
            this.txtLevel.Location = new System.Drawing.Point(147, 19);
            this.txtLevel.Name = "txtLevel";
            this.txtLevel.Size = new System.Drawing.Size(43, 21);
            this.txtLevel.TabIndex = 6;
            // 
            // btnSaveAppSettings
            // 
            this.btnSaveAppSettings.Location = new System.Drawing.Point(136, 43);
            this.btnSaveAppSettings.Name = "btnSaveAppSettings";
            this.btnSaveAppSettings.Size = new System.Drawing.Size(82, 21);
            this.btnSaveAppSettings.TabIndex = 5;
            this.btnSaveAppSettings.Text = "Save Settings";
            this.btnSaveAppSettings.UseVisualStyleBackColor = true;
            this.btnSaveAppSettings.Click += new System.EventHandler(this.btnSaveAppSettings_Click);
            // 
            // txtPort
            // 
            this.txtPort.Location = new System.Drawing.Point(52, 19);
            this.txtPort.Name = "txtPort";
            this.txtPort.Size = new System.Drawing.Size(43, 21);
            this.txtPort.TabIndex = 4;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.txtDevBShape);
            this.groupBox2.Controls.Add(this.txtDevAShape);
            this.groupBox2.Controls.Add(this.chkEnableShapes);
            this.groupBox2.Controls.Add(this.btnSaveSection);
            this.groupBox2.Controls.Add(this.btnOpenSection);
            this.groupBox2.Location = new System.Drawing.Point(12, 98);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(224, 137);
            this.groupBox2.TabIndex = 5;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "MySection";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(33, 85);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(65, 12);
            this.label4.TabIndex = 8;
            this.label4.Text = "DevB Shape";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(33, 62);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(65, 12);
            this.label3.TabIndex = 7;
            this.label3.Text = "DevA Shape";
            // 
            // txtDevBShape
            // 
            this.txtDevBShape.Location = new System.Drawing.Point(108, 82);
            this.txtDevBShape.Name = "txtDevBShape";
            this.txtDevBShape.Size = new System.Drawing.Size(53, 21);
            this.txtDevBShape.TabIndex = 6;
            // 
            // txtDevAShape
            // 
            this.txtDevAShape.Location = new System.Drawing.Point(108, 59);
            this.txtDevAShape.Name = "txtDevAShape";
            this.txtDevAShape.Size = new System.Drawing.Size(53, 21);
            this.txtDevAShape.TabIndex = 5;
            // 
            // chkEnableShapes
            // 
            this.chkEnableShapes.AutoSize = true;
            this.chkEnableShapes.Location = new System.Drawing.Point(36, 43);
            this.chkEnableShapes.Name = "chkEnableShapes";
            this.chkEnableShapes.Size = new System.Drawing.Size(126, 16);
            this.chkEnableShapes.TabIndex = 4;
            this.chkEnableShapes.Text = "Enable UnitShapes";
            this.chkEnableShapes.UseVisualStyleBackColor = true;
            // 
            // btnSaveSection
            // 
            this.btnSaveSection.Enabled = false;
            this.btnSaveSection.Location = new System.Drawing.Point(136, 110);
            this.btnSaveSection.Name = "btnSaveSection";
            this.btnSaveSection.Size = new System.Drawing.Size(82, 21);
            this.btnSaveSection.TabIndex = 3;
            this.btnSaveSection.Text = "Save Section";
            this.btnSaveSection.UseVisualStyleBackColor = true;
            this.btnSaveSection.Click += new System.EventHandler(this.btnSaveSection_Click);
            // 
            // btnOpenSection
            // 
            this.btnOpenSection.Location = new System.Drawing.Point(6, 18);
            this.btnOpenSection.Name = "btnOpenSection";
            this.btnOpenSection.Size = new System.Drawing.Size(82, 21);
            this.btnOpenSection.TabIndex = 2;
            this.btnOpenSection.Text = "Open Section";
            this.btnOpenSection.UseVisualStyleBackColor = true;
            this.btnOpenSection.Click += new System.EventHandler(this.btnOpenSection_Click);
            // 
            // btnViewFile
            // 
            this.btnViewFile.Location = new System.Drawing.Point(55, 244);
            this.btnViewFile.Name = "btnViewFile";
            this.btnViewFile.Size = new System.Drawing.Size(128, 21);
            this.btnViewFile.TabIndex = 6;
            this.btnViewFile.Text = "View Configuration File";
            this.btnViewFile.UseVisualStyleBackColor = true;
            this.btnViewFile.Click += new System.EventHandler(this.btnViewFile_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(250, 276);
            this.Controls.Add(this.btnViewFile);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "Form1";
            this.Text = "Configuration Demo";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox txtLevel;
		private System.Windows.Forms.Button btnSaveAppSettings;
		private System.Windows.Forms.TextBox txtPort;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.CheckBox chkEnableShapes;
		private System.Windows.Forms.Button btnSaveSection;
		private System.Windows.Forms.Button btnOpenSection;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.TextBox txtDevBShape;
		private System.Windows.Forms.TextBox txtDevAShape;
		private System.Windows.Forms.Button btnViewFile;

	}
}

