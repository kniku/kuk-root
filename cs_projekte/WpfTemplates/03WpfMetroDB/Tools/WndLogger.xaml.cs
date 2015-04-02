using System;
using System.Collections.Generic;
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
using log4net;
using log4net.Appender;
using log4net.Core;
using log4net.Repository.Hierarchy;
using System.Windows.Threading;

namespace _03WpfMetroDB.Tools
{
	/// <summary>
	/// Interaktionslogik für WndLogger.xaml
	/// </summary>
	public partial class WndLogger : Window
	{
		static MemoryAppender LogMemAppender;
		static DispatcherTimer dispatcherTimer;

		static WndLogger mWndLogger;

		//public static WndLogger CreateAndShowWndLogger()
		//{
		//	if (mWndLogger == null)
		//	{
		//		CreateWndLogger();
		//		mWndLogger.Show();
		//	}
		//	else
		//	{
		//		mWndLogger.Activate();
		//	}
		//	return mWndLogger;
		//}

		public static WndLogger CreateOrGetWndLogger(Window iOwner)
		{
			if (mWndLogger == null)
			{
				mWndLogger = new WndLogger();
				mWndLogger.Owner = iOwner;
			}
			return mWndLogger;
		}

		public static bool IsAlreadyOpen()
		{
			return mWndLogger != null;
		}

		private WndLogger()
		{
			InitializeComponent();

			if (LogMemAppender == null)
			{
				// Memory Appender ins Logging einhaengen
				LogMemAppender = new MemoryAppender();
//				LogMemAppender.Threshold = Level.All;
				LogMemAppender.ActivateOptions();

				Hierarchy hierarchy = (Hierarchy)LogManager.GetRepository();
				hierarchy.Root.AddAppender(LogMemAppender);

				//configure the logging at the root.  
				//hierarchy.Root.Level = Level.All;

				//mark repository as configured and  
				//notify that is has changed.  
				hierarchy.Configured = true;
				hierarchy.RaiseConfigurationChanged(EventArgs.Empty);
			}
			dispatcherTimer = new DispatcherTimer();
			dispatcherTimer.Tick += new EventHandler(dispatcherTimer_Tick);
			dispatcherTimer.Interval = new TimeSpan(0, 0, 0, 0, 200);
		}

		//~WndLogger()
		//{
		//	dispatcherTimer.Stop();
		//}


		private void dispatcherTimer_Tick(object sender, EventArgs e)
		{
//			listLog.Items.Add(DateTime.Now.ToString() + "-tick()");
			bool found = false;
			
			foreach (LoggingEvent logevent in LogMemAppender.GetEvents())
			{
				found = true;
				listLog.Items.Add(string.Format("{2} {0}: {1}", logevent.Level, logevent.RenderedMessage, logevent.TimeStamp));
			}

			if (found)
			{
				LogMemAppender.Clear();
				CommandManager.InvalidateRequerySuggested();
				listLog.Items.MoveCurrentToLast();
				listLog.SelectedItem = listLog.Items.CurrentItem;
				listLog.ScrollIntoView(listLog.Items.CurrentItem);
			}
		}

		private void Button_Click(object sender, RoutedEventArgs e)
		{
			Close();
		}

		private void WndLogger1_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			dispatcherTimer.Stop();
			mWndLogger = null;
		}

		private void WndLogger1_Loaded(object sender, RoutedEventArgs e)
		{
			dispatcherTimer.Start();
		}
	}
}
