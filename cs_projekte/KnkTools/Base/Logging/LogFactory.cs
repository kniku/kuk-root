using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;
using log4net;
using log4net.Appender;
using log4net.Config;
using log4net.Core;
using log4net.Repository.Hierarchy;
using log4net.Util;

namespace Knk.Base.Logging
{
    public static class LogFactory
    {
        private static bool initialized;
        public static ILog GetLogger(Type type)
        {
            if (type == null)
                return new LoggerDevConsole();

            if (!initialized)
            {
                var executingAssembly = Assembly.GetEntryAssembly();
                XmlConfigurator.Configure(LogManager.GetRepository(executingAssembly),
                    new System.IO.FileInfo($"{executingAssembly.ManifestModule.Name}.log4net"));
                initialized = true;
            }

            return new LoggerLog4NetWrapper(log4net.LogManager.GetLogger(type));
        }

        class log4netLogItem : ILogItem
        {
            public string message;
            string ILogItem.GetMessage()
            {
                return message;
            }
        }
        class log4netDispatcher : MemoryAppender, ILogDispatcher
        {
            ILogItem[] ILogDispatcher.PopAllEvents()
            {
                var list = PopAllEvents();
                ILogItem [] r = new ILogItem[list.Length];
                for (int i = 0; i < list.Length; i++)
                {
                    r[i] = new log4netLogItem {message = list[i].MessageObject.ToString()};
                }

                return r;
            }
        }

        public static ILogDispatcher RegisterLogDispatcher()
        {
            Hierarchy hierarchy = (Hierarchy)LogManager.GetRepository(Assembly.GetEntryAssembly());

            // Memory Appender ins Logging einhaengen
            log4netDispatcher LogMemAppender = new log4netDispatcher();

            LogMemAppender.ActivateOptions();
            hierarchy.Root.AddAppender(LogMemAppender);

            //configure the logging at the root.  
            //hierarchy.Root.Level = Level.All;

            //mark repository as configured and  
            //notify that is has changed.  
            hierarchy.Configured = true;
            hierarchy.RaiseConfigurationChanged(EventArgs.Empty);

            return LogMemAppender;
        }

        public static void UnregisterLogDispatcher(ILogDispatcher dispatcher)
        {
            if (dispatcher is log4netDispatcher d)
            {
                d.Clear();
                Hierarchy hierarchy = (Hierarchy) LogManager.GetRepository(Assembly.GetEntryAssembly());
                hierarchy.Root.RemoveAppender(d);
            }
        }

        class LoggerLog4NetWrapper : ILog
        {
            readonly log4net.ILog nativeLogger;
            public LoggerLog4NetWrapper(log4net.ILog log4netLogger)
            {
                nativeLogger = log4netLogger;
            }

            public bool IsDebugEnabled => nativeLogger.IsDebugEnabled;
            public bool IsInfoEnabled => nativeLogger.IsInfoEnabled;
            public bool IsWarnEnabled => nativeLogger.IsWarnEnabled;
            public bool IsErrorEnabled => nativeLogger.IsErrorEnabled;
            public bool IsFatalEnabled => nativeLogger.IsFatalEnabled;

            public void Debug(string message, Exception ex = null)
            {
                if (ex == null)
                    nativeLogger.Debug(message);
                else
                    nativeLogger.DebugExt(message, ex);
            }

            public void Error(string message, Exception ex = null)
            {
                if (ex == null)
                    nativeLogger.Error(message);
                else
                    nativeLogger.ErrorExt(message, ex);
            }

            public void Fatal(string message, Exception ex = null)
            {
                if (ex == null)
                    nativeLogger.Fatal(message);
                else
                    nativeLogger.FatalExt(message, ex);
            }

            public void Info(string message, Exception ex = null)
            {
                if (ex == null)
                    nativeLogger.Info(message);
                else
                    nativeLogger.InfoExt(message, ex);
            }

            public void Warn(string message, Exception ex = null)
            {
                if (ex == null)
                    nativeLogger.Warn(message);
                else
                    nativeLogger.WarnExt(message, ex);
            }
        }


        class LoggerDevConsole : ILog
        {
            public bool IsDebugEnabled => true;
            public bool IsInfoEnabled => true;
            public bool IsWarnEnabled => true;
            public bool IsErrorEnabled => true;
            public bool IsFatalEnabled => true;

            public void Debug(string message, Exception ex = null)
            {
                if (IsDebugEnabled)
                    System.Diagnostics.Debug.WriteLine(message);
            }

            public void Error(string message, Exception ex = null)
            {
                if (IsErrorEnabled)
                    System.Diagnostics.Debug.WriteLine(message);
            }

            public void Fatal(string message, Exception ex = null)
            {
                if (IsFatalEnabled)
                    System.Diagnostics.Debug.WriteLine(message);
            }

            public void Info(string message, Exception ex = null)
            {
                if (IsInfoEnabled)
                    System.Diagnostics.Debug.WriteLine(message);
            }

            public void Warn(string message, Exception ex = null)
            {
                if (IsWarnEnabled)
                    System.Diagnostics.Debug.WriteLine(message);
            }
        }
    }
}
