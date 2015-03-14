using System;
using System.Configuration.Install;
using System.ComponentModel;
using System.ServiceProcess;
using System.IO;
using System.Net.Sockets;
using System.Net;
using System.Threading;

namespace DCS_Service
{

	[RunInstaller(true)]
	public class MyInstaller : Installer
	{
		private ServiceInstaller m_ThisService;
		private ServiceProcessInstaller m_ThisServiceProcess;

		public MyInstaller()
		{
			m_ThisService = new ServiceInstaller();
			m_ThisServiceProcess = new ServiceProcessInstaller();

			m_ThisServiceProcess.Account = ServiceAccount.NetworkService;
			m_ThisService.ServiceName = "RW Data Collector";
			m_ThisService.StartType = ServiceStartMode.Manual;

			Installers.Add(m_ThisService);
			Installers.Add(m_ThisServiceProcess);
		}
	}

	public class DCS_Service : ServiceBase
	{
		protected override void OnStart(string[] args)
		{
			DCS.ManagedBeans.BeanManager.createInstance().start();
		}

		protected override void OnStop()
		{
			DCS.ManagedBeans.BeanManager.createInstance().stop();
		}
	}
}