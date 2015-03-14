using System;
using System.Collections;
using System.Linq;
using System.Text;

namespace DCS.ManagedBeans.Implementations
{
	class MB_DBLogger : AbstractManagedThreadedBean, IReceivingBean
	{
		const int MAX_LOGS = 50;
		const int WATERMARK_LOGS = 500;
		ArrayList arrLogs = new ArrayList();

		protected override int getTimeoutMS()
		{
			return 100;
		}

		public override void init()
		{
			base.init();
/*			AppGlobal.instance.dbtools.execute_sql(
				"CREATE TABLE knk_log (" +
				" ts timestamp without time zone," +
				" lvl character varying(10)," +
				" msg character varying)");
 * */
		}

		public override void beanRun()
		{
#if false
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
						AppGlobal.instance.logger.LogError("Logs verworfen: {0}", count - MAX_LOGS);
					}
					// kopiere Inhalt
					AppGlobal.instance.logger.LogInfo("Log count: {0}", count);
					msgs = arrLogs.ToArray();
					// Array ausleeren...
					arrLogs.Clear();
				}
			}

			if (msgs != null)
			{
				DateTime ts = DateTime.Now;
				for (int idx = 0; idx < msgs.Count(); idx++)
				{
					AppGlobal.instance.dbtools.execute_sql("insert into knk_log (ts,msg) values ('" + AppGlobal.instance.dbtools.getSqlDateTimeStr(ts) + "','" + msgs[idx] + "')");
				}
			}
#endif
		}

		public override string getName()
		{
			return "DB Logger";
		}

		public override string getDescription()
		{
			return "Database Log Handler";
		}

		#region IReceivingBean Member

		public void handleData(ICallingBean iCaller, params object[] iArgs)
		{
			if (getState() == BeanState.running)
			{
				if (iArgs.Count() == 2 && iArgs[0] is DCS.Logger.LogLevel && iArgs[1] is string)
				{
					lock (arrLogs)
					{
						if (arrLogs.Count < WATERMARK_LOGS)
						{
							arrLogs.Add(iArgs[1]);
						}
					}
				}
			}
		}

		#endregion

	}
}
