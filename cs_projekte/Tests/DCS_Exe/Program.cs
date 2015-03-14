using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DCS_Exe
{
	class Program
	{
		static void Main(string[] args)
		{
			AppGlobal.createInstance();
			DCS.ManagedBeans.BeanManager.createInstance().start();
		}

	}
}
