using System;
using System.Windows;
using KLib.Sql;
using KLib.Wpf;
using log4net;
using log4net.Config;
using System.ComponentModel;

namespace Test_WPF
{
	/// <summary>
	/// Interaktionslogik für MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		static readonly ILog log = LogManager.GetLogger(typeof(MainWindow));

		public MainWindow()
		{
			InitializeComponent();

			BasicConfigurator.Configure();
			log.Info("test logger");

//			string setting = System.Configuration.ConfigurationSettings.AppSettings[ ConfigurationManager.AppSettings["setting1"];

			DbManager.addConnectionManager("testdb", new DbConnectionManager(DbConnectionManager.ProviderType.Postgres, null, "testdb", "admin", "sorting", 0));
			DbManager.addConnectionManager("rcsdb", new DbConnectionManager(DbConnectionManager.ProviderType.Postgres, null, "rcsdb", "admin", "sorting", 0));
			DbManager.addConnectionManager("axavia", new DbConnectionManager(DbConnectionManager.ProviderType.SqlServer, "axavia", "BT_ZETS", "axavia", "Axavia#2011", 0));
			DbManager.addConnectionManager("taifun", new DbConnectionManager(DbConnectionManager.ProviderType.SqlServer, "192.168.0.6", "TAIFUN software", "axavia", "axavia", 0));
			DbManager.addConnectionManager("sqlite", new DbConnectionManager(DbConnectionManager.ProviderType.SQLite, null, @"C:\knk\tmp\sqlite_test.db", null, null, 0));

		}


		class CTestObj
		{
			public string valsql_id { get; set; }
			public string valsql_ValStr { get; set; }
			public static int MyProperty3 { get; set; }
		}


		void worker_DoWork(object sender, DoWorkEventArgs e)
		{
			int max = (int)e.Argument;
			int result = 0;
			for (int i = 0; i < max; i++)
			{
				int progressPercentage = Convert.ToInt32(((double)i / max) * 100);
				if (i % 42 == 0)
				{
					result++;
					//(sender as BackgroundWorker).ReportProgress(progressPercentage, i);
					(sender as BackgroundWorker).ReportProgress(progressPercentage, "TEST " + i + ":");
				}
				else
					if (progressPercentage % 2 == 0) (sender as BackgroundWorker).ReportProgress(progressPercentage);
				System.Threading.Thread.Sleep(1);

			}
			e.Result = result;
		}


		void _testPostgres()
		{
			Button_Click(btnLog, null);

			string DATENBANK = "sqlite";

			DbConnection conx = DbManager.getConnectionManager(DATENBANK).getConnection();
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

				//IEnumerator<DataRow> ie = conx.execSQL_select("select * from klib_test_01");
				//while (ie.MoveNext())
				//{
				//	object Value = ie.Current["name"];	// access by name
				//	Value = ie.Current[0];		// access by index, etc...
				//}

				log.Info("showing content of klib_test_01...");
				KLib.Wpf.Sql.WndSqlView sv = new KLib.Wpf.Sql.WndSqlView();
				sv.SqlConnection = DATENBANK;
				sv.SqlQuery = "select * from klib_test_01";
				sv.ShowDialog();

				log.Info("droping table klib_test_01...");
				r = conx.execSQL("drop table klib_test_01");
				log.InfoFormat("result={0}", r);

			}
			

			conx.close();

			return;

			// ############### TEST WndSqlView ###############

			if (true)
			{
				KLib.Wpf.Sql.WndSqlView sv = new KLib.Wpf.Sql.WndSqlView();
				sv.SqlConnection = "rcsdb";
				//			sv.SqlQuery = "select * from config";
				sv.SqlQuery = "select * from statistic limit 5000";
				//			sv.SqlQuery = "select * from TFW_M002.PhUDF01";
				sv.Show();

				return;
				// ############### TEST END ###############

				// ############### TEST WndProgress ###############

				WndProgress xxx = new WndProgress(this);
				xxx.AddWorker(worker_DoWork, "worker 1:", true, 5000);
				xxx.AddWorker(worker_DoWork, "worker 2:", false, 7000);
				xxx.AddWorker(worker_DoWork, "worker 3:", true, 3000);
				xxx.AddWorker(worker_DoWork, "worker 4:", true, 4000);
				xxx.AddWorker(worker_DoWork, "worker 5:", true, 8000);
				xxx.StartAllTasks(false);

				return;
			}
			// ############### TEST END ###############


			//DbConnectionManager cm = new DbConnectionManager(DbConnectionManager.ProviderType.Postgres, null, "bankinfo_echt", "admin", "sorting", 0);
			//DbConnectionManager cm = new DbConnectionManager(DbConnectionManager.ProviderType.Postgres, null, "testdb", "kuk", "anlusa", 0);

			DbConnection con1 = DbManager.getConnectionManager("testdb").getConnection();
			DbConnection con2 = DbManager.getConnectionManager("testdb").getConnection();

			if (con1.open())
			{
				CTestObj testObj = new CTestObj();
				con1.execSQL_selectIntoObject(testObj, "select * from test");

				testObj.valsql_id = "new";
				testObj.valsql_ValStr = "knk_" + testObj.valsql_id;

				con1.execSQL_insertFromObject("test", testObj);

				con1.close();
			}

#if false
//			MessageBox.Show("ok, con1=" + con1.getState());

			//con2.open();
//			MessageBox.Show("ok, con2=" + con2.getState());

			//MessageBox.Show(con1.execSQL("select now()"));
			//MessageBox.Show(con2.execSQL("select now()"));
			Object xx = con1.execSQL("select count(*) from positionen");
			Object x = con1.execSQL("select count(*) from positionen where amt>:1", 200);
			//MessageBox.Show(x.ToString());
			//MessageBox.Show(con2.execSQL("select count(*) from positionen"));

			//				bool xx = false;

			//				IEnumerator<DataRow> list = con1.execSQL_select("select * from positionen where amt<:1 and amt>:2", -100000, 200000);
			IEnumerator<DataRow> list = con1.execSQL_select("select * from positionen where curcode=:2 and ktoid=:1 and amt>:3", 1, "EUR", 100.0);

			if (list.MoveNext()) gridData.ItemsSource = list.Current.Table.DefaultView;
			/*				while (list.MoveNext())
							{
								if (!xx)
								{
									//gridData.ItemsSource = list.Current.Table.Rows;
									gridData.ItemsSource = list.Current.Table.DefaultView;
								}

								xx = true;
								object x = list.Current["amt"];
							}
			*/
			con2.close();
			con1.close();
#endif
		}

		void _testSqlServer()
		{

			DbConnection con1 = DbManager.getConnectionManager("axavia").getConnection();

			con1.open();
//			MessageBox.Show("ok, con1=" + con1.NativeConnection.State);
			log.Info("ok, con1=" + con1.getState());

//			MessageBox.Show(con1.execSQL("select count(*) from zets_data_shadow"));
			log.Info(con1.execSQL("select count(*) from zets_data_shadow"));
//			MessageBox.Show(con1.execSQL("select count(*) from positionen where amt<:2 and amt>:1", 100, 200));
//			MessageBox.Show(con2.execSQL("select count(*) from positionen"));

			//				bool xx = false;

			//				IEnumerator<DataRow> list = con1.execSQL_select("select * from positionen where amt<:1 and amt>:2", -100000, 200000);
			//IEnumerator<DataRow> list = con1.execSQL_select("select * from positionen where curcode=:2 and ktoid=:1 and amt>:3", 1, "EUR", 100.0);

			//if (list.MoveNext()) gridData.ItemsSource = list.Current.Table.DefaultView;
			/*				while (list.MoveNext())
							{
								if (!xx)
								{
									//gridData.ItemsSource = list.Current.Table.Rows;
									gridData.ItemsSource = list.Current.Table.DefaultView;
								}

								xx = true;
								object x = list.Current["amt"];
							}
			*/
			con1.close();
		}

		void btnConnect_Click(object sender, RoutedEventArgs e)
		{
			try
			{
				_testPostgres();
				//_testSqlServer();

			}
			catch (Exception ex)
			{
				log.Fatal(ex.Message, ex);
				//MessageBox.Show(ex.Message);
			}
		}

		private void MenuItem_Click(object sender, RoutedEventArgs e)
		{
			Close();
		}

		private void Button_Click(object sender, RoutedEventArgs e)
		{
			if (!WndLogger.IsAlreadyOpen())
			{
				KLib.Wpf.WndLogger dlg = WndLogger.CreateOrGetWndLogger(this);
				dlg.Top = this.Top;
				dlg.Left = this.Left + this.Width;
				dlg.Show();
			}
		}
	}

}
