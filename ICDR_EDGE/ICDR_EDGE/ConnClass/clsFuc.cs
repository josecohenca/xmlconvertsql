using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
namespace Icdr.Conn
{
    static class ModFuc
    {

        public static object ExecuteCmd(string cmd)
        {
            ProcessStartInfo startInfo = new ProcessStartInfo("cmd.exe");
            //调用程序名 
            {
                startInfo.Arguments = "/C " + cmd;
                //调用命令 CMD 
                startInfo.RedirectStandardError = true;
                startInfo.RedirectStandardOutput = true;
                startInfo.UseShellExecute = false;
                startInfo.CreateNoWindow = true;
            }

            Process p = Process.Start(startInfo);
            string strOutput = p.StandardOutput.ReadToEnd();
            string strError = p.StandardError.ReadToEnd();

            p.WaitForExit();

            if ((strOutput.Length != 0))
            {
                return strOutput;
            }
            else if ((strError.Length != 0))
            {
                return strError;
            }

            return "";
        }

    }

}
