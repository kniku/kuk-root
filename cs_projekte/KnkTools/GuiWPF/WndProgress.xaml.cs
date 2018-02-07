using System.Collections;
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;
using Knk.Base.Logging;

namespace Knk.GuiWPF
{
	/// <summary>
	/// Interaktionslogik für WndProgress.xaml
	/// </summary>
	public partial class WndProgress : Window, INotifyPropertyChanged
	{
	    private static ILog Logger = LogFactory.GetLogger(typeof(WndProgress));
		//private static readonly log4net.ILog Logger = log4net.LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);
		public event PropertyChangedEventHandler PropertyChanged;

		protected void NotifyPropertyChanged(string sProp)
		{
			if (PropertyChanged != null)
			{
				PropertyChanged(this, new PropertyChangedEventArgs(sProp));
			}
		}

		public WndProgress(Window iOwner)
		{
			InitializeComponent();
			PanelProgress.DataContext = this;
			Owner = iOwner;
		}

		private void Window_Loaded(object sender, RoutedEventArgs e)
		{
		}


		ArrayList listWorker = new ArrayList();
		
		class CWorkerInfo
		{
			public DoWorkEventHandler mHandler; public string mTitle; public object mCalldata;
			public BackgroundWorker mWorker; public Label mLabel; public ProgressBar mProgressBar; public Grid mGrid; public TextBlock mTextBlock;
			public bool isCompleted; public bool mWorkerReportsProgress;
		}

		public void AddWorker(DoWorkEventHandler iHandler, string iTitle, bool iWorkerReportsProgress, object iCalldata)
		{
			listWorker.Add(new CWorkerInfo() { mHandler = iHandler, mTitle = iTitle, mCalldata = iCalldata, isCompleted = false, mWorkerReportsProgress = iWorkerReportsProgress });
		}

		public void StartAllTasks(bool iAsync)
		{
			Logger.Info("worker starts");

			foreach (CWorkerInfo ci in listWorker)
			{

				Label lbl = new Label();
				lbl.Content = ci.mTitle;

				Grid grid = new Grid();
				TextBlock tb = new TextBlock();
				tb.HorizontalAlignment = System.Windows.HorizontalAlignment.Center;
				tb.VerticalAlignment = System.Windows.VerticalAlignment.Center;

				ProgressBar pb = new ProgressBar();
				pb.Width = 200;
				pb.IsIndeterminate = !ci.mWorkerReportsProgress;

				PanelProgress.Children.Add(lbl);
				grid.Children.Add(pb);
				grid.Children.Add(tb);
				PanelProgress.Children.Add(grid);

				ci.mLabel = lbl;
				ci.mProgressBar = pb;
				ci.mGrid = grid;
				ci.mTextBlock = tb;

				ci.mWorker = new BackgroundWorker();
				ci.mWorker.WorkerReportsProgress = true;
				ci.mWorker.DoWork += ci.mHandler;
				ci.mWorker.ProgressChanged += worker_ProgressChanged;
				ci.mWorker.RunWorkerCompleted += worker_RunWorkerCompleted;
				ci.mWorker.WorkerSupportsCancellation = true;
				ci.mWorker.RunWorkerAsync(ci.mCalldata);
			}

			if (iAsync) Show();
			else ShowDialog();
		}

		void worker_ProgressChanged(object sender, ProgressChangedEventArgs e)
		{
			foreach (CWorkerInfo ci in listWorker)
			{
				if (ci.mWorker == (BackgroundWorker)sender)
				{
					{
						if (ci.mWorkerReportsProgress)
						{
							ci.mProgressBar.Value = e.ProgressPercentage;
							ci.mTextBlock.Text = e.ProgressPercentage.ToString() + " %";
						}
						if (e.UserState is string) ci.mLabel.Content = e.UserState;
						if (e.ProgressPercentage % 5 == 0) Logger.Info("worker progress [" + ci.mTitle + "]:" + e.ProgressPercentage);
					}
					break;
				}
			}
		}

		void worker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
		{
			bool all_completed = true;

			foreach (CWorkerInfo ci in listWorker)
			{
				if (ci.mWorker == (BackgroundWorker)sender)
				{
					ci.isCompleted = true;
					PanelProgress.Children.Remove(ci.mLabel);
					PanelProgress.Children.Remove(ci.mGrid);
					Logger.Info($"worker [{ci.mTitle}] completed");
				}
				else
				{
					if (ci.isCompleted == false) all_completed = false;
				}
			}

			if (all_completed) Close();
		}

		private void Window_Closing(object sender, CancelEventArgs e)
		{
			bool oneBusyFound = false;

			foreach (CWorkerInfo ci in listWorker)
			{
				if (!ci.isCompleted)
				{
					oneBusyFound = true;
					if (ci.mWorker != null /* && ci.mWorker.WorkerSupportsCancellation*/)
					{
						ci.mWorker.CancelAsync();
					}
					else
					{
						oneBusyFound = true;
					}
				}
			}

			if (oneBusyFound) e.Cancel = true;	// RunWorkerCompleted schließt Dialog endgültig...
		}

	}
}
