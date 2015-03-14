using System;

namespace knk.shared.logger
{
	public static class Generator
	{
		public static ILogger createLogger(LogProvider iProvider)
		{
			switch (iProvider)
			{
				default:
				case LogProvider.None:
					return new impl.None();
				case LogProvider.File:
					string x = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData);
					x += "/" + System.Diagnostics.Process.GetCurrentProcess().MainModule.ModuleName + ".log";
					return new impl.RollingFile(x);
				case LogProvider.Console:
					return new impl.ConsoleColored();
			}
		}
	}
}
