<Query Kind="Program">
  <Connection>
    <ID>f2f6e2b4-0f40-48df-b2c7-82584a77c393</ID>
    <AttachFileName>&lt;ApplicationData&gt;\LINQPad\Nutshell.mdf</AttachFileName>
    <Server>.\SQLEXPRESS</Server>
    <AttachFile>true</AttachFile>
    <UserInstance>true</UserInstance>
  </Connection>
  <Reference>&lt;ProgramFiles&gt;\Microsoft Visual Studio 9.0\Common7\IDE\PublicAssemblies\Microsoft.VisualStudio.QualityTools.UnitTestFramework.dll</Reference>
  <Reference>&lt;RuntimeDirectory&gt;\System.XML.dll</Reference>
  <Namespace>System.Xml.Serialization</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
{
	
	ShoppingList myList = new ShoppingList();
	myList.AddItem( new Item( "eggs",1.49 ) );
	myList.AddItem( new Item( "ground beef",3.69 ) );
	myList.AddItem( new Item( "bread",0.89 ) );
	
	// Serialization
	XmlSerializer s = new XmlSerializer( typeof( ShoppingList ) );
	TextWriter w = new StreamWriter( @"c:\list.xml" );
	s.Serialize( w, myList );
	w.Close();
	
	// Deserialization
	ShoppingList newList;
	TextReader r = new StreamReader(  @"c:\list.xml" );
	newList = (ShoppingList)s.Deserialize( r );
	r.Close();
	
		XmlDocument doc = new XmlDocument();
					doc.Load( @"c:\list.xml");
				XmlElement root = doc.DocumentElement;
				var q=from c in doc.ChildNodes .Cast <XmlNode>()
				      select c;
					  q.Dump ();
	
}
// Shopping list class which will be serialized
	[XmlRoot("shoppingList")]
	public class ShoppingList {
	  private ArrayList listShopping;
	
	  public ShoppingList() {
		listShopping = new ArrayList();
	  }
	
	  [XmlElement("item")]
	  public Item[] Items {
		get {
		  Item[] items = new Item[ listShopping.Count ];
		  listShopping.CopyTo( items );
		  return items;
		}
		set {
		  if( value == null ) return;
		  Item[] items = (Item[])value;
		  listShopping.Clear();
		  foreach( Item item in items )
			listShopping.Add( item );
		}
	  }
	
	  public int AddItem( Item item ) {
		return listShopping.Add( item );
	  }
	}
	
	// Items in the shopping list
	public class Item {
	  [XmlAttribute("name")] public string name;
	  [XmlAttribute("price")] public double price;
	
	  public Item() {
	  }
	public Item( string Name, double Price ) {
//	  public Item( string Name, string Price ) {
		name = Name;
		price = Price;
	  }
	}
// Define other methods and classes here
