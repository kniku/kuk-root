using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;

namespace DCS.ManagedBeans
{
	public class BeanManager
	{
		Hashtable hashBeans = new Hashtable();	// Hash mit allen registrierten Beans
		static BeanManager _instance = null;	// Die Singleton Instanz

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

#if false
				IManagedBean cb_log = new Implementations.MB_DBLogger();
				_instance.hashBeans.Add(cb_log.getName(), cb_log);

				DCS.Logger.ILogger logger = new ManagedBeans.Implementations.LoggingBean(cb_log as IReceivingBean);
#else

				IManagedBean cb_log = new Implementations.MB_Logger();
				_instance.hashBeans.Add(cb_log.getName(), cb_log);

//				DCS.Logger.ILogger logger = new DCS.Logger.LoggerImpl_ConsoleColored();
//				DCS.Logger.ILogger logger = new ManagedBeans.Implementations.LoggingBean(cb_log as IReceivingBean);
				DCS.Logger.ILogger logger = new ManagedBeans.Implementations.LoggingBean();
				(logger as ManagedBeans.Implementations.LoggingBean).addReceivingBean(cb_log as IReceivingBean);
#endif

				// Master Bean für alle non-threaded Beans
				cb = new Implementations.MB_MasterBean();
				_instance.hashBeans.Add(cb.getName(), cb);

				// Controller Bean
				cb = new Implementations.MB_ControllerBean();
				_instance.hashBeans.Add(cb.getName(), cb);

				/*
								cb = new Implementations.MB_dummyThreaded();
								_instance.hashBeans.Add(cb.getName(), cb);

								cb = new Implementations.MB_dummy();
								cb.addReceivingBean(new Implementations.DB_dummy());
								cb.addReceivingBean(new Implementations.DB_dummy());
								_instance.hashBeans.Add(cb.getName(), cb);
								cb = new Implementations.MB_DCSController();
								_instance.hashBeans.Add(cb.getName(), cb);
				*/

				foreach (IManagedBean aBean in _instance.hashBeans.Values)
				{
					aBean.setLogger(logger);
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
			IManagedBean ctrl = null;

			foreach (IManagedBean aBean in Beans.Values)
			{
				if (aBean is Implementations.MB_ControllerBean)
				{
					ctrl = aBean;
				}
				else
				{
					aBean.stop();
				}
			}
			// den controller zuletzt stoppen
			if (ctrl != null) ctrl.stop();
			return this;
		}

		/// <summary>
		/// Startet alle Beans.
		/// </summary>
		/// <returns>Den Bean Manager</returns>
		public BeanManager start()
		{
//			isRunning = true;
			startAllBeans();

			return this;
#if false
			// Falls zumindest ein Bean ohne eigenen Thread eingehängt,
			// den globalen Thread starten, der diese zyklisch aufruft
			foreach (IManagedBean aBean in Beans.Values)
			{
				if (!(aBean is AbstractManagedThreadedBean))
				{

				}
			}
			while (isRunning)
			{
				System.Threading.Thread.Sleep(100);
				foreach (IManagedBean aBean in Beans.Values)
				{
					if (!(aBean is AbstractManagedThreadedBean))
					{
						aBean.beanRun();
					}
				}
			}
			stopAllBeans();
#endif
		}

		public BeanManager stop()
		{
			stopAllBeans();
			return this;
		}
	}
}
