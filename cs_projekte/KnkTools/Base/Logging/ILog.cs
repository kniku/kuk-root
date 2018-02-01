﻿using System;
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
        void Debug(string message, Exception ex = null);
        void Info(string message, Exception ex = null);
        void Warn(string message, Exception ex = null);
        void Error(string message, Exception ex = null);
        void Fatal(string message, Exception ex = null);
    }

    public interface ILogItem
    {
        string GetMessage();
    }

    public interface ILogDispatcher
    {
        ILogItem [] PopAllEvents();
    }
}
