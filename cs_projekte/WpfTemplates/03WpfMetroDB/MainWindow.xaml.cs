using KLib.Sql;
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

namespace _03WpfMetroDB
{
	/// <summary>
	/// Interaktionslogik für MainWindow.xaml
	/// </summary>
	public partial class MainWindow : MetroWindow
	{
		public MainWindow()
		{
			InitializeComponent();

			txtInfo.Text = @"
Aktive Packages:
- log4net: Log Tool
- MahApps:Metro: Metro L&F
- KLib.Sql: SQL Lib";
		}

		private void btnConnect_Click(object sender, RoutedEventArgs e)
		{
			DbConnectionManager dbManager = new DbConnectionManager(DbConnectionManager.ProviderType.Postgres, "192.168.0.230", "rcsdb", "admin", "sorting");
			DbConnection conn = dbManager.getConnection();

			conn.open();

			conn.close();
		}
	}
}
