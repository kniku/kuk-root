﻿namespace DCS.ManagedBeans.Implementations
{
	class MB_dummyThreaded : AbstractManagedThreadedBean
	{

		public override string getName()
		{
			return "Dummy 2";
		}

		public override string getDescription()
		{
			return "Test (AbstractManagedThreadedBean)";
		}

		protected override int getTimeoutMS()
		{
			return 1;
		}

		public override void beanRun()
		{
			logger.pushPrefix(getName()).LogDebug(": beanRun...\n").popPrefix();
//			doHandleData(getName() + ": beanRun...");
		}
	}
}
