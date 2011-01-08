using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Configuration;
using Microsoft.Practices.EnterpriseLibrary.Common.Configuration;
using Microsoft.Practices.EnterpriseLibrary.Data;
using Microsoft.Practices.EnterpriseLibrary.Common.Configuration.Manageability;
using Microsoft.Practices.EnterpriseLibrary.Data.Configuration;
using Microsoft.Practices.EnterpriseLibrary.Logging;


namespace cn.wei.hp.icdr
{
    class Program
    {
        static void Main(string[] args)
        {
            //从Data.config配置文件获取设置
            DatabaseProviderFactory dbFactory =
                new DatabaseProviderFactory(GetFileConfigurationSource("Data Configuration Source"));

            Database db = dbFactory.Create("Connection String");
            Console.WriteLine(db.ConnectionString);

            //从Log.config的配置文件获取设置
            LogWriter lw = new LogWriterFactory(GetFileConfigurationSource("Log Configuration Source")).Create();
            lw.Write("123");
        }

        private static FileConfigurationSource GetFileConfigurationSource(string SourceName)
        {
            //获取App.config
            Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);

            //获取资源节点集合
            ConfigurationSourceSection section =
                (ConfigurationSourceSection)config.GetSection(ConfigurationSourceSection.SectionName);

            //获取重定向配置文件资源配置节点
            FileConfigurationSourceElement elem =
                (FileConfigurationSourceElement)section.Sources.Get(SourceName);

            //获取重定向配置文件资源
            FileConfigurationSource fileSource = new FileConfigurationSource(elem.FilePath);
            return fileSource;
        }
    }
}
