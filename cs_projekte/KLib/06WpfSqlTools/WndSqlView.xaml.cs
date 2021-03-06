﻿using KLib.Sql;
using KLib.Wpf;
using System.ComponentModel;
using System.Windows;

namespace KLib.WpfSql
{
	/// <summary>
	/// Interaktionslogik für WndSqlView.xaml
	/// </summary>
	public partial class WndSqlView : Window
	{
		/// <summary>
		/// Die zu verwendende Datenbankverbindung (siehe DbManager)
		/// </summary>
		public string SqlConnection { get; set; }

		/// <summary>
		/// Das SQL Statement, das aktuell zum Laden verwendet wird.
		/// </summary>
		public string SqlQuery { get; set; }

		public WndSqlView()
		{
			InitializeComponent();
		}

		void worker_DoWork(object sender, DoWorkEventArgs e)
		{
			DbConnection con1 = DbManager.getConnectionManager(SqlConnection).getConnection();
			con1.open();
			if (con1.getState() == System.Data.ConnectionState.Open)
			{
				DV = con1.execSQL_select(SqlQuery).DefaultView;

				//foreach (var item in con1.execSQL_select(SqlQuery).Rows)
				//{
					
				//}
				con1.close();
			}
		}

		System.Data.DataView DV;

		void reload()
		{
			WndProgress xxx = new WndProgress(this);
			xxx.AddWorker(worker_DoWork, "loading...", false, null);
			xxx.StartAllTasks(false);

			gridData.DataContext = DV;
		}

		private void btnReload_Click(object sender, RoutedEventArgs e)
		{
			SqlQuery = txtSqlQuery.Text;
			reload();
		}

		private void btnClose_Click(object sender, RoutedEventArgs e)
		{
			Close();
		}

		private void Window_Loaded(object sender, RoutedEventArgs e)
		{
			txtSqlQuery.Text = SqlQuery;
			reload();
		}
	}
}
