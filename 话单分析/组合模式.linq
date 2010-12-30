<Query Kind="Expression">
  <Reference>&lt;ProgramFiles&gt;\Microsoft Visual Studio 9.0\Common7\IDE\PublicAssemblies\Microsoft.VisualStudio.QualityTools.UnitTestFramework.dll</Reference>
  <Reference>&lt;RuntimeDirectory&gt;\System.XML.dll</Reference>
</Query>

static void Main()
			{
			
				string path = "";
				XmlDocument doc = new XmlDocument();
				doc.Load(@"F:\LenovoHP\ICDR_EDGE\Temp\test.xml");
				XmlElement root = doc.DocumentElement;
				root0 = new Composite(@"F:\LenovoHP\ICDR_EDGE\Temp\test.xml");
//				var q=from c in doc.Cast<XmlNode>()
////				.Cast <XmlNode>()
////				      where c.HasChildNodes ==true
//				      select c.InnerXml ;
//					  q.Dump ();
//					  
////foreach (char s  in q)
////
////		Console.WriteLine (s);
////					  q.Dump ();				
				Get(root,path);
//		foreach (KeyValuePair<int,metacdr> kv in d)          //  One ; 1
//	Console.WriteLine (kv.Key + "; " + kv.Value.path +";"+kv.Value .innertext ); 
root0.Diaplay (1);
//root0.Diaplay (2);
			}
//			public static  Dictionary<int,metacdr> d=new Dictionary<int,metacdr>();
//			public static int i=0;
			public static 		Composite root0 = new Composite();
//			public static 		Composite root1 = new Composite("根节点root1");
//TreeNode tr=new TreeNode("a");
	  public static 	 Composite 	comp=new Composite( );
			public static void  Get(XmlNode node,string path)
			{
				if (!node.HasChildNodes)
				{
//					Console.WriteLine(path);
//					Console.WriteLine(node.InnerText);
//					metacdr singleb=new metacdr(path,node.InnerText );
//					d.Add (i,singleb);
//					i++;
//
//				Composite root = new Composite("根节点root");
////			root.Add( new metacdr(path,node.InnerText));
//	Composite root1;
//if(node.OfType<XComment>().Any())
//(node.OfType<XComment>().Any())
//					root1 = new Composite(node.InnerText );
////			root1.Add(new Leaf(path));
//			root0.Add (root1);
	
			if(node.InnerText .IndexOf ("message")!=-1)
			{
				comp=new Composite(node.InnerText );
			root0.Add(comp);
		
			}
			else
				comp.Add (new Leaf(node.InnerText ));		
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
			public class metacdr
			{
			   private string _path;
			   private string _innertext;
				public string path
				{
				get { return _path; } set { _path= value; }
				}
				public string innertext
				{get { return _innertext; } set { _innertext = value; }
				}
			   
			   public metacdr(string path,string innertext)
				{
				this._path =path;
				this._innertext=innertext ;
				}
			}
				public abstract class Component
	{
		protected string name;

		public Component(string name)
		{
			this.name = name;
		}
				public Component()
		{

		}

		public abstract void Add(Component c);
		public abstract void Remove(Component c);
		public abstract void Diaplay(int depth);
	}
		public class Composite : Component
	{

		List<Component> children;

		public Composite(string name)
			:base(name)
		{
			if (children == null)
			{
				children = new List<Component>();
			}
		}
public Composite()
		{
		}
		public override void Add(Component c)
		{
			this.children.Add(c);
		}

		public override void Remove(Component c)
		{
			this.children.Remove(c);
		}

		public override void Diaplay(int depth)
		{
			Console.WriteLine(new String('-',depth)+name);
			foreach (Component component in children)
			{
				component.Diaplay(depth + 2);
			}
		}
	}
	public class Leaf:Component
	{

		public Leaf(string name)
			:base(name)
		{
			
		}

		public override void Add(Component c)
		{
			Console.WriteLine("不能向叶子节点添加子节点");
		}

		public override void Remove(Component c)
		{
			Console.WriteLine("叶子节点没有子节点");
		}

		public override void Diaplay(int depth)
		{
			Console.WriteLine(new string('-',depth)+name);
		}
	}