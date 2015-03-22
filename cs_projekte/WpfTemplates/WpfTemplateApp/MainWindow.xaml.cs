using MahApps.Metro.Controls;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using MahApps.Metro.Controls.Dialogs;

namespace WpfTemplateApp
{
	/// <summary>
	/// Interaktionslogik für MainWindow.xaml
	/// </summary>
	public partial class MainWindow : MetroWindow
	{
		private static readonly log4net.ILog log = log4net.LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);

		public MainWindow()
		{
			InitializeComponent();
			log.Error("MainWindow initialized");
		}

		private void mcClose(object sender, RoutedEventArgs e)
		{
			Close();
		}
		private void mcAbout(object sender, RoutedEventArgs e)
		{
			this.ShowMessageAsync("WpfTemplateApp", "Einfaches Anwendungsgerüst einer Wpf Applikation im Metro-Stil...");
		}
	}
}
