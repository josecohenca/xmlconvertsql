using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using System.Data.OleDb;
using System.Data.Common;
using System.Data.Sql;
using System.Data.SqlClient;

namespace Icdr.Conn
{
     public enum DBType
    {
        Access,
        SQL,
        DB2,
        Oracle,
        MySQL
    }

    public interface IDBAccess
    {
        void Init(string strServer, string strDataBase, string strUser, string strPwd);
        void Open();
        void Close();
        bool TestConn();
        int RunNoQuery(string strCmd);
        DataTable RunQuery(string strCmd);
        DBType DBType { get;}
        int GetFiledMax(string strTable, string strField);
        DataTable Tables { get; }
        DataTable GetColumns();
        DataTable GetColumns(string strTable);
    }

    public static class DBAccessFactory
    {
        public static IDBAccess Create(DBType type)
        {
            IDBAccess IRet = null;
            switch (type)
            {
                case DBType.Access:
                    IRet = new Access(type);
                    break;

                case DBType.SQL:
                    IRet = new SQL(type);
                    break;

                default:
                    break;
            }
            return IRet;
        }

        private abstract class DBAccess : IDBAccess
        {
            protected DbConnection m_oConn = null;
            protected const string CON_strServer = "Server";
            protected const string CON_strDataBase = "Data Source";
            protected const string CON_strUser = "UID";
            protected const string CON_strPwd = "PWD";
            protected const string CON_strConnTimeOut = "Connect Timeout = 2";
            private DBType m_eDBType = DBType.Access;

            protected DBAccess(DBType type)
            {
                this.m_eDBType = type;
            }

            public DBType DBType
            {
                get { return this.m_eDBType; }
            }

            public void Init(string strServer, string strDataBase, string strUser, string strPwd)
            {
                this.InitConn(strServer, strDataBase, strUser, strPwd);
            }

            public void Open()
            {
                if (this.m_oConn != null)
                {
                    this.m_oConn.Open();
                }
            }

            public int RunNoQuery(string strCmd)
            {
                int iRet = 0;
                try
                {
                    DbCommand oCmd = this.GetCmd(strCmd);
                    if (oCmd != null)
                    {
                        iRet = oCmd.ExecuteNonQuery();
                    }
                }
                catch (Exception ex)
                {
                    throw (new Exception(ex.Message));
                }
                return iRet;
            }

            public int GetFiledMax(string strTable, string strField)
            {
                int iRet = -1;
                DataTable dt = this.RunQuery("Select Max(" + strField + ") From " + strTable);
                if (dt != null && dt.Rows.Count == 1)
                {
                    iRet = dt.Rows[0][0] is DBNull ? 0 : Convert.ToInt32(dt.Rows[0][0]);
                }
                return iRet;
            }

            public DataTable RunQuery(string strCmd)
            {
                DataTable dt = new DataTable();
                DbDataAdapter adp = this.DbAdp;
                adp.SelectCommand = this.GetCmd(strCmd);
                adp.Fill(dt);
                return dt;
            }

            public void Close()
            {
                if (this.m_oConn != null && this.m_oConn.State == System.Data.ConnectionState.Open)
                {
                    this.m_oConn.Close();
                }
            }

            public bool TestConn()
            {
                bool bRet = true;
                try
                {
                    if (this.m_oConn.State != System.Data.ConnectionState.Open)
                    {
                        this.m_oConn.Open();
                    }
                    bRet = this.m_oConn.State == System.Data.ConnectionState.Open;
                }
                catch
                {
                    bRet = false;
                }
                this.Close();
                return bRet;
            }

            public abstract DataTable Tables { get; }
            public abstract DataTable GetColumns();
            public abstract DataTable GetColumns(string strTable);

            protected abstract void InitConn(string strServer, string strDataBase, string strUser, string strPwd);
            protected abstract DbCommand GetCmd(string strCmd);
            protected abstract DbDataAdapter DbAdp { get;}
        }

        //Access, SQL
    #region Access, SQL

        private class Access : DBAccess
        {
            public Access(DBType type)
                : base(type)
            {
            }

            protected override void InitConn(string strServer, string strDataBase, string strUser, string strPwd)
            {
                string strConn = "Provider = ";
                switch (strDataBase.Substring(strDataBase.LastIndexOf('.') + 1).ToLower())
                {
                    case "mdb":     // 2000, 2003
                        strConn += "Microsoft.Jet.OleDb.4.0;";
                        break;

                    case "accdb":   // 2007
                        strConn += "Microsoft.ACE.OLEDB.12.0;";
                        break;

                    default:
                        throw (new Exception("Unknown Access Version."));
                    //break;
                }
                strConn += CON_strDataBase + " = " + strDataBase;
                //strConn += CON_strUser + " = " + strUser;
                //strConn += CON_strPwd + " = " + strPwd;
                base.m_oConn = new OleDbConnection(strConn);
            }

            protected override DbCommand GetCmd(string strCmd)
            {
                return new OleDbCommand(strCmd, (OleDbConnection)base.m_oConn);
            }

            protected override DbDataAdapter DbAdp
            {
                get { return new OleDbDataAdapter(); }
            }

            public override DataTable Tables
            {
                get
                {
                    return ((OleDbConnection)base.m_oConn).GetOleDbSchemaTable(OleDbSchemaGuid.Tables, new object[] { null, null, null, "Table" });
                }
            }

            public override DataTable GetColumns()
            {
                DataTable dt = new DataTable();
                foreach (DataRow row in this.Tables.Rows)
                {
                    dt.Merge(this.GetColumns(row["TABLE_NAME"].ToString()));
                }
                return dt;
            }

            public override DataTable GetColumns(string strTable)
            {
                return ((OleDbConnection)base.m_oConn).GetOleDbSchemaTable(OleDbSchemaGuid.Columns, new object[] { null, null, strTable, null });
            }
        }

        private class SQL : DBAccess
        {
            public SQL(DBType type)
                : base(type)
            {
            }

            protected override void InitConn(string strServer, string strDataBase, string strUser, string strPwd)
            {
                string strConn = CON_strServer + " = " + strServer + ";";
                strConn += CON_strDataBase + " = " + strDataBase + ";";
                strConn += CON_strUser + " = " + strUser + ";";
                strConn += CON_strPwd + " = " + strPwd + ";";
                strConn += CON_strConnTimeOut;
                base.m_oConn = new SqlConnection(strConn);
            }

            protected override DbCommand GetCmd(string strCmd)
            {
                return new SqlCommand(strCmd, (SqlConnection)base.m_oConn);
            }

            protected override DbDataAdapter DbAdp
            {
                get { return new SqlDataAdapter(); }
            }

            public override DataTable Tables
            {
                get { return ((SqlConnection)base.m_oConn).GetSchema("Tables", null); }
            }

            public override DataTable GetColumns()
            {
                return ((SqlConnection)base.m_oConn).GetSchema("Columns", null);
            }

            public override DataTable GetColumns(string strTable)
            {
                return ((SqlConnection)base.m_oConn).GetSchema("Columns", new string[] { null, null, strTable, null });
            }
        }

        #endregion
    }


}
