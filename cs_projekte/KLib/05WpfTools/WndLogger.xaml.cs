using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using log4net;
using log4net.Appender;
using log4net.Core;
using log4net.Repository.Hierarchy;
using System.Windows.Threading;
using System.Diagnostics;
using log4net.Layout;

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

		/// <summary>
		/// Zum Erzeugen verwende CreateOrGetWndLogger()!
		/// </summary>
		private WndLogger()
		{
			InitializeComponent();

			Hierarchy hierarchy = (Hierarchy)LogManager.GetRepository();

			if (LogMemAppender == null)
			{
				// Memory Appender ins Logging einhaengen
				LogMemAppender = new MemoryAppender();

				//PatternLayout patternLayout = new PatternLayout();
				//patternLayout.ConversionPattern = "%date [%thread] %-5level %logger - %message%newline";
				//patternLayout.ActivateOptions();
				//LogMemAppender.Layout = patternLayout;
				
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
//				listLog.Items.Add(string.Format("{2} {0}: {1}", logevent.Level, logevent.RenderedMessage, logevent.TimeStamp));
				//textLog.SelectionBrush = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Colors.Yellow);
				//textLog.AppendText(string.Format("{2} {0}: {1}", logevent.Level, logevent.RenderedMessage, logevent.TimeStamp));
				//textLog.SelectionBrush = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Colors.Green);
				//textLog.AppendText(string.Format("{2} {0}: {1}", logevent.Level, logevent.RenderedMessage, logevent.TimeStamp));

				System.Windows.Documents.TextRange rangeOfText1 = new System.Windows.Documents.TextRange(textLog.Document.ContentEnd, textLog.Document.ContentEnd);
//				rangeOfText1.Text = string.Format("{2} {0}: {1}\r", logevent.Level, logevent.RenderedMessage, logevent.TimeStamp).Replace("\n","");
				rangeOfText1.Text = string.Format("{2} {0}: {1} ({3}:{4})\r", logevent.Level, logevent.RenderedMessage, logevent.TimeStamp,
					logevent.LocationInformation.FileName, logevent.LocationInformation.LineNumber).Replace("\n", "");
				
				
				System.Windows.Media.SolidColorBrush brush;

				if (logevent.Level >= Level.Error)
				{
					brush = System.Windows.Media.Brushes.Red;
				}
				else if (logevent.Level >= Level.Warn)
				{
					brush = System.Windows.Media.Brushes.Blue;
				}
				else if (logevent.Level >= Level.Info)
				{
					brush = System.Windows.Media.Brushes.Black;
				}
				else
				{
					brush = System.Windows.Media.Brushes.Gray;
				}
				rangeOfText1.ApplyPropertyValue(System.Windows.Documents.TextElement.ForegroundProperty, brush);
			}

			if (found)
			{
				LogMemAppender.Clear();
				CommandManager.InvalidateRequerySuggested();
				//listLog.Items.MoveCurrentToLast();
				//listLog.SelectedItem = listLog.Items.CurrentItem;
				//listLog.ScrollIntoView(listLog.Items.CurrentItem);
				textLog.ScrollToEnd();
			}
		}

		private void Button_Click(object sender, RoutedEventArgs e)
		{
			Close();
		}

		private void WndLogger1_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			//dispatcherTimer.Stop();
			//mWndLogger = null;
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
				//foreach (var item in listLog.Items)
				//{
				//	file.WriteLine(item.ToString());
				//}
				textLog.SelectAll();
				file.WriteLine(textLog.Selection.Text);
			}

			Process P = new Process();
			P.StartInfo.FileName = fn;
//			P.StartInfo.Arguments = (file_log as knk.shared.logger.impl.RollingFile).getFileName();
			P.Start();
		}

		private void btnClear_Click(object sender, RoutedEventArgs e)
		{
//			listLog.Items.Clear();
			textLog.SelectAll();
			textLog.Selection.Text = "";
		}

		private void WndLogger1_Closed(object sender, EventArgs e)
		{
			dispatcherTimer.Stop();
			mWndLogger = null;
		}
	}
}
