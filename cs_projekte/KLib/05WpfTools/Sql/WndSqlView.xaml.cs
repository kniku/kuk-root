using KLib.Sql;
using System;
using System.Collections.Generic;
using System.ComponentModel;
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

namespace KLib.Wpf.Sql
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
				DV = con1.execSQL_selectDS(SqlQuery).Tables[0].DefaultView;
				con1.close();
			}
		}

		System.Data.DataView DV;

		void reload()
		{
			WndProgress xxx = new WndProgress(this);
			xxx.AddWorker(worker_DoWork, "loading...", false, 10000);
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
