using System;

namespace Knk.Base.Logging
{
    public enum Level
    {
        All,
        Debug,
        Info,
        Warn,
        Error,
        Fatal,
        Off
    }

    public interface ILog
    {
        /* Test if a level is enabled for logging */
        bool IsDebugEnabled { get; }
        bool IsInfoEnabled { get; }
        bool IsWarnEnabled { get; }
        bool IsErrorEnabled { get; }
        bool IsFatalEnabled { get; }
        
        /* Log a message */
        void Debug(string format, params object [] parameters);
        void Info(string format, params object [] parameters);
        void Warn(string format, params object [] parameters);
        void Error(string format, params object [] parameters);
        void Fatal(string format, params object [] parameters);
    }

    public interface ILogItem
    {
        object Message { get; }
        Level Level { get; }
    }

    public interface ILogItemProvider
    {
        ILogItem [] PopAllEvents();
    }
}
