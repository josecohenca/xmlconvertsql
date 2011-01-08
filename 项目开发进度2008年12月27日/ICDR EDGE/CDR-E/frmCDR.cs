using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using Asn1Processor;


namespace CDR_E
{
    public partial class frmCDR : Form
    {
        public frmCDR()
        {
            InitializeComponent();
        }
        Asn1Parser xParser = new Asn1Parser();
        Asn1Node RootNode
        {
            get
            {
                return xParser.RootNode;
            }
        }


        private void toolStripButton1_Click(object sender, EventArgs e)

        {
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    xParser.LoadData(openFileDialog.FileName);
                }
                catch (Exception ex)
                {
                    string msg = ex.Message;
                    MessageBox.Show(msg);
                }
            }

           
        }

                private void toolStripButton2_Click(object sender, EventArgs e)
 
        {
            for (int m = 0; m < 5; m++)
            {

                MemoryStream ms = new MemoryStream(xParser.RawData);
                xParser.LoadData(ms);
                Asn1Parser xparser = new Asn1Parser();
                uint mask = 0;
                int i,j, k, l;
                for (i = 0; i < RootNode.ChildNodeCount; i++)
                {
                    Asn1TreeNode tmpNode = new Asn1TreeNode(RootNode.GetChildNode(i), mask);
                    listBox1.Items.Add(tmpNode.ANode.Path + "|" + tmpNode.ANode.GetDataStr(true));
                    string hexStr = Asn1Unit.ConvertNodeValue(tmpNode.ANode.Path, tmpNode.ANode.GetDataStr(true));
                    listBox2.Items.Add(tmpNode.ANode.TagName  + "|" + hexStr);

                    for (j = 0; j < tmpNode.ANode.ChildNodeCount; j++)
                        {
                            Asn1TreeNode tmpNodeChild = new Asn1TreeNode(tmpNode.ANode.GetChildNode(j), mask);
                            listBox1.Items.Add(tmpNodeChild.ANode.Path + "|" + tmpNodeChild.ANode.GetDataStr(true));
                            hexStr = Asn1Unit.ConvertNodeValue(tmpNodeChild.ANode.Path, tmpNodeChild.ANode.GetDataStr(true));
                            listBox2.Items.Add(tmpNodeChild.ANode.TagName + "|" + hexStr);
                            for (k = 0; k < tmpNodeChild.ANode.ChildNodeCount; k++)
                            {
                                Asn1TreeNode tmpNodeChild1 = new Asn1TreeNode(tmpNodeChild.ANode.GetChildNode(k), mask);
                                listBox1.Items.Add(tmpNodeChild1.ANode.Path + "|" + tmpNodeChild1.ANode.GetDataStr(true));
                                hexStr = Asn1Unit.ConvertNodeValue(tmpNodeChild1.ANode.Path, tmpNodeChild1.ANode.GetDataStr(true));
                                listBox2.Items.Add(tmpNodeChild1.ANode.TagName + "|" + hexStr);
                                for (l = 0; l < tmpNodeChild1.ANode.ChildNodeCount; l++)
                                {
                                    Asn1TreeNode tmpNodeChild2 = new Asn1TreeNode(tmpNodeChild1.ANode.GetChildNode(l), mask);
                                    listBox1.Items.Add(tmpNodeChild2.ANode.Path + "|" + tmpNodeChild2.ANode.GetDataStr(true));
                                    hexStr = Asn1Unit.ConvertNodeValue(tmpNodeChild2.ANode.Path, tmpNodeChild2.ANode.GetDataStr(true));
                                    listBox2.Items.Add(tmpNodeChild2.ANode.TagName + "|" + hexStr);
                                }
                            }

                         }

                }

            }
        }

                private void toolStripButton3_Click(object sender, EventArgs e)
                {
                    Application.Exit();
                }

                private void frmCDR_Load(object sender, EventArgs e)
                {
                    this.WindowState = FormWindowState.Maximized;
                }
    }
}
