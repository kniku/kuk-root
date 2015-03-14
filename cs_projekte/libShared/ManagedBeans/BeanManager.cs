using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using knk.shared.logger;

namespace knk.shared.ManagedBeans
{
	public class BeanManager
	{
		Hashtable hashBeans = new Hashtable();	// Hash mit allen registrierten Beans
		static BeanManager _instance = null;	// Die Singleton Instanz
//		static ILogger _beanLogger;
		static ManagedBeans.Implementations.LoggingBean _internalLogger;
		static Implementations.MB_Logger _loggingBean;

		/// <summary>
		/// Dictionary der verwalteteten Beans
		/// </summary>
		public IDictionary Beans { get { return hashBeans; } }
		
		/// <summary>
		/// Zugriff auf die globale Instanz des Bean Managers (Singleton)
		/// </summary>
		public static BeanManager instance { get { return _instance; } private set { _instance = value; } }	// Globaler Zugriff auf die einzige Instanz

		//		public volatile bool isRunning;

		// private constructor (=Singleton!)
		BeanManager() { }
	
		/// <summary>
		/// Erzeugt den Bean Manager (Singleton)
		/// </summary>
		/// <returns>Den neuen Bean Manager</returns>
		public static BeanManager createInstance()
		{
			if (_instance == null)
			{
				_instance = new BeanManager();

				IManagedBean cb;


				_loggingBean = new Implementations.MB_Logger();
				_instance.addBean(_loggingBean);

				_internalLogger = new ManagedBeans.Implementations.LoggingBean();
				_internalLogger.addReceivingBean(_loggingBean as IReceivingBean);

				// Master Bean für alle non-threaded Beans
				cb = new Implementations.MB_MasterBean();
				_instance.addBean(cb);


				foreach (IManagedBean aBean in _instance.hashBeans.Values)
				{
					aBean.setLogger(_internalLogger);
					aBean.init();
				}
			}
			return _instance;
		}

		/// <summary>
		/// Alle Beans starten...
		/// </summary>
		/// <returns>Den Bean Manager</returns>
		private BeanManager startAllBeans()
		{
			foreach (IManagedBean aBean in Beans.Values)
			{
				aBean.start();
			}
			return this;
		}

		/// <summary>
		/// Stoppt alle Beans
		/// </summary>
		/// <returns>Den Bean Manager</returns>
		private BeanManager stopAllBeans()
		{
			foreach (IManagedBean aBean in Beans.Values)
			{
				aBean.stop();
			}
			return this;
		}

		/// <summary>
		/// Startet alle Beans.
		/// </summary>
		/// <returns>Den Bean Manager</returns>
		public BeanManager start()
		{
			startAllBeans();

			return this;
		}

		public BeanManager stop()
		{
			stopAllBeans();
			return this;
		}

		public BeanManager addBean(IManagedBean iBean)
		{
			iBean.setLogger(_internalLogger);
			_instance.hashBeans.Add(iBean.getName(), iBean);
			return this;
		}

		public BeanManager setBeanLogger(ILogger iLogger)
		{
			_loggingBean.ExternalLogger = iLogger;
			return this;
		}

	}
}
