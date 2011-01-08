using System;
using System.Configuration;
using System.Text;

namespace CustomConfigExample
{
	public class AShapeSetting : ConfigurationElement
	{
		public AShapeSetting() { }

		[ConfigurationProperty("Shape", DefaultValue = "Circle")]
		public string Shape
		{
			get { return (string)this["Shape"]; }
			set { this["Shape"] = value; }
		}

		[ConfigurationProperty("Size", DefaultValue = "12")]
		public int Size
		{
			get { return (int)this["Size"]; }
			set { this["Size"] = value; }
		}
	}
}
