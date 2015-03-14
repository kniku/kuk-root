using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DCS_Service
{
	class Program
	{
		static void Main(string[] args)
		{
			System.ServiceProcess.ServiceBase.Run(new DCS_Service());
		}
	}
}
