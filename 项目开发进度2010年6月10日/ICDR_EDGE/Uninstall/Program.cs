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
        
            //p.StartInfo.Arguments = @"/x {5BDCE305-D565-45E6-9237-C09BB567E8D4}";
            p.StartInfo.Arguments = @"/x {0832BD8A-3374-4822-A92F-45608B9F4E3F}";
            p.Start(); 
        }
    }
}
