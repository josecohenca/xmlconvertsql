namespace 示例
{
    partial class Form1
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
            this.校验注册号代码Btn = new System.Windows.Forms.Button();
            this.核心功能代码代表Btn = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // 校验注册号代码Btn
            // 
            this.校验注册号代码Btn.Location = new System.Drawing.Point(74, 182);
            this.校验注册号代码Btn.Name = "校验注册号代码Btn";
            this.校验注册号代码Btn.Size = new System.Drawing.Size(206, 72);
            this.校验注册号代码Btn.TabIndex = 0;
            this.校验注册号代码Btn.Text = "校验注册号代码";
            this.校验注册号代码Btn.UseVisualStyleBackColor = true;
            this.校验注册号代码Btn.Click += new System.EventHandler(this.校验注册号代码Btn_Click);
            // 
            // 核心功能代码代表Btn
            // 
            this.核心功能代码代表Btn.Location = new System.Drawing.Point(74, 75);
            this.核心功能代码代表Btn.Name = "核心功能代码代表Btn";
            this.核心功能代码代表Btn.Size = new System.Drawing.Size(206, 38);
            this.核心功能代码代表Btn.TabIndex = 1;
            this.核心功能代码代表Btn.Text = "核心功能代码代表";
            this.核心功能代码代表Btn.UseVisualStyleBackColor = true;
            this.核心功能代码代表Btn.Click += new System.EventHandler(this.核心功能代码代表Btn_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(74, 12);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(100, 21);
            this.textBox1.TabIndex = 2;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 266);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.核心功能代码代表Btn);
            this.Controls.Add(this.校验注册号代码Btn);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button 校验注册号代码Btn;
        private System.Windows.Forms.Button 核心功能代码代表Btn;
        private System.Windows.Forms.TextBox textBox1;
    }
}

