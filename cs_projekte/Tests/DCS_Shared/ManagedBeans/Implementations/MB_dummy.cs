using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace DCS.ManagedBeans.Implementations
{
	class MB_dummy : AbstractManagedBean
	{
		public override string getName()
		{
			return "Dummy 1";
		}

		public override string getDescription()
		{
			return "Test (AbstractManagedBean)";
		}

		public override void init()
		{
			base.init();
			logger.pushPrefix(getName()).LogDebug(": init...\n").popPrefix();
		}

		public override void start()
		{
			base.start();
			logger.pushPrefix(getName()).LogDebug(": start...\n").popPrefix();
		}

		public override void stop()
		{
			base.stop();
			logger.pushPrefix(getName()).LogDebug(": stop...\n").popPrefix();
		}

		public override void beanRun()
		{
			logger.pushPrefix(getName()).LogDebug(": run...\n").popPrefix();
			sendDataToAllReceivers("hello world");
		}
	}
}
