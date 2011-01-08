using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//namespace  Icdr.Conn
//{
//    static class ModXml
//    {
//        public static void populateTreeControl(System.Xml.XmlNode document, System.Windows.Forms.TreeNodeCollection nodes)
//        {
//        //    System.Xml.XmlNode node = default(System.Xml.XmlNode);
//        //    foreach (var node in document.ChildNodes)
//        //    {
//        //        string text = null;
//        //        if (node.Value != null)
//        //        {
//        //            text = node.Value;
//        //        }
//        //        else
//        //        {
//        //            if ((node.Attributes != null) & node.Attributes.Count > 0)
//        //            {
//        //                text = node.Attributes(1).Value;
//        //            }
//        //            else
//        //            {
//        //                text = node.Name;
//        //            }
//        //        }
//        //        TreeNode new_child = new TreeNode(text);
//        //        nodes.Add(new_child);
//        //        populateTreeControl(node, new_child.Nodes);
//        //    }
//        }
//        public static byte HexToByte(string HexNum)
//        {
//            return (byte)Conversion.Val("&H" + HexNum);
//        }
//        public static string HexToBin(string H)
//        {
//            string functionReturnValue = null;
//            switch (Strings.UCase(H))
//            {
//                case "0":
//                    functionReturnValue = "0000";
//                    break;
//                case "1":
//                    functionReturnValue = "0001";
//                    break;
//                case "2":
//                    functionReturnValue = "0010";
//                    break;
//                case "3":
//                    functionReturnValue = "0011";
//                    break;
//                case "4":
//                    functionReturnValue = "0100";
//                    break;
//                case "5":
//                    functionReturnValue = "0101";
//                    break;
//                case "6":
//                    functionReturnValue = "0110";
//                    break;
//                case "7":
//                    functionReturnValue = "0111";
//                    break;
//                case "8":
//                    functionReturnValue = "1000";
//                    break;
//                case "9":
//                    functionReturnValue = "1001";
//                    break;
//                case "A":
//                    functionReturnValue = "1010";
//                    break;
//                case "B":
//                    functionReturnValue = "1011";
//                    break;
//                case "C":
//                    functionReturnValue = "1100";
//                    break;
//                case "D":
//                    functionReturnValue = "1101";
//                    break;
//                case "E":
//                    functionReturnValue = "1110";
//                    break;
//                case "F":
//                    functionReturnValue = "1111";
//                    break;
//                default:
//                    functionReturnValue = "";
//                    break;
//            }
//            return functionReturnValue;
//        }
//        public static string Hex2Bin(string sHex)
//        {
//            const var s1 = "0000101001101111000", s2 = "0125A4936DB7FEC8";
//            int i = 0;
//            string sBin = null;
//            sHex = Strings.UCase(sHex);
//            sBin = Constants.vbNullString;
//            for (i = 1; i <= Strings.Len(sHex); i++)
//            {
//                sBin = sBin + Strings.Mid(s1, Strings.InStr(1, s2, Strings.Mid(sHex, i, 1)), 4);
//            }
//            return sBin;
//        }
//    } 
//}
