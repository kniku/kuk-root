using System;

namespace knk.shared.logger.impl
{
	public class ConsoleColored : AbstractLogger
	{
		public ConsoleColored()
			: base("ConsoleColored")
		{
		}

		public override void Log(LogLevel iLevel, string iMsg)
		{
			ConsoleColor startupColor = 0, newColor = 0;
			switch (iLevel)
			{
				case LogLevel.Error:
					newColor = ConsoleColor.Red;
					break;
				case LogLevel.Warning:
					newColor = ConsoleColor.Yellow;
					break;
				case LogLevel.Info:
					newColor = ConsoleColor.White;
					break;
			}
			if (newColor != 0)
			{
				startupColor = System.Console.ForegroundColor;
				System.Console.ForegroundColor = newColor;
			}
			System.Console.WriteLine("{0}: {1}", DateTime.Now, iMsg);
			if (newColor != 0) System.Console.ForegroundColor = startupColor;
		}
	}
}
