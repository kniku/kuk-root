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
    public static class LogManager
    {
        private static bool initialized;
        public static ILog GetLogger(Type type)
        {
            if (type == null)
                return new LoggerDevConsole();

            if (!initialized)
            {
                var executingAssembly = Assembly.GetEntryAssembly();
                XmlConfigurator.Configure(log4net.LogManager.GetRepository(executingAssembly),
                    new System.IO.FileInfo($"{executingAssembly.ManifestModule.Name}.log4net"));
                initialized = true;
            }

            return new LoggerLog4NetWrapper(log4net.LogManager.GetLogger(type));
        }

        class log4netLogItem : ILogItem
        {
            public object message;
            public Level level;

            object ILogItem.Message => message;
            Level ILogItem.Level => level;

        }

        static Level log4netLevel2Level(log4net.Core.Level l)
        {
            if (l.Value == log4net.Core.Level.Info.Value)
                return Level.Info;
            if (l.Value == log4net.Core.Level.Warn.Value)
                return Level.Warn;
            if (l.Value == log4net.Core.Level.Error.Value)
                return Level.Error;
            if (l.Value == log4net.Core.Level.Fatal.Value)
                return Level.Fatal;

            return Level.Debug;
        }
        class log4netDispatcher : MemoryAppender, ILogItemProvider
        {
            ILogItem[] ILogItemProvider.PopAllEvents()
            {
                var list = PopAllEvents();
                ILogItem [] r = new ILogItem[list.Length];
                for (int i = 0; i < list.Length; i++)
                {
                    r[i] = new log4netLogItem {message = list[i].MessageObject.ToString(), level = log4netLevel2Level(list[i].Level)};
                }

                return r;
            }
        }

        public static ILogItemProvider RegisterLogDispatcher()
        {
            Hierarchy hierarchy = (Hierarchy)log4net.LogManager.GetRepository(Assembly.GetEntryAssembly());

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

        public static void UnregisterLogDispatcher(ILogItemProvider dispatcher)
        {
            if (dispatcher is log4netDispatcher d)
            {
                d.Clear();
                Hierarchy hierarchy = (Hierarchy) log4net.LogManager.GetRepository(Assembly.GetEntryAssembly());
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

            void ILog.Debug(string format, params object[] parameter)
            {
                if (IsDebugEnabled)
                {
                    nativeLogger.DebugFormat(format, parameter);
                }
            }

            void ILog.Info(string format, params object[] parameter)
            {
                if (IsInfoEnabled)
                {
                    nativeLogger.InfoFormat(format, parameter);
                }
            }

            void ILog.Warn(string format, params object[] parameter)
            {
                if (IsWarnEnabled)
                {
                    nativeLogger.WarnFormat(format, parameter);
                }
            }

            void ILog.Error(string format, params object[] parameter)
            {
                if (IsErrorEnabled)
                {
                    nativeLogger.ErrorFormat(format, parameter);
                }
            }

            void ILog.Fatal(string format, params object[] parameter)
            {
                if (IsFatalEnabled)
                {
                    nativeLogger.FatalFormat(format, parameter);
                }
            }
        }


        class LoggerDevConsole : ILog
        {
            public bool IsDebugEnabled => true;
            public bool IsInfoEnabled => true;
            public bool IsWarnEnabled => true;
            public bool IsErrorEnabled => true;
            public bool IsFatalEnabled => true;

            void ILog.Debug(string format, params object [] parameter)
            {
                if (IsDebugEnabled)
                    System.Diagnostics.Debug.WriteLine(string.Format(format, parameter));
            }

            void ILog.Info(string format, params object [] parameter)
            {
                if (IsInfoEnabled)
                    System.Diagnostics.Debug.WriteLine(string.Format(format, parameter));
            }
            
            void ILog.Warn(string format, params object [] parameter)
            {
                if (IsWarnEnabled)
                    System.Diagnostics.Debug.WriteLine(string.Format(format, parameter));
            }
            
            void ILog.Error(string format, params object [] parameter)
            {
                if (IsErrorEnabled)
                    System.Diagnostics.Debug.WriteLine(string.Format(format, parameter));
            }

            void ILog.Fatal(string format, params object [] parameter)
            {
                if (IsFatalEnabled)
                    System.Diagnostics.Debug.WriteLine(string.Format(format, parameter));
            }
        }
    }
}
