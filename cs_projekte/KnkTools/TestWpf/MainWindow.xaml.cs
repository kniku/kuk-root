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
    }
}
