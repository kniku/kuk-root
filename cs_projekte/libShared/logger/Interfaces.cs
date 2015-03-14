using System;
using knk.shared.tools;

namespace knk.shared.logger
{
	public enum LogLevel { None, [StringValue("Debug")] Debug, [StringValue("Info")] Info, [StringValue("Warning")] Warning, [StringValue("Error")] Error };
	public enum LogProvider { [StringValue("none")] None, [StringValue("console")] Console, [StringValue("file")] File };

	public interface ILogger
	{
		ILogger LogDebug(string iFormat, params Object[] iArgs);
		ILogger LogInfo(string iFormat, params Object[] iArgs);
		ILogger LogWarning(string iFormat, params Object[] iArgs);
		ILogger LogError(string iFormat, params Object[] iArgs);
		ILogger LogAlways(string iFormat, params Object[] iArgs);
		ILogger Log(LogLevel iLevel, string iFormat, params Object[] iArgs);
		// Log Level Support
		bool isLevelEnabled(LogLevel iLevel);
		LogLevel getLogLevel();
		string getLogLevelAsString(LogLevel iLevel);
		LogLevel getLogLevelFromString(string iLevel);
		ILogger setLogLevel(LogLevel iLevel);
		// Prefix Support
		ILogger pushPrefix(string iPrefix);
		ILogger popPrefix();
	}
}
