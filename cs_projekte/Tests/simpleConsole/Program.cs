using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using knk.shared;
using knk.shared.tools;
using knk.shared.logger;
using knk.shared.ManagedBeans;
using log4net;
using log4net.Config;

namespace simpleConsole
{
	public class AppGlobal : AppGlobalSimple
	{
	}

	class Program
	{
		private static readonly ILog log = LogManager.GetLogger(typeof(Program));

		static void Main(string[] args)
		{
			BasicConfigurator.Configure();

			IAppGlobal AP = AppGlobalSimple.createInstance("simpleConsole.AppGlobal");
			AP.setLogger(knk.shared.logger.Generator.createLogger(LogProvider.Console));
			//AP.getLogger().LogAlways("hello world");
			log.Error("hello world");

			AP.getLogger().LogAlways("mem usage: {0}", GC.GetTotalMemory(true));

			BeanManager BM = BeanManager.createInstance();
			BM.setBeanLogger(AP.getLogger());
			IManagedBean bean = new MB_dummyThreaded();
			BM.addBean(bean);
			BM.start();

			System.Threading.Thread.Sleep(1000);
			BM.stop();
			AP.getLogger().LogAlways("mem usage: {0}", GC.GetTotalMemory(true));

		}
	}
}
