using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Globalization;
using System.Linq;
using System.Threading;
using System.Windows;
using Knk.Base.File;
using Knk.Base.Logging;
using Knk.Base.Threading;
using Knk.GuiWPF;
using NLog.Targets;
using ILog = Knk.Base.Logging.ILog;

//[assembly: log4net.Config.XmlConfigurator(ConfigFileExtension = "log4net", Watch = true)]

namespace TestWpf
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        //private static readonly log4net.ILog Log = LogManager.GetLogger(typeof(MainWindow));
        static ILog Logger = LogManager.GetLogger(typeof(MainWindow));
        private static NLog.ILogger nLogger = NLog.LogManager.GetCurrentClassLogger();
        
        public MainWindow()
        {
            InitializeComponent();
            Logger.Debug("Startup");
            nLogger.Debug("TEST");
        }

        class CData
        {
            public string Name { get; set; }
            public int Nummer { get; set; }
            public string Test { get; set; }
        }

        private ILogViewWindow LogWindow;
        void openLoggerWindow()
        {
            LogWindow = Factory.ShowWindowLogger(this);
            LogWindow.AttachToBaseLogging();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            NLog.Targets.MemoryTarget target = new MemoryTarget("MemTarget");
            NLog.LogManager.Configuration.AddTarget("MemAppender", target);

            foreach (var t in NLog.LogManager.Configuration.AllTargets)
            {
                Logger.Info($"target: {t.Name}, {t.GetType().Name}");
            }

            nLogger.Debug(@"Button_Click
foreach (var performanceCounterCategory in PerformanceCounterCategory.GetCategories().OrderBy(p => p.CategoryName)/*.Where(p=>p.CategoryName==""Physikalischer Datenträger"")*/)");

            var logs = NLog.LogManager.Configuration.FindTargetByName<MemoryTarget>("MemAppender").Logs;
            foreach (var l in logs)
            {
                Logger.Info(l);
            }

            openLoggerWindow();
            //ILogViewWindow log = Factory.ShowWindowLogger(this);
            //log.AttachToBaseLogging();

            Logger.Error("ERROR....");
            Logger.Warn("Warning....");

            Logger.Error("log4net");

            //CData data = new CData {Name = "Kurt", Nummer = 10};
            //SimplePropertyWindow w = new SimplePropertyWindow("TEST", data);
            //w.ShowDialog();

            LogWindow.Info($"RAM free : {Knk.Base.Framework.Diagnostic.ManagementObject.GetFreePhysicalMemory}");
            LogWindow.Info($"RAM total: {Knk.Base.Framework.Diagnostic.ManagementObject.GetTotalRamSize}");


            if (_perCpu == null && false)
            {

                Thread.CurrentThread.CurrentCulture = CultureInfo.InvariantCulture;

                LogWindow.Info("Categories:");
                foreach (var performanceCounterCategory in PerformanceCounterCategory.GetCategories().OrderBy(p => p.CategoryName))
                {
                    LogWindow.Warn($"  [{performanceCounterCategory.CategoryName}]");
                }

                foreach (var performanceCounterCategory in PerformanceCounterCategory.GetCategories().OrderBy(p => p.CategoryName)/*.Where(p=>p.CategoryName=="Physikalischer Datenträger")*/)
                {
                    if (!performanceCounterCategory.CategoryName.Contains("xNetwork Interface"))
                        continue;

                    LogWindow.Warn($"    {performanceCounterCategory.CategoryName}");

                    if (performanceCounterCategory.CategoryType == PerformanceCounterCategoryType.MultiInstance)
                    {
                        foreach (var instanceName in performanceCounterCategory.GetInstanceNames())
                        {
                            foreach (var counter in performanceCounterCategory.GetCounters(instanceName))
                            {
                                LogWindow.Debug($"    .... [{performanceCounterCategory.CategoryName}]/[{instanceName}]: [{counter.CounterName}]");
                            }
                        }
                    }
                    else
                    {
                        foreach (var counter in performanceCounterCategory.GetCounters())
                        {
                            LogWindow.Debug($"    .... [{performanceCounterCategory.CategoryName}]/- : [{counter.CounterName}]");
                        }
                    }

                }

                _perCpu = new PerformanceCounter("Processor", "% Processor Time","_Total");
                _perMem = new PerformanceCounter("Memory", "Available MBytes");
                _perCpuProc = new PerformanceCounter("Process", "% Processor Time", Process.GetCurrentProcess().ProcessName);
                _perMemProc = new PerformanceCounter("Network Interface", "Bytes Sent/sec", "Intel[R] Ethernet Connection [2] I219-LM _2");
            }

            if (_perCpu != null)
            {
                LogWindow.Info($"CPU: {_perCpu.NextValue()}");
                LogWindow.Info($"MEM: {_perMem.NextValue() * 1024}");
                LogWindow.Info($"CPU(process): {_perCpuProc.NextValue()}");
                LogWindow.Info($"NET: {_perMemProc.NextValue()}");
            }

            //watcher?.Dispose();
            //watcher = new DirectoryWatcher(@"c:\temp", false, false);
            //watcher.WatchedElementChanged += (o, args) => { Logger.Info($"file {args.FullPath}: {args.ChangeType} (old: {args.FullPathOld})"); };
            //watcher.Start();

            MyActivity activity = new MyActivity("TEST");
            activity.Start();
            Thread.Sleep(1500);
            LogWindow.Info($"elapsed(ms): {activity.ElapsedMilliseconds()}");
            Thread.Sleep(500);
            activity.Stop();
            LogWindow.Info($"elapsed(ms): {activity.ElapsedMilliseconds()}");

            using (var act = new MyActivity("TEST2", true))
            {
                Thread.Sleep(100);
            }
            using (var act = new MyActivity("TEST", true))
            {
                using (var act2 = new MyActivity("TEST", true))
                {
                    Thread.Sleep(50);
                }
                Thread.Sleep(25);
            }

            foreach (var act in MonitoredActivityManager.GetMonitoredActivities())
            {
                LogWindow.Info($"Activity [{act.Name}]: Events={act.CountEvents}, Total(ms)={act.TotalElapsedMilliseconds}");
            }


        }

        public interface IMonitoredActivity
        {
            string Name { get; }
            long CountEvents { get; }
            double TotalElapsedMilliseconds { get; }
        }
        static class MonitoredActivityManager
        {
            class MonitoredActivityAggregator : IMonitoredActivity
            {

                public MonitoredActivityAggregator(string iName)
                {
                    Name = iName;
                }

                public string Name { get; }
                public long CountEvents { get; private set; }
                public double TotalElapsedMilliseconds { get; private set; }

                public void RegisterActivityEvent(MonitoredActivity iActivity)
                {
                    CountEvents++;
                    TotalElapsedMilliseconds += iActivity.ElapsedMilliseconds();
                }
            }

            // TODO: singleton
            static Dictionary<string, MonitoredActivityAggregator> dict = new Dictionary<string, MonitoredActivityAggregator>();

            static MonitoredActivityAggregator GetActivityAggregator(string iName)
            {
                if (!dict.ContainsKey(iName))
                {
                    var mon = new MonitoredActivityAggregator(iName);
                    dict.Add(iName, mon);
                    return mon;
                }

                return dict[iName];
            }

            public static IEnumerable<IMonitoredActivity> GetMonitoredActivities()
            {
                return dict.Values;
            }

            public static void RegisterActivity(MonitoredActivity iActivity)
            {
                var monitor = GetActivityAggregator(iActivity.Name);
                monitor.RegisterActivityEvent(iActivity);
            }
        }


        public class MyActivity : MonitoredActivity
        {
            public MyActivity(string iName, bool iStart = false) : base(iName, iStart)
            {
            }
        }

        public abstract class MonitoredActivity : IDisposable
        {
            private Stopwatch stopwatch;

            public string Name { get; }

            public MonitoredActivity(string iName, bool iStart = false)
            {
                Name = iName;

                if (iStart)
                    Start();
            }
            public void Start()
            {
                if (stopwatch == null)
                {
                    stopwatch = Stopwatch.StartNew();
                }
            }
            public void Stop()
            {
                if (stopwatch != null && stopwatch.IsRunning)
                {
                    stopwatch.Stop();
                    MonitoredActivityManager.RegisterActivity(this);
                }
            }

            public long ElapsedMilliseconds()
            {
                return stopwatch?.ElapsedMilliseconds ?? 0;
            }

            #region IDisposable Support
            private bool disposedValue = false; // To detect redundant calls

            protected virtual void Dispose(bool disposing)
            {
                if (!disposedValue)
                {
                    if (disposing)
                    {
                        Stop();
                    }
                    disposedValue = true;
                }
            }

            // This code added to correctly implement the disposable pattern.
            void IDisposable.Dispose()
            {
                Dispose(true);
                GC.SuppressFinalize(this);
            }
            #endregion
        }


        //private Knk.Base.File.DirectoryWatcher watcher;

        PerformanceCounter _perCpu, _perMem, _perCpuProc, _perMemProc;

        void worker_DoWork(object sender, DoWorkEventArgs e)
        {
            int xxx = (int) e.Argument;
            Logger.Warn($"Thread #{Thread.CurrentThread.ManagedThreadId}-START, arg={xxx}, supended={s1.Suspended}");
            if (xxx > 500)
            {
                //s1.Suspended = true;
            }
            Thread.Sleep(new Random().Next(2000));
            if ((xxx > 500 && !s1.Suspended) || (xxx <= 500 && s1.Suspended))
                Logger.Error($"Thread #{Thread.CurrentThread.ManagedThreadId}-END, arg={xxx}, supended={s1.Suspended}");
            else
                Logger.Warn($"Thread #{Thread.CurrentThread.ManagedThreadId}-END, arg={xxx}, supended={s1.Suspended}");
            return;

            int max = (int)e.Argument;
            int result = 0;
            for (int i = 0; i < max; i++)
            {
                // Auf Abbruch checken....
                if ((sender as BackgroundWorker).CancellationPending && max != 400)
                {
                    break;
                }


                int progressPercentage = Convert.ToInt32(((double)i / max) * 100);
                if (i % 42 == 0)
                {
                    result++;
                    //(sender as BackgroundWorker).ReportProgress(progressPercentage, i);
                    (sender as BackgroundWorker).ReportProgress(progressPercentage, "TEST " + i + ":");
                }
                else
                if (progressPercentage % 2 == 0) (sender as BackgroundWorker).ReportProgress(progressPercentage);
                System.Threading.Thread.Sleep(10);

            }
            e.Result = result;
        }

        Suspender s1,s2;

        private void BProgress_OnClick(object sender, RoutedEventArgs e)
        {
            openLoggerWindow();

            using (s1 = new Suspender())
            {
                Logger.Warn($"Thread #{Thread.CurrentThread.ManagedThreadId}-INIT, suspended={s1.Suspended}");
                s2 = new Suspender(false);
                WndProgress xxx = new WndProgress(this);
                xxx.AddWorker(worker_DoWork, "worker 1:", true, 500);
                xxx.AddWorker(worker_DoWork, "worker 2:", false, 700);
                xxx.AddWorker(worker_DoWork, "worker 3:", true, 300);
                xxx.AddWorker(worker_DoWork, "worker 4:", true, 400);
                xxx.AddWorker(worker_DoWork, "worker 5:", true, 800);
                xxx.StartAllTasks(false);
            }
            Logger.Warn($"Thread #{Thread.CurrentThread.ManagedThreadId}-DONE, suspended={s1.Suspended}");

            //WndProgressManager manager = new WndProgressManager(this, false);
            //manager.AddTask(worker_DoWork, "worker 1:", true, 500);
        }
    }
}
