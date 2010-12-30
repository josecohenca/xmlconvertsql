<Query Kind="Program">
  <Reference>&lt;ProgramFiles&gt;\Microsoft Visual Studio 9.0\Common7\IDE\PublicAssemblies\Microsoft.VisualStudio.QualityTools.UnitTestFramework.dll</Reference>
  <Reference>&lt;RuntimeDirectory&gt;\System.XML.dll</Reference>
  <Namespace>System.Xml.Serialization</Namespace>
</Query>


	
		 void Main()
		{

			ShoppingList myList = new ShoppingList();

			string path = "";
			string file = @"F:\LenovoHP\ICDR_EDGE\Temp\test.xml";
			XmlDocument docu = new XmlDocument();
			docu.Load(file);
			XmlElement roo = docu.DocumentElement;
			Get(roo, path,myList  );

			// Serialization
		 XmlSerializer s = new XmlSerializer( typeof( ShoppingList ) );
			TextWriter w = new StreamWriter(@"c:\list.xml");
			s.Serialize(w, myList);
			w.Close();

			// Deserialization
			ShoppingList newList;
			TextReader r = new StreamReader(@"c:\list.xml");
			newList = (ShoppingList)s.Deserialize(r);
			r.Close();

   newList.Dump();
//	XmlDocument doc = new XmlDocument();
//	doc.Load( @"c:\list.xml");
//	XmlElement root = doc.DocumentElement;
//	var q=from c in doc.ChildNodes .Cast <XmlNode>()
//	where c.InnerText.Length >1
//	    select c.InnerXml;
//	q.Dump ();
		}

	   static string cdrmessage = null;
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
				path += node.Name + "/";
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

			public Item()
			{
			}
			public Item(string Message,string Name, string Price)
			{
				mess = Message;
				name = Name;
				price = Price;
			}
		}
	