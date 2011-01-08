
//listBox1.Items.Add(xParser.RawData.Length);
//listBox1.Items.Add(xParser.RootNode.DataLength);
//listBox1.Items.Add(xParser.RootNode.Tag);
//listBox1.Items.Add(xParser.RootNode.TagName);

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using Icdr.Conn;


namespace Asn1Processor
{
    public class Asn1Unit
    {
        /// <summary>
        /// Convert  bin to string,ip,dec
        /// </summary>
        /// <param name="Path"></param>
        /// <returns></returns>
        public static string ConvertNodeValue(string Path, string GetDataStr)
        {
            try
            {
                string strg;
                if (GetDataStr.Length > 0 - 1)
                {
                    switch (Path)
                    {
                        case "/0":
                            //return Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                            return Icdr.Conn.HexConversion.hextodec(GetDataStr);
                        case "/1":
                            strg = Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                            //return strg.Substring(0, strg.LastIndexOf("F"));                       //IMSI
                            return Icdr.Conn.HexConversion.hextoimsi(strg);
                        case "/2/0":
                            strg = Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                            //return HexToStr(strg);
                            return Icdr.Conn.HexConversion.hextoip(strg);                           //IP
                        case "/3":
                            strg = Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                            //return HexToStr(strg);
                            return Icdr.Conn.HexConversion.hextoip(strg);                           //IP
                        //IPAddress address = IPAddress.Parse(GetDataStr);
                        //return address.ToString ();                                            
                        case "/4/0":
                            strg = Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                            return Icdr.Conn.HexConversion.hextoip(strg);                         //ip
                        case "/5":
                            strg = Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                            return Icdr.Conn.HexConversion.hextostr (strg);                          //APN
                        case "/6":
                            strg = Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                            return Icdr.Conn.HexConversion.hextodec(strg);                                                   //APN
                        case "/7/0/0":
                            strg = Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                            return Icdr.Conn.HexConversion.hextoip(strg);   //IP
                        case "/8/0/0":
                            strg = Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                            return HexToStr(strg);                                                 //IP
                        case "/9/0/0":
                            strg = Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                            return Icdr.Conn.HexConversion.hextostr(strg);                          //APN
                        case "/9/0/1":
                            strg = Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                            return Icdr.Conn.HexConversion.hextostr(strg);                          //APN
                        case "/9/0/2":
                            strg = Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                            return Icdr.Conn.HexConversion.hextostr(strg);                          //APN
                        case "/9/0/4":
                            strg = Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                            return Icdr.Conn.HexConversion.hextotime(strg);                          //APN
                        case "/10":
                            strg = Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                            return Icdr.Conn.HexConversion.hextotime(strg);                          //APN
                        case "/10/0/5":
                            strg = Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                            return strg.Substring(0, strg.LastIndexOf("2B"));                       //START TIME
                        case "/11":
                            strg = Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                            return strg.Substring(0, strg.LastIndexOf("2B"));                       //END TIME
                        case "/12":
                            return Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                        case "/13":
                            return Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                        case "/14":
                            strg = Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                            return Icdr.Conn.HexConversion.hextostr(strg);                          //APN
                        case "/15/0/0":
                            strg = Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                            return HexToStr(strg);                                                 //IP
                        case "/16":
                            strg = Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                            return Icdr.Conn.HexConversion.hextomsdn(strg);                        //APN
                        case "/17":
                            return Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                        case "/18/0/0":
                            strg = Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                            return HexToStr(strg);                                                 //IP
                        //case "/19":
                        //    return System.Net.IPAddress.Parse(GetDataStr).ToString();
                        case "/20":
                            strg = Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                            return strg.Substring(0, strg.LastIndexOf("F"));   //MSISDN
                        case "/21":
                            return Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                        //case "/22":
                        //    strg=Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                        //    return strg.Substring(0, strg.LastIndexOf("F"));   //MSISDN
                        case "/23":
                            return Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                        case "/24":
                            return Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                        case "/25":
                            return Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                        case "/26":
                            return Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                        case "/27":
                            return Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                        case "/28":
                            return Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                        case "/29":
                            return Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                        case "/30":
                            return Asn1Util.ToHexString(Asn1Util.HexStrToBytes(GetDataStr));
                        default:
                            return GetDataStr;
                    }
                }
                else
                {
                    return GetDataStr;
                }
            }
            catch
            {
                return GetDataStr;
            }
        }
        /// <summary>
        /// Convert  hex to string
        /// </summary>
        /// <param name="hexStr"></param>
        /// <returns></returns>
        public static string HexToStr(string hexStr)
        {
            string text = "";
            for (int i = 0; i < hexStr.Length; i += 2)
            {
                string hexDigits = hexStr.Substring(i, 2);
                text += (char)System.Convert.ToInt32(hexDigits, 16);
            }
            return text;
        }
    }
}
