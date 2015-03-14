using System;
using System.Collections;
using knk.shared.logger;

namespace knk.shared
{
	public interface IAppGlobal
	{
		IAppGlobal getInstance();
		IAppGlobal addProperty(string iName, object iProperty);
		object getProperty(string iName);
		IAppGlobal setLogger(ILogger iLogger);
		ILogger getLogger();
	}

    public class AppGlobalSimple : IAppGlobal
    {
		private static IAppGlobal _Instance;
        protected Hashtable hashProperties = new Hashtable();
		public IAppGlobal Instance { get { return getInstance(); } }
		public ILogger Log;


		public static IAppGlobal createInstance(string iName)
		{
			System.Reflection.Assembly a = System.Reflection.Assembly.GetCallingAssembly();
			Type t = a.GetType(iName);
			if (t == null)
			{
				a = System.Reflection.Assembly.GetEntryAssembly();
				t = a.GetType(iName);
			}

			return createInstance(t);
		}

		public static IAppGlobal createInstance(Type iType)
		{
			if (_Instance == null && iType != null)
			{
				_Instance = Activator.CreateInstance(iType) as IAppGlobal;
			}
			return _Instance;
		}

		public static IAppGlobal createInstance()
		{
			return createInstance(typeof(AppGlobalSimple));
		}

		#region Implementation interface IAppGlobal

		public IAppGlobal getInstance()
		{
			return _Instance;
		}

		public IAppGlobal addProperty(string iName, object iProperty)
		{
			hashProperties.Add(iName, iProperty);
			return this;
		}

		public object getProperty(string iName)
		{
			return hashProperties[iName];
		}

		public IAppGlobal setLogger(ILogger iLogger)
		{
			Log = iLogger;
			return this;
		}

		public ILogger getLogger()
		{
			return Log;
		}

		#endregion interface IAppGlobal

	}
}
