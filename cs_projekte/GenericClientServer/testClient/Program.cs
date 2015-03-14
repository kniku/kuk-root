using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using log4net;
using log4net.Config;
using log4net.Appender;
using log4net.Repository.Hierarchy;

namespace testClient
{
	static class Program
	{
		public static MemoryAppender LogMemAppender;
		static readonly ILog log = LogManager.GetLogger(typeof(Program));

		/// <summary>
		/// Der Haupteinstiegspunkt für die Anwendung.
		/// </summary>
		[STAThread]
		static void Main()
		{
			//BasicConfigurator.Configure();
			log4net.GlobalContext.Properties["LogFileName"] = "tcp-client.log";
			XmlConfigurator.Configure(new System.IO.FileInfo("log4net-config.xml"));

			LogMemAppender = new MemoryAppender();
			LogMemAppender.ActivateOptions();
			Hierarchy hierarchy = (Hierarchy)LogManager.GetRepository();
			hierarchy.Root.AddAppender(LogMemAppender);

			log.Info("starting tcp client");

			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new FormMain());
		}
	}
}
