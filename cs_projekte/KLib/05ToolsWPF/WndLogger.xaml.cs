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
using System.Diagnostics;

namespace KLib.Wpf
{
	/// <summary>
	/// Interaktionslogik für WndLogger.xaml
	/// </summary>
	public partial class WndLogger : Window
	{
		private static readonly log4net.ILog Logger = log4net.LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);
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

			Hierarchy hierarchy = (Hierarchy)LogManager.GetRepository();

			if (LogMemAppender == null)
			{
				// Memory Appender ins Logging einhaengen
				LogMemAppender = new MemoryAppender();
				LogMemAppender.ActivateOptions();
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

			comboLevel.Items.Add(Level.All);
			comboLevel.Items.Add(Level.Debug);
			comboLevel.Items.Add(Level.Info);
			comboLevel.Items.Add(Level.Warn);
			comboLevel.Items.Add(Level.Error);
			comboLevel.Items.Add(Level.Fatal);
			comboLevel.Items.Add(Level.Off);
			comboLevel.Text = hierarchy.Root.Level.ToString();

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

		private void comboLevel_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			Hierarchy hierarchy = (Hierarchy)LogManager.GetRepository();
			hierarchy.Root.Level = Level.Info;
			Logger.InfoFormat("Log level changed to {0}", comboLevel.SelectedItem);
			hierarchy.Root.Level = comboLevel.SelectedItem as Level;
			hierarchy.RaiseConfigurationChanged(EventArgs.Empty);
		}

		private void Button_Click_1(object sender, RoutedEventArgs e)
		{

		}

		private void btnEditor_Click(object sender, RoutedEventArgs e)
		{

			string fn = System.IO.Path.GetTempPath() + @"\mylog.txt";

			using (System.IO.StreamWriter file = new System.IO.StreamWriter(fn))
			{
				foreach (var item in listLog.Items)
				{
					file.WriteLine(item.ToString());
				}
			}

			Process P = new Process();
			P.StartInfo.FileName = fn;
//			P.StartInfo.Arguments = (file_log as knk.shared.logger.impl.RollingFile).getFileName();
			P.Start();
		}

		private void btnClear_Click(object sender, RoutedEventArgs e)
		{
			listLog.Items.Clear();
		}
	}
}
