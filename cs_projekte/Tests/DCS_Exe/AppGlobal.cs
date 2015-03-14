using System;
using System.Collections;
using System.Linq;
using System.Text;
using DCS.Logger;
using DCS.Configurator;

namespace DCS_Exe
{
	class AppGlobal
	{
		public ILogger Logger { get; set; }
		public IConfigurator Config { get; set; }

		private static AppGlobal _instance;
		public static AppGlobal Instance { get { return getInstance(); } protected set { _instance = value; } }

		public static AppGlobal createInstance()
		{
			return getInstance();
		}

		private static AppGlobal getInstance()
		{
			if (_instance == null)
			{
				_instance = new AppGlobal();
			}
			return _instance;
		}

		protected Hashtable hashProperties = new Hashtable();

		public void addProperty(string iName, object iProperty)
		{
			hashProperties.Add(iName, iProperty);
		}

		public object getProperty(string iName)
		{
			return hashProperties[iName];
		}

		protected AppGlobal()
		{
			Config = ConfiguratorXML.createInstance();
			Logger = new DCS.Logger.LoggerImpl_ConsoleColored();
		}
	}
}
