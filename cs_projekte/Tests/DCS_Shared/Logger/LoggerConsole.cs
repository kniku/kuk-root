using System;
using System.Linq;
using System.Collections;

namespace DCS.Logger
{
	public class LoggerImpl_ConsoleColored : AbstractLogger
	{
		public LoggerImpl_ConsoleColored()
			: base("ConsoleColored")
		{
		}

		protected override void Log(LogLevel iLevel, string iMsg)
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
//			System.Console.Write("[" + getLogLevelStr(iLevel) + "] " + iMsg);
			System.Console.WriteLine(iMsg);
			if (newColor != 0) System.Console.ForegroundColor = startupColor;
		}
	}

}
