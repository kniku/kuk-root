using System;
using System.Collections;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using knk.shared.logger;

namespace BI_Gui.dialogs
{
	/// <summary>
	/// Interaktionslogik für Log.xaml
	/// </summary>
	public partial class LogViewer : Window
	{
		public bool realClosing;

		public LogViewer()
		{
			InitializeComponent();
		}

		private void Button_Click(object sender, RoutedEventArgs e)
		{
			Button b = sender as Button;
			if (b.Name.Equals("btnClose"))
			{
				Hide();
			}
			else if (b.Name.Equals("btnClear"))
			{
				ListView viewer = FindName("listLog") as ListView;
				if (viewer != null)
				{
					viewer.Items.Clear();
				}
			}
		}

		private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			if (!realClosing)
			{
				Hide();
				e.Cancel = true;
			}
		}

	}

	public class Logger : AbstractLogger
	{
		int mMaxLines;
		LogViewer mLogWindow;

		public Logger(int iMaxLines)
			: base("BI_Gui_Logger")
		{
			mMaxLines = iMaxLines;
			mLogWindow = new LogViewer();
			mLogWindow.realClosing = false;
		}

		public override void Log(LogLevel iLevel, string iMsg)
		{
			ListView viewer = mLogWindow.FindName("listLog") as ListView;
			if (viewer != null)
			{
				ListViewItem item = new ListViewItem();
				item.Content = iMsg;

				if (iLevel == LogLevel.Warning) item.Foreground = Brushes.Blue;
				if (iLevel == LogLevel.Error) item.Foreground = Brushes.Red;
				
				if (viewer.Items.Count >= mMaxLines)
				{
					viewer.Items.RemoveAt(0);
				}
				viewer.Items.Add(item);
				viewer.ScrollIntoView(item);
			}
		}

		public LogViewer getViewer()
		{
			return mLogWindow;
		}

		public void OpenViewer()
		{
			mLogWindow.Show();
		}
		public void CloseViewer()
		{
			mLogWindow.realClosing = true;
			mLogWindow.Close();
		}
	}

}
