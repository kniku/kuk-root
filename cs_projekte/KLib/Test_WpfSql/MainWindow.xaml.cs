using KLib.Sql;
using KLib.Wpf;
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

[assembly: log4net.Config.XmlConfigurator(ConfigFile = "Logging.config", Watch = true)]

namespace Test_WpfSql
{
	/// <summary>
	/// Interaktionslogik für MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		static readonly log4net.ILog log = log4net.LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);

		public MainWindow()
		{
			InitializeComponent();
			log.Info("Startup ...");
			//DbManager.addConnectionManager("postgres", new DbConnectionManager(DbConnectionManager.ProviderType.Postgres, null, "testdb", "admin", "sorting", 0));
			//DbManager.addConnectionManager("sqlite", new DbConnectionManager(DbConnectionManager.ProviderType.SQLite, null, @"sqlite_test.db", null, null, 0));

		}

		private void Window_Loaded(object sender, RoutedEventArgs e)
		{
			// Log Window aufmachen
			KLib.Wpf.WndLogger dlg = WndLogger.CreateOrGetWndLogger(this);
			dlg.Top = this.Top;
			dlg.Left = this.Left + this.Width;
			dlg.Show();

			log.Info("Loaded ...");
		}

		private void btnTest_Click(object sender, RoutedEventArgs e)
		{
			try
			{
				log.InfoFormat("öffne Datenbank \"{0}\"...", comboDatenbank.Text);
				DbConnection conx = DbManager.getConnectionManager(comboDatenbank.Text).getConnection();
				if (conx.open())
				{
					object r;

					log.Info("creating table klib_test_01...");
					r = conx.execSQL(@"
create table klib_test_01 (
id integer primary key not null,
name varchar(256),
birthday timestamp,
address varchar(256),
salary float,
a_num decimal(12,2))"
						);
					log.InfoFormat("result={0}", r);

					log.Info("creating data for klib_test_01...");
					int cnt = 0;
					for (int i = 0; i < 10; i++)
					{
						r = conx.execSQL("insert into klib_test_01 (id,name,birthday,address,salary,a_num) values (:1,'Kurt','19670228','Graz 8020',55.123,123456.78)", i);
						if (r.ToString() == "1") cnt++;
					}
					log.InfoFormat("result={0}", cnt);

					//IEnumerator<DataRow> ie = conx.execSQL_select("select * from klib_test_01");
					//while (ie.MoveNext())
					//{
					//	object Value = ie.Current["name"];	// access by name
					//	Value = ie.Current[0];		// access by index, etc...
					//}

					log.Info("showing content of klib_test_01...");
					KLib.WpfSql.WndSqlView sv = new KLib.WpfSql.WndSqlView();
					sv.SqlConnection = comboDatenbank.Text;
					sv.SqlQuery = "select * from klib_test_01";
					sv.ShowDialog();

					log.Info("droping table klib_test_01...");
					r = conx.execSQL("drop table klib_test_01");
					log.InfoFormat("result={0}", r);

				}


				log.Info("closing connection...");
				conx.close();

			}
			catch (Exception _ex)
			{
				log.Error("Error: " + _ex.Message);
			}
		}
	}
}
