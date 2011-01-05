using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

namespace softerCell_U3_v0._01
{
    class listConvertSql
    {
        public static void saveStreamMessageSql(string tableName)
        {
            string strsql = null;
            streamMessagePool._stream_message user_field = new streamMessagePool._stream_message();
            FieldInfo[] fields = user_field.GetType().GetFields();

            strsql = kpiCommonClass.createListTable(fields);
           
            strsql = "create table " + tableName + " (" + strsql + " ) ";
            streamMessagePool .Acc.RunNoQuery(strsql);

            foreach (streamMessagePool._stream_message sm in streamMessagePool.ml)
            {

                strsql = kpiCommonClass.insertListTable(fields, sm);

                strsql = "insert into " + tableName + " values(" + strsql + ")";
                streamMessagePool .Acc.RunNoQuery(strsql);
            }
        }

        public static void mobile_kpi_stat_flow(List<streamMessagePool._kpi_stat> kl, string kpi)
        {
            string strsql = null;
            streamMessagePool._kpi_stat user_field = new streamMessagePool._kpi_stat();
            FieldInfo[] fields = user_field.GetType().GetFields();

            strsql = kpiCommonClass.createListTable(fields);

            strsql = "create table " + streamMessagePool.dataTableName + "_kpi_stat" + kpi + " (" + strsql + " ) ";
            streamMessagePool.Acc.RunNoQuery(strsql);

            foreach (streamMessagePool._kpi_stat sm in kl)
            {

                strsql = kpiCommonClass.insertListTable(fields, sm);

                strsql = "insert into " + streamMessagePool.dataTableName + "_kpi_stat" + kpi + " values(" + strsql + ")";
                streamMessagePool.Acc.RunNoQuery(strsql);
            }
        } 
    }
}
