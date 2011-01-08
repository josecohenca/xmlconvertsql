using System;
using System.Configuration;
using System.Text;

namespace CustomConfigExample
{
	class MySection : ConfigurationSection
	{
		private MySection() { }

		#region Public Methods

		///<summary>
		///Get this configuration set from the application's default config file
		///</summary>
		public static MySection Open()
		{
			System.Reflection.Assembly assy = System.Reflection.Assembly.GetEntryAssembly();
			return Open(assy.Location);
		}

		///<summary>
		/// Get this configuration set from a specific config file
		///</summary>
		public static MySection Open(string path)
		{
			if ((object)instance == null)
			{
				if (path.EndsWith(".config", StringComparison.InvariantCultureIgnoreCase))
					spath = path.Remove(path.Length - 7);
				else
					spath = path;
				Configuration config = ConfigurationManager.OpenExeConfiguration(spath);
				if (config.Sections["MySection"] == null)
				{
					instance = new MySection();
					config.Sections.Add("MySection", instance);
					config.Save(ConfigurationSaveMode.Modified);
				}
				else
					instance = (MySection)config.Sections["MySection"];
			}
			return instance;
		}

		///<summary>
		///Create a full copy of the current properties
		///</summary>
		public MySection Copy()
		{
			MySection copy = new MySection();
			string xml = SerializeSection(this, "MySection", ConfigurationSaveMode.Full);
			System.Xml.XmlReader rdr = new System.Xml.XmlTextReader(new System.IO.StringReader(xml));
			copy.DeserializeSection(rdr);
			return copy;
		}

		///<summary>
		///Save the current property values to the config file
		///</summary>
		public void Save()
		{
			Configuration config = ConfigurationManager.OpenExeConfiguration(spath);
			MySection section = (MySection)config.Sections["MySection"];
			//
			// TODO: Add code to copy all properties from "this" to "section"
			//
			section.SectorConfig = this.SectorConfig;
			section.UnitShapeConfig = this.UnitShapeConfig;
			config.Save(ConfigurationSaveMode.Full); //Try with "Modified" to see the difference
		}

		#endregion Public Methods

		#region Properties

		public static MySection Default
		{
			get { return defaultInstance; }
		}

		[ConfigurationProperty("Sector")]
		public SomeSettings SectorConfig
		{
			get { return (SomeSettings)this["Sector"]; }
			set { this["Sector"] = value; }
		}

		[ConfigurationProperty("UnitShapes")]
		public ShapeSettings UnitShapeConfig
		{
			get { return (ShapeSettings)this["UnitShapes"]; }
			set { this["UnitShapes"] = value; }
		}

		#endregion Properties

		#region Fields
		private static string spath;
		private static MySection instance = null;
		private static readonly MySection defaultInstance = new MySection();
		#endregion Fields
	}
}
