using System;
using System.Collections.Generic;
using System.Configuration;
using System.Text;

namespace $rootnamespace$
{
	class $safeitemname$ : ConfigurationSection
	{
		private $safeitemname$() { }

		#region Public Methods

		///<summary>Get this configuration set from the application's default config file</summary>
		public static $safeitemname$ Open()
		{
			System.Reflection.Assembly assy = System.Reflection.Assembly.GetEntryAssembly();
			return Open(assy.Location);
		}

		///<summary>Get this configuration set from a specific config file</summary>
		public static $safeitemname$ Open( string path )
		{
			if ((object)instance == null)
			{
				if (path.EndsWith(".config", StringComparison.InvariantCultureIgnoreCase))
					spath = path.Remove(path.Length - 7);
				else
					spath = path;
				Configuration config = ConfigurationManager.OpenExeConfiguration(spath);
				if (config.Sections["$safeitemname$"] == null)
				{
					instance = new $safeitemname$();
					config.Sections.Add("$safeitemname$", instance);
					config.Save(ConfigurationSaveMode.Modified);
				}
				else
					instance = ($safeitemname$)config.Sections["$safeitemname$"];
			}
			return instance;
		}

		///<summary>Create a full copy of the current properties</summary>
		public $safeitemname$ Copy()
		{
			$safeitemname$ copy = new $safeitemname$();
			string xml = SerializeSection(this, "$safeitemname$", ConfigurationSaveMode.Full);
			System.Xml.XmlReader rdr = new System.Xml.XmlTextReader(new System.IO.StringReader(xml));
			copy.DeserializeSection(rdr);
			return copy;
		}

		///<summary>Save the current property values to the config file</summary>
		public void Save()
		{
			// The Configuration has to be opened anew each time we want to update the file contents.
			// Otherwise, the update of other custom configuration sections will cause an exception
			// to occur when we try to save our modifications, stating that another app has modified
			// the file since we opened it.
			Configuration config = ConfigurationManager.OpenExeConfiguration(spath);
			$safeitemname$ section = ($safeitemname$)config.Sections["$safeitemname$"];
			//
			// TODO: Add code to copy all properties from "this" to "sect"
			//
			if (section.Sample != this.Sample)	// This test is optional
				section.Sample = this.Sample;
			//
			config.Save(ConfigurationSaveMode.Modified);
		}

		#endregion Public Methods

		#region Properties

		public static $safeitemname$ Default
		{
			get { return defaultInstance; }
		}

		// TODO: Add your custom properties and elements here.
		// All properties should have both get and set accessors to implement the Save function correctly
		[ConfigurationProperty("Sample", DefaultValue="sample string property")]
		public string Sample
		{
			get { return (string)this["Sample"]; }
			set { this["Sample"] = value; }
		}

		#endregion Properties

		#region Fields
		private static string spath;
		private static $safeitemname$ instance = null;
		private static readonly $safeitemname$ defaultInstance = new $safeitemname$();
		#endregion Fields
	}
}
