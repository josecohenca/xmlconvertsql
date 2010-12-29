namespace DB_CONN
{
    partial class frmDBCONN
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmDBCONN));
            this.tbDBCONN = new System.Windows.Forms.TabControl();
            this.ACCDB = new System.Windows.Forms.TabPage();
            this.label1 = new System.Windows.Forms.Label();
            this.txtACCDB = new System.Windows.Forms.TextBox();
            this.MDB = new System.Windows.Forms.TabPage();
            this.label7 = new System.Windows.Forms.Label();
            this.txtMDB = new System.Windows.Forms.TextBox();
            this.MYSQL = new System.Windows.Forms.TabPage();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.txtMYSQLdir = new System.Windows.Forms.TextBox();
            this.txtMYSQLname = new System.Windows.Forms.TextBox();
            this.SQL_SERVER = new System.Windows.Forms.TabPage();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.txtPASSWORD = new System.Windows.Forms.TextBox();
            this.txtUSER = new System.Windows.Forms.TextBox();
            this.txtSQLSERVER = new System.Windows.Forms.TextBox();
            this.Oracle = new System.Windows.Forms.TabPage();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.saveconnToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.退出EToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tbDBCONN.SuspendLayout();
            this.ACCDB.SuspendLayout();
            this.MDB.SuspendLayout();
            this.MYSQL.SuspendLayout();
            this.SQL_SERVER.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tbDBCONN
            // 
            this.tbDBCONN.Alignment = System.Windows.Forms.TabAlignment.Bottom;
            this.tbDBCONN.Controls.Add(this.ACCDB);
            this.tbDBCONN.Controls.Add(this.MDB);
            this.tbDBCONN.Controls.Add(this.MYSQL);
            this.tbDBCONN.Controls.Add(this.SQL_SERVER);
            this.tbDBCONN.Controls.Add(this.Oracle);
            this.tbDBCONN.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbDBCONN.Location = new System.Drawing.Point(0, 24);
            this.tbDBCONN.Multiline = true;
            this.tbDBCONN.Name = "tbDBCONN";
            this.tbDBCONN.SelectedIndex = 0;
            this.tbDBCONN.Size = new System.Drawing.Size(673, 319);
            this.tbDBCONN.TabIndex = 0;
            // 
            // ACCDB
            // 
            this.ACCDB.Controls.Add(this.label1);
            this.ACCDB.Controls.Add(this.txtACCDB);
            this.ACCDB.Location = new System.Drawing.Point(4, 4);
            this.ACCDB.Name = "ACCDB";
            this.ACCDB.Padding = new System.Windows.Forms.Padding(3);
            this.ACCDB.Size = new System.Drawing.Size(665, 292);
            this.ACCDB.TabIndex = 0;
            this.ACCDB.Text = "ACCDB";
            this.ACCDB.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(1, 54);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(89, 12);
            this.label1.TabIndex = 2;
            this.label1.Text = "Database Name:";
            // 
            // txtACCDB
            // 
            this.txtACCDB.AcceptsReturn = true;
            this.txtACCDB.Location = new System.Drawing.Point(3, 69);
            this.txtACCDB.Name = "txtACCDB";
            this.txtACCDB.Size = new System.Drawing.Size(659, 21);
            this.txtACCDB.TabIndex = 0;
            // 
            // MDB
            // 
            this.MDB.Controls.Add(this.label7);
            this.MDB.Controls.Add(this.txtMDB);
            this.MDB.Location = new System.Drawing.Point(4, 4);
            this.MDB.Name = "MDB";
            this.MDB.Padding = new System.Windows.Forms.Padding(3);
            this.MDB.Size = new System.Drawing.Size(665, 292);
            this.MDB.TabIndex = 1;
            this.MDB.Text = "MDB";
            this.MDB.UseVisualStyleBackColor = true;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(6, 71);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(89, 12);
            this.label7.TabIndex = 2;
            this.label7.Text = "Database Name:";
            // 
            // txtMDB
            // 
            this.txtMDB.Location = new System.Drawing.Point(8, 86);
            this.txtMDB.Name = "txtMDB";
            this.txtMDB.Size = new System.Drawing.Size(654, 21);
            this.txtMDB.TabIndex = 0;
            // 
            // MYSQL
            // 
            this.MYSQL.Controls.Add(this.label3);
            this.MYSQL.Controls.Add(this.label2);
            this.MYSQL.Controls.Add(this.txtMYSQLdir);
            this.MYSQL.Controls.Add(this.txtMYSQLname);
            this.MYSQL.Location = new System.Drawing.Point(4, 4);
            this.MYSQL.Name = "MYSQL";
            this.MYSQL.Size = new System.Drawing.Size(665, 292);
            this.MYSQL.TabIndex = 2;
            this.MYSQL.Text = "MYSQL";
            this.MYSQL.UseVisualStyleBackColor = true;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(3, 125);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(65, 12);
            this.label3.TabIndex = 3;
            this.label3.Text = "Directory:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 37);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(89, 12);
            this.label2.TabIndex = 2;
            this.label2.Text = "Database Name:";
            // 
            // txtMYSQLdir
            // 
            this.txtMYSQLdir.Location = new System.Drawing.Point(5, 140);
            this.txtMYSQLdir.Name = "txtMYSQLdir";
            this.txtMYSQLdir.Size = new System.Drawing.Size(652, 21);
            this.txtMYSQLdir.TabIndex = 1;
            // 
            // txtMYSQLname
            // 
            this.txtMYSQLname.AcceptsReturn = true;
            this.txtMYSQLname.Location = new System.Drawing.Point(5, 52);
            this.txtMYSQLname.Name = "txtMYSQLname";
            this.txtMYSQLname.Size = new System.Drawing.Size(652, 21);
            this.txtMYSQLname.TabIndex = 0;
            // 
            // SQL_SERVER
            // 
            this.SQL_SERVER.Controls.Add(this.label6);
            this.SQL_SERVER.Controls.Add(this.label5);
            this.SQL_SERVER.Controls.Add(this.label4);
            this.SQL_SERVER.Controls.Add(this.txtPASSWORD);
            this.SQL_SERVER.Controls.Add(this.txtUSER);
            this.SQL_SERVER.Controls.Add(this.txtSQLSERVER);
            this.SQL_SERVER.Location = new System.Drawing.Point(4, 4);
            this.SQL_SERVER.Name = "SQL_SERVER";
            this.SQL_SERVER.Size = new System.Drawing.Size(665, 292);
            this.SQL_SERVER.TabIndex = 3;
            this.SQL_SERVER.Text = "SQL_SERVER";
            this.SQL_SERVER.UseVisualStyleBackColor = true;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(103, 190);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(59, 12);
            this.label6.TabIndex = 5;
            this.label6.Text = "Password:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(103, 130);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(65, 12);
            this.label5.TabIndex = 4;
            this.label5.Text = "User Name:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(103, 69);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(89, 12);
            this.label4.TabIndex = 3;
            this.label4.Text = "Database Name:";
            // 
            // txtPASSWORD
            // 
            this.txtPASSWORD.Location = new System.Drawing.Point(198, 181);
            this.txtPASSWORD.Name = "txtPASSWORD";
            this.txtPASSWORD.Size = new System.Drawing.Size(334, 21);
            this.txtPASSWORD.TabIndex = 2;
            // 
            // txtUSER
            // 
            this.txtUSER.AcceptsReturn = true;
            this.txtUSER.Location = new System.Drawing.Point(198, 130);
            this.txtUSER.Name = "txtUSER";
            this.txtUSER.Size = new System.Drawing.Size(334, 21);
            this.txtUSER.TabIndex = 1;
            // 
            // txtSQLSERVER
            // 
            this.txtSQLSERVER.Location = new System.Drawing.Point(198, 69);
            this.txtSQLSERVER.Name = "txtSQLSERVER";
            this.txtSQLSERVER.Size = new System.Drawing.Size(334, 21);
            this.txtSQLSERVER.TabIndex = 0;
            // 
            // Oracle
            // 
            this.Oracle.Location = new System.Drawing.Point(4, 4);
            this.Oracle.Name = "Oracle";
            this.Oracle.Size = new System.Drawing.Size(665, 292);
            this.Oracle.TabIndex = 4;
            this.Oracle.Text = "Oracle";
            this.Oracle.UseVisualStyleBackColor = true;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveconnToolStripMenuItem,
            this.退出EToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(673, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // saveconnToolStripMenuItem
            // 
            this.saveconnToolStripMenuItem.Name = "saveconnToolStripMenuItem";
            this.saveconnToolStripMenuItem.Size = new System.Drawing.Size(60, 20);
            this.saveconnToolStripMenuItem.Text = " 保存(&S)";
            this.saveconnToolStripMenuItem.Click += new System.EventHandler(this.saveconnToolStripMenuItem_Click);
            // 
            // 退出EToolStripMenuItem
            // 
            this.退出EToolStripMenuItem.Name = "退出EToolStripMenuItem";
            this.退出EToolStripMenuItem.Size = new System.Drawing.Size(57, 20);
            this.退出EToolStripMenuItem.Text = "退出(&E)";
            this.退出EToolStripMenuItem.Click += new System.EventHandler(this.退出EToolStripMenuItem_Click);
            // 
            // frmDBCONN
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(673, 343);
            this.Controls.Add(this.tbDBCONN);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "frmDBCONN";
            this.Text = "DB-CONN";
            this.Load += new System.EventHandler(this.frmDBCONN_Load);
            this.tbDBCONN.ResumeLayout(false);
            this.ACCDB.ResumeLayout(false);
            this.ACCDB.PerformLayout();
            this.MDB.ResumeLayout(false);
            this.MDB.PerformLayout();
            this.MYSQL.ResumeLayout(false);
            this.MYSQL.PerformLayout();
            this.SQL_SERVER.ResumeLayout(false);
            this.SQL_SERVER.PerformLayout();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TabControl tbDBCONN;
        private System.Windows.Forms.TabPage ACCDB;
        private System.Windows.Forms.TabPage MDB;
        private System.Windows.Forms.TabPage MYSQL;
        private System.Windows.Forms.TabPage SQL_SERVER;
        private System.Windows.Forms.TabPage Oracle;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem saveconnToolStripMenuItem;
        private System.Windows.Forms.TextBox txtACCDB;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txtMYSQLdir;
        private System.Windows.Forms.TextBox txtMYSQLname;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox txtPASSWORD;
        private System.Windows.Forms.TextBox txtUSER;
        private System.Windows.Forms.TextBox txtSQLSERVER;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox txtMDB;
        private System.Windows.Forms.ToolStripMenuItem 退出EToolStripMenuItem;

    }
}

