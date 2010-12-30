<Query Kind="Program">
  <Reference>&lt;ProgramFiles&gt;\Microsoft Visual Studio 9.0\Common7\IDE\PublicAssemblies\Microsoft.VisualStudio.QualityTools.UnitTestFramework.dll</Reference>
  <Reference>&lt;RuntimeDirectory&gt;\System.XML.dll</Reference>
  <Namespace>System.Xml.Serialization</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

static void Main()
	{
	cdrlist = new cdr();
	
		string path = "";
		string file=@"F:\LenovoHP\ICDR_EDGE\Temp\test.xml";
		XmlDocument docu = new XmlDocument();
		docu.Load(file);
		XmlElement roo= docu.DocumentElement;
		Get(roo, path);
       
	   	// Serialization
	XmlSerializer s = new XmlSerializer( typeof( cdr ) );
	TextWriter w = new StreamWriter( @"c:\list.xml" );
	s.Serialize( w, cdrlist  );
	w.Close();
	
	// Deserialization
	cdr newList;
	TextReader r = new StreamReader(  @"c:\list.xml" );
	newList = (cdr)s.Deserialize( r );
	r.Close();
	
	XmlDocument doc = new XmlDocument();
	doc.Load( @"c:\list.xml");
	XmlElement root = doc.DocumentElement;
	var q=from c in doc.ChildNodes .Cast <XmlNode>()
	    select c;
	q.Dump ();
	}
		public static cdr cdrlist;
		public static cdrMessage cdrmessage;
		public static	cdrMessageContext cdrmessagecontext;
	public static void Get(XmlNode node, string path)
	{
		if (!node.HasChildNodes)
		{
			if (node.InnerText.IndexOf("message") != -1)
			{
			 cdrmessage=new cdrMessage(node.InnerText );     
		    }
			else
			{
	         cdrmessagecontext = new cdrMessageContext(path,node.InnerText );
             cdrmessage.context.Add (cdrmessagecontext );
	       	}
		}
		else
		{
			path += node.Name + "/";
			foreach (XmlNode n in node.ChildNodes)
			{
				Get(n, path);
			}
		}
		}
	
/// <remarks/>
[Serializable]
public  class cdr {
	
//	private cdrMessage[] itemsField;
   
	List<cdrMessage> itemsField;
	
	/// <remarks/>
//    [DataContract]
//	public cdrMessage[] Items {
	public 	List<cdrMessage> items{
		get {
			return this.itemsField;
		}
		set {
			this.itemsField = value;
		}
	}
	
	public cdr(){
//	items=new List<cdrMessage>();
	}
}

[Serializable]
public  class cdrMessage {
	
	private string numberField;
	
	
	public cdrMessage(){}
	
	private List<cdrMessageContext> contextField;
	
	/// <remarks/>
	 [XmlAttribute()]
	public string number {
		get {
			return this.numberField;
		}
		set {
			this.numberField = value;
		}
	}
	
	/// <remarks/>
	 [XmlArray()]
 [XmlArrayItem(typeof(cdrMessageContext))]
	public List<cdrMessageContext> context {
		get {
			return this.contextField;
		}
		set {
			this.contextField = value;
		}
	}
	
		
	public cdrMessage(string num){
	this.numberField =num;
	this.contextField=new List<cdrMessageContext>();
	}
}

/// <remarks/>
[Serializable]
public  class cdrMessageContext {
	
	[XmlAttribute("name")]private string nameField;
	
	[XmlAttribute("price")]private string metavalueField;
	
	/// <remarks/>
	[System.Xml.Serialization.XmlAttributeAttribute()]
	public string name {
		get {
			return this.nameField;
		}
		set {
			this.nameField = value;
		}
	}
	
	/// <remarks/>
	[System.Xml.Serialization.XmlAttributeAttribute()]
	public string metavalue {
		get {
			return this.metavalueField;
		}
//		set {
			this.metavalueField = value;
		}
	}
	
	public cdrMessageContext(string nameField,string metavalueField){
	this.nameField=nameField;
	this.metavalueField=metavalueField;
	}
}