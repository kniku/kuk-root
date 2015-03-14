using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DCS.ManagedBeans.Implementations
{
	class DB_dummy : IReceivingBean
	{
		static volatile int counter;
		int nr;

		#region IReceivingBean Member

		public DB_dummy()
		{
			counter++;
			nr = counter;
		}

		public void handleData(ICallingBean iCaller, params object[] iArgs)
		{
			string msg = iArgs[0] as string;

			if (msg != null)
			{
				iCaller.getLogger().pushPrefix("DB_dummy(" + nr + ")").LogDebug(": handleData(" + msg + ")\n").popPrefix();
			}
		}

		#endregion
	}
}
