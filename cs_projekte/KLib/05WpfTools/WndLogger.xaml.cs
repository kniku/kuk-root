using System;
using System.Drawing;
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

	    public static WndLogger Instance
	    {
	        get
	        {
	            if (!InstanceExists)
	            {
	                mWndLogger = new WndLogger();
	            }
	            return mWndLogger;
	        }
        }

	    public enum RelativePositionToOwner
	    {
	        None, Left, Right, Above, Below
	    }

        public static void InstanceShow(Window iOwner = null, RelativePositionToOwner Pos = RelativePositionToOwner.None, Rect? PosAndSize = null)
	    {
	        if (!InstanceExists)
	        {
	            if (iOwner != null)
	            {
	                Instance.Owner = iOwner;
                    //iOwner.LocationChanged += (sender, args) => { Instance.SetRelativePosition(Pos, PosAndSize); };
	                //iOwner.SizeChanged += (sender, args) => { Instance.SetRelativePosition(Pos, PosAndSize); };
	            }
	            Instance.SetRelativePosition(Pos, PosAndSize);
                Instance.Show();
	        }
	    }

        public static void InstanceClose()
	    {
	        if (InstanceExists)
	        {
	            Instance.Close();
	        }
	    }

        public static bool InstanceExists
		{
		    get { return mWndLogger != null; }
		}

	    private void SetRelativePosition(RelativePositionToOwner Pos = RelativePositionToOwner.None, Rect? PosAndSize = null)
	    {
	        switch (Pos)
	        {
	            case RelativePositionToOwner.None:
	                if (PosAndSize.HasValue)
	                {
	                    Top = PosAndSize.Value.Top;
	                    Left = PosAndSize.Value.Left;
	                    Height = PosAndSize.Value.Height;
	                    Width = PosAndSize.Value.Width;
	                }
	                break;
	            case RelativePositionToOwner.Right:
	                if (Owner != null)
	                {
	                    Left = Owner.Left + Owner.Width;
	                    if (PosAndSize.HasValue)
	                    {
	                        Top = PosAndSize.Value.Top;
	                        Height = PosAndSize.Value.Height;
	                        Width = PosAndSize.Value.Width;
	                    }
	                    else
	                    {
	                        Top = Owner.Top;
	                        Height = Owner.Height;
	                        Width = 400;
	                    }
	                }
	                break;
	            case RelativePositionToOwner.Below:
	                if (Owner != null)
	                {
	                    Top = Owner.Top + Owner.ActualHeight;
	                    if (PosAndSize.HasValue)
	                    {
	                        Left = PosAndSize.Value.Left;
	                        Height = PosAndSize.Value.Height;
	                        Width = PosAndSize.Value.Width;
	                    }
	                    else
	                    {
	                        Left = Owner.Left;
	                        Height = 300;
	                        Width = Owner.Width;
	                    }
	                }
	                break;
	        }
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

		private void dispatcherTimer_Tick(object sender, EventArgs e)
		{
			bool found = false;
			
			foreach (LoggingEvent logevent in LogMemAppender.GetEvents())
			{
				found = true;

				System.Windows.Documents.TextRange rangeOfText1 = new System.Windows.Documents.TextRange(textLog.Document.ContentEnd, textLog.Document.ContentEnd);
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
				textLog.ScrollToEnd();
			}
		}

		private void Button_Click(object sender, RoutedEventArgs e)
		{
			Close();
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

		private void btnEditor_Click(object sender, RoutedEventArgs e)
		{

			string fn = System.IO.Path.GetTempPath() + @"\mylog.txt";

			using (System.IO.StreamWriter file = new System.IO.StreamWriter(fn))
			{
				textLog.SelectAll();
				file.WriteLine(textLog.Selection.Text);
			}

			Process P = new Process();
			P.StartInfo.FileName = fn;
			P.Start();
		}

		private void btnClear_Click(object sender, RoutedEventArgs e)
		{
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
