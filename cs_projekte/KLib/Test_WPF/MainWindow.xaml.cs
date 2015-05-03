using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using KLib.Sql;
using KLib.Wpf;
using System.Data;
using log4net;
using log4net.Core;
using log4net.Config;
using log4net.Appender;
using log4net.Repository.Hierarchy;
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

			log.Info("starting tcp client");

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
                    (sender as BackgroundWorker).ReportProgress(progressPercentage, i);
                }
                else
                    (sender as BackgroundWorker).ReportProgress(progressPercentage);
                System.Threading.Thread.Sleep(1);

            }
            e.Result = result;
        }


		void _testPostgres()
		{
			Button_Click(btnLog, null);
            // ############### TEST ###############

            WndProgress xxx = new WndProgress();
            xxx.AddWorker(worker_DoWork, "worker 1", 5000);
            xxx.AddWorker(worker_DoWork, "worker 2", 7000);
            xxx.AddWorker(worker_DoWork, "worker 3", 5000);
            xxx.RunAsync();

            return;
            // ############### TEST END ###############




			//DbConnectionManager cm = new DbConnectionManager(DbConnectionManager.ProviderType.Postgres, null, "bankinfo_echt", "admin", "sorting", 0);
			DbConnectionManager cm = new DbConnectionManager(DbConnectionManager.ProviderType.Postgres, null, "testdb", "kuk", "anlusa", 0);

			DbConnection con1 = cm.getConnection();
			DbConnection con2 = cm.getConnection();

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
			DbConnectionManager cm = new DbConnectionManager(DbConnectionManager.ProviderType.SqlServer, "axavia", "BT_ZETS", "axavia", "Axavia#2011", 0);

			DbConnection con1 = cm.getConnection();

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
