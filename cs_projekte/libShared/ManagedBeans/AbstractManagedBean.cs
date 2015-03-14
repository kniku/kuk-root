using System;
using System.Collections;
using System.Linq;
using System.Text;
using knk.shared.logger;

namespace knk.shared.ManagedBeans
{
	public abstract class AbstractManagedBean : IManagedBean
	{
		BeanState mState = BeanState.none;
		protected ArrayList listReceivingBeans = new ArrayList();
		protected knk.shared.logger.ILogger logger;

		/// <summary>
		/// Ruft handleData aller registriereten IReceivingBeans
		/// </summary>
		/// <param name="iData">Die zu sendenden Daten</param>
		protected void sendDataToAllReceivers(params object[] iData)
		{
			foreach (IReceivingBean aBean in listReceivingBeans)
			{
				aBean.handleData(this, iData);
			}
		}

		#region IManagedBean Member

		public virtual BeanState getState()
		{
			return mState;
		}

		public abstract string getName();
		public abstract string getDescription();

		public virtual void init()
		{
			logger.pushPrefix(getName()).LogDebug(": init...").popPrefix();
			mState = BeanState.initialized;
		}

		public virtual void start()
		{
			logger.pushPrefix(getName()).LogDebug(": start...").popPrefix();
			mState = BeanState.running;
		}

		public virtual void stop()
		{
			logger.pushPrefix(getName()).LogDebug(": stop...").popPrefix();
			mState = BeanState.stopped;
		}

		public virtual void done()
		{
			mState = BeanState.none;
		}

		public void addReceivingBean(IReceivingBean iBean)
		{
			listReceivingBeans.Add(iBean);
		}

		public void setLogger(ILogger iLogger)
		{
			logger = iLogger;
		}

		public ILogger getLogger()
		{
			return logger;
		}

		#endregion

		public abstract void beanRun();
	}
}
