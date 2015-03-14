using System;

namespace DCS.Logger
{
	public enum LogLevel { Debug, Info, Warning, Error };

	public interface ILogger
	{
		ILogger LogDebug(string iFormat, params Object[] iArgs);
		ILogger LogInfo(string iFormat, params Object[] iArgs);
		ILogger LogWarning(string iFormat, params Object[] iArgs);
		ILogger LogError(string iFormat, params Object[] iArgs);
		ILogger LogAlways(string iFormat, params Object[] iArgs);
		ILogger setLogLevel(LogLevel iLevel);
		bool isLevelEnabled(LogLevel iLevel);
		ILogger pushPrefix(string iPrefix);
		ILogger popPrefix();
	}
}
