using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace knk.shared.ManagedBeans
{
	public abstract class AbstractManagedThreadedBean : AbstractManagedBean
	{
		Thread myThread = null;
//		int sleeptime;

		/// <summary>
		/// Anfrage für Sleeptime im run
		/// </summary>
		/// <returns>Sleeptime (in Millisekunden)</returns>
		protected abstract int getTimeoutMS();

		void run()
		{
			while (true)
			{
				int sleeptime = getTimeoutMS();
				if (sleeptime < 5) sleeptime = 5;

				System.Threading.Thread.Sleep(sleeptime);
				beanRun();
			}
		}

		public override void start()
		{
			base.start();
			
//			sleeptime = getTimeoutMS();
//			if (sleeptime < 5) sleeptime = 5;

			myThread = new Thread(new ThreadStart(run));
			myThread.Start();
		}

		public override void stop()
		{
			base.stop();
			if (myThread != null)
			{
				myThread.Abort();
			}
			myThread = null;
		}

	}
}
