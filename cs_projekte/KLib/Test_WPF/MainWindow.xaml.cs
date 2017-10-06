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
		private static readonly log4net.ILog log = log4net.LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);

		public MainWindow()
		{
			InitializeComponent();

//			BasicConfigurator.Configure();
			log.Info("test logger");

//			string setting = System.Configuration.ConfigurationSettings.AppSettings[ ConfigurationManager.AppSettings["setting1"];

			//DbManager.addConnectionManager("testdb", new DbConnectionManager(DbConnectionManager.ProviderType.Postgres, null, "testdb", "admin", "sorting", 0));
			//DbManager.addConnectionManager("rcsdb", new DbConnectionManager(DbConnectionManager.ProviderType.Postgres, null, "rcsdb", "admin", "sorting", 0));
			//DbManager.addConnectionManager("axavia", new DbConnectionManager(DbConnectionManager.ProviderType.SqlServer, "axavia", "BT_ZETS", "axavia", "Axavia#2011", 0));
			//DbManager.addConnectionManager("taifun", new DbConnectionManager(DbConnectionManager.ProviderType.SqlServer, "192.168.0.6", "TAIFUN software", "axavia", "axavia", 0));
			//DbManager.addConnectionManager("sqlite", new DbConnectionManager(DbConnectionManager.ProviderType.SQLite, null, @"C:\knk\tmp\sqlite_test.db", null, null, 0));

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


		void _testPostgres()
		{
		    WndLogger.InstanceShow(this);

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

		private void MenuItem_Click(object sender, RoutedEventArgs e)
		{
			Close();
		}

		private void Button_Click(object sender, RoutedEventArgs e)
		{
		    WndLogger.InstanceShow(this);
			//if (!WndLogger.IsAlreadyOpen())
            //{
            //	KLib.Wpf.WndLogger dlg = WndLogger.CreateOrGetWndLogger(this);
            //	dlg.Top = this.Top;
            //	dlg.Left = this.Left + this.Width;
            //	dlg.Show();
            //}
        }

        private void MenuItem_Click_1(object sender, RoutedEventArgs e)
		{
			Window w = new WndTestDatenbanken();
			w.WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;
			w.Show();
		}

		private void btnTestProgress_Click(object sender, RoutedEventArgs e)
		{
		    WndLogger.InstanceShow(this);

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
