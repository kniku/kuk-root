using System;
using System.Collections;
using System.Linq;
using System.Text;
using knk.shared.logger;

namespace knk.shared.ManagedBeans.Implementations
{
	class LogInfo
	{
		public string msg;
		public LogLevel lvl;
		public LogInfo(LogLevel iLvl, string iMsg)
		{
			msg = iMsg;
			lvl = iLvl;
		}
	}

	class MB_Logger : AbstractManagedThreadedBean, IReceivingBean
	{
		const int MAX_LOGS = 50;
		const int WATERMARK_LOGS = 500;
		ArrayList arrLogs = new ArrayList();

		public ILogger ExternalLogger;

		protected override int getTimeoutMS()
		{
			return 100;
		}

		public override string getName()
		{
			return "Logger";
		}

		public override string getDescription()
		{
			return "Allgemeine Logging Bean";
		}

		public override void stop()
		{
			base.stop();
			lock (arrLogs)
			{
				arrLogs.Clear();
			}
		}

		public override void beanRun()
		{
			object[] msgs = null;

			lock (arrLogs)
			{
				int count = arrLogs.Count;
				if (count > 0)
				{
					if (count > MAX_LOGS)
					{
						// zu viele Daten - wegwerfen...
						arrLogs.RemoveRange(0, count - MAX_LOGS);
//						logger.LogError("Logs verworfen: {0}", count - MAX_LOGS);
						arrLogs.Insert(0, new LogInfo(LogLevel.Error, string.Format("Logs verworfen: {0}", count - MAX_LOGS)));
					}
					// kopiere Inhalt
//					logger.LogInfo("Log count: {0}", count);
					msgs = arrLogs.ToArray();
					// Array ausleeren...
					arrLogs.Clear();
				}
			}

			if (msgs != null && ExternalLogger != null)
			{
				for (int idx = 0; idx < msgs.Count(); idx++)
				{
					LogInfo li = msgs[idx] as LogInfo;
					ExternalLogger.Log(li.lvl, "{0}", li.msg);
				}
			}
		}

		public void handleData(ICallingBean iCaller, params object[] iArgs)
		{
			if (getState() == BeanState.running)
			{
				if (iArgs.Count() == 2 && iArgs[0] is LogLevel && iArgs[1] is string)
				{
					lock (arrLogs)
					{
						if (arrLogs.Count < WATERMARK_LOGS)
						{
							arrLogs.Add(new LogInfo((LogLevel)iArgs[0], iArgs[1] as string));
//							arrLogs.Add(iArgs[1]);
						}
					}
				}
			}
		}
	}
}
