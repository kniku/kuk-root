using System;
using System.ComponentModel;
using System.Windows;
using Knk.Base.Logging;
using Knk.GuiWPF;
using Knk.GuiWPF.DynamicWindows;
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
        static ILog Logger = LogFactory.GetLogger(typeof(MainWindow));
        
        public MainWindow()
        {
            InitializeComponent();
            Logger.Debug("Startup");
        }

        class CData
        {
            public string Name { get; set; }
            public int Nummer { get; set; }
            public string Test { get; set; }
        }
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            ILogViewWindow log = Factory.ShowWindowLogger(this);
            log.Error("ERROR....");
            log.Warn("Warning....");

            Logger.Error("log4net");

            CData data = new CData {Name = "Kurt", Nummer = 10};
            SimplePropertyWindow w = new SimplePropertyWindow("TEST", data);
            w.ShowDialog();
        }

        void worker_DoWork(object sender, DoWorkEventArgs e)
        {
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
        private void BProgress_OnClick(object sender, RoutedEventArgs e)
        {
            WndProgress xxx = new WndProgress(this);
            xxx.AddWorker(worker_DoWork, "worker 1:", true, 500);
            xxx.AddWorker(worker_DoWork, "worker 2:", false, 700);
            xxx.AddWorker(worker_DoWork, "worker 3:", true, 300);
            xxx.AddWorker(worker_DoWork, "worker 4:", true, 400);
            xxx.AddWorker(worker_DoWork, "worker 5:", true, 800);
            xxx.StartAllTasks(false);
        }
    }
}
