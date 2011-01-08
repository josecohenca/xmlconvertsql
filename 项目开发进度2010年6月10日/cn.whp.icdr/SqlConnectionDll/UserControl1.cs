using Microsoft.VisualBasic;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Diagnostics;
using System.Windows.Forms;
using System.ComponentModel;
using System.Data.Odbc;
using System.Data.SqlClient;

internal partial class SQLConnectionDialog
{
    private SqlConnection cp;

    private SQLConnectionDialog uic;
    //Allows the user to change the title of the dialog
    public override string Text
    {
        get { return base.Text; }
        set { base.Text = value; }
    }

    //Pass the original connection string or get the resulting connection string
    public string ConnectionString
    {
        get { return cp.ConnectionString ; }
        set { cp.ConnectionString = value; }
    }


    private void OK_Button_Click(System.Object sender, System.EventArgs e)
    {
        this.DialogResult = System.Windows.Forms.DialogResult.OK;
        this.Close();
    }

    private void Cancel_Button_Click(System.Object sender, System.EventArgs e)
    {
        this.DialogResult = System.Windows.Forms.DialogResult.Cancel;
        this.Close();
    }

    private void Dialog1_Load(System.Object sender, System.EventArgs e)
    {
        this.Padding = new Padding(5);
        Button adv = new Button();
        Button Tst = new Button();

        //Size the form and place the uic, Test connection button, and advanced button
        uic.Load();
        uic.Dock = DockStyle.Top;
        uic.Parent = this;
        this.ClientSize = Size.Add(uic.MinimumSize, new Size(10, (adv.Height + 25)));
        this.MinimumSize = this.Size;
        {
            adv.Text = "Advanced";
            adv.Dock = DockStyle.None;
            adv.Location = new Point((uic.Width - adv.Width), (uic.Bottom + 10));
            adv.Anchor = (AnchorStyles.Right | AnchorStyles.Top);
            adv.Click += this.Advanced_Click;
            adv.Parent = this;
        }

        {
            Tst.Text = "Test Connection";
            Tst.Width = 100;
            Tst.Dock = DockStyle.None;
            Tst.Location = new Point((uic.Width - Tst.Width) - adv.Width - 10, (uic.Bottom + 10));
            Tst.Anchor = (AnchorStyles.Right | AnchorStyles.Top);
            Tst.Click += this.Test_Click;
            Tst.Parent = this;
        }
    }

    private void Advanced_Click(object sender, EventArgs e)
    {
        //Set up a form to display the advanced connection properties
        Form frm = new Form();
        PropertyGrid pg = new PropertyGrid();
        pg.SelectedObject = cp;
        pg.Dock = DockStyle.Fill;
        pg.Parent = frm;
        frm.ShowDialog();
    }

    private void Test_Click(object sender, EventArgs e)
    {
        //Test the connection
        SqlConnection conn = new SqlConnection();
        conn.ConnectionString = cp.ConnectionStringBuilder.ConnectionString;
        try
        {
            conn.Open();
            Interaction.MsgBox("Test Connection Succeeded.", MsgBoxStyle.Exclamation);
        }
        catch (Exception ex)
        {
            Interaction.MsgBox("Test Connection Failed.", MsgBoxStyle.Critical);
        }
        finally
        {
            try
            {
                conn.Close();
            }
            catch (Exception ex)
            {

            }
        }

    }
}
