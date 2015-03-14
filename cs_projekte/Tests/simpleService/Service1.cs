using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Configuration.Install;

namespace simpleService
{
	public partial class Service1 : ServiceBase
	{
		public Service1()
		{
			InitializeComponent();
		}

		protected override void OnStart(string[] args)
		{
		}

		protected override void OnStop()
		{
		}
	}

	[RunInstaller(true)]
	public class TestWinInstaller : Installer
	{
		private ServiceInstaller m_ThisService;
		private ServiceProcessInstaller m_ThisServiceProcess;

		public TestWinInstaller()
		{
			m_ThisService = new ServiceInstaller();
			m_ThisServiceProcess = new ServiceProcessInstaller();

			m_ThisServiceProcess.Account = ServiceAccount.NetworkService;
			m_ThisService.ServiceName = "Simple Test Service";
			m_ThisService.Description = "knk's einfaches Testservice";
			m_ThisService.StartType = ServiceStartMode.Manual;

			Installers.Add(m_ThisService);
			Installers.Add(m_ThisServiceProcess);
		}
	}
}
