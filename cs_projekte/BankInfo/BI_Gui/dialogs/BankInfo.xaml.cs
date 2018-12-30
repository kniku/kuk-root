using System;
using System.Collections;
using System.Linq;
using System.Text;
using System.Data;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using knk.shared;
using knk.DBTools;
using BI_Gui.classes;
using BI_Gui.dialogs;

namespace BI_Gui
{
	/// <summary>
	/// Interaktionslogik für MainWindow.xaml
	/// </summary>
	public partial class BankInfo : Window
	{
		AppGlobalSimple ag;
		PositionDetail dlgPosition;

		public BankInfo()
		{
			InitializeComponent();
			ag = AppGlobalSimple.createInstance() as AppGlobalSimple;
			ag.Log = new BI_Gui.dialogs.Logger(500);

#if false
			string db_host = "localhost";
			string db_user = "kuk";
			string db_pwd = "kuk";
			string db_database = "testdb";
#else
			string db_host = "localhost";
			string db_user = "admin";
		    string db_pwd = "anlusa"; //"sorting";
			string db_database = "bankinfo_echt";
#endif
			ag.Log.LogInfo("Datenbankverbindung: {0}@{1} als User {2}", db_database, db_host, db_user);
			ConnectionPool DBPool = new ConnectionPool(ConnectionPool.ProviderType.Postgres, db_host, db_database, db_user, db_pwd, 0);
			if (!DBPool.checkConnection(ag.Log))
			{
				ag.Log.LogError("Datenbank nicht erreichbar!");
			}
			else
			{
				ag.Log.LogInfo("Datenbankverbindung ok.");
			}

			ag.addProperty("db", DBPool);

			try
			{
				Connection conn = DBPool.getConnection();

				ArrayList arrNR = new ArrayList();
				ArrayList arrID = new ArrayList();
				ArrayList arrCurcode = new ArrayList();
				conn.execSQL_select("select id,ktonr,curcode from konten", arrID, arrNR, arrCurcode);
				for (int i = 0; i < arrNR.Count; i++)
				{
					ListBoxItem item = new ListBoxItem();
					CKonto konto = new CKonto();
					konto.ktoid = (int)arrID[i];
					konto.ktonr = (string)arrNR[i];
					item.Content = konto.ktonr;// +" (" + (string)arrCurcode[i] + ")";
//					item.Content = konto;
//					item.DataContext = konto;
					lbKonten.Items.Add(item);
				}
				conn = DBPool.releaseConnection(conn);
			}
			catch (Exception e)
			{
				ag.Log.LogError("{0}", e.Message);
			}
		}

		private void MenuItem_Click(object sender, RoutedEventArgs e)
		{
			MenuItem mi = sender as MenuItem;
			ContextMenu cm = mi.Parent as ContextMenu;
			switch (((MenuItem)sender).Name)
			{
				case "miExit":
					Close();
					break;
				case "miShowLog":
					BI_Gui.dialogs.Logger logger = ag.Log as BI_Gui.dialogs.Logger;
					if (logger != null)
					{
						logger.OpenViewer();
					}
					break;
				case "miAuszugImport":
					ListBox lb = this.FindName("lbKonten") as ListBox;
					if (lb.SelectedIndex >= 0)
					{
						//MessageBox.Show("selected: " + lb.SelectedIndex);
						Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();
						dlg.CheckFileExists = true;
						dlg.DefaultExt = ".csv";
						dlg.Filter = "Bankauszüge im CSV Format|*.csv|Alle Dateien|*.*"; 
						Nullable<bool> rv = dlg.ShowDialog(this);
						if (rv == true)
						{
//							MessageBox.Show("selected: " + dlg.FileName);
							(ag.Log as BI_Gui.dialogs.Logger).OpenViewer();
							ag.Log.LogInfo("Starte Import der Datei {0}", dlg.FileName);
							ConnectionPool DBPool = (ConnectionPool)ag.getProperty("db");
							Connection conn = DBPool.getConnection();
							string x = conn.execSQL("select id from konten where ktonr=@1", (lb.SelectedItem as ListBoxItem).Content);
							if (x != null)
							{
								if (MessageBox.Show(string.Format("Soll die Datei {0} tatsächlich für das Konto {1} importiert werden?", dlg.FileName, (lb.SelectedItem as ListBoxItem).Content), "Achtung", MessageBoxButton.OKCancel) == MessageBoxResult.OK)
								{
									conn.TSQL_startTransaction();
									ImportStatus erg = AuszugImporter.import_file(conn, dlg.FileName, int.Parse(x));
									ag.Log.LogInfo("Ergebnis: Positionen: {0}, Importiert: {1}", erg.cnt_positionen, erg.cnt_imports);
									if ((erg.cnt_err_imports + erg.cnt_err_positionen) > 0)
									{
										conn.TSQL_rollback();
										ag.Log.LogError("Fehler: Positionen: {0}, Import: {1} : {2}", erg.cnt_err_positionen, erg.cnt_err_imports, erg.first_error);
										ag.Log.LogInfo("Import abgebrochen.");
									}
									else
									{
										conn.TSQL_commit();
										ag.Log.LogInfo("Import abgeschlossen.");
									}
								}
							}
							else
							{
								ag.Log.LogError("Konto ID konnte nicht ermittelt werden!");
							}
							DBPool.releaseConnection(conn);
						}
					}
					break;
			}
		}

		private void lbKonten_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			gridPositionen.Items.Clear();
			ListBox lb = (ListBox)sender;
			if (lb.SelectedIndex >= 0)
			{
				string konto = (string)((ListBoxItem)lb.Items[lb.SelectedIndex]).Content;
				try
				{
					ConnectionPool DBPool = (ConnectionPool)ag.getProperty("db");
					Connection conn = DBPool.getConnection();

					ArrayList arrEffdt = new ArrayList();
					ArrayList arrPosteddt = new ArrayList();
					ArrayList arrAmt = new ArrayList();
					ArrayList arrMemo = new ArrayList();
					ArrayList arrVoucher = new ArrayList();
					
					conn.execSQL_select(conn.prepareSQL("select effdt,posteddt,amt,memo,voucher from positionen where ktoid=(select id from konten where ktonr=@1) order by posteddt desc", konto),
						arrEffdt, arrPosteddt, arrAmt, arrMemo, arrVoucher);

					for (int i = 0; i < arrAmt.Count; i++)
					{
						CPosition pos = new CPosition();
						pos.amt = (decimal)arrAmt[i];
						pos.memo = arrMemo[i] as string;
						pos.voucher = arrVoucher[i] as string;
						pos.effdt = (DateTime)arrEffdt[i];
						pos.posteddt = (DateTime)arrPosteddt[i];
						gridPositionen.Items.Add(pos);
					}
					conn = DBPool.releaseConnection(conn);
					StatusBarItem sbi = (StatusBarItem)StatusBar.Items[0];
					sbi.Content = string.Format("Geladen: {0} Positionen...", arrAmt.Count);
				}
				catch (Exception ex)
				{
					ag.Log.LogError("{0}", ex.Message);
				}
			}
		}

		private void MainWindow_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			BI_Gui.dialogs.Logger logger = ag.Log as BI_Gui.dialogs.Logger;
			if (logger != null) logger.CloseViewer();
		}

		private void onMouseDoubleClick(object sender, MouseButtonEventArgs e)
		{
			if (dlgPosition == null || !dlgPosition.IsVisible)
			{
				dlgPosition = new PositionDetail();
				dlgPosition.setNewPosition(gridPositionen.SelectedItem as CPosition);
				dlgPosition.Owner = this;
				dlgPosition.Show();
			}
		}

		private void gridPositionen_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			if (dlgPosition != null && dlgPosition.IsVisible)
			{
				dlgPosition.setNewPosition(gridPositionen.SelectedItem as CPosition);
			}

		}

		private void gridPositionen_LoadingRow(object sender, DataGridRowEventArgs e)
		{
			// Get the DataRow corresponding to the DataGridRow that is loading.
//			DataRowView item = e.Row.Item as DataRowView;
			CPosition pos = e.Row.Item as CPosition;
			if (pos != null && pos.amt < 0)
			{
//				e.Row.Background = new SolidColorBrush(Colors.Red);
				e.Row.Foreground = new SolidColorBrush(Colors.Firebrick);
			}		
		}

	}

	class CKonto
	{
		public string ktonr { get; set; }
		public int ktoid { get; set; }
	}
	public class CPosition
	{
		public decimal amt { get; set; }
		public string memo { get; set; }
		public string voucher { get; set; }
		public DateTime posteddt { get; set; }
		public DateTime effdt { get; set; }
	}
}
