using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Configuration;

namespace cn.whp.icdr
{
    class ModifyAppConfigRuntime
    {


        ///<summary> 
        ///依据连接串名字connectionName返回数据连接字符串  
        ///</summary> 
        ///<param name="connectionName"></param> 
        ///<returns></returns> 
        private static string GetConnectionStringsConfig(string connectionName)
        {
            string connectionString =
                    ConfigurationManager.ConnectionStrings[connectionName].ConnectionString.ToString();
            Console.WriteLine(connectionString);
            return connectionString;
        }

        ///<summary> 
        ///更新连接字符串  
        ///</summary> 
        ///<param name="newName">连接字符串名称</param> 
        ///<param name="newConString">连接字符串内容</param> 
        ///<param name="newProviderName">数据提供程序名称</param> 
        private static void UpdateConnectionStringsConfig(string newName,
            string newConString,
            string newProviderName)
        {
            bool isModified = false;    //记录该连接串是否已经存在  
            //如果要更改的连接串已经存在  
            if (ConfigurationManager.ConnectionStrings[newName] != null)
            {
                isModified = true;
            }
            //新建一个连接字符串实例  
            ConnectionStringSettings mySettings =
                new ConnectionStringSettings(newName, newConString, newProviderName);
            // 打开可执行的配置文件*.exe.config  
            Configuration config =
                ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
            // 如果连接串已存在，首先删除它  
            if (isModified)
            {
                config.ConnectionStrings.ConnectionStrings.Remove(newName);
            }
            // 将新的连接串添加到配置文件中.  
            config.ConnectionStrings.ConnectionStrings.Add(mySettings);
            // 保存对配置文件所作的更改  
            config.Save(ConfigurationSaveMode.Modified);
            // 强制重新载入配置文件的ConnectionStrings配置节  
            ConfigurationManager.RefreshSection("ConnectionStrings");
        }

        ///<summary> 
        ///返回＊.exe.config文件中appSettings配置节的value项  
        ///</summary> 
        ///<param name="strKey"></param> 
        ///<returns></returns> 
        private static string GetAppConfig(string strKey)
        {
            foreach (string key in ConfigurationManager.AppSettings)
            {
                if (key == strKey)
                {
                    return ConfigurationManager.AppSettings[strKey];
                }
            }
            return null;
        }

        ///<summary>  
        ///在＊.exe.config文件中appSettings配置节增加一对键、值对  
        ///</summary>  
        ///<param name="newKey"></param>  
        ///<param name="newValue"></param>  
        private static void UpdateAppConfig(string newKey, string newValue)
        {
            bool isModified = false;
            foreach (string key in ConfigurationManager.AppSettings)
            {
                if (key == newKey)
                {
                    isModified = true;
                }
            }

            // Open App.Config of executable  
            Configuration config =
                ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
            // You need to remove the old settings object before you can replace it  
            if (isModified)
            {
                config.AppSettings.Settings.Remove(newKey);
            }
            // Add an Application Setting.  
            config.AppSettings.Settings.Add(newKey, newValue);
            // Save the changes in App.config file.  
            config.Save(ConfigurationSaveMode.Modified);
            // Force a reload of a changed section.  
            ConfigurationManager.RefreshSection("appSettings");
        }


        //    首先添加对Microsoft.Data.ConnectionUI.Dialog.dll的引用，这个assembly在VS2005的安装目录下，
        //    D:\Program Files\Microsoft Visual Studio 8\Common7\IDE下边，我的VS安装在D盘，下面是代码：
        //引用 Microsoft.Data.ConnectionUI.Dialog.dll 文件
        //using System;
        //using System.Collections.Generic;
        //using System.Text;
        //using Microsoft.Data.ConnectionUI;
        //using System.Windows.Forms;

        /// <summary>
        /// 获取VS.NET 自带的数据库连接对话框的数据库连接信息
        /// </summary>
        /// <param name="conn">初始化连接字符串</param>
        /// <returns>数据库连接</returns>
        public string GetDatabaseConnectionString(String conn)
        {
            string connString = String.Empty;
            
            Microsoft.Data.ConnectionUI.DataConnectionDialog connDialog = new Microsoft.Data.ConnectionUI.DataConnectionDialog();

            // 添加数据源列表，可以向窗口中添加自己程序所需要的数据源类型 必须增加以下几项中任一一项
            connDialog.DataSources.Add(Microsoft.Data.ConnectionUI.DataSource.AccessDataSource); // Access 
            connDialog.DataSources.Add(Microsoft.Data.ConnectionUI.DataSource.OdbcDataSource); // ODBC
            connDialog.DataSources.Add(Microsoft.Data.ConnectionUI.DataSource.OracleDataSource); // Oracle 
            connDialog.DataSources.Add(Microsoft.Data.ConnectionUI.DataSource.SqlDataSource); // Sql Server
            connDialog.DataSources.Add(Microsoft.Data.ConnectionUI.DataSource.SqlFileDataSource); // Sql Server File

            // 初始化
            connDialog.SelectedDataSource = Microsoft.Data.ConnectionUI.DataSource.SqlDataSource;
            connDialog.SelectedDataProvider = Microsoft.Data.ConnectionUI.DataProvider.SqlDataProvider;
            connDialog.ConnectionString = conn;
            //只能够通过DataConnectionDialog类的静态方法Show出对话框
            //不同使用dialog.Show()或dialog.ShowDialog()来呈现对话框
            if (Microsoft.Data.ConnectionUI.DataConnectionDialog.Show(connDialog) == DialogResult.OK)
            {
                connString = connDialog.ConnectionString;
            }
            return connString;
        }

    }

}


