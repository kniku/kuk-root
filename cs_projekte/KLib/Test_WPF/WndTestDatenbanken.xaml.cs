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
using System.Windows.Shapes;

namespace Test_WPF
{
	/// <summary>
	/// Interaktionslogik für WndTestDatenbanken.xaml
	/// </summary>
	public partial class WndTestDatenbanken : Window
	{
		private static readonly log4net.ILog log = log4net.LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);

		public WndTestDatenbanken()
		{
			InitializeComponent();

			DbManager.addConnectionManager("testdb", new DbConnectionManager(DbConnectionManager.ProviderType.Postgres, null, "testdb", "admin", "sorting", 0));
			DbManager.addConnectionManager("rcsdb", new DbConnectionManager(DbConnectionManager.ProviderType.Postgres, null, "rcsdb", "admin", "sorting", 0));
			DbManager.addConnectionManager("axavia", new DbConnectionManager(DbConnectionManager.ProviderType.SqlServer, "axavia", "BT_ZETS", "axavia", "Axavia#2011", 0));
			DbManager.addConnectionManager("taifun", new DbConnectionManager(DbConnectionManager.ProviderType.SqlServer, "192.168.0.6", "TAIFUN software", "axavia", "axavia", 0));
			DbManager.addConnectionManager("sqlite", new DbConnectionManager(DbConnectionManager.ProviderType.SQLite, null, @"C:\knk\tmp\sqlite_test.db", null, null, 0));

			comboDatenbank.Items.Add("testdb");
			comboDatenbank.Items.Add("rcsdb");
			comboDatenbank.Items.Add("axavia");
			comboDatenbank.Items.Add("taifun");
			comboDatenbank.Items.Add("sqlite");
		}

		private void Window_Loaded(object sender, RoutedEventArgs e)
		{

		}

		private void btnTest_Click(object sender, RoutedEventArgs e)
		{

		    WndLogger.InstanceShow(this);

            try
            {
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
					for (int i = 0; i < 10; i++)
					{
						r = conx.execSQL("insert into klib_test_01 (id,name,birthday,address,salary,a_num) values (:1,'Kurt','19670228','Graz 8020',55.123,123456.78)", i);
						log.InfoFormat("result={0}", r);
					}


					log.Info("showing content of klib_test_01...");
					KLib.WpfSql.WndSqlView sv = new KLib.WpfSql.WndSqlView();
					sv.SqlConnection = comboDatenbank.Text;
					sv.SqlQuery = "select * from klib_test_01";
					sv.ShowDialog();

					log.Info("droping table klib_test_01...");
					r = conx.execSQL("drop table klib_test_01");
					log.InfoFormat("result={0}", r);

				}

				conx.close();
			}
			catch (Exception ex)
			{
				log.Fatal(ex.Message, ex);
			}

			return;
		}

		private void btnTestConnect_Click(object sender, RoutedEventArgs e)
		{
		    WndLogger.InstanceShow(this);

            try
            {
				DbConnectionManager manager = DbManager.getConnectionManager(comboDatenbank.Text);


				log.Info("trying to connect to " + manager.Database + "@" + manager.Server + ", user " + manager.User + " (" + manager.Provider + ")");

				DbConnection conx = manager.getConnection();
				if (conx.open())
				{
					log.Info("connection ok. closing...");
					conx.close();
				}
			}
			catch (Exception ex)
			{
				log.Fatal(ex.Message, ex);
			}

			log.Info("done.");
		}

		private void btnTestRcsdb_Click(object sender, RoutedEventArgs e)
		{
			log.Info("showing content of klib_test_01...");

			KLib.WpfSql.WndSqlView sv = new KLib.WpfSql.WndSqlView();
			sv.SqlConnection = "rcsdb";
			//			sv.SqlQuery = "select * from config";
			sv.SqlQuery = "select * from statistic limit 5000";
			//			sv.SqlQuery = "select * from TFW_M002.PhUDF01";
			sv.Show();

		}
	}
}
