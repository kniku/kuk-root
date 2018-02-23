using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Shell;
using Knk.Base.Logging;

namespace Knk.GuiWPF
{
	/// <summary>
	/// Interaktionslogik für WndProgress.xaml
	/// </summary>
	public partial class WndProgress : Window, INotifyPropertyChanged
	{
	    private static ILog Logger = LogManager.GetLogger(typeof(WndProgress));
		public event PropertyChangedEventHandler PropertyChanged;

		protected void NotifyPropertyChanged(string sProp)
		{
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(sProp));
        }

		public WndProgress(Window iOwner)
		{
			InitializeComponent();
			//PanelProgress.DataContext = this;
			Owner = iOwner;
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
							ci.mTextBlock.Text = e.ProgressPercentage + " %";
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
					//else
					//{
					//	oneBusyFound = true;
					//}
				}
			}

			if (oneBusyFound) e.Cancel = true;	// RunWorkerCompleted schließt Dialog endgültig...
		}

	    void Worker_ProgressChanged(object sender, ProgressChangedEventArgs e)
	    {
	        if (sender is MyBackgroundWorker worker)
	        {
                if (worker.TaskInfo.ReportsProgress)
                {
                    worker.TaskInfo.ProgressBar.Value = e.ProgressPercentage;
                    worker.TaskInfo.TextBlock.Text = e.ProgressPercentage + " %";
                }
                if (e.UserState is string) worker.TaskInfo.Label.Content = e.UserState;
                //if (e.ProgressPercentage % 5 == 0) Logger.Info("worker progress [" + worker.TaskInfo.Title + "]:" + e.ProgressPercentage);
            }
	    }
	    void Worker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
	    {
	        if (sender is MyBackgroundWorker worker)
	        {
                PanelProgress.Children.Remove(worker.TaskInfo.Label);
                PanelProgress.Children.Remove(worker.TaskInfo.ProgressBar.Parent as UIElement);
	            worker.TaskInfo.State = TaskState.Finished;
            }
	        //bool all_completed = true;

	        //foreach (CWorkerInfo ci in listWorker)
	        //{
	        //    if (ci.mWorker == (BackgroundWorker)sender)
	        //    {
	        //        ci.isCompleted = true;
	        //        PanelProgress.Children.Remove(ci.mLabel);
	        //        PanelProgress.Children.Remove(ci.mGrid);
	        //        Logger.Info($"worker [{ci.mTitle}] completed");
	        //    }
	        //    else
	        //    {
	        //        if (ci.isCompleted == false) all_completed = false;
	        //    }
	        //}

	        //if (all_completed) Close();
	    }

	    class MyBackgroundWorker : BackgroundWorker
	    {
	        public MyBackgroundWorker(TaskInfo taskInfo)
	        {
	            TaskInfo = taskInfo;
	        }
	        public TaskInfo TaskInfo { get; }
	    }
	    internal void StartTask(TaskInfo taskinfo)
	    {
	        Label lbl = new Label();
	        lbl.Content = taskinfo.Title;

	        Grid grid = new Grid();
	        TextBlock tb = new TextBlock();
	        tb.HorizontalAlignment = System.Windows.HorizontalAlignment.Center;
	        tb.VerticalAlignment = System.Windows.VerticalAlignment.Center;

	        ProgressBar pb = new ProgressBar();
	        pb.Width = 200;
	        pb.IsIndeterminate = !taskinfo.ReportsProgress;

	        PanelProgress.Children.Add(lbl);
	        grid.Children.Add(pb);
	        grid.Children.Add(tb);
	        PanelProgress.Children.Add(grid);

	        taskinfo.Label = lbl;
	        taskinfo.ProgressBar = pb;
	        //ci.mGrid = grid;
	        taskinfo.TextBlock = tb;

	        taskinfo.Worker = new MyBackgroundWorker(taskinfo);
	        taskinfo.Worker.WorkerReportsProgress = true;
	        taskinfo.Worker.DoWork += taskinfo.Handler;
	        taskinfo.Worker.ProgressChanged += Worker_ProgressChanged;
	        taskinfo.Worker.RunWorkerCompleted += Worker_RunWorkerCompleted;
	        taskinfo.Worker.WorkerSupportsCancellation = true;
	        taskinfo.Worker.RunWorkerAsync(taskinfo.Calldata);
	        taskinfo.State = TaskState.Running;
	    }
	}

    enum TaskState
    {
        Initial,
        Running,
        Finished
    }

    class TaskInfo
    {
        public TaskInfo(DoWorkEventHandler handler, string title = null, bool reportsProgress = false, object calldata = null)
        {
            Handler = handler;
            Calldata = calldata;
            ReportsProgress = reportsProgress;
            Title = title;
        }

        public DoWorkEventHandler Handler { get; }
        public string Title { get; }
        public bool ReportsProgress { get; }
        public object Calldata  { get; }
        public TaskState State { get; set; } = TaskState.Initial;
        public BackgroundWorker Worker { get; set; }
        public ProgressBar ProgressBar { get; set; }
        public TextBlock TextBlock { get; set; }
        public Label Label { get; set; }
    }

    public class WndProgressManager
    {
        private WndProgress wnd;
        private bool _modal;
        private Window _owner;

        List<TaskInfo> listTasks = new List<TaskInfo>();

        public WndProgressManager(Window owner, bool modal)
        {
            _owner = owner;
            _modal = modal;
        }

        public void AddTask(DoWorkEventHandler iHandler, string iTitle, bool iWorkerReportsProgress, object iCalldata)
        {
            lock (listTasks)
            {
                listTasks.Add(new TaskInfo(iHandler, iTitle, iWorkerReportsProgress, iCalldata));
            }
            startNewTasks();
        }

        void startNewTasks()
        {
            if (wnd == null)
            {
                wnd = new WndProgress(_owner);
                if (_modal)
                    wnd.ShowDialog();
                else
                    wnd.Show();
            }

            lock (listTasks)
            {
                foreach (var task in listTasks.Where(t => t.State == TaskState.Initial))
                {
                    wnd.StartTask(task);
                }
            }
        }
    }
}
