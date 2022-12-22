using Knk.Base.Logging;
using Knk.GuiWPF;
using System;
using System.IO;
using System.Windows;
using System.Windows.Shapes;

namespace DirectoryWatcher
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            tbDirName.Text = @"c:\temp";
            tbIgnorePattern.Text = "...";
        }

        private Knk.Base.File.DirectoryWatcher watcher;
        private ILog Logger;

        void openLogger()
        {
            var LogWindow = Factory.ShowWindowLogger(this);
            LogWindow.AttachToBaseLogging();
            Logger = LogWindow;
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            System.Diagnostics.Process.Start(tbDirName.Text);
        }

        private void cbWatch_Checked(object sender, RoutedEventArgs e)
        {
            openLogger();

            Logger.Warn($"Watching: {tbDirName.Text}...");
            int len_dir = tbDirName.Text.Length;

            try
            {
                watcher?.Dispose();
                watcher = new Knk.Base.File.DirectoryWatcher(tbDirName.Text, true, (cbRecurse.IsChecked != null && cbRecurse.IsChecked.Value));
                string IgnorePattern = tbIgnorePattern.Text;

                watcher.ErrorReceived += (s, args) =>
                {
                    Logger.Fatal($"Error received: {args.GetException()?.Message}");
                };

                watcher.WatchedElementChanged += (o, args) =>
                {
                    string path = args.FullPath.Substring(len_dir + 1);
                    string path_old = args.FullPathOld?.Substring(len_dir + 1);

                    tbIgnorePattern.Dispatcher.Invoke(() =>
                    {
                        if (tbIgnorePattern.Text != IgnorePattern)
                        {
                            IgnorePattern = tbIgnorePattern.Text;
                            Logger.Warn($"Ignoring: [{IgnorePattern}]");
                        }
                    });

                    if (!string.IsNullOrEmpty(IgnorePattern))
                    {
                        foreach (string token in IgnorePattern.Split(','))
                        {
                            if (path.ToUpper().Contains(token.ToUpper()))
                                return;
                        }
                    }

                    switch (args.ChangeType)
                    {
                        case WatcherChangeTypes.Changed:
                        case WatcherChangeTypes.Created:
                        case WatcherChangeTypes.Deleted:
                            Logger.Info($"{args.ChangeType}: [{path}]");
                            break;
                        case WatcherChangeTypes.Renamed:
                            Logger.Info($"{args.ChangeType}: [{path_old}] => [{path}]");
                            break;
                        default:
                            Logger.Info($"?????: [{path_old}] => [{path}]");
                            break;
                    }
                };
                watcher.Start();
                cbWatch.Content = "watching...";
            }
            catch (Exception exception)
            {
                Logger.Error(exception.Message);
            }
        }

        private void CbWatch_OnUnchecked(object sender, RoutedEventArgs e)
        {
            watcher?.Dispose();
            watcher = null;
            
            openLogger();
            Logger.Warn($"Watching: {tbDirName.Text} stopped...");
            cbWatch.Content = "Start";
        }
    }
}
