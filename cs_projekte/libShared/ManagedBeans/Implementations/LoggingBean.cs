using System;
using System.Collections;
using System.Linq;
using System.Text;
using knk.shared.logger;

namespace knk.shared.ManagedBeans.Implementations
{
	class LoggingBean : AbstractLogger, ICallingBean
	{
//		IReceivingBean logListener;
		ArrayList listReceivingBeans = new ArrayList();

		public LoggingBean(/*IReceivingBean iListener*/)
			: base("LoggingBean")
		{
//			logListener = iListener;
		}

		public override void Log(LogLevel iLevel, string iMsg)
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

		public ILogger getLogger()
		{
			return this;
		}

		public void setLogger(ILogger iLogger)
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
