using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Icdr.Conn
{
    static class HexConversion
    {
        public static string hextotbcd(string strs)
        {
            string text = "";
            for (int i = 0; i < strs.Length; i += 2)
            {
                string hexDigits = strs.Substring(i, 2);//每次取2个16机制数做一次转换
                string n = hexDigits.Substring(1, 1) + hexDigits.Substring(0, 1);//每2个16机制置换
                text += n;//每2个置换后累加
            }
            return text.Substring(0, text.LastIndexOf("F"));
        }
        public static string hextoimsi(string strs)
        {
            string text = "";
            for (int i = 0; i < strs.Length; i += 2)
            {
                string hexDigits = strs.Substring(i, 2);//每次取2个16机制数做一次转换
                string n = hexDigits.Substring(1, 1) + hexDigits.Substring(0, 1);//每2个16机制置换
                text += n;//每2个置换后累加
            }
            try
            {
                return text.Substring(0, text.LastIndexOf("F"));
            }
            catch { return strs; }
        }
        public static string hextomsdn(string strs)
        {
            string text = "";
            for (int i = 0; i < strs.Length; i += 2)
            {
                string hexDigits = strs.Substring(i, 2);//每次取2个16机制数做一次转换
                string n = hexDigits.Substring(1, 1) + hexDigits.Substring(0, 1);//每2个16机制置换
                text += n;//每2个置换后累加
            }
            try
            {
                return text.Substring(4, text.LastIndexOf("F") - 4);
            }
            catch { return strs; }
        }
        public static string hextoip(string strs)
        {
            string text = "";
            for (int i = 0; i < strs.Length; i += 2)
            {
                string hexDigits = strs.Substring(i, 2);//每次取2个16机制数做一次转换
                text += int.Parse(hexDigits, System.Globalization.NumberStyles.HexNumber);//转换到10机制
                if (i != strs.Length - 2)
                    text += ".";
            }
            return text;
        }
        public static string hextostr(string strs)
        {
            string text = "";
            for (int i = 0; i < strs.Length; i += 2)
            {
                string hexDigits = strs.Substring(i, 2);//每次取2个16机制数做一次转换
                text += (char)System.Convert.ToInt32(hexDigits, 16);//转换到char，即字符
            }
            return text;
        }
        public static string hextotime(string strs)
        {
            try
            {
                return strs.Substring(0, strs.LastIndexOf("2B0800"));
            }
            catch { return strs; }
        }
        public static string hextoenum(string strs)
        {
            //object iptmp = null;
            //object n = null;
            //short i = 0;
            //iptmp = "";
            //for (i = 1; i <= Strings.Len(strs); i++)
            //{
            //    n = Strings.Mid(strs, i, 2);
            //    iptmp = iptmp + n;
            //}
            //return iptmp;
            return strs;
        }
        public static string hextohex(string strs)
        {
            return strs;
        }
        public static string hextobcd(string strs)
        {
            //return Strings.Left(strs, Strings.Len(strs) - 1);
            return strs;
        }
        public static string hextonip(string strs)
        {
            //string[] nip = null;
            //nip = Strings.Split(strs, "E");
            //return hextostr(nip(0) + "E") + hextostr(nip(1) + "E") + hextostr(nip(2) + "E") + hextostr(nip(3));
            return strs;
        }
        public static string hextooctet(string strs)
        {
            //object iptmp = null;
            //object n = null;
            //short i = 0;
            //iptmp = "";
            //for (i = 1; i <= Strings.Len(strs); i++)
            //{
            //    n = Strings.Mid(strs, i, 2);
            //    n = Strings.Asc(n);
            //    iptmp = iptmp + n;
            //}
            //return iptmp;
            return strs;
        }
        // Convert the hex string back to the number
        public static string hextodec(string strs)
        {
            try
            {
                int dec = int.Parse(strs, System.Globalization.NumberStyles.HexNumber);//16机制转10机制
                return dec.ToString();
            }
            catch
            {
                return strs;
            }
        }
        //OCTET STRING (SIZE(1)) 
        public static string hextootectstr1(string strs)
        {
            
            int i = Convert.ToInt32(strs, 16);  //16进制字符串转整数，定义i是16进制
            string bytesbit = Convert.ToString(i, 2);//16进制整数转2进制字符串，转换i到2进制
            string bytes = null;//取1个8位2进制 
            try
            {
                bytes = bytes + "<GEA1>";
                bytes = bytes + bytesbit.Substring(0, 1);
                bytes = bytes + "<SMdedicatedchannels>";
                bytes = bytes + bytesbit.Substring(1, 1);
                bytes = bytes + "<SMGPRSchannels>";
                bytes = bytes + bytesbit.Substring(2, 1);
                bytes = bytes + "<UCS2>";
                bytes = bytes + bytesbit.Substring(3, 1);
                bytes = bytes + "<SS>";
                bytes = bytes + bytesbit.Substring(4, 2);
                bytes = bytes + "<SoLSA>";
                bytes = bytes + bytesbit.Substring(6, 1);
                bytes = bytes + "<Revision>";
                bytes = bytes + bytesbit.Substring(7, 1);
                //bytes = bytes & "<PFC feature mode: bit>" 
                //bytes = bytes & "<Extended GEA bits>" 
                //bytes = bytes & "< LCS VA capability: bit >" 
                //bytes = bytes & "<Spare bits>" 
            }
            catch {}
            return bytes;
         
        }
        //OCTET STRING (SIZE(2)) 
        public static string hextootectstr2(string strs)
        {

            return strs;
        }
        //Quality of service 
        public static string hextoqos(string strs)
        {

            object[] qosstr = new object[11];
            string bytes = "";
            for (int i = 0; i < strs.Length; i ++)
            {
                string hexDigits = strs.Substring(i, 1);//每次取1个16机制数做一次转换
                int j = Convert.ToInt32(hexDigits, 16);  //16进制字符串转整数，定义i是16进制
                bytes += Convert.ToString(j, 2);//16进制整数转2进制字符串，转换i到2进制
            }

            try
            {
                //取第0个8位Type = 135 (Decimal) 
                qosstr[0] = "Type" + Convert.ToInt32(OtherConvertToTen(bytes.Substring(0, 8), 2).ToString());
                //取第1个8位Length 
                qosstr[1] = "Length" + Convert.ToInt32(OtherConvertToTen(bytes.Substring(8, 8), 2).ToString());
                //取第2个8位Allocation/Retention Priority 
                qosstr[2] = "Priority" + Convert.ToInt32(OtherConvertToTen(bytes.Substring(16, 8), 2).ToString());
                //取第3个8位 
                //--------------------------------------------- 
                //2位spare 
                //qosstr(3] = BIN_to_DEC(Mid(bytes, 25, 2)) 
                qosstr[3] = "spare";
                //3位Delay class 
                qosstr[4] = "Delay" + Convert.ToInt32(OtherConvertToTen(bytes.Substring(26, 3), 2).ToString());
                //3位Reliability class 
                qosstr[5] = "Reliability" + Convert.ToInt32(OtherConvertToTen(bytes.Substring(29, 3), 2).ToString());
                //--------------------------------------------- 
                //取第4个8位 
                //--------------------------------------------- 
                //4位Peak throughput 
                qosstr[6] = "Peak" + Convert.ToInt32(OtherConvertToTen(bytes.Substring(32, 4), 2).ToString());
                //1位spare 
                //qosstr(7] = BIN_to_DEC(Mid(bytes, 37, 1)) 
                qosstr[7] = "spare";
                //3位Precedence class 
                qosstr[8] = "Precedence" +Convert.ToInt32(OtherConvertToTen( bytes.Substring(37, 3), 2).ToString());
                //--------------------------------------------- 
                //取第5个8位 
                //--------------------------------------------- 
                //3位spare 
                //qosstr(9] = BIN_to_DEC(Mid(bytes, 41, 3)) 
                qosstr[9] = "spare";
                //5位Mean throughput 
                qosstr[10] = "Mean" + Convert.ToInt32(OtherConvertToTen(bytes.Substring(43, 5), 2).ToString());
                //--------------------------------------------- 
            }
            catch { }
                string qosstring = null;
                for (int i = 0; i <= 10; i++)
                {
                    if (i==0) qosstring += qosstr[i];
                    else qosstring += "-" + qosstr[i];
                }
                return qosstring;
        }

        //调用   
        //string   str="A10B";   
        //int   m=Convert.ToInt32(OtherConvertToTen(str,10))//16   to   10   
        //string   str1=TenConvertToOther(m,2);//10   to   2  
        #region   各进制之间的转换
        public static string TenConvertToOther(int num, int type)
        {
            //1)10进制转换为2,8,10,16进制   
            //2)numm是十进制数   
            //3)type是转换类型,可以是2,8,10,16   
            string str = Convert.ToString(num, type);
            return str;
        }
        public static string OtherConvertToTen(string num, int type)
        {
            //1)2,8,10,16进制转换为10进制   
            //2)numm是2,8,10,16进制   
            //3)type是转换类型,可以是2,8,10,16   
            string str = Convert.ToInt32(num, type).ToString();
            return str;
        }
        #endregion

    }
}
