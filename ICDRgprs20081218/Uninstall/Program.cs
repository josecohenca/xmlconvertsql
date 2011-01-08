using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Uninstall
{
    　　///<summary>
　　///Class1的摘要说明。
　　///</summary>
    class Program
    {　　　　
        ///<summary>
　　　　///应用程序的主入口点。
　　　　///</summary>
　　　　[STAThread]
        static void Main(string[] args)
        {
            System.Diagnostics.Process  p = new System.Diagnostics.Process(); 
            p.StartInfo.FileName = "msiexec.exe";
            p.StartInfo.Arguments = @"/x {5BDCE305-D565-45E6-9237-C09BB567E8D4}";
            p.Start(); 
        }
    }
}
