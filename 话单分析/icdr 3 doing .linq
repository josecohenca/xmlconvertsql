<Query Kind="Program">
  <Reference>&lt;RuntimeDirectory&gt;\System.XML.dll</Reference>
  <Namespace>System.Xml.Serialization</Namespace>
</Query>

static string cdrmessage = null;
void Main()
		{

			ShoppingList myList = new ShoppingList();

			string path = "";
			string file = @"F:\LenovoHP\ICDR_EDGE\Temp\test.xml";
			XmlDocument docu = new XmlDocument();
			docu.Load(file);
			XmlElement roo = docu.DocumentElement;
			Get(roo, path,myList  );


		     myList.Dump ();
//		  	myList.Dump ();
//         convList.Dump ();
		 
		 			// Serialization
		 XmlSerializer s = new XmlSerializer( typeof( ShoppingList ) );
			TextWriter w = new StreamWriter(@"c:\list.xml");
			s.Serialize(w,   myList);
			w.Close();

			// Deserialization
			ShoppingList newList;
			TextReader r = new StreamReader(@"c:\list.xml");
			  myList = (ShoppingList)s.Deserialize(r);
			r.Close();
		 
		}
		
	


		public static void Get(XmlNode node, string path, ShoppingList _cdr)
		{
			if (!node.HasChildNodes)
			{
				if (node.InnerText.IndexOf("message") != -1)
				{
					cdrmessage = node.InnerText;
				}
				else
				{
				   _cdr .AddItem (new Item (cdrmessage,path ,node.InnerText ));
				}
			}
			else
			{
				path += node.Name ;
				foreach (XmlNode n in node.ChildNodes)
				{
					Get(n, path,_cdr );
				}
			}
		}

		// Shopping list class which will be serialized
		[XmlRoot("shoppingList")]
		public class ShoppingList
		{
			private ArrayList listShopping;

			public ShoppingList()
			{
				listShopping = new ArrayList();
			}

			[XmlElement("item")]
			public Item[] Items
			{
				get
				{
					Item[] items = new Item[listShopping.Count];
					listShopping.CopyTo(items);
					return items;
				}
				set
				{
					if (value == null) return;
					Item[] items = (Item[])value;
					listShopping.Clear();
					foreach (Item item in items)
						listShopping.Add(item);
				}
			}

			public int AddItem(Item item)
			{
				return listShopping.Add(item);
			}
		}

		// Items in the shopping list
		public class Item
		{
			[XmlAttribute("message")]
			public string mess;
			
			[XmlAttribute("path")]
			public string name;
			
			[XmlAttribute("value")]
			public string price;
			
			[XmlAttribute("format")]
			public string format;
			
			[XmlAttribute("field")]
			public string field;

			[XmlAttribute("newtext")]
			public string newvtext;
			
			//字段名称
            private void fielddecoder(string path)
			{ 
			XmlDocument document = new XmlDocument();
			document.Load(@"F:\LenovoHP\ICDR_EDGE\Workspace\CdrFieldName.xml");
			XmlNodeList nodes = document.GetElementsByTagName("GsnFieldName");
			XmlNode node = default(XmlNode);
			node = null;
				for (int x = 0; x <= nodes.Count - 1; x++)
				{
					if (nodes.Item(x).Attributes.Item(0).Value == path )
					{
	                   {
		                  node = nodes.Item(x);;
			              field=node.Attributes.Item(3).Value.ToString();
			              format=node.Attributes.Item(2).Value.ToString();
	                    }									
					}
				}
			}
			
			private string hexdecoder(string text,string format)
			{
			string hexdecode=null;
			string strdec=null;

							switch (format)
							{
								case "hextodec":
									hexdecode=hextodec(text);
									break;
								//dec 
								case "hextostr":
									hexdecode=hextostr(text);
									break;
								//str 
								case "hextoip":
									hexdecode=hextoip(text);
									break;
								//ip 
								case "hextonip":
									hexdecode=hextonip(text);
									break;
								//nip 
								case "hextotbcd":
									hexdecode=hextotbcd(text);
									break;
								//tbcd 
								case "hextobcd":
									hexdecode=hextobcd(text);
									break;
								//bcd 
								case "hextotime":
									hexdecode=hextotime(text);
									break;
								//time 
								case "hextohex":
									hexdecode=hextohex(text);
									break;
								//hex 
								case "hextoimsi":
									hexdecode=hextoimsi(text);
									break;
								//imsi 
								case "hextomsdn":
									hexdecode=hextomsdn(text);
									break;
								//msdn 
								case "hextoqos":
									hexdecode=hextoqos(text);
									break;
								//qos 
								case "hextootectstr1":
									hexdecode=hextootectstr1(text);
									break;
								//octect string size1 
								case "hextootectstr2":
									hexdecode=hextootectstr2(text);
									break;
								//octect string size2 
								case "Hex2CallEventRecordType":
									strdec =hextodec(text);
									ModEum.CallEventRecordType myCallEventRecordType =
										(ModEum.CallEventRecordType)Enum.Parse(typeof(ModEum.CallEventRecordType), strdec, true);
									hexdecode= myCallEventRecordType.ToString() + "(" + strdec + ")";
									break;
								case "Hex2CauseForRecClosing":
									strdec =hextodec(text);
									ModEum.CauseForRecClosing myCauseForRecClosing =
										(ModEum.CauseForRecClosing)Enum.Parse(typeof(ModEum.CauseForRecClosing), strdec, true);
									hexdecode=myCauseForRecClosing.ToString() + "(" + strdec + ")";
									break;
								case "Hex2ChangeCondition":
									strdec =hextodec(text);
									ModEum.ChangeCondition myChangeCondition =
										(ModEum.ChangeCondition)Enum.Parse(typeof(ModEum.ChangeCondition), strdec, true);
									hexdecode=myChangeCondition.ToString() + "(" + strdec + ")";
									break;
								case "Hex2Diagnostics":
									strdec = hextodec(text);
									ModEum.Diagnostics myDiagnostics =
										(ModEum.Diagnostics)Enum.Parse(typeof(ModEum.Diagnostics), strdec, true);
									hexdecode= myDiagnostics.ToString() + "(" + strdec + ")";
									break;
								case "Hex2APNSelectionMode":
									strdec =hextodec(text);
									ModEum.APNSelectionMode myAPNSelectionMode =
										(ModEum.APNSelectionMode)Enum.Parse(typeof(ModEum.APNSelectionMode), strdec, true);
									hexdecode= myAPNSelectionMode.ToString() + "(" + strdec + ")";
									break;
								case "Hex2ConsolidationResult":
									strdec = hextodec(text);
									ModEum.ConsolidationResult myConsolidationResult =
										(ModEum.ConsolidationResult)Enum.Parse(typeof(ModEum.ConsolidationResult), strdec, true);
									hexdecode= myConsolidationResult.ToString() + "(" + strdec + ")";
									break;
								default:
									hexdecode = text;
									break;
							}
							return hexdecode;
							}


			public Item()
			{
			}
			public Item(string Message,string Name, string Price)
			{
				mess = Message.Replace (" message ","") ;
				name = Name;
				price = Price;
				fielddecoder(Name);
				newvtext=hexdecoder(price,format  );
			}
//			public Item(string Message,string Name, string Price,string  Format)
//			{
//				mess = Message;
//				name = Name;
//				price = Price;
//				format=Format;
//				fielddecoder(Name);
////				newvtext=newText;
//			}
			
					public  string hextotbcd(string strs)
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
		public  string hextoimsi(string strs)
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
				text += (char)System.Convert.ToInt64(hexDigits, 16);//转换到char，即字符
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
		//16机制转2进制
		public static string hextoenum(string strs)
		{
			int m = Convert.ToInt32(OtherConvertToTen(strs, 16));//16   to   10   
			string str1 = TenConvertToOther(m, 2);//10   to   2 
			while (str1.Length < 8) { str1 += "0"; }
			return str1;
		}
		//2机制转10进制
		public static string bintodec(string strs)
		{
			int m = Convert.ToInt32(OtherConvertToTen(strs, 2));//16   to   10   
			return m.ToString();
		}
		public static string hextohex(string strs)
		{
			return strs;
		}
		public static string hextobcd(string strs)
		{
			//return Strings.Left(strs, Strings.Len(strs) - 1)
			return strs;
		}
		public static string hextonip(string strs)
		{
			return strs;
		}
		public static string hextooctet(string strs)
		{
			return strs;
		}
		// Convert the hex string back to the number
		public static string hextodec(string strs)
		{
			if (strs != "")
			{
				double i = Convert.ToInt64(strs, 16);
				return i.ToString();
			}
			else
			{
				return strs;
			}

		}
		//OCTET STRING (SIZE(1)) 
		public static string hextootectstr1(string strs)
		{
			string bytesbit = null;
			for (int i = 0; i < strs.Length; i++)
			{
				string hexDigits = strs.Substring(i, 1);//每次取2个16机制数做一次转换
				int j = Convert.ToInt32(hexDigits, 16);  //16进制字符串转整数，定义i是16进制
				string text = Convert.ToString(j, 2);//16进制整数转2进制字符串，转换i到2进制
				bytesbit = bytesbit + text;
			}


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
			catch { }
			return bytes;

		}
		//OCTET STRING (SIZE(2)) 
		public static string hextootectstr2(string strs)
		{
			return strs;
		}
		//调用   
		//string   str="A10B";   
		//int   m=Convert.ToInt64(OtherConvertToTen(str,10))//16   to   10   
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
			string str = Convert.ToInt64(num, type).ToString();
			return str;
		}
		#endregion
		#region   qos规范
		// 7.7.34   Quality of Service (QoS) Profile
		//The Quality of Service (QoS) Profile shall include the values of the defined QoS parameters.
		//Octet 4 carries the allocation/retention priority octet that is defined in 3GPP TS 23.107. 
		//The allocation/retention priority octet encodes each priority level defined in 3GPP TS 23.107 as the binary value of the priority level. 
		//The allocation/retention priority field shall be ignored by the receiver if: 
		//-	the QoS profile is pre-Release '99.
		//-	the QoS profile IE is used to encode the Quality of Service Requested (QoS Req) field of the PDP context IE.
		//Octets 5 – n (QoS Profile Data field) are coded according to 3GPP TS 24.008 [5] Quality of Service IE, octets 3-m.
		//The minimum length of the field QoS Profile Data is 3 octets; the maximum length is 254 octets.
		//Clause 11.1.6 "Error handling" defines the handling of the case when sent QoS Profile information element has a Length different 
		//from the Length expected by the receiving GTP entity.
		//        Bits	
		//    Octets	8	7	6	5	4	3	2	1	
		//    1 	Type = 135 (Decimal)	
		//    2-3	Length	
		//    4	Allocation/Retention Priority	
		//    5-n	QoS Profile Data	
		//Figure 48: Quality of Service (QoS) Profile Information Element
		//10.5.6.5	Quality of service 
		//The purpose of the quality of service information element is to specify the QoS parameters for a PDP context.
		//The QoS IE is defined to allow backward compatibility to earlier version of Session Management Protocol.
		//The quality of service is a type 4 information element with a minimum length of 14 octets and a maximum length of 16 octets. The QoS requested by the MS shall be encoded both in the QoS attributes specified in octets 3-5 and in the QoS attributes specified in octets 6-14.
		//In the MS to network direction and in the network to MS direction the following applies:
		//-	Octets 15 and 16 are optional. If octet 15 is included, then octet 16 shall also be included.
		//-	A QoS IE received without octets 6-16, without octets 14-16, or without octets 15-16 shall be accepted by the receiving entity.
		//NOTE:	This behavior is required for interworking with entities supporting an earlier version of the protocol, or when the Maximum bit rate for downlink is negotiated to a value lower than 8700 kbps.
		//The quality of service information element is coded as shown in figure 10.5.138/3GPP TS 24.008 and table 10.5.156/3GPP TS 24.008.
		//8	7	6	5	4	3	2	1	
		//Quality of service IEI	octet 1
		//Length of quality of service IE	Octet 2
		//0	0
		//spare	Delay
		//class	Reliability
		//class	octet 3
		//Peak 
		//throughput	0
		//spare	Precedence
		//class	octet 4
		//0	0	0
		//spare	Mean
		//throughput	octet 5
		//Traffic Class	Delivery order	Delivery of erroneous SDU	Octet 6
		//Maximum SDU size	Octet 7
		//Maximum bit rate for uplink	Octet 8
		//Maximum bit rate for downlink	Octet 9
		//Residual BER	SDU error ratio	Octet 10
		//Transfer delay	Traffic Handling priority	Octet 11
		//Guaranteed bit rate for uplink	Octet 12
		//Guaranteed bit rate for downlink	Octet 13
		//0	0	0
		//spare	Signal-ling Indicat-ion	Source Statistics Descriptor	Octet 14
		//Maximum bit rate for downlink (extended)	Octet 15
		//Guaranteed bit rate for downlink (extended)	Octet 16
		//Figure 10.5.138/3GPP TS 24.008: Quality of service information element
		#endregion

		public static string hextoqos(string strs)
		{
			//return strs;
			  //02,23,62,1F,93,96,40,68,74,FF,FF,FF
			object[] qosstr = new object[11];
			string bytes = null;
			try
			{
				//    1 	Type = 135 (Decimal)	
				//    2-3	Length	
				//    4	Allocation/Retention Priority	
				//    5-n	QoS Profile Data	
				//取第0个8位Type = 135 (Decimal) 
				//qosstr[0] = "Type" + strs.Substring(0, 2);
				//取第2个8位Length 
				//qosstr[1] = "Length" + strs.Substring(2, 4);
				//取第1个8位Allocation/Retention Priority 
				//qosstr[2] = "Allocation" + strs.Substring(6, 2);
				//--------------------------------------------- 
				//取第3个8位 
				bytes = hextoenum(strs.Substring(8, 2));
				//--------------------------------------------- 
				//2位spare
				//qosstr[3] = "spare"+ bintodec(bytes.Substring(0, 2));
				//3位Delay class 
				//qosstr[4] = "Delay" + bintodec(bytes.Substring(2, 3));
				qosstr[4] = (ModEum.QoSDelay)Enum.Parse(typeof(ModEum.QoSDelay), bintodec(bytes.Substring(2, 3)), true);
				//3位Reliability class 
				//qosstr[5] = "Reliability" +bintodec( bytes.Substring(5, 3));
				qosstr[5] = (ModEum.QoSReliability)Enum.Parse(typeof(ModEum.QoSReliability), bintodec(bytes.Substring(5, 3)), true);
				//--------------------------------------------- 
				//取第4个8位 
				bytes = hextoenum(strs.Substring(10, 2));
				//--------------------------------------------- 
				//4位Peak throughput 
				//qosstr[6] = "Peak" + bintodec(bytes.Substring(0, 4));
				qosstr[6] = (ModEum.QoSPeakThroughput)Enum.Parse(typeof(ModEum.QoSPeakThroughput), bintodec(bytes.Substring(0, 4)), true);
				//1位spare 
				//qosstr[7] = "spare" + bintodec(bytes.Substring(4, 1)) ;
				//3位Precedence class 
				//qosstr[8] = "Precedence" +bintodec(bytes.Substring(5, 3)) ;
				qosstr[8] = (ModEum.QoSPrecedence)Enum.Parse(typeof(ModEum.QoSPrecedence), bintodec(bytes.Substring(5, 3)), true);
				//--------------------------------------------- 
				//取第5个8位 
				bytes = hextoenum(strs.Substring(12, 2));
				//--------------------------------------------- 
				//3位spare 
				//qosstr[9] = "spare" + bintodec(bytes.Substring(0, 3)) ;
				//5位Mean throughput 
				//qosstr[10] = "Mean" + bintodec(bytes.Substring(3, 5));
				qosstr[10] = (ModEum.QoSMeanThroughput)Enum.Parse(typeof(ModEum.QoSMeanThroughput), bintodec(bytes.Substring(3, 5)), true);
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
		}
		 public class ModEum
	{
		//枚举字段
		public enum CauseForRecClosing
		{
			normalRelease = 0,
			abnormalRelease = 4,
			volumeLimit = 16,
			timeLimit = 17,
			sGSNChange = 18,
			maxChangeCond = 19
		}

		//枚举字段
		public enum ChangeCondition
		{
			qoSChange = 0,
			tariffTime = 1,
			recordClosure = 2
		}

		//枚举字段
		public enum Diagnostics
		{
			gsm0408Cause=0,	//	[0] INTEGER,
			gsm0902MapErrorValue=1,//.[1] INTEGER,
			ccittQ767Cause=2	,//	[2] INTEGER,
			networkSpecificCause=3,//	[3] ManagementExtension,
			manufacturerSpecificCause=4//		[4] ManagementExtension

		}
		//枚举字段
	   public enum  APNSelectionMode
		{
	        mSorNetworkProvidedSubscriptionVerified=0,
	        mSProvidedSubscriptionNotVerified=1,
	        networkProvidedSubscriptionNotVerified=2
		}
	   //枚举字段
		public enum  ConsolidationResult 
		{
	        normal=0,             //			(0),
	        notNormal=1,             //			(1),
	        forInterSGSNConsolidation=2,//		(2),
 	        reachLimit=3,            //			(3),
	        onlyOneCDRGenerated=4         //	(4)
		}
		public enum CallEventRecordType
		{
			sgsnPDPRecord = 18,
			ggsnPDPRecord = 19,
			sgsnMMRecord = 20,
			sgsnSMORecord = 21,
			sgsnSMTRecord = 22,
			ggsnPDPRecordwap = 70
		}
		public enum QoSDelay
		{
			delayClass1 = 1,
			delayClass2 = 2,
			delayClass3 = 3,
			delayClass4 = 4
		}

		public enum QoSMeanThroughput
		{
			bestEffort = 0,
			mean100octetPh = 1,
			mean200octetPh = 2,
			mean500octetPh = 3,
			mean1000octetPh = 4,
			mean2000octetPh = 5,
			mean5000octetPh = 6,
			mean10000octetPh = 7,
			mean20000octetPh = 8,
			mean50000octetPh = 9,
			mean100000octetPh = 10,
			mean200000octetPh = 11,
			mean500000octetPh = 12,
			mean1000000octetPh = 13,
			mean2000000octetPh = 14,
			mean5000000octetPh = 15,
			mean10000000octetPh = 16,
			mean20000000octetPh = 17,
			mean50000000octetPh = 18
		}
		public enum QoSPeakThroughput
		{
			unspecified = 0,
			upTo100OctetPs = 1,
			upTo200OctetPs = 2,
			upTo400OctetPs = 3,
			upTo800OctetPs = 4,
			upTo1600OctetPs = 5,
			upTo3200OctetPs = 6,
			upTo6400OctetPs = 7,
			upTo12800OctetPs = 8,
			upTo25600OctetPs = 9
		}
		public enum QoSPrecedence
		{
			unspecified = 0,
			highPriority = 1,
			normalPriority = 2,
			lowPriority = 3
		}
		public enum QoSReliability
		{
			unspecifiedReliability = 0,
			acknowledgedGTP = 1,
			unackGTPAcknowLLC = 2,
			unackGTPLLCAcknowRLC = 3,
			unackGTPLLCRLC = 4,
			unacknowUnprotectedData = 5
		}
	}