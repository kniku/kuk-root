using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Threading;
using Knk.Base.Logging;
using ILog = Knk.Base.Logging.ILog;
using Level = Knk.Base.Logging.Level;

namespace Knk.GuiWPF
{
    /// <summary>
    /// Interaction logic for LogViewWindow.xaml
    /// </summary>
    public partial class LogViewWindow : Window, ILogViewWindow
    {
        internal LogViewWindow()
        {
            InitializeComponent();
        }

        private void WndLoaded_Loaded(object sender, RoutedEventArgs e)
        {
            // TODO: attach to log with separate method
            attachToLog();
        }

        private void WndClosed_Closed(object sender, EventArgs e)
        {
            // TODO: dettach from log with separate method
            detachFromLog();
        }

        private void Log(Base.Logging.Level level, string msg)
        {
            if (LogList.Dispatcher.CheckAccess())
                _WriteLog(level, msg);
            else
                LogList.Dispatcher.Invoke(() => { _WriteLog(level, msg); });
        }

        Brush level2color(Base.Logging.Level level)
        {
            Brush r = Brushes.Black;
            switch (level)
            {
                case Level.Debug:
                    r = Brushes.Gray;
                    break;
                case Level.Warn:
                    r = Brushes.Blue;
                    break;
                case Level.Error:
                    r = Brushes.IndianRed;
                    break;
                case Level.Fatal:
                    r = Brushes.Red;
                    break;
            }

            return r;
        }
        void _WriteLog(Base.Logging.Level level, string Text)
        {
            LogList.Items.Add(new ListBoxItem
            {
                Content = $"{DateTime.Now:O}: {Text}",
                Foreground = level != Level.Info ? level2color(level) : Foreground
            });
            LogList.Dispatcher.Invoke(DispatcherPriority.Render, new Action(() => {})); // refresh immediately...
            LogList.ScrollIntoView(LogList.Items[LogList.Items.Count - 1]);
            LogList.SelectedIndex = LogList.Items.Count - 1;
        }

        private void comboLevel_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            // TODO: implement log level filter
        }

        private void btnEditor_Click(object sender, RoutedEventArgs e)
        {
            if (LogDispatcher == null)
                return;

            foreach (ILogItem item in LogDispatcher.PopAllEvents())
            {
                ((ILog) this).Debug(item.GetMessage());
            }
        }

        private void btnClear_Click(object sender, RoutedEventArgs e)
        {
            LogList.Dispatcher.Invoke(() => { LogList.Items.Clear(); });
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private ILogDispatcher LogDispatcher;
        void attachToLog()
        {
            if (LogDispatcher == null)
            {
                LogDispatcher = LogFactory.RegisterLogDispatcher();
            }
        }

        void detachFromLog()
        {
            if (LogDispatcher != null)
            {
                LogFactory.UnregisterLogDispatcher(LogDispatcher);
                LogDispatcher = null;
            }
        }

        #region ILog implementation
        bool ILog.IsDebugEnabled => true;
        bool ILog.IsInfoEnabled => true;
        bool ILog.IsWarnEnabled => true;
        bool ILog.IsErrorEnabled => true;
        bool ILog.IsFatalEnabled => true;

        void ILog.Debug(string message, Exception ex)
        {
            Log(Base.Logging.Level.Debug, message);
        }

        void ILog.Info(string message, Exception ex)
        {
            Log(Base.Logging.Level.Info, message);
        }

        void ILog.Warn(string message, Exception ex)
        {
            Log(Base.Logging.Level.Warn, message);
        }

        void ILog.Error(string message, Exception ex)
        {
            Log(Base.Logging.Level.Error, message);
        }

        void ILog.Fatal(string message, Exception ex)
        {
            Log(Base.Logging.Level.Fatal, message);
        }
        #endregion ILog implementation
    }

    public interface ILogViewWindow : ILog
    {

    }

    public static class Factory
    {
        static LogViewWindow windowLogger;

        public static ILogViewWindow ShowWindowLogger(Window iOwner = null)
        {
            if (windowLogger == null)
            {
                windowLogger = new LogViewWindow();
                if (iOwner != null)
                    windowLogger.Owner = iOwner;

                windowLogger.WindowStartupLocation = WindowStartupLocation.CenterOwner;

                windowLogger.Closed += (sender, args) => { windowLogger = null; };
                windowLogger.Show();
            }

            return windowLogger;
        }
    }
}
