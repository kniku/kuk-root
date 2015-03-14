using System;
using System.Collections;

namespace DCS.Configurator
{
	public class ConfiguratorXML : IConfigurator
	{
		Hashtable hashParams = new Hashtable();

		private static ConfiguratorXML _instance;
		public static ConfiguratorXML Instance { get { return getInstance(); } protected set { _instance = value; } }

		public static ConfiguratorXML createInstance()
		{
			return getInstance();
		}

		private static ConfiguratorXML getInstance()
		{
			if (_instance == null)
			{
				_instance = new ConfiguratorXML();
			}
			return _instance;
		}

		public IConfigurator loadConfig()
		{
			// TODO
			return this;
		}

		public bool getParameter(string iParamName, bool iDefault)
		{
			bool r;
			string s = getParameter(iParamName, null);
			if (!bool.TryParse(s, out r))
			{
				r = iDefault;
			}

			return r;
		}

		public int getParameter(string iParamName, int iDefault)
		{
			int r;
			string s = getParameter(iParamName, null);
			if (!int.TryParse(s, out r))
			{
				r = iDefault;
			}

			return r;
		}

		public double getParameter(string iParamName, double iDefault)
		{
			double r;
			string s = getParameter(iParamName, null);
			if (!double.TryParse(s, out r))
			{
				r = iDefault;
			}

			return r;
		}

		public string getParameter(string iParamName, string iDefault)
		{
			string r = hashParams[iParamName] as string;
			return r == null ? iDefault : r;
		}
	}
}
