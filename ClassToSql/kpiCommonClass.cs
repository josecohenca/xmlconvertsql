using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

namespace softerCell_U3_v0._01
{
    class kpiCommonClass
    {
        private static string strsql;
        private static int k;
        private static int class_length;
        public static string createListTable(FieldInfo[] fields)
        {
            strsql = null;
            k = 0;
            class_length = fields.Length;

            foreach (var field in fields)
            {
                string sqlfieldname = field.Name;
                k++;
                if (k != class_length)
                {
                    if (sqlfieldname != "iu_release_request_message_radioNetwork"
                        && sqlfieldname != "message_radioNetwork" 
                        && sqlfieldname != "message_Cause")
                    {
                        strsql = strsql + "[" + sqlfieldname + "]" + " varchar(50),";
                    }
                    else
                    {
                        strsql = strsql + "[" + sqlfieldname + "]" + " varchar(200),";
                    }
                }
                else
                {
                    if (sqlfieldname != "iu_release_request_message_radioNetwork"
                        && sqlfieldname != "message_radioNetwork" 
                        && sqlfieldname != "message_Cause")
                    {
                        strsql = strsql + "[" + sqlfieldname + "]" + " varchar(50)";
                    }
                    else
                    {
                        strsql = strsql + "[" + sqlfieldname + "]" + " varchar(200)";
                    }
                }
            }
            //GC.Collect();
            return strsql;
        }

        public static string insertListTable(FieldInfo[] fields, object alist)
        {
            k = 0;
            strsql = null;
            class_length = fields.Length;

            foreach (var field in fields)
            {
                var sqlfieldvalue = field.GetValue(alist);
                k++;
                if (k != class_length)
                {
                    strsql = strsql + "'" + sqlfieldvalue + "',";
                }
                else
                {
                    strsql = strsql + "'" + sqlfieldvalue + "'";
                }
            }
            //GC.Collect();
            return strsql;
        }
    }
}
