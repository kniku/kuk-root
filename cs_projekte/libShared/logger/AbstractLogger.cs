using System;
using System.Collections;
using System.Linq;
using knk.shared.tools;

namespace knk.shared.logger
{
	public abstract class AbstractLogger : ILogger
	{
		private string[] lvltxt = { "--- ", "DBG ", "INFO", "WARN", "ERR ", "*   " };
		private string mFacility;
		private LogLevel mLevel;
		protected ArrayList mPrefixes = new ArrayList();

		protected AbstractLogger(string iFacility)
		{
			Facility = iFacility;
			Level = LogLevel.Debug;
		}

		public string Facility
		{
			get { return mFacility; }
			set { mFacility = value; }
		}

		public LogLevel Level
		{
			get { return mLevel; }
			set { mLevel = value; }
		}

		public abstract void Log(LogLevel iLevel, string iMsg);

		public ILogger Log(LogLevel iLevel, string iFormat, params object[] iArgs)
		{
			if (mPrefixes.Count > 0)
			{
				string prefix = "";
				foreach (string aPrefix in mPrefixes)
				{
					prefix = prefix + aPrefix;
				}
				iFormat = prefix + iFormat;
			}
//			iFormat = DateTime.Now.ToShortDateString() + "," + DateTime.Now.ToLongTimeString() + " [" + getLogLevelStr(iLevel) + "] " + iFormat;
			iFormat = "[" + getLogLevelStr(iLevel) + "] " + iFormat;
			Log(iLevel, string.Format(iFormat, iArgs));
			return this;
		}

		protected string getLogLevelStr(LogLevel iLevel)
		{
			iLevel = iLevel > (LogLevel)10 ? (LogLevel)(lvltxt.Count() - 1) : iLevel;
			return lvltxt[(int)iLevel]; ;
		}

		#region interface ILogger implementation

		public string getLogLevelAsString(LogLevel iLevel)
		{
			return StringEnum.GetStringValue(iLevel);
		}

		public LogLevel getLogLevelFromString(string iLevel)
		{
			LogLevel r = LogLevel.None;
			if (StringEnum.IsStringDefined(typeof(LogLevel), iLevel))
			{
				r = (LogLevel)StringEnum.Parse(typeof(LogLevel), iLevel);
			}
			return r;
		}

		public ILogger LogDebug(string iFormat, params Object[] iArgs)
		{
			if (isLevelEnabled(LogLevel.Debug)) Log(LogLevel.Debug, iFormat, iArgs);
			return this;
		}

		public ILogger LogInfo(string iFormat, params Object[] iArgs)
		{
			if (isLevelEnabled(LogLevel.Info)) Log(LogLevel.Info, iFormat, iArgs);
			return this;
		}

		public ILogger LogWarning(string iFormat, params Object[] iArgs)
		{
			if (isLevelEnabled(LogLevel.Warning)) Log(LogLevel.Warning, iFormat, iArgs);
			return this;
		}

		public ILogger LogError(string iFormat, params Object[] iArgs)
		{
			if (isLevelEnabled(LogLevel.Error)) Log(LogLevel.Error, iFormat, iArgs);
			return this;
		}

		public ILogger LogAlways(string iFormat, params Object[] iArgs)
		{
			Log((LogLevel)100, iFormat, iArgs);
			return this;
		}

		public LogLevel getLogLevel()
		{
			return mLevel;
		}

		public ILogger setLogLevel(LogLevel iLevel)
		{
			mLevel = iLevel;
			return this;
		}

		public bool isLevelEnabled(LogLevel iLevel)
		{
			return (iLevel >= Level);
		}

		public ILogger pushPrefix(string iPrefix)
		{
			if (iPrefix != null) mPrefixes.Add(iPrefix);
			return this;
		}

		public ILogger popPrefix()
		{
			if (mPrefixes.Count > 0) mPrefixes.RemoveAt(mPrefixes.Count - 1);
			return this;
		}
		#endregion interface ILogger implementation
	}
}
