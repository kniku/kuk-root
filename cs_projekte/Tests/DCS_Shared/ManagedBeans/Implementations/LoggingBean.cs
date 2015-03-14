using System;
using System.Collections;
using System.Linq;
using System.Text;

namespace DCS.ManagedBeans.Implementations
{
	class LoggingBean : DCS.Logger.AbstractLogger, ICallingBean
	{
//		IReceivingBean logListener;
		ArrayList listReceivingBeans = new ArrayList();

		public LoggingBean(/*IReceivingBean iListener*/)
			: base("LoggingBean")
		{
//			logListener = iListener;
		}

		protected override void Log(DCS.Logger.LogLevel iLevel, string iMsg)
		{
			sendDataToAllReceivers(iLevel, iMsg);
		}


		public string getName()
		{
			return "LoggingBean";
		}

		public string getDescription()
		{
			return "LoggingBean";
		}

		public BeanState getState()
		{
			return BeanState.none;
		}

		public DCS.Logger.ILogger getLogger()
		{
			return this;
		}

		public void setLogger(Logger.ILogger iLogger)
		{
		}

		public void addReceivingBean(IReceivingBean iBean)
		{
			listReceivingBeans.Add(iBean);
		}

		public void sendDataToAllReceivers(params object[] iData)
		{
			foreach (IReceivingBean aBean in listReceivingBeans)
			{
				aBean.handleData(this, iData);
			}
		}
	}
}
