using System;
using System.Diagnostics;
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

        private void WndClosed_Closed(object sender, EventArgs e)
        {
            ((ILogViewWindow)this).DetachFromBaseLogging();
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
            string fn = System.IO.Path.GetTempPath() + @"\LogList.txt";

            using (System.IO.StreamWriter file = new System.IO.StreamWriter(fn))
            {
                foreach (ListBoxItem logListItem in LogList.Items)
                {
                    file.WriteLine(logListItem.Content.ToString());
                }
            }

            Process P = new Process();
            P.StartInfo.FileName = fn;
            P.Start();

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
        private DispatcherTimer LogDispatcherTimer;
        
        void ILogViewWindow.AttachToBaseLogging()
        {
            if (LogDispatcher == null)
            {
                LogDispatcher = LogFactory.RegisterLogDispatcher();
                LogDispatcherTimer = new DispatcherTimer();
                LogDispatcherTimer.Tick += dispatcherTimer_Tick;
                LogDispatcherTimer.Interval = new TimeSpan(0, 0, 0, 0, 200);
                LogDispatcherTimer.Start();
            }
        }

        void ILogViewWindow.DetachFromBaseLogging()
        {
            if (LogDispatcher != null)
            {
                LogDispatcherTimer.Stop();
                LogFactory.UnregisterLogDispatcher(LogDispatcher);
                LogDispatcherTimer = null;
                LogDispatcher = null;
            }
        }

        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {
            if (LogDispatcher == null)
                return;

            foreach (ILogItem item in LogDispatcher.PopAllEvents())
            {
                Log(item.Level, item.Message);
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
        void AttachToBaseLogging();
        void DetachFromBaseLogging();
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
