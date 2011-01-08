using System;
using System.Configuration;
using System.Text;

namespace CustomConfigExample
{
	public class ShapeSettings : ConfigurationElement
	{
		public ShapeSettings() { }

		[ConfigurationProperty("SizeMultiple", DefaultValue = "2")]
		public int SizeMultiple
		{
			get { return (int)this["SizeMultiple"]; }
			set { this["SizeMultiple"] = value; }
		}

		[ConfigurationProperty("Enable", DefaultValue = "Yes")]
		private string pEnable
		{
			get { return this["Enable"].ToString(); }
			set { this["Enable"] = value; }
		}

		public bool Enable
		{
			get { return pEnable.ToLower() == "yes"; }
			set { pEnable = value ? "Yes" : "No"; }
		}

		[ConfigurationProperty("DevA")]
		public AShapeSetting DevA
		{
			get { return (AShapeSetting)this["DevA"]; }
		}

		[ConfigurationProperty("DevB")]
		public AShapeSetting DevB
		{
			get { return (AShapeSetting)this["DevB"]; }
		}
	}
}
