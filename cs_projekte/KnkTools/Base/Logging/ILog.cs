using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;

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
        void Debug(object message, Exception ex = null);
        void Info(object message, Exception ex = null);
        void Warn(object message, Exception ex = null);
        void Error(object message, Exception ex = null);
        void Fatal(object message, Exception ex = null);
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
