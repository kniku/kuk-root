using System.Collections;
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;

namespace KLib.Wpf
{
	/// <summary>
	/// Interaktionslogik für WndProgress.xaml
	/// </summary>
	public partial class WndProgress : Window, INotifyPropertyChanged
	{
		private static readonly log4net.ILog Logger = log4net.LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);

		//public double Progress1 { get; set; }
		//public double Progress2 { get; set; }
		//public double Progress3 { get; set; }
		//public double Progress4 { get; set; }
		//public double Progress5 { get; set; }

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

//			int idx = 1;
			foreach (CWorkerInfo ci in listWorker)
			{

				Label lbl = new Label();
				lbl.Content = ci.mTitle;

				Grid grid = new Grid();
				TextBlock tb = new TextBlock();
				tb.HorizontalAlignment = System.Windows.HorizontalAlignment.Center;
				tb.VerticalAlignment = System.Windows.VerticalAlignment.Center;
				//tb.Text = "34 %";

				ProgressBar pb = new ProgressBar();
				pb.Width = 200;
				//pb.Height = 15;
				pb.IsIndeterminate = !ci.mWorkerReportsProgress;
				//var binding = new Binding("Progress" + idx++);
				//BindingOperations.SetBinding(pb, ProgressBar.ValueProperty, binding);

				PanelProgress.Children.Add(lbl);
				grid.Children.Add(pb);
				grid.Children.Add(tb);
				PanelProgress.Children.Add(grid);

				ci.mLabel = lbl;
				ci.mProgressBar = pb;
				ci.mGrid = grid;
				ci.mTextBlock = tb;

				ci.mWorker = new BackgroundWorker();
				ci.mWorker.WorkerReportsProgress = true;// ci.mWorkerReportsProgress;
				ci.mWorker.DoWork += ci.mHandler;
				ci.mWorker.ProgressChanged += worker_ProgressChanged;
				ci.mWorker.RunWorkerCompleted += worker_RunWorkerCompleted;
				ci.mWorker.RunWorkerAsync(ci.mCalldata);
			}

		//    ShowDialog();
			if (iAsync) Show();
			else ShowDialog();
		}

		void worker_ProgressChanged(object sender, ProgressChangedEventArgs e)
		{
			foreach (CWorkerInfo ci in listWorker)
			{
				if (ci.mWorker == (BackgroundWorker)sender)
				{
					//if (ci.mProgressBar.Value != e.ProgressPercentage)
					{
						if (ci.mWorkerReportsProgress)
						{
							ci.mProgressBar.Value = e.ProgressPercentage;
							ci.mTextBlock.Text = e.ProgressPercentage.ToString() + " %";
						}
						if (e.UserState is string) ci.mLabel.Content = e.UserState;
						//NotifyPropertyChanged("Progress1");
						if (e.ProgressPercentage % 5 == 0) Logger.Info("worker progress [" + ci.mTitle + "]:" + e.ProgressPercentage);
					}
					break;
				}
			}
		}

		void worker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
		{
			bool all_completed = true;

			//            MessageBox.Show("Numbers between 0 and 10000 divisible by 7: " + e.Result);
			foreach (CWorkerInfo ci in listWorker)
			{
				if (ci.mWorker == (BackgroundWorker)sender)
				{
					ci.isCompleted = true;
					PanelProgress.Children.Remove(ci.mLabel);
					PanelProgress.Children.Remove(ci.mGrid);
					Logger.InfoFormat("worker [{0}] completed", ci.mTitle);
				}
				else
				{
					if (ci.isCompleted == false) all_completed = false;
				}
			}

			if (all_completed) Close();
		}

	}
}
