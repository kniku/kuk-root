using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Windows;

namespace WpfOnlyApp
{
	/// <summary>
	/// Interaktionslogik für "App.xaml"
	/// </summary>
	public partial class App : Application
	{
		private static readonly log4net.ILog log = log4net.LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);

		protected override void OnStartup(StartupEventArgs e)
		{
			log.Info(@"
=========================================================
				Application startup
=========================================================");
			base.OnStartup(e);
		}
	}
}
