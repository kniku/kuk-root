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
using System.Data;
using log4net;
using log4net.Core;
using log4net.Config;
using log4net.Appender;
using log4net.Repository.Hierarchy;

namespace Test_WPF
{
	/// <summary>
	/// Interaktionslogik für MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		public static MemoryAppender LogMemAppender;
		static readonly ILog log = LogManager.GetLogger(typeof(MainWindow));

		public MainWindow()
		{
			InitializeComponent();

			BasicConfigurator.Configure();
			//log4net.GlobalContext.Properties["LogFileName"] = "tcp-client.log";
			//XmlConfigurator.Configure(new System.IO.FileInfo("log4net-config.xml"));

			LogMemAppender = new MemoryAppender();
			LogMemAppender.ActivateOptions();
			Hierarchy hierarchy = (Hierarchy)LogManager.GetRepository();
			hierarchy.Root.AddAppender(LogMemAppender);

			log.Info("starting tcp client");

		}

		void _testPostgres()
		{
			DbConnectionManager cm = new DbConnectionManager(DbConnectionManager.ProviderType.Postgres, null, "bankinfo_echt", "kuk", "anlusa", 0);
			//DbConnectionManager cm = new DbConnectionManager(DbConnectionManager.ProviderType.MySql, null, "testdb", "root", null, 0);

			DbConnection con1 = cm.getConnection();
			DbConnection con2 = cm.getConnection();

			con1.open();
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
			foreach (LoggingEvent logevent in LogMemAppender.GetEvents())
			{
				listLogger.SelectedIndex = listLogger.Items.Add(string.Format("{2} {0}: {1}", logevent.Level, logevent.RenderedMessage, logevent.TimeStamp));
			}
			LogMemAppender.Clear();
		}

		private void MenuItem_Click(object sender, RoutedEventArgs e)
		{
			Close();
		}
	}
}
