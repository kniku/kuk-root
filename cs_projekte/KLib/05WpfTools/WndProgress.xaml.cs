using System;
using System.Collections;
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

		public WndProgress()
		{
			InitializeComponent();
			PanelProgress.DataContext = this;
		}

		private void Window_Loaded(object sender, RoutedEventArgs e)
		{
		}


		ArrayList listWorker = new ArrayList();
		
		class CWorkerInfo
		{
			public DoWorkEventHandler mHandler; public string mTitle; public object mCalldata;
			public BackgroundWorker mWorker; public Label mLabel;  public ProgressBar mProgressBar;
			public bool isCompleted;
		}

		public void AddWorker(DoWorkEventHandler iHandler, string iTitle, object iCalldata)
		{
			listWorker.Add(new CWorkerInfo() { mHandler = iHandler, mTitle = iTitle, mCalldata = iCalldata, isCompleted = false });
		}

		public void RunAsync()
		{
			Logger.Info("worker starts");

			int idx = 1;
			foreach (CWorkerInfo ci in listWorker)
			{

				Label lbl = new Label();
				lbl.Content = ci.mTitle;

				ProgressBar pb = new ProgressBar();
				pb.Width = 200;
				pb.Height = 15;
				var binding = new Binding("Progress" + idx++);
				BindingOperations.SetBinding(pb, ProgressBar.ValueProperty, binding);

				PanelProgress.Children.Add(lbl);
				PanelProgress.Children.Add(pb);

				ci.mLabel = lbl;
				ci.mProgressBar = pb;

				ci.mWorker = new BackgroundWorker();
				ci.mWorker.WorkerReportsProgress = true;
				ci.mWorker.DoWork += ci.mHandler;
				ci.mWorker.ProgressChanged += worker_ProgressChanged;
				ci.mWorker.RunWorkerCompleted += worker_RunWorkerCompleted;
				ci.mWorker.RunWorkerAsync(ci.mCalldata);
			}

		//    ShowDialog();
			Show();
		}

		void worker_ProgressChanged(object sender, ProgressChangedEventArgs e)
		{
			foreach (CWorkerInfo ci in listWorker)
			{
				if (ci.mWorker == (BackgroundWorker)sender)
				{
					if (ci.mProgressBar.Value != e.ProgressPercentage)
					{
						ci.mProgressBar.Value = e.ProgressPercentage;
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
					PanelProgress.Children.Remove(ci.mProgressBar);
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
