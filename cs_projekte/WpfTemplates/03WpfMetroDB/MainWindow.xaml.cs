using KLib.Sql;
using KLib.Wpf;
using MahApps.Metro.Controls;
using System;
using System.Collections.Generic;
using System.Data;
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
		private static readonly log4net.ILog Logger = log4net.LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);

		public MainWindow()
		{
			InitializeComponent();

			txtInfo.Text = @"
Aktive Packages:
- log4net: Log Tool
- MahApps:Metro: Metro L&F
- KLib.Sql: SQL Lib
- KLib.Wpf: Wpf Tools";

		}

		private void btnConnect_Click(object sender, RoutedEventArgs e)
		{

			DbConnectionManager dbManager = new DbConnectionManager(DbConnectionManager.ProviderType.Postgres, "192.168.0.230", "rcsdb", "admin", "sorting");
			DbConnection conn = dbManager.getConnection();

			conn.open();

			Object x = conn.execSQL("select now()");

			/*
			x = conn.execSQL("select count(*),12 from config");
			x = conn.execSQL("select * from config");
			x = conn.execSQL("select count(*) from config where grp=:2 and key1=:1", "", "CUSTOM");
			x = conn.execSQL("select count(*) from statistic_d where ts<:1", DateTime.Now);
			int xx = 200;
			x = conn.execSQL("select count(*) from statistic_d where cnt>:1", xx);
			
			IEnumerator<DataRow> List = conn.execSQL_select("select * from config");
			while (List.MoveNext())
			{
				object Value = List.Current["grp"];	// access by name
				Value = List.Current[1];		// access by index, etc...
			}
			*/
			conn.close();
		}

		private void btnLog_Click(object sender, RoutedEventArgs e)
		{
			WndLogger wnd = WndLogger.CreateOrGetWndLogger(this);

			if (!wnd.IsVisible)
			{
				wnd.Left = Left + Width;
				wnd.Top = Top;
				wnd.Show();
			}
			else
			{
				if (wnd.WindowState == System.Windows.WindowState.Minimized)
				{
					wnd.WindowState = System.Windows.WindowState.Normal;
				}
				wnd.Activate();
			}
		}

		private void btnSql_Click(object sender, RoutedEventArgs e)
		{
			DbConnectionManager dbManager = new DbConnectionManager(DbConnectionManager.ProviderType.Postgres, "192.168.0.230", "rcsdb", "admin", "sorting");
			DbConnection conn = dbManager.getConnection();

			conn.open();

			Logger.InfoFormat("Results of: {0}", txtSql.Text);
			long cnt = 0;

			IEnumerator<DataRow> List = conn.execSQL_select(txtSql.Text);
			while (List != null && List.MoveNext())
			{
				string x = "|";

				if (cnt == 0)
				{
					foreach (DataColumn item in List.Current.Table.Columns)
					{
						x += item.ColumnName + '|';
					}
					Logger.InfoFormat("{0}", x);
					x = "|";
				}
				
				foreach (var item in List.Current.ItemArray)
				{
					x += item.ToString() + '|';
				}
				//object Value = List.Current[0];
				Logger.InfoFormat("{0}", x);
				cnt++;
			}
			Logger.InfoFormat("Rows: {0}", cnt);

			conn.close();
		}

		private void btnExit_Click(object sender, RoutedEventArgs e)
		{
			Close();
		}
	}
}
