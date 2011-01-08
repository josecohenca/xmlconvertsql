using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Data;
using System.Windows.Forms;
using Microsoft.Office.Interop.Excel;

namespace Icdr.Conn
{
    class EXL  
    {

        private static System.Windows.Forms.DataGridView gridView;
        private static System.Windows.Forms.ToolStripProgressBar toolStripProgressBar1;
        private static Timer time;
        private static DataSet objSet = new DataSet();
        private static SaveFileDialog saveFileDialog = new SaveFileDialog();
        private static SaveFileDialog saveFileDialog2 = new SaveFileDialog();


        public static System.Windows.Forms.DataGridView _gridView
        {
            get { return gridView; }
            set { gridView = value; }
        }
        public static System.Windows.Forms.ToolStripProgressBar _toolStripProgressBar1
        {
            get { return toolStripProgressBar1; }
            set { toolStripProgressBar1 = value; }
        }
        public static Timer _time
        {
            get { return time; }
            set { time = value; }
        }
        public static DataSet _objSet
        {
            get { return objSet; }
            set { objSet.Clear(); objSet = value; }
        }

        //导出当前页DataGridView中的数据到EXcel中
        public static void ExportTOExcel()
        {
            if (gridView.Rows.Count == 0)
            {
                MessageBox.Show("没有数据可供导出！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }
            else
            {
                saveFileDialog.Filter = "Execl files (*.xls)|*.xls";
                saveFileDialog.FilterIndex = 0;
                saveFileDialog.RestoreDirectory = true;
                saveFileDialog.CreatePrompt = true;
                saveFileDialog.Title = "导出文件保存路径";
                saveFileDialog.ShowDialog();
                string strName = saveFileDialog.FileName;
                if (strName.Length != 0)
                {
                    toolStripProgressBar1.Visible = true;
                    System.Reflection.Missing miss = System.Reflection.Missing.Value;
                    Microsoft.Office.Interop.Excel.ApplicationClass excel = new Microsoft.Office.Interop.Excel.ApplicationClass();
                    excel.Application.Workbooks.Add(true); ;
                    excel.Visible = false;//若是true，则在导出的时候会显示EXcel界面。
                    if (excel == null)
                    {
                        MessageBox.Show("EXCEL无法启动！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return;
                    }
                    Microsoft.Office.Interop.Excel.Workbooks books = (Microsoft.Office.Interop.Excel.Workbooks)excel.Workbooks;
                    Microsoft.Office.Interop.Excel.Workbook book = (Microsoft.Office.Interop.Excel.Workbook)(books.Add(miss));
                    Microsoft.Office.Interop.Excel.Worksheet sheet = (Microsoft.Office.Interop.Excel.Worksheet)book.ActiveSheet;
                    sheet.Name = "test";

                    //生成字段名称
                    for (int i = 0; i < gridView.ColumnCount; i++)
                    {
                        excel.Cells[1, i + 1] = gridView.Columns[i].HeaderText.ToString();
                    }
                    //填充数据
                    for (int i = 0; i < gridView.RowCount - 1; i++)
                    {
                        for (int j = 0; j < gridView.ColumnCount; j++)
                        {
                            if (gridView[j, i].Value == typeof(string))
                            {
                                excel.Cells[i + 2, j + 1] = "" + gridView[i, j].Value.ToString();
                            }
                            else
                            {
                                excel.Cells[i + 2, j + 1] = gridView[j, i].Value.ToString();

                            }
                        }
                        toolStripProgressBar1.Value += 100 / gridView.RowCount;
                    }
                    sheet.SaveAs(strName, miss, miss, miss, miss, miss, Microsoft.Office.Interop.Excel.XlSaveAsAccessMode.xlNoChange, miss, miss, miss);
                    book.Close(false, miss, miss);
                    books.Close();
                    excel.Quit();
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(sheet);
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(book);
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(books);
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(excel);
                    GC.Collect();
                    MessageBox.Show("数据已经成功导出到：" + saveFileDialog.FileName.ToString(), "导出完成", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    toolStripProgressBar1.Value = 0;
                    toolStripProgressBar1.Visible = false;
                }
            }
        }

        //-------------------------------------------------------------------------------------------------------------------------------------
        //导出整个DataGridView中的数据到Excel中
        public static void ExportTOExcel2()
        {
            if (gridView.Rows.Count == 0)
            {
                MessageBox.Show("没有数据可供导出！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }
            else
            {
                saveFileDialog2.Filter = "Execl files (*.xls)|*.xls";
                saveFileDialog2.FilterIndex = 0;
                saveFileDialog2.RestoreDirectory = true;
                saveFileDialog2.CreatePrompt = true;
                saveFileDialog2.Title = "导出文件保存路径";
                saveFileDialog2.FileName = null;
                saveFileDialog2.ShowDialog();
                string FileName =  saveFileDialog2.FileName;

                if (FileName.Length != 0)
                {
                    toolStripProgressBar1.Visible = true;
                    System.Data.DataTable dt = objSet.Tables[0];

                    FileStream objFileStream;
                    StreamWriter objStreamWriter;
                    string strLine = "";
                    objFileStream = new FileStream(FileName, FileMode.OpenOrCreate, FileAccess.Write);
                    objStreamWriter = new StreamWriter(objFileStream, System.Text.Encoding.Unicode);
                    toolStripProgressBar1.Value = 0;

                    for (int i = 0; i < dt.Columns.Count; i++)
                    {
                        strLine = strLine + dt.Columns[i].ColumnName.ToString() + Convert.ToChar(9);

                    }
                    objStreamWriter.WriteLine(strLine);
                    strLine = "";

                    for (int i = 0; i < dt.Rows.Count; i++)
                    {
                        strLine = strLine + (i + 1) + Convert.ToChar(9);
                        for (int j = 1; j < dt.Columns.Count; j++)
                        {
                            strLine = strLine + dt.Rows[i][j].ToString() + Convert.ToChar(9);

                        }
                        objStreamWriter.WriteLine(strLine);
                        toolStripProgressBar1.Value += 100 / dt.Rows.Count;
                        strLine = "";
                    }
                    objStreamWriter.Close();
                    objFileStream.Close();
                    MessageBox.Show("数据已经成功导出到：" + saveFileDialog2.FileName.ToString(), "导出完成", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    toolStripProgressBar1.Value = 0;

                }
            }
        }

        //————————————————————————————————————————————————————————————————————
        //导出到XML(整个数据源)

        public static void ExportTOXML()
        {
            if (gridView.Rows.Count == 0)
            {
                MessageBox.Show("没有数据可供导出！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }
            else
            {
                saveFileDialog2.Filter = "XML files (*.xml)|*.xml";
                saveFileDialog2.FilterIndex = 0;
                saveFileDialog2.RestoreDirectory = true;
                saveFileDialog2.CreatePrompt = true;
                saveFileDialog2.Title = "导出文件保存路径";
                saveFileDialog2.FileName = null;
                saveFileDialog2.ShowDialog();
                string FileName = saveFileDialog2.FileName;

                if (FileName.Length != 0)
                {
                    toolStripProgressBar1.Visible = true;
                    objSet.WriteXml(saveFileDialog2.FileName.ToString());
                    for (int i = 0; i < objSet.Tables[0].Rows.Count; i++)
                    {
                        toolStripProgressBar1.Value += 100 / objSet.Tables[0].Rows.Count;
                    }
                    MessageBox.Show("数据已经成功导出到：" + saveFileDialog2.FileName.ToString(), "导出完成", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    toolStripProgressBar1.Value = 0;
                    toolStripProgressBar1.Visible = false;
                }
            }
        }
        public static void DataGridView2Excel(System.Windows.Forms.DataGridView datagridview, string SheetName)
        {

            int iRows = 0;
            int iCols = 0;
            int iTrueCols = 0;
            Microsoft.Office.Interop.Excel.Application app = new Microsoft.Office.Interop.Excel.Application();
            Microsoft.Office.Interop.Excel.Workbook wb = app.Workbooks.Add(System.Reflection.Missing.Value);
            Microsoft.Office.Interop.Excel.Worksheet ws = null;
            if (wb.Worksheets.Count > 0)
            {
                ws = (Microsoft.Office.Interop.Excel.Worksheet)wb.Worksheets.get_Item(1);
            }
            else
            {
                wb.Worksheets.Add(System.Reflection.Missing.Value, System.Reflection.Missing.Value, System.Reflection.Missing.Value, System.Reflection.Missing.Value);
                ws = (Microsoft.Office.Interop.Excel.Worksheet)wb.Worksheets.get_Item(1);
            }
            if (ws != null)
            {
                if (SheetName.Trim() != "")
                {
                    ws.Name = SheetName;
                }
                iRows = datagridview.Rows.Count;      //加上列头行
                iTrueCols = datagridview.Columns.Count;   //包含隐藏的列，一共有多少列
                //求列数，省略Visible = false的列
                for (int i = 0; i < datagridview.Columns.Count; i++)
                {
                    if (datagridview.Columns[i].Visible) iCols++;
                }
                string[,] dimArray = new string[iRows + 1, iCols];
                for (int j = 0, k = 0; j < iTrueCols; j++)
                {
                    //省略Visible = false的列
                    if (datagridview.Columns[j].Visible)
                    {
                        dimArray[0, k] = datagridview.Columns[j].HeaderText;
                        k++;
                    }
                }
                for (int i = 0; i < iRows; i++)
                {
                    for (int j = 0, k = 0; j < iTrueCols; j++)
                    {
                        //省略Visible = false的列
                        if (datagridview.Columns[j].Visible)
                        {
                            dimArray[i + 1, k] = datagridview.Rows[i].Cells[j].Value.ToString();
                            k++;
                        }
                    }
                }
                ////ws.get_Range(ws.Cells[1, 1], ws.Cells[iRows + 1, iCols]).Value2 = dimArray;
                ////ws.get_Range(ws.Cells[1, 1], ws.Cells[1, iCols]).Font.Bold = true;
                ////ws.get_Range(ws.Cells[1, 1], ws.Cells[iRows + 1, iCols]).Font.Size = 10.0;
                ////ws.get_Range(ws.Cells[1, 1], ws.Cells[iRows + 1, iCols]).RowHeight = 14.25;
                //ws.Columns[.ColumnWidth = datagridview.Columns[0].Width;
                for (int j = 0, k = 0; j < iTrueCols; j++)
                {
                    //省略Visible = false的列
                    if (datagridview.Columns[j].Visible)
                    {
                        ws.get_Range(ws.Cells[1, k + 1], ws.Cells[1, k + 1]).ColumnWidth = (datagridview.Columns[j].Width / 8.4) > 255 ? 255 : (datagridview.Columns[j].Width / 8.4);
                        //ws.Columns.c = datagridview.Columns[j].Width;
                        k++;
                   }
                }
            }
            app.Visible = true;

        }
        //导出到EXCEL速度比较快的方法
        public static bool ExportForDataGridview(DataGridView gridView, string fileName, bool isShowExcle)
        {
            //建立Ｅｘｃｅｌ对象 
            Microsoft.Office.Interop.Excel.Application app = new Microsoft.Office.Interop.Excel.Application();
            try
            {
                if (app == null)
                {
                    return false;
                }

                app.Visible = isShowExcle;
                Workbooks workbooks = app.Workbooks;
                _Workbook workbook = workbooks.Add(XlWBATemplate.xlWBATWorksheet);
                Sheets sheets = workbook.Worksheets;
                _Worksheet worksheet = (_Worksheet)sheets.get_Item(1);
                if (worksheet == null)
                {
                    return false;
                }
                string sLen = "";
                //取得最后一列列名 
                char H = (char)(64 + gridView.ColumnCount / 26);
                char L = (char)(64 + gridView.ColumnCount % 26);
                if (gridView.ColumnCount < 26)
                {
                    sLen = L.ToString();
                }
                else
                {
                    sLen = H.ToString() + L.ToString();
                }
                //名称
                worksheet.Name = fileName;
                //标题 
                string sTmp = sLen + "1";
                Range ranCaption = worksheet.get_Range(sTmp, "A1");
                string[] asCaption = new string[gridView.ColumnCount];
                for (int i = 0; i < gridView.ColumnCount; i++)
                {
                    asCaption[i] = gridView.Columns[i].HeaderText;
                }
                ranCaption.Value2 = asCaption;
                //数据 
                object[] obj = new object[gridView.Columns.Count];
                for (int r = 0; r < gridView.RowCount - 1; r++)
                {
                    for (int l = 0; l < gridView.Columns.Count; l++)
                    {
                        if (gridView[l, r].ValueType == typeof(DateTime))
                        {
                            obj[l] = gridView[l, r].Value.ToString();
                        }
                        else
                        {
                            obj[l] = gridView[l, r].Value;
                        }
                    }
                    string cell1 = sLen + ((int)(r + 2)).ToString();
                    string cell2 = "A" + ((int)(r + 2)).ToString();
                    Range ran = worksheet.get_Range(cell1, cell2);
                    ran.Value2 = obj;

                }
                //保存 
                //workbook.SaveCopyAs(fileName);
                //workbook.Saved = true;
            }
            finally
            {
                //关闭 
                //app.UserControl = false;
                //app.Quit();
            }
            return true;
        }
        /// <summary> 
        /// 另存新档按钮 
        /// </summary> 
        private void SaveAs(DataGridView dgvAgeWeekSex) //另存新档按钮   导出成Excel 
        {
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog.Filter = "Execl files (*.xls)|*.xls";
            saveFileDialog.FilterIndex = 0;
            saveFileDialog.RestoreDirectory = true;
            saveFileDialog.CreatePrompt = true;
            saveFileDialog.Title = "Export Excel File To";
            saveFileDialog.ShowDialog();
            Stream myStream;
            myStream = saveFileDialog.OpenFile();
            //StreamWriter sw = new StreamWriter(myStream, System.Text.Encoding.GetEncoding("gb2312")); 
            StreamWriter sw = new StreamWriter(myStream, System.Text.Encoding.GetEncoding(-0));
            string str = "";
            try
            {
                //写标题 
                for (int i = 0; i < dgvAgeWeekSex.ColumnCount; i++)
                {
                    if (i > 0)
                    {
                        str += "\t";
                    }
                    str += dgvAgeWeekSex.Columns[i].HeaderText;
                }
                sw.WriteLine(str);
                //写内容 
                for (int j = 0; j < dgvAgeWeekSex.Rows.Count; j++)
                {
                    string tempStr = "";
                    for (int k = 0; k < dgvAgeWeekSex.Columns.Count; k++)
                    {
                        if (k > 0)
                        {
                            tempStr += "\t";
                        }
                        tempStr += dgvAgeWeekSex.Rows[j].Cells[k].Value.ToString();
                    }

                    sw.WriteLine(tempStr);
                }
                sw.Close();
                myStream.Close();
            }
            catch (Exception e)
            {
                MessageBox.Show(e.ToString());
            }
            finally
            {
                sw.Close();
                myStream.Close();
            }
        } 

    }
}

