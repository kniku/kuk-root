using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Configuration;
using simpleConfiguration.Properties;
using System.Collections.Specialized;

namespace simpleConfiguration
{
	class Program
	{
		static void Main(string[] args)
		{
			Settings setting = new Settings();
			string variable1 = setting.db;

			NameValueCollection col = ConfigurationManager.AppSettings;
			ConnectionStringSettings conSettings = ConfigurationManager.ConnectionStrings["db"];
			string x;
			if (conSettings != null)
			{
				x = conSettings.ConnectionString;
			}
			x = col["test"];
			x = "";
		}
	}
}
